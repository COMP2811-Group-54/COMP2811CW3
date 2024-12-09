#include "Dataset.hpp"
#include "Measurement.hpp"
#include "csv.hpp"
#include "Compliance.hpp"

#include <iostream>
#include <unordered_set>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "GlobalDataModel.hpp"

// Column constants
const std::string ID_COLUMN = "@id";
const std::string SAMPLE_POINT_COLUMN = "sample.samplingPoint";
const std::string SAMPLING_POINT_LABEL_COLUMN = "sample.samplingPoint.label";
const std::string SAMPLE_DATE_TIME_COLUMN = "sample.sampleDateTime";
const std::string DETERMINAND_LABEL_COLUMN = "determinand.label";
const std::string DETERMINAND_DEFINITION_COLUMN = "determinand.definition";
const std::string DETERMINAND_UNIT_LABEL_COLUMN = "determinand.unit.label";
const std::string RESULT_COLUMN = "result";
const std::string DETERMINAND_NOTATION_COLUMN = "determinand.notation";

// Index maps
std::unordered_map<std::string, std::vector<Measurement> > measurementsByDeterminand;
std::unordered_map<std::string, std::vector<Measurement> > measurementsBySamplingPoint;

void Dataset::loadDataset(const std::string &path) {
    csv::CSVReader csvReader(path);
    data.clear();
    measurementsByDeterminand.clear();
    measurementsBySamplingPoint.clear();

    ComplianceChecker complianceChecker;
    std::unordered_set<std::string> validCompounds;

    for (const auto &pair: complianceChecker.complianceThresholds) {
        validCompounds.insert(pair.first);
    }

    for (const auto &row: csvReader) {
        const std::string determinandLabel = row[DETERMINAND_LABEL_COLUMN].get<>();
        const std::string samplingPoint = row[SAMPLING_POINT_LABEL_COLUMN].get<>();

        if (validCompounds.find(determinandLabel) != validCompounds.end()) {
            Measurement measurement{
                row[ID_COLUMN].get<>(),
                row[SAMPLE_POINT_COLUMN].get<>(),
                row[SAMPLING_POINT_LABEL_COLUMN].get<>(),
                row[SAMPLE_DATE_TIME_COLUMN].get<>(),
                determinandLabel,
                row[DETERMINAND_DEFINITION_COLUMN].get<>(),
                row[DETERMINAND_UNIT_LABEL_COLUMN].get<>(),
                row[RESULT_COLUMN].get<double>(),
                row[DETERMINAND_NOTATION_COLUMN].get<int>()
            };
            data.push_back(measurement);

            measurementsByDeterminand[determinandLabel].push_back(measurement);
            measurementsBySamplingPoint[samplingPoint].push_back(measurement);
        }
    }

    // Fetch latitude and longitude for all sampling points
    fetchLatLonForSamplingPoints();
}

std::unordered_map<std::string, int> Dataset::getSamplingPointCounts() const {
    std::unordered_map<std::string, int> pointCounts;
    for (const Measurement &m: data) {
        QString samplingPoint = QString::fromStdString(m.getSamplingPoint());
        samplingPoint.replace("http://", "https://");
        pointCounts[samplingPoint.toStdString()]++;
    }
    return pointCounts;
}

void Dataset::fetchLatLonForSamplingPoints() {
    auto samplingCounts = getSamplingPointCounts();
    pendingRequests = data.size(); // Initialize the counter with the number of requests we expect

    for (const auto &sample: data) {
        QString urlString = QString::fromStdString(sample.getSamplingPoint());
        urlString.replace("http://", "https://");
        QUrl url(urlString);
        std::cout << urlString.toStdString() << std::endl;
        QNetworkRequest request(url);

        QNetworkReply *reply = networkManager.get(request);
        connect(reply, &QNetworkReply::finished, this, [=]() { handleNetworkData(reply); });
    }
}

void Dataset::handleNetworkData(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        double lat = jsonObj["items"].toArray().first().toObject()["lat"].toDouble();
        double lon = jsonObj["items"].toArray().first().toObject()["long"].toDouble();

        QString samplingPointId = reply->url().toString();
        samplingPointCoordinates[samplingPointId.toStdString()] = std::make_pair(lat, lon);

        qDebug() << "Pending Reqs: " << pendingRequests << " Lat:" << lat << " Lon:" << lon;

        // Find the corresponding point in your model and update it
        // Ensure model gets updated here
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();

    if (--pendingRequests == 0) {
        std::cout << "feteches complete" << std::endl;
        emit GlobalDataModel::instance().emitFetchesComplete();
    }
}

void Dataset::checkDataExists() const {
    if (size() == 0) {
        throw std::runtime_error("Dataset is empty!");
    }
}
