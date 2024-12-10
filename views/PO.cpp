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

// *UI Job* msg boxes for pollutant categories

void PollutantOverview::heavyMetalsInfoMsgBox()
{
    QMessageBox::information(this, "Heavy Metals Info", 
    "Heavy metals, such as lead, mercury, and cadmium, are toxic even at low concentrations. "
    "They accumulate in living organisms, causing long-term damage to ecosystems and human health. "
    "Heavy metals can contaminate water sources and are difficult to remediate once released.\n\n"
    "Pollutants in this category:\n"
    "- Arsenic\n"
    "- Chromium (III)\n"
    "- Chromium (IV) (Hexavalent Chromium)\n"
    "- Copper\n"
    "- Iron\n"
    "- Manganese\n"
    "- Zinc");
}

void PollutantOverview::organicChemicalsInfoMsgBox()
{
    QMessageBox::information(this, "Organic Chemicals Info", 
    "Organic chemicals include pesticides, herbicides, fungicides, and industrial chemicals. "
    "These chemicals can contaminate soil and water, disrupting ecosystems and causing long-term harm to plants, animals, and humans.\n\n"
    "Pollutants in this category:\n"
    "- Carbendazim (fungicide)\n"
    "- Chlorothalonil (fungicide)\n"
    "- Cypermethrin (insecticide)\n"
    "- Diazinon (insecticide)\n"
    "- Dimethoate (insecticide)\n"
    "- Glyphosate (herbicide)\n"
    "- Linuron (herbicide)\n"
    "- Mecoprop (herbicide)\n"
    "- Pendimethalin (herbicide)\n"
    "- Permethrin (insecticide)\n"
    "- Methiocarb (insecticide)\n"
    "- Benzyl butyl phthalate (plasticizer)\n"
    "- Tetrachloroethane (solvent)\n"
    "- Toluene (solvent)\n"
    "- 2,4-dichlorophenol\n"
    "- Phenol\n"
    "- Triclosan (antibacterial/antifungal agent)");
}

void PollutantOverview::inorganicChemicalsInfoMsgBox()
{
    QMessageBox::information(this, "Inorganic Chemicals Info", 
    "Inorganic chemicals, including metals, salts, and acids, can affect water pH levels, disrupt aquatic habitats, "
    "and pose risks to plants and animals. These substances are often introduced through industrial discharge and mining activities.\n\n"
    "Pollutants in this category:\n"
    "- Chlorine\n"
    "- Cyanide\n"
    "- Fluoride\n"
    "- Silica/Silicate\n"
    "- Bicarbonate");
}

void PollutantOverview::nutrientsInfoMsgBox()
{
    QMessageBox::information(this, "Nutrients Info", 
    "Excess nutrients, particularly nitrogen and phosphorus, can lead to eutrophication in water bodies. "
    "This process depletes oxygen levels, causing algal blooms and fish kills, and disrupting aquatic ecosystems.\n\n"
    "Pollutants in this category:\n"
    "- Nitrate\n"
    "- Phosphate");
}

void PollutantOverview::volatileOrganicCompoundsInfoMsgBox()
{
    QMessageBox::information(this, "Volatile Organic Compounds Info", 
    "Volatile Organic Compounds (VOCs) include hydrocarbons and halogenated hydrocarbons. "
    "Hydrocarbons can contaminate water and soil, depleting oxygen levels and harming wildlife. "
    "Halogenated hydrocarbons, such as chlorinated solvents, are persistent pollutants that bioaccumulate, "
    "pose toxicity risks, and contribute to ozone depletion.\n\n"
    "Pollutants in this category:\n"
    "- Benzene\n"
    "- Toluene\n"
    "- Ethylbenzene\n"
    "- Xylenes\n"
    "- Chloroform\n"
    "- Trichloroethylene (TCE)\n"
    "- Tetrachloroethylene (PCE)\n"
    "- Vinyl Chloride\n"
    "- Carbon Tetrachloride\n"
    "- Dichloromethane (Methylene Chloride)");
}
