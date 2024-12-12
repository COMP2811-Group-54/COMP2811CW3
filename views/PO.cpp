#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "PO.hpp"

#include "../utils/Compliance.hpp"
#include "../utils/Measurement.hpp"
#include "../utils/GlobalDataModel.hpp"

PollutantOverview::PollutantOverview(QWidget *parent): QWidget(parent) {
    createTitle();
    // Initialize popChartView with a placeholder chart
    QChart *placeholderChart = new QChart();
    placeholderChart->setTitle(tr("Awaiting Data...")); // Awaiting data placeholder
    createSearchBar(); // Add search bar initialization
    overviewChartView = new QChartView(placeholderChart);
    overviewChartView->setMinimumSize(1000, 400);

    createButtons();
    createFilters();
    createComplianceLabels();
    arrangeWidgets();
}

void PollutantOverview::createTitle() {
    title = new QLabel("Pollutants Overview");
    QFont titleFont("Arial", 20, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
}

void PollutantOverview::createChart(const std::vector<Measurement> &filteredData) {
    QChart *overviewChart = new QChart();
    overviewChart->removeAllSeries();
    overviewChart->setAnimationOptions(QChart::SeriesAnimations);

    // X-Axis: Use QDateTimeAxis for human-readable timestamps
    auto *xAxis = new QDateTimeAxis();
    xAxis->setFormat("dd-MM-yyyy HH:mm");
    xAxis->setTitleText("Time");
    xAxis->setTickCount(10);
    overviewChart->addAxis(xAxis, Qt::AlignBottom);

    // Y-Axis: Dynamically update only in the expected positive range
    auto *yAxis = new QValueAxis();
    yAxis->setTitleText("Level (ug/l)");
    yAxis->setLabelFormat("%.5f");
    yAxis->setTickCount(10);
    overviewChart->addAxis(yAxis, Qt::AlignLeft);

    // Iterate through all pollutant categories dynamically
    const std::map<QString, std::vector<std::string> > pollutantCategories = {
        {"PFAs", ComplianceChecker::getPFAs()},
        {"POPs", ComplianceChecker::getPOPs()},
        {"Metals", ComplianceChecker::getMetals()},
        {"VOCs", ComplianceChecker::getVOCs()}
    };

    for (const auto &[category, pollutants]: pollutantCategories) {
        for (const auto &compound: pollutants) {
            auto *series = new QLineSeries();
            bool seriesHasData = false;

            for (const auto &measurement: filteredData) {
                if (measurement.getCompoundName() == compound) {
                    series->append(measurement.getDatetime().toMSecsSinceEpoch(), measurement.getValue());
                    seriesHasData = true;
                }
            }

            if (seriesHasData) {
                series->setName(QString::fromStdString(compound));
                overviewChart->addSeries(series);
                series->attachAxis(xAxis);
                series->attachAxis(yAxis);
            } else {
                delete series;
            }
        }
    }

    // Add compliance threshold lines
    double baseThreshold = 0.1; // Default threshold value
    auto *greenThreshold = new QLineSeries();
    auto *orangeThreshold = new QLineSeries();
    auto *redThreshold = new QLineSeries();

    for (const auto &measurement: filteredData) {
        if (measurement.getDatetime().isValid()) {
            qint64 timestamp = measurement.getDatetime().toMSecsSinceEpoch();
            greenThreshold->append(timestamp, baseThreshold * 0.8);
            orangeThreshold->append(timestamp, baseThreshold);
            redThreshold->append(timestamp, baseThreshold * 1.2);
        }
    }

    overviewChart->setTitle(tr("Pollutant Overview for %1").arg(
        pollutant->currentText().isEmpty() ? "All" : pollutant->currentText()));
    // Example of adding compliance level lines to chart
    if (pollutant->currentText() != "All pollutants") {
        overviewChart->addSeries(greenThreshold);
        greenThreshold->attachAxis(xAxis);
        greenThreshold->attachAxis(yAxis);
        greenThreshold->setName("Green Threshold");

        overviewChart->addSeries(orangeThreshold);
        orangeThreshold->attachAxis(xAxis);
        orangeThreshold->attachAxis(yAxis);
        orangeThreshold->setName("Orange Threshold");

        overviewChart->addSeries(redThreshold);
        redThreshold->attachAxis(xAxis);
        redThreshold->attachAxis(yAxis);
        redThreshold->setName("Red Threshold");

        addComplianceLevelLine(overviewChart, xAxis, yAxis, baseThreshold * 0.8, "green", "Green Threshold");
        addComplianceLevelLine(overviewChart, xAxis, yAxis, baseThreshold, "orange", "Orange Threshold");
        addComplianceLevelLine(overviewChart, xAxis, yAxis, baseThreshold * 1.2, "red", "Red Threshold");
        green->show();
        orange->show();
        red->show();
    } else {
        green->hide();
        orange->hide();
        red->hide();
    }
    overviewChartView->setChart(overviewChart);
    overviewChartView->setMinimumSize(1000, 400);
}

void PollutantOverview::updateChart() {
    // If 'All pollutants' is selected, hide compliance labels and remove threshold lines
    if (pollutant->currentText() == "All pollutants") {
        green->hide();
        orange->hide();
        red->hide();
        overviewChartView->chart()->removeAllSeries(); // Clear any threshold lines
    }

    // Validate dropdown logic: 'All pollutants' and 'All locations' not allowed
    if (pollutant->currentText() == "All pollutants" && location->currentText() == "All locations") {
        QMessageBox::warning(this, tr("Invalid Selection"),
                             tr(
                                 "You cannot select 'All pollutants' and 'All locations' simultaneously. Resetting pollutant selection."));
        pollutant->setCurrentIndex(1); // Reset to a specific pollutant
        return;
    }
    const std::vector<Measurement> dataset = GlobalDataModel::instance().getDataset().sortByTimestamp();
    const QString selectedLocation = location->currentText();
    const QString selectedPollutant = pollutant->currentText();
    const QString selectedTimeRange = timeRange->currentText();
    qint64 lastTimestamp = 0;

    if (!dataset.empty()) {
        lastTimestamp = dataset.back().getDatetime().toMSecsSinceEpoch();
    }

    std::vector<Measurement> filteredData;
    qint64 filterStartTime = lastTimestamp;

    // Time range filter
    if (selectedTimeRange == "3 days") {
        filterStartTime -= 3ll * 24 * 60 * 60 * 1000;
    } else if (selectedTimeRange == "1 week") {
        filterStartTime -= 7ll * 24 * 60 * 60 * 1000;
    } else if (selectedTimeRange == "6 months") {
        filterStartTime -= 180ll * 24 * 60 * 60 * 1000;
    } else if (selectedTimeRange == "1 year") {
        filterStartTime -= 365ll * 24 * 60 * 60 * 1000;
    }

    for (const auto &measurement: dataset) {
        const qint64 timestamp = measurement.getDatetime().toMSecsSinceEpoch();
        if (timestamp < filterStartTime || timestamp > lastTimestamp) {
            continue;
        }

        // Location and pollutant filters
        bool matchLocation = (selectedLocation == "All locations" || measurement.getLabel() == selectedLocation.
                              toStdString());
        bool matchPollutant = (selectedPollutant == "All pollutants" || measurement.getCompoundName() ==
                               selectedPollutant.toStdString());

        if (matchLocation && matchPollutant) {
            filteredData.push_back(measurement);
        }
    }

    if (filteredData.empty()) {
        QMessageBox::warning(this, tr("No Data"), tr("No data available for the selected filters."));
        overviewChartView->chart()->removeAllSeries();
    } else {
        createChart(filteredData);
    }
}

void PollutantOverview::createButtons()
{
    // *UI Job* buttons for more info on pollutant categories

    // Heavy Metals Info Button
    heavyMetalsInfo = new QPushButton("Heavy Metals Info");
    heavyMetalsInfo->setMinimumHeight(75);
    connect(heavyMetalsInfo, &QPushButton::clicked, this, &PollutantOverview::heavyMetalsInfoMsgBox);

    // Organic Chemicals Info Button
    organicChemicalsInfo = new QPushButton("Organic Chemicals Info");
    organicChemicalsInfo->setMinimumHeight(75);
    connect(organicChemicalsInfo, &QPushButton::clicked, this, &PollutantOverview::organicChemicalsInfoMsgBox);

    // Inorganic Chemicals Info Button
    inorganicChemicalsInfo = new QPushButton("Inorganic Chemicals Info");
    inorganicChemicalsInfo->setMinimumHeight(75);
    connect(inorganicChemicalsInfo, &QPushButton::clicked, this, &PollutantOverview::inorganicChemicalsInfoMsgBox);

    // Nutrients Info Button
    nutrientsInfo = new QPushButton("Nutrients Info");
    nutrientsInfo->setMinimumHeight(75);
    connect(nutrientsInfo, &QPushButton::clicked, this, &PollutantOverview::nutrientsInfoMsgBox);

    // Volatile Organic Compounds (VOCs) Info Button
    volatileOrganicCompoundsInfo = new QPushButton("Volatile Organic Compounds Info");
    volatileOrganicCompoundsInfo->setMinimumHeight(75);
    connect(volatileOrganicCompoundsInfo, &QPushButton::clicked, this, &PollutantOverview::volatileOrganicCompoundsInfoMsgBox);

}

void PollutantOverview::createFilters() {
    // Location combo box setup
    location = new QComboBox();
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

    // Connect to chart update for the first selection made
    connect(location, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PollutantOverview::updateChart);

    connect(location, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        // Ensure validity of selections
        if (pollutant->currentText() == "All pollutants" && location->currentText() == "All locations") {
            QMessageBox::warning(this, tr("Invalid Selection"),
                                 tr(
                                     "You cannot select 'All pollutants' and 'All locations' simultaneously. Resetting location selection."));
            location->setCurrentIndex(1); // Reset to a specific location
        } else {
            updateChart();
        }
    });

    // Time range combo box setup
    QStringList timeRangeOptions;
    timeRangeOptions << "3 days" << "1 week" << "2 weeks"
            << "3 months" << "6 months" << "9 months"
            << "1 year";
    timeRange = new QComboBox();
    timeRange->addItems(timeRangeOptions);
    timeRange->setCurrentText("1 year");
    timeRangeLabel = new QLabel("&Time Range:");
    timeRangeLabel->setBuddy(timeRange);


    connect(timeRange, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &PollutantOverview::updateChart);

    // Fetch and set pollutants
    QStringList pollutantOptions{"All pollutants"};
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

    connect(pollutant, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PollutantOverview::updateChart);
}

void PollutantOverview::createComplianceLabels() {
    // Default compliance information
    double threshold = 0.1; // Default threshold for compliance levels
    if (pollutant->currentText() != "All pollutants") {
        threshold = ComplianceChecker().getComplianceThreshold(pollutant->currentText().toStdString());
    }

    // Update labels dynamically based on threshold
    red = new QLabel(QString("Red level: >%1").arg(threshold * 1.2)); // Example: 1.2x threshold
    red->setStyleSheet("background-color: red; color: white;");
    red->setToolTip("Exceeds compliance threshold by 20%");

    orange = new QLabel(QString("Orange level: %1-%2").arg(threshold * 0.8).arg(threshold));
    orange->setStyleSheet("background-color: orange; color: white;");
    orange->setToolTip("Within compliance threshold");

    green = new QLabel(QString("Green level: <%1").arg(threshold * 0.8));
    green->setStyleSheet("background-color: green; color: white;");
    green->setToolTip("Below compliance threshold by 20%");
}

void PollutantOverview::arrangeWidgets()
{
    // Title

    QHBoxLayout* header = new QHBoxLayout();
    header->addWidget(title);

    // Filters and Compliance Indicators

    QHBoxLayout* filters = new QHBoxLayout();
    filters->setSizeConstraint(QLayout::SetMinimumSize);
    filters->addWidget(locationLabel);
    filters->addWidget(location);
    filters->addSpacing(15);
    filters->addWidget(timeRangeLabel);
    filters->addWidget(timeRange);
    filters->addSpacing(15);
    filters->addWidget(pollutantLabel);
    filters->addWidget(pollutant);
    filters->addSpacing(15);
    filters->addWidget(pollutantSearchBar);


    QHBoxLayout* chartContext = new QHBoxLayout();
    chartContext->setSizeConstraint(QLayout::SetMinimumSize);
    chartContext->addLayout(filters);
    chartContext->addStretch();
    chartContext->addSpacing(20);
    chartContext->addWidget(green);
    chartContext->addWidget(orange);
    chartContext->addWidget(red);

    // Graph layout

    QVBoxLayout* chart = new QVBoxLayout();
    chart->setSizeConstraint(QLayout::SetMinimumSize);
    chart->addWidget(overviewChartView, 19);
    chart->addLayout(chartContext, 1);
    chart->addStretch();

    QVBoxLayout* info = new QVBoxLayout();
    info->addStretch();
    info->addWidget(heavyMetalsInfo);
    info->addSpacing(10);
    info->addWidget(organicChemicalsInfo);
    info->addSpacing(10);
    info->addWidget(inorganicChemicalsInfo);
    info->addSpacing(10);
    info->addWidget(nutrientsInfo);
    info->addSpacing(10);
    info->addWidget(volatileOrganicCompoundsInfo);
    info->addSpacing(10);
    info->addStretch();

    // Main body layout

    QHBoxLayout* body = new QHBoxLayout();
    body->setSizeConstraint(QLayout::SetMinimumSize);
    body->addLayout(chart, 4);
    body->addLayout(info, 1);
    body->addStretch();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->addLayout(header);
    layout->addLayout(body);

    setLayout(layout);
}

void PollutantOverview::retranslateUI() {
    title->setText(tr("PO_TITLE"));

    locationLabel->setText(tr("PO_LOCATION_LABEL"));
    timeRangeLabel->setText(tr("PO_TIME_RANGE_LABEL"));
    pollutantLabel->setText(tr("PO_POLLUTANT_LABEL"));

    heavyMetalsInfo->setText(tr("PO_HEAVY_METALS_BUTTON"));
    organicChemicalsInfo->setText(tr("PO_ORGANIC_CHEMICALS_BUTTON"));
    inorganicChemicalsInfo->setText(tr("PO_INORGANIC_CHEMICALS_BUTTON"));
    nutrientsInfo->setText(tr("PO_NUTRIENTS_BUTTON"));
    volatileOrganicCompoundsInfo->setText(tr("PO_VOLATILE_ORGANIC_COMPOUNDS_BUTTON"));

    red->setText(tr("PO_COMPLIANCE_RED"));
    red->setToolTip(tr("PO_COMPLIANCE_RED_TOOLTIP"));
    orange->setText(tr("PO_COMPLIANCE_ORANGE"));
    orange->setToolTip(tr("PO_COMPLIANCE_ORANGE_TOOLTIP"));
    green->setText(tr("PO_COMPLIANCE_GREEN"));
    green->setToolTip(tr("PO_COMPLIANCE_GREEN_TOOLTIP"));

}

void PollutantOverview::createSearchBar() {
    pollutantSearchBar = new QLineEdit(this);
    pollutantSearchBar->setPlaceholderText("Search pollutant...");
    connect(pollutantSearchBar, &QLineEdit::returnPressed, this, [this]() {
        QString searchText = pollutantSearchBar->text();
        pollutant->setCurrentText(searchText); // Dynamically update pollutant filter
        updateChart();
    });
}

void PollutantOverview::addComplianceLevelLine(QChart *chart, QAbstractAxis *xAxis, QAbstractAxis *yAxis,
                                               double levelValue, const QString &color, const QString &label) {
    QLineSeries *complianceLine = new QLineSeries();
    complianceLine->setColor(QColor(color));
    complianceLine->setName(label);

    // Add horizontal line using x-axis range
    if (xAxis && xAxis->type() == QAbstractAxis::AxisTypeDateTime) {
        const QDateTimeAxis *dateTimeAxis = qobject_cast<QDateTimeAxis *>(xAxis);
        if (dateTimeAxis) {
            qint64 minX = dateTimeAxis->min().toMSecsSinceEpoch();
            qint64 maxX = dateTimeAxis->max().toMSecsSinceEpoch();
            complianceLine->append(minX, levelValue);
            complianceLine->append(maxX, levelValue);
        }
    }

    chart->addSeries(complianceLine);
    complianceLine->attachAxis(xAxis);
    complianceLine->attachAxis(yAxis);
}

// *UI Job* msg boxes for pollutant categories
void PollutantOverview::heavyMetalsInfoMsgBox()
{
    QMessageBox::information(this, tr("PO_HEAVY_METALS_TITLE"),
    tr("PO_HEAVY_METALS_DESC_1") + "\n" +
    tr("PO_HEAVY_METALS_DESC_2") + "\n" +
    tr("PO_HEAVY_METALS_DESC_3") + "\n\n" +
    tr("PO_HEAVY_METALS_1") + "\n" +
    tr("PO_HEAVY_METALS_2") + "\n" +
    tr("PO_HEAVY_METALS_3") + "\n" +
    tr("PO_HEAVY_METALS_4") + "\n" +
    tr("PO_HEAVY_METALS_5") + "\n" +
    tr("PO_HEAVY_METALS_6") + "\n" +
    tr("PO_HEAVY_METALS_7"));
}

void PollutantOverview::organicChemicalsInfoMsgBox()
{
    QMessageBox::information(this, tr("PO_ORGANIC_CHEMICALS_TITLE"),
    tr("PO_ORGANIC_CHEMICALS_DESC_1") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_DESC_2") + "\n\n" +
    tr("PO_ORGANIC_CHEMICALS_1") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_2") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_3") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_4") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_5") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_6") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_7") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_8") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_9") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_10") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_11") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_12") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_13") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_14") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_15") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_16") + "\n" +
    tr("PO_ORGANIC_CHEMICALS_17"));
}

void PollutantOverview::inorganicChemicalsInfoMsgBox()
{
    QMessageBox::information(this, tr("PO_INORGANIC_CHEMICALS_TITLE"),
    tr("PO_INORGANIC_CHEMICALS_DESC_1") + "\n" +
    tr("PO_INORGANIC_CHEMICALS_DESC_2") + "\n\n" +
    tr("PO_INORGANIC_CHEMICALS_1") + "\n" +
    tr("PO_INORGANIC_CHEMICALS_2") + "\n" +
    tr("PO_INORGANIC_CHEMICALS_3") + "\n" +
    tr("PO_INORGANIC_CHEMICALS_4") + "\n" +
    tr("PO_INORGANIC_CHEMICALS_5"));
}

void PollutantOverview::nutrientsInfoMsgBox()
{
    QMessageBox::information(this, tr("PO_NUTRIENTS_TITLE"),
    tr("PO_NUTRIENTS_DESC_1") + "\n" +
    tr("PO_NUTRIENTS_DESC_2") + "\n\n" +
    tr("PO_NUTRIENTS_1") + "\n" +
    tr("PO_NUTRIENTS_2"));
}

void PollutantOverview::volatileOrganicCompoundsInfoMsgBox()
{
    QMessageBox::information(this, tr("PO_VOC_TITLE"),
    tr("PO_VOC_DESC_1") + "\n" +
    tr("PO_VOC_DESC_2") + "\n" +
    tr("PO_VOC_DESC_3") + "\n\n" +
    tr("PO_VOC_1") + "\n" +
    tr("PO_VOC_2") + "\n" +
    tr("PO_VOC_3") + "\n" +
    tr("PO_VOC_4") + "\n" +
    tr("PO_VOC_5") + "\n" +
    tr("PO_VOC_6") + "\n" +
    tr("PO_VOC_7") + "\n" +
    tr("PO_VOC_8") + "\n" +
    tr("PO_VOC_9") + "\n" +
    tr("PO_VOC_10"));
}