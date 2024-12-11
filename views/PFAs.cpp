#include <QtWidgets>
#include <QtCharts>
#include "PFAs.hpp"

#include <iostream>

#include "../utils/Compliance.hpp"
#include "../utils/GlobalDataModel.hpp"

PFApage::PFApage(QWidget *parent) : QWidget(parent) {
    createTitle();

    // Initialize pfaChartView with a placeholder chart
    QChart *placeholderChart = new QChart();
    placeholderChart->setTitle("Awaiting Data...");
    pfaChartView = new QChartView(placeholderChart);
    pfaChartView->setMinimumSize(600, 400);

    createButtons();
    createBoxes();
    createFilters();
    createComplianceLabels();
    arrangeWidgets();
}

void PFApage::createTitle() {
    title = new QLabel("Poly-fluorinated Compounds");
    QFont titleFont("Arial", 20, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
}

void PFApage::createButtons() {
    moreInfo = new QPushButton("More Info");
    connect(moreInfo, &QPushButton::clicked, this, &PFApage::moreInfoMsgBox);

    viewList = new QPushButton("View List");
    connect(viewList, &QPushButton::clicked, this, &PFApage::viewListMsgBox);
}

void PFApage::createBoxes() {
    QFont infoBoxFont("Arial", 8);

    pfas = new QLabel("<h2>PFAs<h2>"
        "<p>Per- and polyfluoroalkyl substances (PFAS) are a large, complex group of synthetic chemicals<p>");
    pfas->setFont(infoBoxFont);
    pfas->setWordWrap(true);
    pfas->setAlignment(Qt::AlignCenter);

    otherPfas = new QLabel("<h2>Other PFAs<h2>"
        "Examples include PFOAs and PFOS. These substances have various origins and effects<p>");
    otherPfas->setFont(infoBoxFont);
    otherPfas->setWordWrap(true);
    otherPfas->setAlignment(Qt::AlignCenter);
}

void PFApage::createFilters() {
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
    connect(location, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PFApage::updateChart);

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


    connect(timeRange, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PFApage::updateChart);

    // Pollutant combo box setup
    QStringList pollutantOptions;
    pollutantOptions << "All pollutants";
    std::vector<std::string> pfasChemicals = ComplianceChecker::getPFAs();
    for (const std::string &chemical: pfasChemicals) {
        pollutantOptions << QString::fromStdString(chemical);
    }
    pollutant = new QComboBox();
    pollutant->addItems(pollutantOptions);
    pollutantLabel = new QLabel("&Pollutant:");
    pollutantLabel->setBuddy(pollutant);

    connect(pollutant, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PFApage::updateChart);
}

void PFApage::createComplianceLabels() {
    red = new QLabel("Red level: >10");
    red->setStyleSheet("background-color: red; color: white;");
    red->setToolTip("Info about red compliance level");

    orange = new QLabel("Orange level: 5-10");
    orange->setStyleSheet("background-color: orange; color: white;");
    orange->setToolTip("Info about orange compliance level");

    green = new QLabel("Green Level: <5");
    green->setStyleSheet("background-color: green; color: white;");
    green->setToolTip("Info about green compliance level");
}

void PFApage::arrangeWidgets() {
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
    chart->addWidget(pfaChartView, 19);
    chart->addLayout(chartContext, 1);
    chart->addStretch();

    auto moreInfoFrame = new QFrame();
    moreInfoFrame->setFrameShape(QFrame::Box);
    moreInfoFrame->setLineWidth(1);
    moreInfoFrame->setFixedSize(200, 200);

    QVBoxLayout *moreInfoLayout = new QVBoxLayout(moreInfoFrame);
    moreInfoLayout->addWidget(pfas);
    moreInfoLayout->addWidget(moreInfo);

    auto viewListFrame = new QFrame();
    viewListFrame->setFrameShape(QFrame::Box);
    viewListFrame->setLineWidth(1);
    viewListFrame->setFixedSize(200, 200);

    QVBoxLayout *viewListLayout = new QVBoxLayout(viewListFrame);
    viewListLayout->addWidget(otherPfas);
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

void PFApage::updateChart() {
    const std::vector<Measurement> dataset = GlobalDataModel::instance().getDataset().sortByTimestamp();
    const QString selectedLocation = location->currentText();
    const QString selectedPollutant = pollutant->currentText();
    const QString selectedTimeRange = timeRange->currentText();
    qint64 lastTimestamp = 0;

    if (!dataset.empty()) {
        lastTimestamp = dataset.back().getDatetime().toMSecsSinceEpoch(); // Get the last data timestamp
    }

    std::cout << "Last timestamp: " << lastTimestamp << std::endl;

    std::vector<Measurement> filteredData;
    qint64 filterStartTime = lastTimestamp; // Initialize start time for filtering
    std::cout << "initialised start time: " << filterStartTime << std::endl;

    if (selectedTimeRange == "3 days") {
        filterStartTime -= 3ll * 24 * 60 * 60 * 1000; // 3 days in milliseconds
    } else if (selectedTimeRange == "1 week") {
        filterStartTime -= 7ll * 24 * 60 * 60 * 1000; // 1 week
    } else if (selectedTimeRange == "2 weeks") {
        filterStartTime -= 14ll * 24 * 60 * 60 * 1000; // 2 weeks
    } else if (selectedTimeRange == "3 months") {
        cout << "adjustin 3 months" << std::endl;
        filterStartTime -= 90ll * 24 * 60 * 60 * 1000; // Approx. 3 months
    } else if (selectedTimeRange == "6 months") {
        cout << "adjustin 6 months" << std::endl;
        filterStartTime -= 180ll * 24 * 60 * 60 * 1000; // Approx. 6 months
    } else if (selectedTimeRange == "9 months") {
        cout << "adjustin 9 months" << std::endl;
        filterStartTime -= 270ll * 24 * 60 * 60 * 1000; // Approx. 9 months
    } else if (selectedTimeRange == "1 year") {
        cout << "adjustin 1 yeaer" << std::endl;
        filterStartTime -= 365ll * 24 * 60 * 60 * 1000; // Approx. 1 year
    }
    std::cout << "Filtered timestamp: " << filterStartTime << std::endl;


    for (const auto &measurement: dataset) {
        const qint64 timestamp = measurement.getDatetime().toMSecsSinceEpoch();
        std::cout << "Timestamp: " << timestamp << std::endl;
        if (timestamp < filterStartTime || timestamp > lastTimestamp) {
            continue; // Skip if not within the selected time range
        }
        std::cout << "Filtered timestamp: " << timestamp << std::endl;

        bool matchLocation = selectedLocation == "All locations" || measurement.getLabel() == selectedLocation.
                             toStdString();
        bool matchPollutant = selectedPollutant == "All pollutants" || measurement.getCompoundName() ==
                              selectedPollutant.toStdString();

        if (matchLocation && matchPollutant) {
            filteredData.push_back(measurement);
        }
    }

    if (filteredData.empty()) {
        QMessageBox::warning(this, "No Data", "No data available for selected location and time range.");
        pfaChartView->chart()->removeAllSeries(); // Clear the chart if no data is found
    } else {
        createChart(filteredData);
    }
}

void PFApage::createChart(const std::vector<Measurement> &filteredData) {
    auto pfaChart = new QChart();
    pfaChart->removeAllSeries();
    pfaChart->setAnimationOptions(QChart::SeriesAnimations);

    // X-Axis: Use QDateTimeAxis for human-readable timestamps
    auto *xAxis = new QDateTimeAxis();
    xAxis->setFormat("dd-MM-yyyy HH:mm"); // Human-readable timestamps
    xAxis->setTitleText("Time");
    xAxis->setTickCount(10); // Adjust the number of ticks for clarity
    pfaChart->addAxis(xAxis, Qt::AlignBottom);

    // Y-Axis: Dynamically update only in the expected positive range
    auto *yAxis = new QValueAxis();
    yAxis->setTitleText("Level (ug/l)");
    yAxis->setLabelFormat("%.5f");
    yAxis->setTickCount(10); // Adjust tick counts as necessary
    pfaChart->addAxis(yAxis, Qt::AlignLeft);

    // Add series for each compound
    for (const auto &compound: ComplianceChecker::getPFAs()) {
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
            pfaChart->addSeries(series);
            series->attachAxis(xAxis);
            series->attachAxis(yAxis);
        } else {
            delete series;
        }
    }

    // Add compliance level lines
    addComplianceLevelLine(pfaChart, xAxis, yAxis, 0.01 * 0.8, Qt::green, "Green (0.8x)");
    addComplianceLevelLine(pfaChart, xAxis, yAxis, 0.01 * 1.0, QColor(255, 165, 0), "Orange (1.0x)");
    addComplianceLevelLine(pfaChart, xAxis, yAxis, 0.01 * 1.2, Qt::red, "Red (1.2x)");

    pfaChart->setTitle("Filtered PFAs Chart");
    pfaChartView->setChart(pfaChart);
}

void PFApage::retranslateUI() {
    title->setText(tr("PFA_TITLE"));
    pfas->setText(tr("PFA_PFAS_DESCRIPTION"));
    otherPfas->setText(tr("PFA_OTHER_PFAS_DESCRIPTION"));
    moreInfo->setText(tr("PFA_MORE_INFO"));
    viewList->setText(tr("PFA_VIEW_LIST"));

    locationLabel->setText(tr("PFA_LOCATION"));
    timeRangeLabel->setText(tr("PFA_TIME_RANGE"));
    pollutantLabel->setText(tr("PFA_POLLUTANT"));

    red->setText(tr("PFA_COMPLIANCE_RED"));
    red->setToolTip(tr("PFA_RED_TOOL_TIP"));
    orange->setText(tr("PFA_COMPLIANCE_ORANGE"));
    orange->setToolTip(tr("PFA_ORANGE_TOOL_TIP"));
    green->setText(tr("PFA_COMPLIANCE_GREEN"));
    green->setToolTip(tr("PFA_GREEN_TOOL_TIP"));
}

void PFApage::moreInfoMsgBox() {
    QMessageBox::information(this, tr("PFA_MORE_INFO_TITLE"), 
    tr("PFA_MORE_INFO_BODY"));
}

void PFApage::addComplianceLevelLine(QChart *chart, QDateTimeAxis *xAxis, QValueAxis *yAxis,
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

void PFApage::moreInfoMsgBox() {
    QMessageBox::information(this, "PFA Info", "More info about PFAs.");
}

void PFApage::viewListMsgBox() {
    QMessageBox::information(this, tr("PFA_VIEW_LIST_TITLE"), 
    tr("PFA_VIEW_LIST_BODY_1")+
    "\n"+
    tr("PFA_VIEW_LIST_BODY_2")+
    "\n"+
    tr("PFA_VIEW_LIST_BODY_3")+
    "\n"+
    tr("PFA_VIEW_LIST_BODY_4")+
    "\n"+
    tr("PFA_VIEW_LIST_BODY_5")+
    "\n"+
    tr("PFA_VIEW_LIST_BODY_6")+
    "\n"+
    tr("PFA_VIEW_LIST_BODY_7")+
    "\n");
}