#include <QtWidgets>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QMovie>
#include <iostream>
#include "DataPage.hpp"
#include "../utils/DataModel.hpp"
#include "../utils/GlobalDataModel.hpp"

static const int MIN_WIDTH = 620;

DataPage::DataPage(QWidget *parent)
    : QWidget(parent),
      model(GlobalDataModel::instance().getDataModel()) {
    createTable();
    createButtons();
    createLoadingSpinner();
    createStatsLabel();
    setMainLayout();
    setMinimumWidth(MIN_WIDTH);
}

void DataPage::createLoadingSpinner() {
    loadingSpinner = new QLabel(this);
    loadingMovie = new QMovie("./qml/spinner.gif"); // Assumes a spinner.gif in resources

    // Create loading text label
    loadingText = new QLabel(tr("Loading..."), this);
    loadingText->setAlignment(Qt::AlignCenter);
    loadingText->setVisible(false);

    // Customize spinner appearance
    loadingSpinner->setMovie(loadingMovie);
    loadingSpinner->setAlignment(Qt::AlignCenter);
    loadingSpinner->setVisible(false);

    // Set a reasonable size for the spinner
    QSize spinnerSize(50, 50);
    loadingMovie->setScaledSize(spinnerSize);
    loadingSpinner->setFixedSize(spinnerSize);
}

void DataPage::createStatsLabel() {
    statsLabel = new QLabel(this);
    statsLabel->setAlignment(Qt::AlignLeft);
    statsLabel->setWordWrap(true);
}

void DataPage::updateLoadingStats() {
    // Get current loading progress from GlobalDataModel
    int currentRowCount = model.rowCount(QModelIndex());
    int currentColumnCount = model.columnCount(QModelIndex());
    int currentDataSize = currentRowCount * currentColumnCount * sizeof(QVariant);

    // Update stats label with current loading progress
    statsLabel->setText(QString(tr("Loading: Rows %1 | Columns %2 | Estimated Data Size %3 bytes"))
        .arg(currentRowCount)
        .arg(currentColumnCount)
        .arg(currentDataSize));
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

void DataPage::setMainLayout() {
    QVBoxLayout *buttons = new QVBoxLayout();
    buttons->addWidget(setLocationButton);
    buttons->addWidget(loadButton);
    buttons->addWidget(statsButton);

    // Create a vertical layout for the table, spinner, and stats
    QVBoxLayout *mainContentLayout = new QVBoxLayout();
    mainContentLayout->addWidget(table);

    // Create a horizontal layout for spinner and loading text
    QHBoxLayout *spinnerLayout = new QHBoxLayout();
    spinnerLayout->addStretch();
    spinnerLayout->addWidget(loadingSpinner);
    spinnerLayout->addWidget(loadingText);
    spinnerLayout->addStretch();

    mainContentLayout->addLayout(spinnerLayout);
    mainContentLayout->addWidget(statsLabel);

    QHBoxLayout *dataPage = new QHBoxLayout();
    dataPage->addLayout(buttons, 1);
    dataPage->addLayout(mainContentLayout, 9);

    setLayout(dataPage);
}

void DataPage::openCSV() {
    if (dataLocation.isEmpty()) {
        QMessageBox::critical(this, tr("DP_CRITICAL_MSG_TITLE"),
                              tr("DP_CRITICAL_MSG_FIRST_LINE") + "\n" + tr("DP_CRITICAL_MSG_SECOND_LINE"));
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

    // Disable the load button and show spinner
    loadButton->setEnabled(false);
    loadingSpinner->setVisible(true);
    loadingText->setVisible(true);
    loadingMovie->start();

    // Create a timer to update loading stats every 100ms
    loadingTimer = new QTimer(this);
    connect(loadingTimer, &QTimer::timeout, this, &DataPage::updateLoadingStats);
    loadingTimer->start(100); // Update every 100 milliseconds

    // Clear previous stats
    statsLabel->clear();

    try {
        GlobalDataModel::instance().loadData(path.toStdString());
        model.setDataset(&GlobalDataModel::instance().getDataset());

        connect(&GlobalDataModel::instance(), &GlobalDataModel::dataReady, this, &DataPage::onDataLoaded);
    } catch (const std::exception &error) {
        QMessageBox::critical(this, tr("DP_CSV_ERROR_TITLE"), error.what());
    }
}

void DataPage::onDataLoaded() const {
    // Stop and hide the spinner and loading text
    loadingMovie->stop();
    loadingSpinner->setVisible(false);
    loadingText->setVisible(false);
    loadingTimer->stop();

    // Update stats label
    int rowCount = model.rowCount(QModelIndex());
    int columnCount = model.columnCount(QModelIndex());
    int dataSize = rowCount * columnCount * sizeof(QVariant);

    statsLabel->setText(QString(tr("Rows: %1 | Columns: %2 | Data Size: %3 bytes"))
        .arg(rowCount)
        .arg(columnCount)
        .arg(dataSize));

    // Inform the view about data changes
    emit model.layoutChanged();
    table->resizeColumnsToContents();

    // Re-enable the load button
    loadButton->setEnabled(true);
}

void DataPage::setDataLocation() {
    QString directory = QFileDialog::getExistingDirectory(
        this, "Data Location", ".",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!directory.isEmpty()) {
        dataLocation = directory;
    }
}

void DataPage::displayStats() {
    int rowCount = model.rowCount(QModelIndex());
    int columnCount = model.columnCount(QModelIndex());
    // Assuming each cell is a QVariant and calculating data size roughly
    int dataSize = rowCount * columnCount * sizeof(QVariant);

    QString stats = QString(
                tr("DP_STATS_FIRST_LINE") + "%1\n" + tr("DP_STATS_SECOND_LINE") + "%2\n" + tr("DP_STATS_THIRD_LINE_P1")
                + "%3 " + tr("DP_STATS_THIRD_LINE_P2"))
            .arg(rowCount)
            .arg(columnCount)
            .arg(dataSize);

    QMessageBox::information(this, tr("DP_STATS_POPUP_TITLE"), stats);
}

void DataPage::retranslateUI() {
    // Force re-translate of all UI components (this ensures that UI updates even if translator is installed after initial load)
    setLocationButton->setText(tr("DP_SET_LOCATION"));
    loadButton->setText(tr("DP_LOAD"));
    statsButton->setText(tr("DP_STATS"));
}
