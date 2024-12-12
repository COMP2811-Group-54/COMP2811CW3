#include <QtWidgets>
#include <QtCharts>
#include "POPs.hpp"

#include <iostream>

#include "../utils/Compliance.hpp"
#include "../utils/GlobalDataModel.hpp"

PersistentOrganicPollutants::PersistentOrganicPollutants(QWidget *parent) : QWidget(parent) {
    createTitle();

    // Initialize popChartView with a placeholder chart
    QChart *placeholderChart = new QChart();
    placeholderChart->setTitle("Awaiting data..."); // Awaiting data placeholder
    popChartView = new QChartView(placeholderChart);
    popChartView->setMinimumSize(1000, 400);

    createBoxes();
    createButtons();
    createFilters();
    createComplianceLabels();
    arrangeWidgets();
}

void PersistentOrganicPollutants::createTitle() {
    title = new QLabel("Persistent Organic Pollutants");
    QFont titleFont("Arial", 20, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
}

void PersistentOrganicPollutants::createButtons() {
    moreInfo = new QPushButton("More Info");
    connect(moreInfo, &QPushButton::clicked, this, &PersistentOrganicPollutants::moreInfoMsgBox);

    viewList = new QPushButton("View List");
    connect(viewList, &QPushButton::clicked, this, &PersistentOrganicPollutants::viewListMsgBox);
}

void PersistentOrganicPollutants::createBoxes() {
    QFont infoBoxFont("Arial", 12);

    pops = new QLabel("PCBs (Polychlorinated Biphenyls)");
    pops->setFont(infoBoxFont);
    pops->setWordWrap(true);
    pops->setAlignment(Qt::AlignCenter);

    otherPops = new QLabel("POP List");
    otherPops->setFont(infoBoxFont);
    otherPops->setWordWrap(true);
    otherPops->setAlignment(Qt::AlignCenter);
}

void PersistentOrganicPollutants::createFilters() {
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
    connect(location, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &PersistentOrganicPollutants::updateChart);

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
            &PersistentOrganicPollutants::updateChart);

    // Pollutant combo box setup
    QStringList pollutantOptions;
    pollutantOptions << "All pollutants";
    std::vector<std::string> pfasChemicals = ComplianceChecker::getPOPs();
    for (const std::string &chemical: pfasChemicals) {
        pollutantOptions << QString::fromStdString(chemical);
    }
    pollutant = new QComboBox();
    pollutant->addItems(pollutantOptions);
    pollutantLabel = new QLabel("&Pollutant:");
    pollutantLabel->setBuddy(pollutant);

    connect(pollutant, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &PersistentOrganicPollutants::updateChart);
}

void PersistentOrganicPollutants::createComplianceLabels() {
    red = new QLabel("Red level:");
    red->setStyleSheet("background-color: red; color: white;");
    red->setToolTip("Over the safety threshold");

    orange = new QLabel("Orange level:");
    orange->setStyleSheet("background-color: orange; color: white;");
    orange->setToolTip("Between 0.8 to 1 of the safety threshold");

    green = new QLabel("Green Level:");
    green->setStyleSheet("background-color: green; color: white;");
    green->setToolTip("Below 0.8 of the safety threshold");
}

void PersistentOrganicPollutants::arrangeWidgets() {
    QHBoxLayout *filters = new QHBoxLayout();
    filters->setSizeConstraint(QLayout::SetMinimumSize);
    filters->addWidget(locationLabel);
    filters->addWidget(location);
    filters->addSpacing(15);
    filters->addWidget(timeRangeLabel);
    filters->addWidget(timeRange);
    filters->addSpacing(15);
    filters->addWidget(pollutantLabel);
    filters->addWidget(pollutant);

    QHBoxLayout *chartContext = new QHBoxLayout();
    chartContext->setSizeConstraint(QLayout::SetMinimumSize);
    chartContext->addLayout(filters);
    chartContext->addStretch();
    chartContext->addSpacing(20);
    chartContext->addWidget(green);
    chartContext->addWidget(orange);
    chartContext->addWidget(red);

    QVBoxLayout *chart = new QVBoxLayout();
    chart->setSizeConstraint(QLayout::SetMinimumSize);
    chart->addWidget(popChartView, 19);
    chart->addLayout(chartContext, 1);
    chart->addStretch();

    auto moreInfoFrame = new QFrame();
    moreInfoFrame->setFrameShape(QFrame::Box);
    moreInfoFrame->setLineWidth(1);
    moreInfoFrame->setMinimumSize(200, 250);

    QVBoxLayout *moreInfoLayout = new QVBoxLayout(moreInfoFrame);
    moreInfoLayout->addWidget(pops);
    moreInfoLayout->addWidget(moreInfo);

    auto viewListFrame = new QFrame();
    viewListFrame->setFrameShape(QFrame::Box);
    viewListFrame->setLineWidth(1);
    viewListFrame->setFixedSize(200, 250);

    QVBoxLayout *viewListLayout = new QVBoxLayout(viewListFrame);
    viewListLayout->addWidget(otherPops);
    viewListLayout->addWidget(viewList);

    QVBoxLayout *info = new QVBoxLayout();
    info->addStretch();
    info->addWidget(moreInfoFrame);
    info->addSpacing(50);
    info->addWidget(viewListFrame);
    info->addSpacing(50);
    info->addStretch();

    QHBoxLayout *body = new QHBoxLayout();
    body->setSizeConstraint(QLayout::SetMinimumSize);
    body->addLayout(chart, 4);
    body->addLayout(info, 1);
    body->addStretch();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->addWidget(title);
    layout->addLayout(body);

    setLayout(layout);
}

void PersistentOrganicPollutants::updateChart() {
    const std::vector<Measurement> dataset = GlobalDataModel::instance().getDataset().sortByTimestamp();
    const QString selectedLocation = location->currentText();
    const QString selectedPollutant = pollutant->currentText();
    const QString selectedTimeRange = timeRange->currentText();
    qint64 lastTimestamp = 0;

    if (!dataset.empty()) {
        lastTimestamp = dataset.back().getDatetime().toMSecsSinceEpoch(); // Get the last data timestamp
    }


    std::vector<Measurement> filteredData;
    qint64 filterStartTime = lastTimestamp; // Initialize start time for filtering

    if (selectedTimeRange == "3 days") {
        filterStartTime -= 3ll * 24 * 60 * 60 * 1000; // 3 days in milliseconds
    } else if (selectedTimeRange == "1 week") {
        filterStartTime -= 7ll * 24 * 60 * 60 * 1000; // 1 week
    } else if (selectedTimeRange == "2 weeks") {
        filterStartTime -= 14ll * 24 * 60 * 60 * 1000; // 2 weeks
    } else if (selectedTimeRange == "3 months") {
        filterStartTime -= 90ll * 24 * 60 * 60 * 1000; // Approx. 3 months
    } else if (selectedTimeRange == "6 months") {
        filterStartTime -= 180ll * 24 * 60 * 60 * 1000; // Approx. 6 months
    } else if (selectedTimeRange == "9 months") {
        filterStartTime -= 270ll * 24 * 60 * 60 * 1000; // Approx. 9 months
    } else if (selectedTimeRange == "1 year") {
        filterStartTime -= 365ll * 24 * 60 * 60 * 1000; // Approx. 1 year
    }

    for (const auto &measurement: dataset) {
        const qint64 timestamp = measurement.getDatetime().toMSecsSinceEpoch();
        if (timestamp < filterStartTime || timestamp > lastTimestamp) {
            continue; // Skip if not within the selected time range
        }


        bool matchLocation = selectedLocation == "All locations" || measurement.getLabel() == selectedLocation.
                             toStdString();
        bool matchPollutant = selectedPollutant == "All pollutants" || measurement.getCompoundName() ==
                              selectedPollutant.toStdString();

        if (matchLocation && matchPollutant) {
            // Apply additional time filtering if necessary
            filteredData.push_back(measurement);
        }
    }

    if (filteredData.empty()) {
        QMessageBox::warning(this, "No Data", "No data available for selected location.");
        popChartView->chart()->removeAllSeries(); // Clear chart if no data is found
    } else {
        createChart(filteredData);
    }
}

void PersistentOrganicPollutants::createChart(const std::vector<Measurement> &filteredData) {
    auto popChart = new QChart();
    popChart->removeAllSeries();
    popChart->setAnimationOptions(QChart::SeriesAnimations);


    // X-Axis: Use QDateTimeAxis for human-readable timestamps
    auto *xAxis = new QDateTimeAxis();
    xAxis->setFormat("dd-MM-yyyy HH:mm"); // Human-readable timestamps
    xAxis->setTitleText("Time");
    xAxis->setTickCount(10); // Adjust the number of ticks for clarity
    popChart->addAxis(xAxis, Qt::AlignBottom);

    // Y-Axis: Dynamically update only in the expected positive range
    auto *yAxis = new QValueAxis();
    yAxis->setTitleText("Level (ug/l)");
    yAxis->setLabelFormat("%.5f");
    yAxis->setTickCount(10); // Adjust tick counts as necessary
    popChart->addAxis(yAxis, Qt::AlignLeft);

    for (const auto &compound: ComplianceChecker::getPOPs()) {
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
            popChart->addSeries(series);
            series->attachAxis(xAxis);
            series->attachAxis(yAxis);
        } else {
            delete series;
        }
    }

    // Add compliance level lines
    addComplianceLevelLine(popChart, xAxis, yAxis, 0.1 * 0.8, Qt::green, "Green (0.8x)");
    addComplianceLevelLine(popChart, xAxis, yAxis, 0.1 * 1.0, QColor(255, 165, 0), "Orange (1.0x)");
    addComplianceLevelLine(popChart, xAxis, yAxis, 0.1 * 1.2, Qt::red, "Red (1.2x)");

    popChart->setTitle(tr("Filtered POPs Chart"));
    popChartView->setChart(popChart);
}

void PersistentOrganicPollutants::addComplianceLevelLine(QChart *chart, QDateTimeAxis *xAxis, QValueAxis *yAxis,
                                                         double level, QColor color, const QString &label) {
    auto *lineSeries = new QLineSeries();

    // Use the min and max of the X-axis to draw the horizontal line
    double minX = xAxis->min().toMSecsSinceEpoch();
    double maxX = xAxis->max().toMSecsSinceEpoch();

    lineSeries->append(minX, level);
    lineSeries->append(maxX, level);

    // Customize the pen for the line
    QPen pen(color); // Set the line color
    pen.setStyle(Qt::DotLine); // Make the line dotted
    pen.setWidth(3); // Set the line thickness (e.g., 3 pixels)
    lineSeries->setPen(pen);

    lineSeries->setColor(color);
    lineSeries->setName(label);

    // Add the line to the chart and bind it to the axes
    chart->addSeries(lineSeries);
    lineSeries->attachAxis(xAxis);
    lineSeries->attachAxis(yAxis);
}

void PersistentOrganicPollutants::retranslateUI() {
    title->setText(tr("POPs_TITLE"));
    popChartView->chart()->setTitle(tr("POPs_AWAITING_DATA"));
    pops->setText(tr("POPs_PCBS_INFO"));
    otherPops->setText(tr("POPs_OTHER_INFO"));
    moreInfo->setText(tr("POPs_MORE_INFO"));
    viewList->setText(tr("POPs_VIEW_LIST"));

    locationLabel->setText(tr("POPs_LOCATION_LABEL"));
    timeRangeLabel->setText(tr("POPs_TIME_RANGE_LABEL"));
    pollutantLabel->setText(tr("POPs_POLLUTANT_LABEL"));

    red->setText(tr("POPs_COMPLIANCE_RED"));
    red->setToolTip(tr("POPs_COMPLIANCE_RED_TOOLTIP"));
    orange->setText(tr("POPs_COMPLIANCE_ORANGE"));
    orange->setToolTip(tr("POPs_COMPLIANCE_ORANGE_TOOLTIP"));
    green->setText(tr("POPs_COMPLIANCE_GREEN"));
    green->setToolTip(tr("POPs_COMPLIANCE_GREEN_TOOLTIP"));
}

void PersistentOrganicPollutants::moreInfoMsgBox() {
    QMessageBox::information(this, tr("POPs_INFO_TITLE"), 
    tr("POPs_INFO_BODY"));
}

void PersistentOrganicPollutants::viewListMsgBox() {
    QMessageBox::information(this, tr("POPs_LIST_TITLE"), 
    tr("POPs_BODY_1") +
    "\n" +
    tr("POPs_BODY_2") +
    "\n" +
    tr("POPs_BODY_3") +
    "\n" +
    tr("POPs_BODY_4") +
    "\n" +
    tr("POPs_BODY_5") +
    "\n" +
    tr("POPs_BODY_6") +
    "\n" +
    tr("POPs_BODY_7") +
    "\n" +
    tr("POPs_BODY_8") +
    "\n" +
    tr("POPs_BODY_9"));
}