// ComplianceDashboard.cpp

#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "CD.hpp"
#include "DataPage.hpp"
#include "../utils/GlobalDataModel.hpp"
#include "../utils/Compliance.hpp"

static const int MIN_WIDTH = 620;

ComplianceDashboard::ComplianceDashboard(QWidget *parent)
    : QWidget(parent),
      model(GlobalDataModel::instance().getDataModel()),
      complianceChecker() {
    createTitle();
    createTable();
    createFilters();
    createPollutantIndicator();
    setMainLayout();
    setMinimumWidth(MIN_WIDTH);
}

void ComplianceDashboard::createTable() {
    table = new QTableView();
    table->setModel(&model);
    table->setItemDelegate(new ComplianceDelegate(this)); // Assuming ComplianceDelegate is set

    QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    table->setFont(tableFont);
}

void ComplianceDashboard::createTitle() {
    title = new QLabel(tr("CD_TITLE"));
    QFont titleFont("Arial", 20, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
}

void ComplianceDashboard::createFilters() {
    // Fetch and set locations
    QStringList locationOptions{tr("CD_LOCATION_OPTIONS")};
    std::vector<std::string> complianceLocations = ComplianceChecker::getLocations();
    for (const std::string &locationStr: complianceLocations) {
        locationOptions << QString::fromStdString(locationStr);
    }

    location = new searchableComboBox();
    location->setOptions(locationOptions);

    locationLabel = new QLabel("&Location:");
    locationLabel->setBuddy(location);
    locationLabel->setWordWrap(true);

    // Connect to dataReady to update dynamically after data loads
    connect(&GlobalDataModel::instance(), &GlobalDataModel::dataReady, this, [this]() {
        QStringList updatedLocationOptions{"All locations"};
        auto complianceLocations = GlobalDataModel::instance().getDataset().getHighDataPointLocations();

        for (const std::string &locationStr: complianceLocations) {
            updatedLocationOptions << QString::fromStdString(locationStr);
        }
        if (location) {
            location->addItems(updatedLocationOptions);
        }
    });

    // Fetch and set pollutants
    QStringList pollutantOptions{tr("CD_POLLUTANT_OPTIONS")};
    std::vector<std::string> pollutants;

    // Adding pollutants from different categories
    std::vector<std::string> pfas = ComplianceChecker::getPFAs();
    pollutants.insert(pollutants.end(), pfas.begin(), pfas.end());

    std::vector<std::string> pops = ComplianceChecker::getPOPs();
    pollutants.insert(pollutants.end(), pops.begin(), pops.end());

    std::vector<std::string> metals = ComplianceChecker::getMetals();
    pollutants.insert(pollutants.end(), metals.begin(), metals.end());

    std::vector<std::string> vocs = ComplianceChecker::getVOCs();
    pollutants.insert(pollutants.end(), vocs.begin(), vocs.end());

    for (const std::string &chemical: pollutants) {
        pollutantOptions << QString::fromStdString(chemical);
    }

    pollutant = new QComboBox();
    pollutant->addItems(pollutantOptions);
    pollutantLabel = new QLabel("&Pollutant:");
    pollutantLabel->setBuddy(pollutant);

    // Set compliance option
    complianceOptions << tr("CD_COMPLIANCE_OPTION1") << tr("CD_COMPLIANCE_OPTION2") << tr("CD_COMPLIANCE_OPTION3") << tr("CD_COMPLIANCE_OPTION4");
    compliance = new QComboBox();
    compliance->addItems(complianceOptions);
    complianceLabel = new QLabel("&Compliance:");
    complianceLabel->setBuddy(compliance);

    // Connect signals for dropdown changes
    connect(location, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ComplianceDashboard::filterData);
    connect(pollutant, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ComplianceDashboard::filterData);
    connect(compliance, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ComplianceDashboard::filterData);
}


void ComplianceDashboard::createPollutantIndicator() {
    currentPollutant = new QLabel(tr("CD_CURRENT_POLLUTANT"));
    currentPollutantCompliance = new QLabel(tr("CD_CURRENT_POLLUTANT_COMPLIANCE"));
    currentPollutantCompliance->setStyleSheet("background-color: red; color: white;");
}

void ComplianceDashboard::setMainLayout() {
    QVBoxLayout *pollutantInfo = new QVBoxLayout();
    pollutantInfo->addWidget(pollutant);
    pollutantInfo->addWidget(currentPollutant);
    pollutantInfo->addWidget(currentPollutantCompliance);

    QVBoxLayout *filters = new QVBoxLayout();
    filters->addStretch();
    filters->addWidget(location);
    filters->addStretch();
    filters->addLayout(pollutantInfo);
    filters->addStretch();
    filters->addWidget(compliance);
    filters->addStretch();

    QHBoxLayout *body = new QHBoxLayout();
    body->addLayout(filters, 1);
    body->addWidget(table, 9);

    QVBoxLayout *finalLayout = new QVBoxLayout();
    finalLayout->addWidget(title);
    finalLayout->addLayout(body);

    setLayout(finalLayout);
}

void ComplianceDashboard::filterData() {
    auto &dataset = GlobalDataModel::instance().getDataset();
    QString selectedLocation = location->currentText();
    QString selectedPollutant = pollutant->currentText();
    QString selectedCompliance = compliance->currentText();

    // Cache filtered data if the filter state does not change
    static QString prevLocation, prevPollutant, prevCompliance;
    static std::vector<Measurement> cachedFilteredData;

    if (selectedLocation == prevLocation && selectedPollutant == prevPollutant && selectedCompliance ==
        prevCompliance) {
        model.setDataset(new Dataset(cachedFilteredData));
        table->setModel(&model);
        return;
    }

    prevLocation = selectedLocation;
    prevPollutant = selectedPollutant;
    prevCompliance = selectedCompliance;

    std::vector<Measurement> filteredData;
    int targetCompliance = 0;
    if (selectedCompliance == "Red") targetCompliance = 3;
    else if (selectedCompliance == "Orange") targetCompliance = 2;
    else if (selectedCompliance == "Green") targetCompliance = 1;

    for (const auto &measurement: dataset) {
        bool matchLocation = selectedLocation == "All locations" || measurement.getLabel() == selectedLocation.
                             toStdString();
        bool matchPollutant = selectedPollutant == "All pollutants" || measurement.getCompoundName() ==
                              selectedPollutant.toStdString();
        if (matchLocation && matchPollutant) {
            int complianceStatus = complianceChecker.complianceCheck(measurement.getCompoundName(),
                                                                     measurement.getValue());
            if (selectedCompliance == "All compliances" || complianceStatus == targetCompliance) {
                filteredData.push_back(measurement);
            }
        }
    }

    // Cache the filtered data
    cachedFilteredData = filteredData;

    model.setDataset(new Dataset(filteredData));
    table->setModel(&model);
}

void ComplianceDashboard::retranslateUI() {
    // Force re-translate of all UI components (this ensures that UI updates even if translator is installed after initial load)
    title->setText(tr("CD_TITLE"));
    currentPollutant->setText(tr("CD_CURRENT_POLLUTANT"));
    currentPollutantCompliance->setText(tr("CD_CURRENT_POLLUTANT_COMPLIANCE"));
}