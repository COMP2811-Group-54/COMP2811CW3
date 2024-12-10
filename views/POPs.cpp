#include <QtWidgets>
#include <QtCharts>
#include "POPs.hpp"
#include "../utils/Compliance.hpp"
#include "../utils/GlobalDataModel.hpp"

PersistentOrganicPollutants::PersistentOrganicPollutants(QWidget *parent) : QWidget(parent) {
    createTitle();

    // Initialize popChartView with a placeholder chart
    QChart *placeholderChart = new QChart();
    placeholderChart->setTitle(tr("Awaiting Data...")); // Awaiting data placeholder
    popChartView = new QChartView(placeholderChart);
    popChartView->setMinimumSize(1000, 400);

    createButtons();
    createFilters();
    createComplianceLabels();
    arrangeWidgets();
}

void PersistentOrganicPollutants::createTitle() {
    title = new QLabel(tr("POPs_TITLE"));
    QFont titleFont("Arial", 20, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
}

void PersistentOrganicPollutants::createButtons() {
    moreInfo = new QPushButton(tr("POPs_MORE_INFO"));
    connect(moreInfo, &QPushButton::clicked, this, &PersistentOrganicPollutants::moreInfoMsgBox);

    viewList = new QPushButton(tr("POPs_VIEW_LIST"));
    connect(viewList, &QPushButton::clicked, this, &PersistentOrganicPollutants::viewListMsgBox);
}

void PersistentOrganicPollutants::createBoxes() {
    QFont infoBoxFont("Arial", 8);

    pcbs = new QLabel(tr("POPs_PCBS_INFO"));
    pcbs->setFont(infoBoxFont);
    pcbs->setWordWrap(true);
    pcbs->setAlignment(Qt::AlignCenter);

    otherPops = new QLabel(tr("POPs_OTHER_INFO"));
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
    locationLabel = new QLabel("&Location:");
    locationLabel->setBuddy(location);
    locationLabel->setWordWrap(true);
    connect(location, QOverload<int>::of(&searchableComboBox::currentIndexChanged), this,
            &PersistentOrganicPollutants::onLocationSelected);

    QStringList timeRangeOptions;
    timeRangeOptions << tr("POPs_ALL_TIME") << tr("POPs_TIME_DAY") << tr("POPs_TIME_WEEK")
            << tr("POPs_TIME_MONTH") << tr("POPs_TIME_YEAR");
    timeRange = new QComboBox();
    timeRange->addItems(timeRangeOptions);
    timeRangeLabel = new QLabel(tr("POPs_TIME_RANGE_[LABEL"));
    timeRangeLabel->setBuddy(timeRange);

    connect(timeRange, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &PersistentOrganicPollutants::updateChart);

    QStringList pollutantOptions;
    pollutantOptions << tr("POPs_ALL_POLLUTANTS") << tr("POPs_POLLUTANT_CHLORINE")
            << tr("POPs_POLLUTANT_ETHANOL");
    pollutant = new searchableComboBox();
    pollutant->setOptions(pollutantOptions);
    pollutantLabel = new QLabel(tr("POPs_POLLUTANT_LABEL"));
    pollutantLabel->setBuddy(pollutant);

    connect(pollutant, QOverload<int>::of(&searchableComboBox::currentIndexChanged), this,
            &PersistentOrganicPollutants::updateChart);
}

void PersistentOrganicPollutants::createComplianceLabels() {
    red = new QLabel(tr("POPs_COMPLIANCE_RED"));
    red->setStyleSheet("background-color: red; color: white;");
    red->setToolTip(tr("POPs_COMPLIANCE_RED_TOOLTIP"));

    orange = new QLabel(tr("POPs_COMPLIANCE_ORANGE"));
    orange->setStyleSheet("background-color: orange; color: white;");
    orange->setToolTip(tr("POPs_COMPLIANCE_ORANGE_TOOLTIP"));

    green = new QLabel(tr("POPs_COMPLIANCE_GREEN"));
    green->setStyleSheet("background-color: green; color: white;");
    green->setToolTip(tr("POPs_COMPLIANCE_GREEN_TOOLTIP"));
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
        bool matchPollutant = selectedPollutant == tr("POPs_ALL_POLLUTANTS") || measurement.getCompoundName() ==
                              selectedPollutant.toStdString();

        if (matchLocation && matchPollutant) {
            // Apply additional time filtering if necessary
            filteredData.push_back(measurement);
        }
    }

    if (filteredData.empty()) {
        QMessageBox::warning(this, tr("No Data"), tr("No data available for selected location."));
        popChartView->chart()->removeAllSeries(); // Clear chart if no data is found
    } else {
        createChart(filteredData);
    }
}

void PersistentOrganicPollutants::createChart(const std::vector<Measurement> &filteredData) {
    auto popChart = new QChart();
    popChart->removeAllSeries();

    auto xAxis = new QValueAxis();
    xAxis->setTitleText(tr("Time"));
    popChart->addAxis(xAxis, Qt::AlignBottom);

    auto yAxis = new QValueAxis();
    yAxis->setTitleText(tr("Level (ug/l)"));
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

    popChart->setTitle(tr("Filtered POPs Chart"));
    popChartView->setChart(popChart);
}

void PersistentOrganicPollutants::moreInfoMsgBox() {
    QMessageBox::information(this, "Persistent Organic Pollutants Info", 
    "Persistent Organic Pollutants (POPs) are toxic chemicals that persist in the environment for long periods, accumulating in the food chain. "
    "They can cause a range of health problems in humans and wildlife, including cancer, immune system disruption, and developmental issues. "
    "These pollutants are difficult to remove from the environment and continue to pose risks even long after their use has been restricted.");
}

void PersistentOrganicPollutants::viewListMsgBox() {
    QMessageBox::information(this, "List of Persistent Organic Pollutants", 
    "- PCBs (Polychlorinated Biphenyls)\n"
    "- DDT\n"
    "- Hexachlorobenzene\n"
    "- Chlordane\n"
    "- Aldrin\n"
    "- Dieldrin\n"
    "- Endrin\n"
    "- Heptachlor\n"
    "- Carbofuran");
}


void PersistentOrganicPollutants::retranslateUI() {
    title->setText(tr("POPs_TITLE"));
    popChartView->chart()->setTitle(tr("POPs_AWAITING_DATA"));
    // Retranslate other components as necessary
}
