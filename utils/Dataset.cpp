#include "Dataset.hpp"

#include <QDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QNetworkAccessManager>
#include <QtConcurrent>
#include <unordered_set>

#include "Compliance.hpp"
#include "csv.hpp"
#include "GlobalDataModel.hpp"
#include "Measurement.hpp"

// Column constants
const std::string ID_COLUMN = "@id";
const std::string SAMPLE_POINT_COLUMN = "sample.samplingPoint";
const std::string SAMPLING_POINT_NOTATION_COLUMN = "sample.samplingPoint.notation";
const std::string SAMPLING_POINT_LABEL_COLUMN = "sample.samplingPoint.label";
const std::string SAMPLE_DATE_TIME_COLUMN = "sample.sampleDateTime";
const std::string DETERMINAND_LABEL_COLUMN = "determinand.label";
const std::string DETERMINAND_DEFINITION_COLUMN = "determinand.definition";
const std::string DETERMINAND_UNIT_LABEL_COLUMN = "determinand.unit.label";
const std::string RESULT_COLUMN = "result";
const std::string DETERMINAND_NOTATION_COLUMN = "determinand.notation";

void Dataset::loadDataset(const std::string &path) {
    // Clear previous data
    data.clear();
    measurementsByDeterminand.clear();
    measurementsBySamplingPoint.clear();

    // Run CSV parsing and processing in a background thread
    //  QTConcurrency has been utilised to minimise "freezing"
    // Derived from: https://stackoverflow.com/questions/70054114/how-to-run-a-member-function-in-qtconcurrent-in-qt6
    QFuture<void> future = QtConcurrent::run([this, path]() {
        // Import csvReader
        csv::CSVReader csvReader(path);

        ComplianceChecker complianceChecker;

        // Preload valid compounds
        std::unordered_set<std::string> validCompounds;
        for (const auto &pair: complianceChecker.complianceThresholds) {
            validCompounds.insert(pair.first);
        }

        // Iterate through each row
        for (const auto &row: csvReader) {
            const std::string determinandLabel = row[DETERMINAND_LABEL_COLUMN].get<>();
            const std::string samplingPoint = row[SAMPLING_POINT_LABEL_COLUMN].get<>();

            if (validCompounds.find(determinandLabel) == validCompounds.end()) {
                continue; // Skip invalid rows
            }

            // Instance a measurement object with the specified data
            Measurement measurement{
                row[ID_COLUMN].get<>(),
                row[SAMPLE_POINT_COLUMN].get<>(),
                row[SAMPLING_POINT_NOTATION_COLUMN].get<>(),
                samplingPoint,
                row[SAMPLE_DATE_TIME_COLUMN].get<>(),
                determinandLabel,
                row[DETERMINAND_DEFINITION_COLUMN].get<>(),
                row[DETERMINAND_UNIT_LABEL_COLUMN].get<>(),
                row[RESULT_COLUMN].get<double>(),
                row[DETERMINAND_NOTATION_COLUMN].get<int>()
            };

            // Push to storage structures
            data.push_back(measurement);
            measurementsByDeterminand[measurement.getCompoundName()].push_back(measurement);
            measurementsBySamplingPoint[measurement.getLabel()].push_back(measurement);
        }
    });

    // Use QFutureWatcher to notify when processing is complete
    QFutureWatcher<void> *watcher = new QFutureWatcher<void>();
    connect(watcher, &QFutureWatcher<void>::finished, this, [this]() {
        qDebug() << "Dataset loaded successfully!";
        emit GlobalDataModel::instance().dataReady();
        fetchLatLonForSamplingPoints();
    });

    watcher->setFuture(future);
}

// Fetch requests to get Latitude and Longitude
void Dataset::fetchLatLonForSamplingPoints() {
    std::unordered_set<std::string> uniqueSamplingPoints;

    for (const auto &sample: data) {
        uniqueSamplingPoints.insert(sample.getSamplingPointNotation());
    }

    pendingRequests = uniqueSamplingPoints.size();

    // Derived from: https://doc.qt.io/qt-6/qnetworkaccessmanager.html
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // Fetch only unique sampling points to minimise fetches
    for (const auto &notation: uniqueSamplingPoints) {
        QString urlString;
        for (const auto &sample: data) {
            if (sample.getSamplingPointNotation() == notation) {
                urlString = QString::fromStdString(sample.getSamplingPoint());
                break;
            }
        }

        urlString.replace("http://", "https://");
        QUrl url(urlString);
        QNetworkRequest request(url);
        QNetworkReply *reply = manager->get(request);

        connect(reply, &QNetworkReply::finished, this, [this, reply, notation]() {
            handleNetworkData(reply, notation);
        });
    }
}

// Derived from: https://doc.qt.io/qt-6/qnetworkaccessmanager.html
void Dataset::handleNetworkData(QNetworkReply *reply, const std::string &locationNotationStr) {
    if (reply->error() == QNetworkReply::NoError) {
        // Decode data and push to appropriate data structures
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        double lat = jsonObj["items"].toArray().first().toObject()["lat"].toDouble();
        double lon = jsonObj["items"].toArray().first().toObject()["long"].toDouble();
        QString locationNotation = jsonObj["items"].toArray().first().toObject()["notation"].toString().trimmed();

        qDebug() << locationNotation;
        samplingPointCoordinates[locationNotation.toStdString()] = std::make_pair(lat, lon);
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();

    // Broadcast that fetching has been completed
    if (--pendingRequests == 0) {
        qDebug() << "All fetches complete.";
        emit GlobalDataModel::instance().emitFetchesComplete();
    }
    emit pendingRequestsChanged();
}

std::unordered_map<std::string, std::pair<int, std::string>> Dataset::getSamplingPointCounts() const {
    std::unordered_map<std::string, std::pair<int, std::string>> samplingPointCounts;

    for (const auto &measurement: data) {
        const std::string &samplingPoint = measurement.getLabel();
        samplingPointCounts[samplingPoint].first++;
        samplingPointCounts[samplingPoint].second = measurement.getSamplingPointNotation();
    }

    return samplingPointCounts;
}


// Only fetch locations that have sufficient measurements
std::vector<std::string> Dataset::getHighDataPointLocations() {
    std::unordered_set<std::string> highDataPointSet;
    std::vector<std::string> highDataPointLocations;

    for (const auto&[fst, snd]: measurementsBySamplingPoint) {
        if (snd.size() > 5) {
            highDataPointSet.insert(fst);
        }
    }

    highDataPointLocations.assign(highDataPointSet.begin(), highDataPointSet.end());
    std::sort(highDataPointLocations.begin(), highDataPointLocations.end());

    return highDataPointLocations;
}

std::vector<std::string> Dataset::getHighDataPointLocations(const std::string &compound) {
    std::vector<std::string> compounds;
    if (compound == "POPS") {
        std::cout << "Populated Pops" << std::endl;
        compounds = ComplianceChecker::getPOPs();
    } else if (compound == "PFAS") {
        compounds = ComplianceChecker::getPFAs();
        std::cout << "Populated PFAS" << std::endl;
    }

    std::set foundCompounds(compounds.begin(), compounds.end());
    std::map<std::string, std::map<std::string, int> > locationCompoundCounts;

    for (const auto &measurement: data) {
        if (foundCompounds.find(measurement.getCompoundName()) != foundCompounds.end()) {
            locationCompoundCounts[measurement.getLabel()][measurement.getCompoundName()]++;
        }
    }

    std::vector<std::string> filteredHighDataPointLocations;

    for (const auto &locationPair: locationCompoundCounts) {
        const auto &compoundCounts = locationPair.second;

        for (const auto &compoundCount: compoundCounts) {
            if (compoundCount.second >= 2) {
                filteredHighDataPointLocations.push_back(locationPair.first);
                break;
            }
        }
    }

    return filteredHighDataPointLocations;
}

void Dataset::checkDataExists() const {
    if (data.empty()) {
        throw std::runtime_error("Dataset is empty!");
    }
}

// Sort by Timestamp
std::vector<Measurement> Dataset::sortByTimestamp() {
    std::sort(data.begin(), data.end(), [](const Measurement &a, const Measurement &b) {
        return a.getDatetime() < b.getDatetime();
    });
    return data;
}
