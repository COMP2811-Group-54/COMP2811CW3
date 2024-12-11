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
    setLocationButton = new QPushButton(tr("DP_SET_LOCATION"));
    loadButton = new QPushButton(tr("DP_LOAD"));
    statsButton = new QPushButton(tr("DP_STATS"));

    connect(loadButton, &QPushButton::clicked, this, &DataPage::openCSV);
    connect(setLocationButton, &QPushButton::clicked, this, &DataPage::setDataLocation);
    connect(statsButton, &QPushButton::clicked, this, &DataPage::displayStats);
}

void DataPage::displayStats() {
    int rowCount = model.rowCount(QModelIndex());
    int columnCount = model.columnCount(QModelIndex());
    // Assuming each cell is a QVariant and calculating data size roughly
    int dataSize = rowCount * columnCount * sizeof(QVariant);

    QString stats = QString(tr("DP_STATS_FIRST_LINE")+"%1\n"+tr("DP_STATS_SECOND_LINE")+"%2\n"+tr("DP_STATS_THIRD_LINE_P1")+"%3 "+tr("DP_STATS_THIRD_LINE_P2"))
            .arg(rowCount)
            .arg(columnCount)
            .arg(dataSize);

    QMessageBox::information(this, tr("DP_STATS_POPUP_TITLE"), stats);
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
        QMessageBox::critical(this, tr("DP_CRITICAL_MSG_TITLE"),tr("DP_CRITICAL_MSG_FIRST_LINE")+"\n"+tr("DP_CRITICAL_MSG_SECOND_LINE"));
        return;
    }

    QDir dir(dataLocation);
    QStringList filePattern = {"Y-*.csv"};
    QFileInfoList fileList = dir.entryInfoList(filePattern, QDir::Files);

    if (fileList.isEmpty()) {
        QMessageBox::warning(this, tr("DP_WARNING_MSG_TITLE"),
                             tr("DP_WARNING_MSG_LINE"));
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
        filename = QInputDialog::getItem(this, tr("DP_INPUT_DIALOG_TITLE"),
                                         tr("DP_INPUT_DIALOG_LINE"), fileNames, 0, false, &ok);
        if (!ok) {
            return; // If the user cancels the operation
        }
    }

    QString path = dataLocation + "/" + filename;

    try {
        GlobalDataModel::instance().loadData(path.toStdString());
        model.setDataset(&GlobalDataModel::instance().getDataset());

        // Inform the view about data changes
        emit model.layoutChanged();
        table->resizeColumnsToContents();
    } catch (const std::exception &error) {
        QMessageBox::critical(this, tr("DP_CSV_ERROR_TITLE"), error.what());
    }
}


void DataPage::retranslateUI() {
    // Force re-translate of all UI components (this ensures that UI updates even if translator is installed after initial load)
    setLocationButton->setText(tr("DP_SET_LOCATION"));
    loadButton->setText(tr("DP_LOAD"));
    statsButton->setText(tr("DP_STATS"));
}