#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "DataPage.hpp"
#include "./utils/DataModel.hpp"
#include "./utils/GlobalDataModel.hpp"

static const int MIN_WIDTH = 620;

// DataPage.cpp
DataPage::DataPage(QWidget *parent)
    : QWidget(parent),
      model(GlobalDataModel::instance().getDataModel()) {  // Use reference from GlobalDataModel
    createTable();
    createButtons();
    setMainLayout();
    setMinimumWidth(MIN_WIDTH);
}

void DataPage::createTable() {
    table = new QTableView();

    // Use the assigned DataModel to manage the data display
    table->setModel(&model);

    QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    table->setFont(tableFont);
}

void DataPage::createButtons() {
    setLocationButton = new QPushButton("Set Data Location");
    loadButton = new QPushButton("Load");
    statsButton = new QPushButton("Stats");

    connect(loadButton, &QPushButton::clicked, this, &DataPage::openCSV);
    connect(setLocationButton, &QPushButton::clicked, this, &DataPage::setDataLocation);
    // connect(statsButton, &QPushButton::clicked, this, &DataPage::displayStats);
}

void DataPage::setMainLayout() {
    QVBoxLayout* buttons = new QVBoxLayout();
    buttons->addWidget(setLocationButton);
    buttons->addWidget(loadButton);
    buttons->addWidget(statsButton);

    QHBoxLayout* dataPage = new QHBoxLayout();
    dataPage->addLayout(buttons, 1);
    dataPage->addWidget(table, 9);

    setLayout(dataPage);
}

void DataPage::setDataLocation() {
    QString directory = QFileDialog::getExistingDirectory(
        this, "Data Location", ".",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!directory.isEmpty()) {
        dataLocation = directory;
    }
}

void DataPage::openCSV() {
    if (dataLocation.isEmpty()) {
        QMessageBox::critical(this, "Data Location Error",
            "Data location has not been set!\nSpecify via the Set Data Location button.");
        return;
    }

    QString filename = "Y-2024.csv";  // Or prompt for a file name
    QString path = dataLocation + "/" + filename;

    try {
        // Load the dataset into GlobalDataset
        GlobalDataModel::instance().getDataset().loadDataset(path.toStdString());

        // Update model for table view using the same data
        model.updateFromFile(path);

        // Refresh the table view
        table->resizeColumnsToContents();
    } catch (const std::exception& error) {
        QMessageBox::critical(this, "CSV File Error", error.what());
    }
}