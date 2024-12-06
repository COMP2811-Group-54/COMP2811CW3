//
// Created by Josh Mundray on 06/12/2024.
//
#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "CD.hpp"
#include "DataPage.hpp"
#include "stats.hpp"
#include "./utils/DataModel.hpp"

static const int MIN_WIDTH = 620;


ComplianceDashboard::ComplianceDashboard(QWidget *parent): QWidget(parent), statsDialog(nullptr)
{
  createTitle();
  createTable();
  createFilters();
  createPollutantIndicator();
  setMainLayout();

  setMinimumWidth(MIN_WIDTH);
}

void ComplianceDashboard::createTitle()
{
  title = new QLabel("Compliance Dashboard");
  QFont titleFont("Arial", 20, QFont::Bold);
  title->setFont(titleFont);
  title->setAlignment(Qt::AlignCenter);
}

void ComplianceDashboard::createTable()
{
  table = new QTableView();
  table->setModel(&model);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);
}

void ComplianceDashboard::createFilters()
{
  QStringList locationOptions;
  locationOptions << "All locations" << "1" << "2" << "3" << "4";
  location = new QComboBox();
  location->addItems(locationOptions);
  locationLabel = new QLabel("&Location:");
  locationLabel->setBuddy(location);
  locationLabel->setWordWrap(true);

  QStringList pollutantOptions;
  pollutantOptions << "All pollutants" << "chlorine" << "ethanol";
  pollutant = new QComboBox();
  pollutant->addItems(pollutantOptions);
  pollutantLabel = new QLabel("&Pollutant:");
  pollutantLabel->setBuddy(pollutant);

  QStringList complianceOptions;
  complianceOptions << "All compliances" << "Red" << "Orange" << "Green";
  compliance = new QComboBox();
  compliance->addItems(complianceOptions);
  complianceLabel = new QLabel("&Time Range:");
  complianceLabel->setBuddy(compliance);
}

void ComplianceDashboard::createPollutantIndicator()
{
  currentPollutant = new QLabel("<current pollutant> is:");
  currentPollutantCompliance = new QLabel("<red/yellow/green>");
  currentPollutantCompliance->setStyleSheet("background-color: red; color: white;");
}

void ComplianceDashboard::setMainLayout()
{
  QVBoxLayout* pollutantInfo = new QVBoxLayout();
  pollutantInfo->addWidget(pollutant);
  pollutantInfo->addWidget(currentPollutant);
  pollutantInfo->addWidget(currentPollutantCompliance);

  QVBoxLayout* filters = new QVBoxLayout();
  filters->addWidget(location);
  filters->addStretch();
  filters->addLayout(pollutantInfo);
  filters->addStretch();
  filters->addWidget(compliance);

  QHBoxLayout* body = new QHBoxLayout();
  body->addLayout(filters, 1);
  body->addWidget(table, 9);

  QVBoxLayout* finalLayout = new QVBoxLayout();
  finalLayout->addWidget(title);
  finalLayout->addLayout(body);

  setLayout(finalLayout);
}