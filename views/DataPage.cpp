#include <QtWidgets>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include "DataPage.hpp"
#include "../utils/DataModel.hpp"
#include "../utils/GlobalDataModel.hpp"


static const int MIN_WIDTH = 620;

// DataPage.cpp
DataPage::DataPage(QWidget *parent)
    : QWidget(parent),
      model(GlobalDataModel::instance().getDataModel()) {
    // Use reference from GlobalDataModel
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
    connect(statsButton, &QPushButton::clicked, this, &DataPage::displayStats);
}

void DataPage::displayStats() {
    int rowCount = model.rowCount(QModelIndex());
    int columnCount = model.columnCount(QModelIndex());
    // Assuming each cell is a QVariant and calculating data size roughly
    int dataSize = rowCount * columnCount * sizeof(QVariant);

    QString stats = QString("Rows: %1\nColumns: %2\nData Size: ~%3 bytes\n")
            .arg(rowCount)
            .arg(columnCount)
            .arg(dataSize);

    QMessageBox::information(this, "Dataset Statistics", stats);
}

void DataPage::setMainLayout() {
    QVBoxLayout *buttons = new QVBoxLayout();
    buttons->addWidget(setLocationButton);
    buttons->addWidget(loadButton);
    buttons->addWidget(statsButton);

    QHBoxLayout *dataPage = new QHBoxLayout();
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

    QDir dir(dataLocation);
    QStringList filePattern = {"Y-*.csv"};
    QFileInfoList fileList = dir.entryInfoList(filePattern, QDir::Files);

    if (fileList.isEmpty()) {
        QMessageBox::warning(this, "No CSV Files Found",
                             "No CSV files found with the pattern Y-[YEAR].csv in the selected directory.");
        return;
    }

    QString filename;
    if (fileList.size() == 1) {
        filename = fileList.first().fileName();
    } else {
        QStringList fileNames;
        for (const QFileInfo &fileInfo: fileList) {
            fileNames << fileInfo.fileName();
        }

        bool ok;
        filename = QInputDialog::getItem(this, "Select CSV File",
                                         "Choose a file:", fileNames, 0, false, &ok);
        if (!ok) {
            return; // If the user cancels the operation
        }
    }

    QString path = dataLocation + "/" + filename;

    try {
        GlobalDataModel::instance().getDataset().loadDataset(path.toStdString());
        model.updateFromFile(path);
        table->resizeColumnsToContents();
    } catch (const std::exception &error) {
        QMessageBox::critical(this, "CSV File Error", error.what());
    }
}
