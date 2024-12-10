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

    otherPfas = new QLabel("<h2>PFA List<h2>");
    otherPfas->setFont(infoBoxFont);
    otherPfas->setWordWrap(true);
    otherPfas->setAlignment(Qt::AlignCenter);
}

void PFApage::createFilters() {
    // Location combo box setup
    QStringList locationOptions;
    std::vector<std::string> complianceLocations = ComplianceChecker::getLocations();
    for (const std::string &locationStr : complianceLocations) {
        locationOptions << QString::fromStdString(locationStr);
    }
    location = new searchableComboBox();
    location->setOptions(locationOptions);
    locationLabel = new QLabel("&Location:");
    locationLabel->setBuddy(location);
    locationLabel->setWordWrap(true);

    // Connect to chart update for the first selection made
    connect(location, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PFApage::onLocationSelected);

    // Time range combo box setup
    QStringList timeRangeOptions;
    timeRangeOptions << "All time" << "day" << "week" << "month" << "year";
    timeRange = new QComboBox();
    timeRange->addItems(timeRangeOptions);
    timeRangeLabel = new QLabel("&Time Range:");
    timeRangeLabel->setBuddy(timeRange);

    connect(timeRange, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PFApage::updateChart);

    // Pollutant combo box setup
    QStringList pollutantOptions;
    pollutantOptions << "All pollutants";
    std::vector<std::string> pfasChemicals = ComplianceChecker::getPFAs();
    for (const std::string &chemical : pfasChemicals) {
        pollutantOptions << QString::fromStdString(chemical);
    }
    pollutant = new searchableComboBox();
    pollutant->setOptions(pollutantOptions);
    pollutantLabel = new QLabel("&Pollutant:");
    pollutantLabel->setBuddy(pollutant);

    connect(pollutant, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PFApage::updateChart);
}

void PFApage::createComplianceLabels() {
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

void PFApage::onLocationSelected(int index) {
    if (index != 0) { // Assuming index 0 is a default/placeholder non-selectable
        updateChart();
    }
}

void PFApage::updateChart() {
    auto &dataset = GlobalDataModel::instance().getDataset();
    const QString selectedLocation = location->currentText();
    const QString selectedPollutant = pollutant->currentText();
    const QString selectedTimeRange = timeRange->currentText();

    std::vector<Measurement> filteredData;

    for (const auto &measurement : dataset) {
        std::cout << measurement.getLabel() + " " << selectedLocation.toStdString() << std::endl;
        bool matchLocation = measurement.getLabel() == selectedLocation.toStdString();
        bool matchPollutant = selectedPollutant == "All pollutants" || measurement.getCompoundName() == selectedPollutant.toStdString();

        if (matchLocation && matchPollutant) {
            // Add time filtering logic if necessary, currently omitted for brevity
            filteredData.push_back(measurement);
        }
    }

    if (filteredData.empty()) {
        QMessageBox::warning(this, "No Data", "No data available for selected location.");
        pfaChartView->chart()->removeAllSeries(); // Clear the chart if no data is found
    } else {
        createChart(filteredData);
    }
}

void PFApage::createChart(const std::vector<Measurement> &filteredData) {
    auto pfaChart = new QChart();
    pfaChart->removeAllSeries();

    auto xAxis = new QValueAxis();
    xAxis->setTitleText("Time");
    pfaChart->addAxis(xAxis, Qt::AlignBottom);

    auto yAxis = new QValueAxis();
    yAxis->setTitleText("Level (ug/l)");
    yAxis->setRange(0.0, 0.0025);
    yAxis->setTickCount(10);
    yAxis->setLabelFormat("%.4f");
    pfaChart->addAxis(yAxis, Qt::AlignLeft);

    for (const auto &compound : ComplianceChecker::getPFAs()) {
        auto *series = new QLineSeries();
        bool seriesHasData = false;

        for (const auto &measurement : filteredData) {
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