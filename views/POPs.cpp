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
    QFont infoBoxFont("Arial", 8);

    pcbs = new QLabel("PCBs (Polychlorinated Biphenyls)");
    pcbs->setFont(infoBoxFont);
    pcbs->setWordWrap(true);
    pcbs->setAlignment(Qt::AlignCenter);

    otherPops = new QLabel("POP List");
    otherPops->setFont(infoBoxFont);
    otherPops->setWordWrap(true);
    otherPops->setAlignment(Qt::AlignCenter);
}


void PersistentOrganicPollutants::createFilters() {
    QStringList locationOptions;
    std::vector<std::string> locations = ComplianceChecker::getLocations();
    for (const std::string &locationStr: locations) {
        locationOptions << QString::fromStdString(locationStr);
    }
    location = new searchableComboBox();
    location->setOptions(locationOptions);
    locationLabel = new QLabel("Location:");
    locationLabel->setBuddy(location);
    locationLabel->setWordWrap(true);
    connect(location, QOverload<int>::of(&searchableComboBox::currentIndexChanged), this,
            &PersistentOrganicPollutants::onLocationSelected);

    QStringList timeRangeOptions;
    timeRangeOptions << "All Time" << "day" << "week" << "month" << "year";
    timeRange = new QComboBox();
    timeRange->addItems(timeRangeOptions);
    timeRangeLabel = new QLabel("Time Range:");
    timeRangeLabel->setBuddy(timeRange);

    connect(timeRange, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &PersistentOrganicPollutants::updateChart);

    QStringList pollutantOptions;
    pollutantOptions << "All Pollutants" << "chlorine"
            << "ethanol";
    pollutant = new searchableComboBox();
    pollutant->setOptions(pollutantOptions);
    pollutantLabel = new QLabel("Pollutant:");
    pollutantLabel->setBuddy(pollutant);

    connect(pollutant, QOverload<int>::of(&searchableComboBox::currentIndexChanged), this,
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
    moreInfoFrame->setFixedSize(200, 200);

    QVBoxLayout *moreInfoLayout = new QVBoxLayout(moreInfoFrame);
    moreInfoLayout->addWidget(pcbs);
    moreInfoLayout->addWidget(moreInfo);

    auto viewListFrame = new QFrame();
    viewListFrame->setFrameShape(QFrame::Box);
    viewListFrame->setLineWidth(1);
    viewListFrame->setFixedSize(200, 200);

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

void PersistentOrganicPollutants::onLocationSelected(int index) {
    if (index != 0) {
        // Assuming index 0 is a default or non-selectable placeholder
        updateChart();
    }
}

void PersistentOrganicPollutants::updateChart() {
    auto &dataset = GlobalDataModel::instance().getDataset();
    const QString selectedLocation = location->currentText();
    const QString selectedPollutant = pollutant->currentText();
    const QString selectedTimeRange = timeRange->currentText();

    std::vector<Measurement> filteredData;

    for (const auto &measurement: dataset) {
        bool matchLocation = measurement.getLabel() == selectedLocation.toStdString();
        bool matchPollutant = selectedPollutant == "All Pollutants" || measurement.getCompoundName() ==
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

    auto xAxis = new QValueAxis();
    xAxis->setTitleText("Time");
    popChart->addAxis(xAxis, Qt::AlignBottom);

    auto yAxis = new QValueAxis();
    yAxis->setTitleText("Level");
    yAxis->setRange(0.0, 0.0025);
    yAxis->setTickCount(10);
    yAxis->setLabelFormat("%.4f");
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

    popChart->setTitle("Filtered POPs Chart");
    popChartView->setChart(popChart);
}


void PersistentOrganicPollutants::retranslateUI() {
    title->setText(tr("POPs_TITLE"));
    popChartView->chart()->setTitle(tr("POPs_AWAITING_DATA"));
    pcbs->setText(tr("POPs_PCBS_INFO"));
    otherPops->setText(tr("POPs_OTHER_INFO"));
    moreInfo->setText(tr("POPs_MORE_INFO"));
    viewList->setText(tr("POPs_VIEW_LIST"));

    locationLabel->setText(tr("POPs_LOCATION_LABEL"));
    timeRangeLabel->setText(tr("POPs_TIME_RANGE_LABEL"));
    pollutantLabel->setText(tr("POPs_POLLUTANT_LABEL"));

    red->setText(tr("POPs_COMPLIANCE_RED"));
    red->setToolTip(tr("POPs_COMPLIANCE_RED_TOOL_TIP"));
    orange->setText(tr("POPs_COMPLIANCE_ORANGE"));
    orange->setToolTip(tr("POPs_COMPLIANCE_ORANGE_TOOL_TIP"));
    green->setText(tr("POPs_COMPLIANCE_GREEN"));
    green->setToolTip(tr("POPs_COMPLIANCE_GREEN_TOOL_TIP"));
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