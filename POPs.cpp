/*
To implement:

- Create line series for each PFA by feeding in each data point
- ComboBox options adapting to data set
- When a pollutant is selected, only their line series is shown on the chart

*/

#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "POPs.hpp"
#include "stats.hpp"

PersistentOrganicPollutants::PersistentOrganicPollutants(QWidget *parent): QWidget(parent)
{
    createTitle();
    createChart();
    createButtons();
    createBoxes();
    createFilters();
    createComplianceLabels();
    arrangeWidgets();
}

void PersistentOrganicPollutants::createTitle()
{
    title = new QLabel("Persistent Organic Pollutants");
    QFont titleFont("Arial", 20, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
}

void PersistentOrganicPollutants::createChart()
{
    auto popChart = new QChart();

    // Graph data initialisation

    // *** Implement appending the line series with specified data points

    auto series = new QLineSeries();
    series->append(0,0);
    series->append(10,10);
    popChart->addSeries(series);

    popChart->setTitle("POPs Chart");

    // Axis creation

    // *** Implement appending unit/ranges for pollutant selected

    auto xAxis = new QValueAxis();
    xAxis->setTitleText("Time");
    xAxis->setRange(0,10);
    popChart->addAxis(xAxis, Qt::AlignBottom);
    series->attachAxis(xAxis);

    auto yAxis = new QValueAxis();
    yAxis->setTitleText("Level (mg)");
    yAxis->setRange(0,10);
    popChart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);

    // Chart view creation

    popChartView = new QChartView(popChart);
    popChartView->setMinimumSize(1000,400);
}

void PersistentOrganicPollutants::createButtons()
{
    moreInfo = new QPushButton("More Info");
    connect(moreInfo, &QPushButton::clicked, this, &PersistentOrganicPollutants::moreInfoMsgBox);

    viewList = new QPushButton("View List");
    connect(viewList, &QPushButton::clicked, this, &PersistentOrganicPollutants::viewListMsgBox);
}

void PersistentOrganicPollutants::createBoxes()
{
    QFont infoBoxFont("Arial", 8);

    pcbs = new QLabel("<h2>PCBs (Polychlorinated Byphenyls)<h2>"
                      "<p>PCBs are a group of man-made organic chemicals consisting of carbon, hydrogen and chlorine atoms<p>");
    pcbs->setFont(infoBoxFont);
    pcbs->setWordWrap(true);
    pcbs->setAlignment(Qt::AlignCenter);

    otherPops = new QLabel("<h2>Other POPs<h2>"
                           "Examples include DDT, chlordane and dioxins. These substances have various origins and effects<p>");
    otherPops->setFont(infoBoxFont);
    otherPops->setWordWrap(true);
    otherPops->setAlignment(Qt::AlignCenter);
}

void PersistentOrganicPollutants::createFilters()
{

    // *** Edit options to apply to dataset

    QStringList locationOptions;
    locationOptions << "All locations" << "1" << "2" << "3" << "4";
    location = new QComboBox();
    location->addItems(locationOptions);
    locationLabel = new QLabel("&Location:");
    locationLabel->setBuddy(location);

    QStringList timeRangeOptions;
    timeRangeOptions << "All time" << "day" << "week" << "month" << "year";
    timeRange = new QComboBox();
    timeRange->addItems(timeRangeOptions);
    timeRangeLabel = new QLabel("&Time Range:");
    timeRangeLabel->setBuddy(timeRange);

    QStringList pollutantOptions;
    pollutantOptions << "All pollutants" << "chlorine" << "ethanol";
    pollutant = new QComboBox();
    pollutant->addItems(pollutantOptions);
    pollutantLabel = new QLabel("&Pollutant:");
    pollutantLabel->setBuddy(pollutant);
}

void PersistentOrganicPollutants::createComplianceLabels()
{
    // *** (Save for 2nd iteration?) Implement changing threshold based on pollutant selected

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