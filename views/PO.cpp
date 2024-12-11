/*
To implement:



*/

#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "PO.hpp"

PollutantOverview::PollutantOverview(QWidget *parent): QWidget(parent)
{
    createTitle();
    createChart();
    createButtons();
    createFilters();
    createComplianceLabels();
    arrangeWidgets();
}

void PollutantOverview::createTitle()
{
    title = new QLabel("Pollutants Overview");
    QFont titleFont("Arial", 20, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
}

void PollutantOverview::createChart()
{
    auto overviewChart = new QChart();

    // Graph data initialisation

    // *** Implement appending the line series with specified data points

    auto series = new QLineSeries();
    series->append(0,0);
    series->append(10,10);
    overviewChart->addSeries(series);

    overviewChart->setTitle("Pollutant Overview Chart");

    // Axis creation

    // *** Implement appending unit/ranges for pollutant selected

    auto xAxis = new QValueAxis();
    xAxis->setTitleText("Time");
    xAxis->setRange(0,10);
    overviewChart->addAxis(xAxis, Qt::AlignBottom);
    series->attachAxis(xAxis);

    auto yAxis = new QValueAxis();
    yAxis->setTitleText("Level (mg)");
    yAxis->setRange(0,10);
    overviewChart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);

    // Chart view creation

    overviewChartView = new QChartView(overviewChart);
    overviewChartView->setMinimumSize(1000,400);
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

void PollutantOverview::createFilters()
{

    // *** Edit options to apply to dataset

    QStringList locationOptions;
    locationOptions << "All locations" << "1" << "2" << "3" << "4";
    location = new searchableComboBox();
    location->setOptions(locationOptions);
    locationLabel = new QLabel("&Location:");
    locationLabel->setBuddy(location);

    QStringList timeRangeOptions;
    timeRangeOptions << "All time" << "day" << "week" << "month" << "year";
    timeRange = new QComboBox();
    timeRange->addItems(timeRangeOptions);
    timeRangeLabel = new QLabel("&Time Range:");
    timeRangeLabel->setBuddy(timeRange);

    QStringList pollutantOptions;
    pollutantOptions << "All pollutants" << "chlorine" << "ethanol" << "fluoride" << "methanol" << "chloroform";
    pollutant = new searchableComboBox();
    pollutant->setOptions(pollutantOptions);
    pollutantLabel = new QLabel("&Pollutant:");
    pollutantLabel->setBuddy(pollutant);
}

void PollutantOverview::createComplianceLabels()
{
    // *** Implement changing threshold based on pollutant selected

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

void PollutantOverview::arrangeWidgets()
{
    // Title and searchbar

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