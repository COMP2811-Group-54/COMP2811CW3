#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "POPs.hpp"
#include "stats.hpp"

PersistentOrganicPollutants::PersistentOrganicPollutants(): QWidget()
{
    // createTitle();
    createChart();
    createButtons();
    createBoxes();
    createFilters();
    createComplianceLabels();
    arrangeWidgets();

    setWindowTitle("Persistent Organic Pollutants");
}

// void PersistentOrganicPollutants::createChart()
// {
//     title = new QLabel("Persistent Organic Pollutants");
//     auto titleFont = QFont();
//     titleFont->setPointSize(40);
    
// }

void PersistentOrganicPollutants::createChart()
{
    auto popChart = new QChart();

    auto series = new QLineSeries();
    series->append(0,0);
    series->append(10,10);
    popChart->addSeries(series);

    popChart->setTitle("POPs Chart");

    auto xAxis = new QValueAxis();
    xAxis->setTitleText("Time");
    xAxis->setRange(0,10);
    popChart->addAxis(xAxis, Qt::AlignBottom);
    series->attachAxis(xAxis);

    auto yAxis = new QValueAxis();
    yAxis->setTitleText("Level");
    yAxis->setRange(0,10);
    popChart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(yAxis);

    popChartView = new QChartView(popChart);
    popChartView->setFixedSize(1000,500);
}

void PersistentOrganicPollutants::createButtons()
{
    viewList = new QPushButton("View List");
    moreInfo = new QPushButton("More Info");
}

void PersistentOrganicPollutants::createBoxes()
{
    pcbs = new QLabel("<h2>PCBs (Polychlorinated Byphenyls)<h2>"
                      "<p>PCBs are a group of man-made organic chemicals consisting of carbon, hydrogen and chlorine atoms<p>");
    otherPops = new QLabel("<h2>Other POPs<h2>"
                           "Examples include DDT, chlordane and dioxins. These substances have various origins and effects<p>");
    pcbs->setWordWrap(true);
    otherPops->setWordWrap(true);
}

void PersistentOrganicPollutants::createFilters()
{
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
}

void PersistentOrganicPollutants::createComplianceLabels()
{
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
    QHBoxLayout* filters = new QHBoxLayout();
    filters->addWidget(locationLabel);
    filters->addWidget(location);
    filters->addWidget(timeRangeLabel);
    filters->addWidget(timeRange);
    filters->addSpacing(50);
    filters->addWidget(green);
    filters->addSpacing(50);
    filters->addWidget(orange);
    filters->addSpacing(50);
    filters->addWidget(red);
    filters->addStretch();

    QVBoxLayout* chart = new QVBoxLayout();
    chart->addWidget(popChartView);
    chart->addLayout(filters);
    chart->addStretch();

    QVBoxLayout* info = new QVBoxLayout();
    info->addWidget(pcbs);
    info->addWidget(moreInfo);
    info->addWidget(otherPops);
    info->addWidget(viewList);
    info->addStretch();

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addLayout(chart);
    layout->addLayout(info);
    layout->addStretch();

    setLayout(layout);
}