/*
To implement:

- Method of sharing model with other pages
- Data set loading fix

*/

#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "DataPage.hpp"
#include "stats.hpp"
#include "./utils/DataModel.hpp"

static const int MIN_WIDTH = 620;


DataPage::DataPage(QWidget *parent): QWidget(parent), statsDialog(nullptr)
{
  createTable();
  createButtons();
  setMainLayout();

  setMinimumWidth(MIN_WIDTH);
}


void DataPage::createTable()
{
  table = new QTableView();
  table->setModel(&model);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);
}

void DataPage::createButtons()
{
  setLocationButton = new QPushButton("Set Data Location");
  loadButton = new QPushButton("Load");
  statsButton = new QPushButton("Stats");

  connect(loadButton, SIGNAL(clicked()), this, SLOT(openCSV()));
  connect(setLocationButton, SIGNAL(clicked()), this, SLOT(setDataLocation()));
  // connect(statsButton, SIGNAL(clicked()), this, SLOT(displayStats()));
}

void DataPage::setMainLayout()
{
  QVBoxLayout* buttons = new QVBoxLayout();
  buttons->addWidget(setLocationButton);
  buttons->addWidget(loadButton);
  buttons->addWidget(statsButton);

  QHBoxLayout* dataPage = new QHBoxLayout();
  dataPage->addLayout(buttons, 1);
  dataPage->addWidget(table, 9);

  setLayout(dataPage);
}

void DataPage::setDataLocation()
{
  QString directory = QFileDialog::getExistingDirectory(
    this, "Data Location", ".",
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (directory.length() > 0) {
    dataLocation = directory;
  }
}


void DataPage::openCSV()
{
  if (dataLocation == "") {
    QMessageBox::critical(this, "Data Location Error",
      "Data location has not been set!\n\n"
      "You can specify this via the Set Data Location button"
    );
    return;
  }

  // auto filename = QString("%1-%2.csv")
  //   .arg(significance->currentText()).arg(period->currentText());

  auto filename = QString("Y-2024.csv");

  auto path = dataLocation + "/" + filename;

  try {
    model.updateFromFile(path);
  }
  catch (const std::exception& error) {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }

  fileInfo->setText(QString("Current file: <kbd>%1</kbd>").arg(filename));
  table->resizeColumnsToContents();

  if (statsDialog != nullptr && statsDialog->isVisible()) {
    statsDialog->update();
  }
}