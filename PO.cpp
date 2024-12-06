/*
To implement:



*/

#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "PO.hpp"
#include "stats.hpp"

PollutantOverview::PollutantOverview(QWidget *parent): QWidget(parent)
{
    createTitle();
    createSearchBar();
    createChart();
    createButtons();
    createBoxes();
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

void PollutantOverview::createSearchBar()
{
    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Search for pollutants");

    connect(searchBar, &QLineEdit::returnPressed, this, &PollutantOverview::searchQuery);
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

    moreInfo = new QPushButton("More Info");
    connect(moreInfo, &QPushButton::clicked, this, &PollutantOverview::moreInfoMsgBox);

    viewList = new QPushButton("View List");
    connect(viewList, &QPushButton::clicked, this, &PollutantOverview::viewListMsgBox);
}

void PollutantOverview::createBoxes()
{
    // *UI Job* boxes for more info on pollutant categories

    QFont infoBoxFont("Arial", 8);

    pcbs = new QLabel("<h2>PCBs (Polychlorinated Byphenyls)<h2>"
                      "<p>PCBs are a group of man-made organic chemicals consisting of carbon, hydrogen and chlorine atoms<p>");
    pcbs->setFont(infoBoxFont);
    pcbs->setWordWrap(true);
    pcbs->setAlignment(Qt::AlignCenter);

    otherPops = new QLabel("<h2>Other PollutantOverview<h2>"
                           "Examples include DDT, chlordane and dioxins. These substances have various origins and effects<p>");
    otherPops->setFont(infoBoxFont);
    otherPops->setWordWrap(true);
    otherPops->setAlignment(Qt::AlignCenter);
}

void PollutantOverview::createFilters()
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
    // pollutant->setEditable(true);
}

void PollutantOverview::createComplianceLabels()
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

void PollutantOverview::arrangeWidgets()
{
    // Title and searchbar

    QHBoxLayout* header = new QHBoxLayout();
    header->addWidget(title);
    header->addWidget(searchBar);

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

    // Info box layout (*UI Job* adjust for pollutant category info boxes)

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
    layout->addLayout(header);
    layout->addLayout(body);

    setLayout(layout);
}

// *UI Job* msg boxes for pollutant categories

void PollutantOverview::moreInfoMsgBox()
{
  QMessageBox::information(this, "PCB Info", "more info about PCBs");
}

void PollutantOverview::viewListMsgBox()
{
  QMessageBox::information(this, "List of Persistent Organic Pollutants", "List of PollutantOverview");
}

QString PollutantOverview::searchQuery()
{
    QString query = searchBar->text();
    return query;
}