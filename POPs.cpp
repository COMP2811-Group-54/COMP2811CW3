#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "POPs.hpp"

#include <iostream>


PersistentOrganicPollutants::PersistentOrganicPollutants(QWidget *parent) : QWidget(parent)
{
    createTitle();
    createChart();
    createButtons();
    createBoxes();
    createFilters();
    createComplianceLabels();
    arrangeWidgets();

    setWindowTitle(tr("POPs_WINDOW_TITLE")); // Identifier for the window title
}

void PersistentOrganicPollutants::createTitle()
{
    title = new QLabel(tr("POPs_TITLE")); // Identifier for the title
    QFont titleFont("Arial", 20, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
}

void PersistentOrganicPollutants::createChart()
{
    auto popChart = new QChart();

    // Graph data initialization
    auto series = new QLineSeries();
    series->append(0, 0);
    series->append(10, 10);
    popChart->addSeries(series);

    popChart->setTitle(tr("POPs_CHART_TITLE")); // Identifier for the chart title

    // Axis creation
    auto xAxis = new QValueAxis();
    xAxis->setTitleText(tr("POPs_X_AXIS_TITLE")); // Identifier for the X-axis
    xAxis->setRange(0, 10);
    popChart->addAxis(xAxis, Qt::AlignBottom);
    series->attachAxis(xAxis);

    auto yAxis = new QValueAxis();
    yAxis->setTitleText(tr("POPs_Y_AXIS_TITLE")); // Identifier for the Y-axis
    yAxis->setRange(0, 10);
    popChart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);

    // Chart view creation
    popChartView = new QChartView(popChart);
    popChartView->setMinimumSize(1000, 400);
}

void PersistentOrganicPollutants::createButtons()
{
    moreInfo = new QPushButton(tr("POPs_MORE_INFO")); // Identifier for "More Info"
    connect(moreInfo, &QPushButton::clicked, this, &PersistentOrganicPollutants::moreInfoMsgBox);

    viewList = new QPushButton(tr("POPs_VIEW_LIST")); // Identifier for "View List"
    connect(viewList, &QPushButton::clicked, this, &PersistentOrganicPollutants::viewListMsgBox);
}

void PersistentOrganicPollutants::createBoxes()
{
    QFont infoBoxFont("Arial", 8);

    pcbs = new QLabel(tr("POPs_PCBS_INFO")); // Identifier for PCBs information
    pcbs->setFont(infoBoxFont);
    pcbs->setWordWrap(true);
    pcbs->setAlignment(Qt::AlignCenter);

    otherPops = new QLabel(tr("POPs_OTHER_INFO")); // Identifier for other POPs information
    otherPops->setFont(infoBoxFont);
    otherPops->setWordWrap(true);
    otherPops->setAlignment(Qt::AlignCenter);
}

void PersistentOrganicPollutants::createFilters()
{
    // Location filter
    QStringList locationOptions;
    locationOptions << tr("POPs_ALL_LOCATIONS") << "1" << "2" << "3" << "4"; // Identifier for "All locations"
    location = new QComboBox();
    location->addItems(locationOptions);
    locationLabel = new QLabel(tr("POPs_LOCATION_LABEL")); // Identifier for "Location"
    locationLabel->setBuddy(location);

    // Time range filter
    QStringList timeRangeOptions;
    timeRangeOptions << tr("POPs_ALL_TIME") << tr("POPs_TIME_DAY") << tr("POPs_TIME_WEEK")
                     << tr("POPs_TIME_MONTH") << tr("POPs_TIME_YEAR"); // Identifiers for time range options
    timeRange = new QComboBox();
    timeRange->addItems(timeRangeOptions);
    timeRangeLabel = new QLabel(tr("POPs_TIME_RANGE_LABEL")); // Identifier for "Time Range"
    timeRangeLabel->setBuddy(timeRange);

    // Pollutant filter
    QStringList pollutantOptions;
    pollutantOptions << tr("POPs_ALL_POLLUTANTS") << tr("POPs_POLLUTANT_CHLORINE")
                     << tr("POPs_POLLUTANT_ETHANOL"); // Identifiers for pollutants
    pollutant = new QComboBox();
    pollutant->addItems(pollutantOptions);
    pollutantLabel = new QLabel(tr("POPs_POLLUTANT_LABEL")); // Identifier for "Pollutant"
    pollutantLabel->setBuddy(pollutant);
}

void PersistentOrganicPollutants::createComplianceLabels()
{
    red = new QLabel(tr("POPs_COMPLIANCE_RED")); // Identifier for red compliance level
    red->setStyleSheet("background-color: red; color: white;");
    red->setToolTip(tr("POPs_COMPLIANCE_RED_TOOLTIP")); // Identifier for red tooltip

    orange = new QLabel(tr("POPs_COMPLIANCE_ORANGE")); // Identifier for orange compliance level
    orange->setStyleSheet("background-color: orange; color: white;");
    orange->setToolTip(tr("POPs_COMPLIANCE_ORANGE_TOOLTIP")); // Identifier for orange tooltip

    green = new QLabel(tr("POPs_COMPLIANCE_GREEN")); // Identifier for green compliance level
    green->setStyleSheet("background-color: green; color: white;");
    green->setToolTip(tr("POPs_COMPLIANCE_GREEN_TOOLTIP")); // Identifier for green tooltip
}


void PersistentOrganicPollutants::arrangeWidgets()
{
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


    QHBoxLayout* chartContext = new QHBoxLayout();
    chartContext->setSizeConstraint(QLayout::SetMinimumSize);
    chartContext->addLayout(filters);
    chartContext->addStretch();
    chartContext->addSpacing(20);
    chartContext->addWidget(green);
    chartContext->addSpacing(20);
    chartContext->addStretch();
    chartContext->addWidget(orange);
    chartContext->addSpacing(20);
    chartContext->addStretch();
    chartContext->addWidget(red);
    chartContext->addStretch();

    // Graph layout

    QVBoxLayout* chart = new QVBoxLayout();
    chart->setSizeConstraint(QLayout::SetMinimumSize);
    chart->addWidget(popChartView, 19);
    chart->addLayout(chartContext, 1);
    chart->addStretch();

    // Info box layout

    auto moreInfoFrame = new QFrame();
    moreInfoFrame->setFrameShape(QFrame::Box);
    moreInfoFrame->setLineWidth(1);
    moreInfoFrame->setFixedSize(200, 200);

    QVBoxLayout* moreInfoLayout = new QVBoxLayout(moreInfoFrame);
    moreInfoLayout->addWidget(pcbs);
    moreInfoLayout->addWidget(moreInfo);

    auto viewListFrame = new QFrame();
    viewListFrame->setFrameShape(QFrame::Box);
    viewListFrame->setLineWidth(1);
    viewListFrame->setFixedSize(200, 200);

    QVBoxLayout* viewListLayout = new QVBoxLayout(viewListFrame);
    viewListLayout->addWidget(otherPops);
    viewListLayout->addWidget(viewList);

    QVBoxLayout* info = new QVBoxLayout();
    info->addStretch();
    info->addWidget(moreInfoFrame);
    info->addSpacing(50);
    info->addWidget(viewListFrame);
    info->addSpacing(50);
    info->addStretch();

    // Main body layout

    QHBoxLayout* body = new QHBoxLayout();
    body->setSizeConstraint(QLayout::SetMinimumSize);
    body->addLayout(chart, 4);
    body->addLayout(info, 1);
    body->addStretch();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->addWidget(title);
    layout->addLayout(body);

    setLayout(layout);
}

void PersistentOrganicPollutants::moreInfoMsgBox()
{
  QMessageBox::information(this, "PCB Info", "more info about PCBs");
}

void PersistentOrganicPollutants::viewListMsgBox()
{
  QMessageBox::information(this, "List of Persistent Organic Pollutants", "List of POPs");
}

void PersistentOrganicPollutants::retranslateUI()
{
    std::cout << "translating" << std::endl;
    // Update the title
    title->setText(tr("POPs_TITLE"));

    // Update the chart title
    popChartView->chart()->setTitle(tr("POPs_CHART_TITLE"));

    // Update axis labels
    auto xAxis = qobject_cast<QValueAxis *>(popChartView->chart()->axes(Qt::Horizontal).first());
    if (xAxis) {
        xAxis->setTitleText(tr("POPs_X_AXIS_TITLE"));
    }

    auto yAxis = qobject_cast<QValueAxis *>(popChartView->chart()->axes(Qt::Vertical).first());
    if (yAxis) {
        yAxis->setTitleText(tr("POPs_Y_AXIS_TITLE"));
    }

    // Update button text
    moreInfo->setText(tr("POPs_MORE_INFO"));
    viewList->setText(tr("POPs_VIEW_LIST"));

    // Update labels in info boxes
    pcbs->setText(tr("POPs_PCBS_INFO"));
    otherPops->setText(tr("POPs_OTHER_INFO"));

    // Update filter labels and options
    locationLabel->setText(tr("POPs_LOCATION_LABEL"));
    timeRangeLabel->setText(tr("POPs_TIME_RANGE_LABEL"));
    pollutantLabel->setText(tr("POPs_POLLUTANT_LABEL"));

    location->setItemText(0, tr("POPs_ALL_LOCATIONS"));
    timeRange->setItemText(0, tr("POPs_ALL_TIME"));
    timeRange->setItemText(1, tr("POPs_TIME_DAY"));
    timeRange->setItemText(2, tr("POPs_TIME_WEEK"));
    timeRange->setItemText(3, tr("POPs_TIME_MONTH"));
    timeRange->setItemText(4, tr("POPs_TIME_YEAR"));

    pollutant->setItemText(0, tr("POPs_ALL_POLLUTANTS"));
    pollutant->setItemText(1, tr("POPs_POLLUTANT_CHLORINE"));
    pollutant->setItemText(2, tr("POPs_POLLUTANT_ETHANOL"));

    // Update compliance labels
    red->setText(tr("POPs_COMPLIANCE_RED"));
    red->setToolTip(tr("POPs_COMPLIANCE_RED_TOOLTIP"));

    orange->setText(tr("POPs_COMPLIANCE_ORANGE"));
    orange->setToolTip(tr("POPs_COMPLIANCE_ORANGE_TOOLTIP"));

    green->setText(tr("POPs_COMPLIANCE_GREEN"));
    green->setToolTip(tr("POPs_COMPLIANCE_GREEN_TOOLTIP"));
}