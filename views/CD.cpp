//
// Created by Josh Mundray on 06/12/2024.
//
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
    title = new QLabel(tr("DASHBOARD_CD"));
    QFont titleFont("Arial", 20, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
}

void ComplianceDashboard::createFilters() {
    QStringList locationOptions;
    locationOptions << "All locations" << "1" << "2" << "3" << "4";
    location = new searchableComboBox();
    location->setOptions(locationOptions);
    locationLabel = new QLabel("&Location:");
    locationLabel->setBuddy(location);
    locationLabel->setWordWrap(true);

    QStringList pollutantOptions;
    pollutantOptions << "All pollutants" << "chlorine" << "ethanol";
    pollutant = new searchableComboBox();
    pollutant->setOptions(pollutantOptions);
    pollutantLabel = new QLabel("&Pollutant:");
    pollutantLabel->setBuddy(pollutant);

    QStringList complianceOptions;
    complianceOptions << "All compliances" << "Red" << "Orange" << "Green";
    compliance = new QComboBox();
    compliance->addItems(complianceOptions);
    complianceLabel = new QLabel("&Time Range:");
    complianceLabel->setBuddy(compliance);

    connect(compliance, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ComplianceDashboard::filterDataByCompliance);
}

void ComplianceDashboard::createPollutantIndicator() {
    currentPollutant = new QLabel("<current pollutant> is:");
    currentPollutantCompliance = new QLabel("<red/yellow/green>");
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


void ComplianceDashboard::filterDataByCompliance() {
    QString selectedCompliance = compliance->currentText();
    int targetCompliance = 0;

    if (selectedCompliance == "Red") {
        targetCompliance = 3;
    } else if (selectedCompliance == "Orange") {
        targetCompliance = 2;
    } else if (selectedCompliance == "Green") {
        targetCompliance = 1;
    } else {
        // Show all compliances
        targetCompliance = 0;
    }

    // Filter logic
    for (int row = 0; row < model.rowCount(QModelIndex()); ++row) {
        QModelIndex index = model.index(row, 0);
        QString name = model.data(model.index(row, 0), Qt::DisplayRole).toString();
        double value = model.data(model.index(row, 3), Qt::DisplayRole).toDouble();

        int complianceStatus = complianceChecker.complianceCheck(name.toStdString(), value);

        // Show or hide the row based on compliance
        bool shouldHide = selectedCompliance != "All compliances" && complianceStatus != targetCompliance;
        table->setRowHidden(row, shouldHide);
    }
}