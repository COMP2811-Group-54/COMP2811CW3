#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "PFAs.hpp"
#include "stats.hpp"
#include "utils/Compliance.hpp"
#include "utils/GlobalDataModel.hpp"

PFApage::PFApage(QWidget *parent) : QWidget(parent) {
    createTitle();

    // Initialize pfaChartView with a placeholder chart
    QChart* placeholderChart = new QChart();
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
    QStringList locationOptions;
    locationOptions << "All locations" << "1" << "2" << "3" << "4";
    location = new QComboBox();
    location->addItems(locationOptions);
    locationLabel = new QLabel("&Location:");
    locationLabel->setBuddy(location);
    locationLabel->setWordWrap(true);

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
    chartContext->addWidget(orange);
    chartContext->addWidget(red);

    QVBoxLayout* chart = new QVBoxLayout();
    chart->setSizeConstraint(QLayout::SetMinimumSize);
    chart->addWidget(pfaChartView, 19);
    chart->addLayout(chartContext, 1);
    chart->addStretch();

    auto moreInfoFrame = new QFrame();
    moreInfoFrame->setFrameShape(QFrame::Box);
    moreInfoFrame->setLineWidth(1);
    moreInfoFrame->setFixedSize(200, 200);

    QVBoxLayout* moreInfoLayout = new QVBoxLayout(moreInfoFrame);
    moreInfoLayout->addWidget(pfas);
    moreInfoLayout->addWidget(moreInfo);

    auto viewListFrame = new QFrame();
    viewListFrame->setFrameShape(QFrame::Box);
    viewListFrame->setLineWidth(1);
    viewListFrame->setFixedSize(200, 200);

    QVBoxLayout* viewListLayout = new QVBoxLayout(viewListFrame);
    viewListLayout->addWidget(otherPfas);
    viewListLayout->addWidget(viewList);

    QVBoxLayout* info = new QVBoxLayout();
    info->addStretch();
    info->addWidget(moreInfoFrame);
    info->addSpacing(50);
    info->addWidget(viewListFrame);
    info->addSpacing(50);
    info->addStretch();

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

void PFApage::createChart() {
    auto popChart = new QChart();
    ComplianceChecker checker;

    auto& dataset = GlobalDataset::instance().getDataset();
    const auto pfas = checker.getPFAs();

    for (const auto& compound : pfas) {
        if (dataset.size() == 0) {
            QMessageBox::warning(this, "No Data", "Dataset is empty, cannot create chart");
            return;
        }

        auto series = new QLineSeries();
        for (size_t i = 0; i < dataset.size(); ++i) {
            auto measurement = dataset[i];
            if (measurement.getCompoundName() == compound) {
                series->append(measurement.getDatetime().toMSecsSinceEpoch(), measurement.getValue());
            }
        }
        series->setName(QString::fromStdString(compound));
        popChart->addSeries(series);

        auto xAxis = new QValueAxis();
        xAxis->setTitleText("Time");
        popChart->addAxis(xAxis, Qt::AlignBottom);
        series->attachAxis(xAxis);

        auto yAxis = new QValueAxis();
        yAxis->setTitleText("Level (ug/l)");
        popChart->addAxis(yAxis, Qt::AlignLeft);
        series->attachAxis(yAxis);
    }

    popChart->setTitle("PFAs Chart");
    pfaChartView->setChart(popChart);
    pfaChartView->setMinimumSize(600, 400);
}

void PFApage::initializeWithData() {
    auto& dataset = GlobalDataset::instance().getDataset();

    if (dataset.size() == 0) {
        QMessageBox::warning(this, "Data Unavailable", "Dataset is empty. Cannot create chart.");
        return;
    }

    createChart();
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
    orange->setText(tr("PFA_COMPLIANCE_ORANGE"));
    green->setText(tr("PFA_COMPLIANCE_GREEN"));
}

void PFApage::moreInfoMsgBox() {
    QMessageBox::information(this, "PFA Info", "more info about pfas");
}

void PFApage::viewListMsgBox() {
    QMessageBox::information(this, "List of Poly-fluorinated Compounds", "List of PFAs");
}