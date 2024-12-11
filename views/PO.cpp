#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "PO.hpp"

#include "../utils/Compliance.hpp"
#include "../utils/Measurement.hpp"
#include "../utils/GlobalDataModel.hpp"

PollutantOverview::PollutantOverview(QWidget *parent): QWidget(parent) {
    createTitle();
    // Initialize popChartView with a placeholder chart
    QChart *placeholderChart = new QChart();
    placeholderChart->setTitle(tr("Awaiting Data...")); // Awaiting data placeholder
    createSearchBar(); // Add search bar initialization
    overviewChartView = new QChartView(placeholderChart);
    overviewChartView->setMinimumSize(1000, 400);

    createButtons();
    createBoxes();
    createFilters();
    createComplianceLabels();
    arrangeWidgets();
}

void PollutantOverview::createTitle() {
    title = new QLabel("Pollutants Overview");
    QFont titleFont("Arial", 20, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
}

void PollutantOverview::createChart(const std::vector<Measurement> &filteredData) {
    QChart *overviewChart = new QChart();
    overviewChart->removeAllSeries();
    overviewChart->setAnimationOptions(QChart::SeriesAnimations);

    // X-Axis: Use QDateTimeAxis for human-readable timestamps
    auto *xAxis = new QDateTimeAxis();
    xAxis->setFormat("dd-MM-yyyy HH:mm");
    xAxis->setTitleText("Time");
    xAxis->setTickCount(10);
    overviewChart->addAxis(xAxis, Qt::AlignBottom);

    // Y-Axis: Dynamically update only in the expected positive range
    auto *yAxis = new QValueAxis();
    yAxis->setTitleText("Level (ug/l)");
    yAxis->setLabelFormat("%.5f");
    yAxis->setTickCount(10);
    overviewChart->addAxis(yAxis, Qt::AlignLeft);

    // Iterate through all pollutant categories dynamically
    const std::map<QString, std::vector<std::string> > pollutantCategories = {
        {"PFAs", ComplianceChecker::getPFAs()},
        {"POPs", ComplianceChecker::getPOPs()},
        {"Metals", ComplianceChecker::getMetals()},
        {"VOCs", ComplianceChecker::getVOCs()}
    };

    for (const auto &[category, pollutants]: pollutantCategories) {
        for (const auto &compound: pollutants) {
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
                overviewChart->addSeries(series);
                series->attachAxis(xAxis);
                series->attachAxis(yAxis);
            } else {
                delete series;
            }
        }
    }

    // Add compliance threshold lines
    double baseThreshold = 0.1; // Default threshold value
    auto *greenThreshold = new QLineSeries();
    auto *orangeThreshold = new QLineSeries();
    auto *redThreshold = new QLineSeries();

    for (const auto &measurement: filteredData) {
        if (measurement.getDatetime().isValid()) {
            qint64 timestamp = measurement.getDatetime().toMSecsSinceEpoch();
            greenThreshold->append(timestamp, baseThreshold * 0.8);
            orangeThreshold->append(timestamp, baseThreshold);
            redThreshold->append(timestamp, baseThreshold * 1.2);
        }
    }

    overviewChart->setTitle(tr("Pollutant Overview for %1").arg(
        pollutant->currentText().isEmpty() ? "All" : pollutant->currentText()));
    // Example of adding compliance level lines to chart
    if (pollutant->currentText() != "All pollutants") {
        overviewChart->addSeries(greenThreshold);
        greenThreshold->attachAxis(xAxis);
        greenThreshold->attachAxis(yAxis);
        greenThreshold->setName("Green Threshold");

        overviewChart->addSeries(orangeThreshold);
        orangeThreshold->attachAxis(xAxis);
        orangeThreshold->attachAxis(yAxis);
        orangeThreshold->setName("Orange Threshold");

        overviewChart->addSeries(redThreshold);
        redThreshold->attachAxis(xAxis);
        redThreshold->attachAxis(yAxis);
        redThreshold->setName("Red Threshold");

        addComplianceLevelLine(overviewChart, xAxis, yAxis, baseThreshold * 0.8, "green", "Green Threshold");
        addComplianceLevelLine(overviewChart, xAxis, yAxis, baseThreshold, "orange", "Orange Threshold");
        addComplianceLevelLine(overviewChart, xAxis, yAxis, baseThreshold * 1.2, "red", "Red Threshold");
        green->show();
        orange->show();
        red->show();
    } else {
        green->hide();
        orange->hide();
        red->hide();
    }
    overviewChartView->setChart(overviewChart);
    overviewChartView->setMinimumSize(1000, 400);
}

void PollutantOverview::updateChart() {
    // If 'All pollutants' is selected, hide compliance labels and remove threshold lines
    if (pollutant->currentText() == "All pollutants") {
        green->hide();
        orange->hide();
        red->hide();
        overviewChartView->chart()->removeAllSeries(); // Clear any threshold lines
    }

    // Validate dropdown logic: 'All pollutants' and 'All locations' not allowed
    if (pollutant->currentText() == "All pollutants" && location->currentText() == "All locations") {
        QMessageBox::warning(this, tr("Invalid Selection"),
                             tr(
                                 "You cannot select 'All pollutants' and 'All locations' simultaneously. Resetting pollutant selection."));
        pollutant->setCurrentIndex(1); // Reset to a specific pollutant
        return;
    }
    const std::vector<Measurement> dataset = GlobalDataModel::instance().getDataset().sortByTimestamp();
    const QString selectedLocation = location->currentText();
    const QString selectedPollutant = pollutant->currentText();
    const QString selectedTimeRange = timeRange->currentText();
    qint64 lastTimestamp = 0;

    if (!dataset.empty()) {
        lastTimestamp = dataset.back().getDatetime().toMSecsSinceEpoch();
    }

    std::vector<Measurement> filteredData;
    qint64 filterStartTime = lastTimestamp;

    // Time range filter
    if (selectedTimeRange == "3 days") {
        filterStartTime -= 3ll * 24 * 60 * 60 * 1000;
    } else if (selectedTimeRange == "1 week") {
        filterStartTime -= 7ll * 24 * 60 * 60 * 1000;
    } else if (selectedTimeRange == "6 months") {
        filterStartTime -= 180ll * 24 * 60 * 60 * 1000;
    } else if (selectedTimeRange == "1 year") {
        filterStartTime -= 365ll * 24 * 60 * 60 * 1000;
    }

    for (const auto &measurement: dataset) {
        const qint64 timestamp = measurement.getDatetime().toMSecsSinceEpoch();
        if (timestamp < filterStartTime || timestamp > lastTimestamp) {
            continue;
        }

        // Location and pollutant filters
        bool matchLocation = (selectedLocation == "All locations" || measurement.getLabel() == selectedLocation.
                              toStdString());
        bool matchPollutant = (selectedPollutant == "All pollutants" || measurement.getCompoundName() ==
                               selectedPollutant.toStdString());

        if (matchLocation && matchPollutant) {
            filteredData.push_back(measurement);
        }
    }

    if (filteredData.empty()) {
        QMessageBox::warning(this, tr("No Data"), tr("No data available for the selected filters."));
        overviewChartView->chart()->removeAllSeries();
    } else {
        createChart(filteredData);
    }
}

void PollutantOverview::createButtons() {
    // *UI Job* buttons for more info on pollutant categories

    moreInfo = new QPushButton("More Info");
    connect(moreInfo, &QPushButton::clicked, this, &PollutantOverview::moreInfoMsgBox);

    viewList = new QPushButton("View List");
    connect(viewList, &QPushButton::clicked, this, &PollutantOverview::viewListMsgBox);
}

void PollutantOverview::createBoxes() {
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

void PollutantOverview::createFilters() {
    // Location combo box setup
    location = new QComboBox();
    locationLabel = new QLabel("&Location:");
    locationLabel->setBuddy(location);
    locationLabel->setWordWrap(true);

    // Connect to dataReady to update dynamically after data loads
    connect(&GlobalDataModel::instance(), &GlobalDataModel::dataReady, this, [this]() {
        QStringList updatedLocationOptions{"All locations"};
        auto complianceLocations = GlobalDataModel::instance().getDataset().getHighDataPointLocations();

        for (const std::string &locationStr: complianceLocations) {
            updatedLocationOptions << QString::fromStdString(locationStr);
        }
        if (location) {
            location->addItems(updatedLocationOptions);
        }
    });

    // Connect to chart update for the first selection made
    connect(location, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PollutantOverview::updateChart);

    connect(location, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        // Ensure validity of selections
        if (pollutant->currentText() == "All pollutants" && location->currentText() == "All locations") {
            QMessageBox::warning(this, tr("Invalid Selection"),
                                 tr(
                                     "You cannot select 'All pollutants' and 'All locations' simultaneously. Resetting location selection."));
            location->setCurrentIndex(1); // Reset to a specific location
        } else {
            updateChart();
        }
    });

    // Time range combo box setup
    QStringList timeRangeOptions;
    timeRangeOptions << "3 days" << "1 week" << "2 weeks"
            << "3 months" << "6 months" << "9 months"
            << "1 year";
    timeRange = new QComboBox();
    timeRange->addItems(timeRangeOptions);
    timeRange->setCurrentText("1 year");
    timeRangeLabel = new QLabel("&Time Range:");
    timeRangeLabel->setBuddy(timeRange);


    connect(timeRange, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &PollutantOverview::updateChart);

    // Fetch and set pollutants
    QStringList pollutantOptions{"All pollutants"};
    std::vector<std::string> pollutants;

    // Adding pollutants from different categories
    std::vector<std::string> pfas = ComplianceChecker::getPFAs();
    pollutants.insert(pollutants.end(), pfas.begin(), pfas.end());

    std::vector<std::string> pops = ComplianceChecker::getPOPs();
    pollutants.insert(pollutants.end(), pops.begin(), pops.end());

    std::vector<std::string> metals = ComplianceChecker::getMetals();
    pollutants.insert(pollutants.end(), metals.begin(), metals.end());

    std::vector<std::string> vocs = ComplianceChecker::getVOCs();
    pollutants.insert(pollutants.end(), vocs.begin(), vocs.end());

    for (const std::string &chemical: pollutants) {
        pollutantOptions << QString::fromStdString(chemical);
    }

    pollutant = new QComboBox();
    pollutant->addItems(pollutantOptions);
    pollutantLabel = new QLabel("&Pollutant:");
    pollutantLabel->setBuddy(pollutant);

    connect(pollutant, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PollutantOverview::updateChart);
}

void PollutantOverview::createComplianceLabels() {
    // Default compliance information
    double threshold = 0.1; // Default threshold for compliance levels
    if (pollutant->currentText() != "All pollutants") {
        threshold = ComplianceChecker().getComplianceThreshold(pollutant->currentText().toStdString());
    }

    // Update labels dynamically based on threshold
    red = new QLabel(QString("Red level: >%1").arg(threshold * 1.2)); // Example: 1.2x threshold
    red->setStyleSheet("background-color: red; color: white;");
    red->setToolTip("Exceeds compliance threshold by 20%");

    orange = new QLabel(QString("Orange level: %1-%2").arg(threshold * 0.8).arg(threshold));
    orange->setStyleSheet("background-color: orange; color: white;");
    orange->setToolTip("Within compliance threshold");

    green = new QLabel(QString("Green level: <%1").arg(threshold * 0.8));
    green->setStyleSheet("background-color: green; color: white;");
    green->setToolTip("Below compliance threshold by 20%");
}

void PollutantOverview::arrangeWidgets() {
    // Title and searchbar
    QHBoxLayout *header = new QHBoxLayout();
    header->addWidget(title);

    // Filters and Compliance Indicators

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
    chartContext->addSpacing(20);
    chartContext->addStretch();
    chartContext->addWidget(orange);
    chartContext->addSpacing(20);
    chartContext->addStretch();
    chartContext->addWidget(red);
    chartContext->addStretch();

    // Graph layout

    QVBoxLayout *chart = new QVBoxLayout();
    chart->setSizeConstraint(QLayout::SetMinimumSize);
    chart->addWidget(overviewChartView, 19);
    chart->addLayout(chartContext, 1);
    chart->addStretch();

    // Info box layout (*UI Job* adjust for pollutant category info boxes)
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

    // Main body layout

    QHBoxLayout *body = new QHBoxLayout();
    body->setSizeConstraint(QLayout::SetMinimumSize);
    body->addLayout(chart, 4);
    body->addLayout(info, 1);
    body->addStretch();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->addLayout(header);
    layout->addLayout(body);

    setLayout(layout);
}

void PollutantOverview::createSearchBar() {
    pollutantSearchBar = new QLineEdit(this);
    pollutantSearchBar->setPlaceholderText("Search pollutant...");
    connect(pollutantSearchBar, &QLineEdit::returnPressed, this, [this]() {
        QString searchText = pollutantSearchBar->text();
        pollutant->setCurrentText(searchText); // Dynamically update pollutant filter
        updateChart();
    });
}

void PollutantOverview::addComplianceLevelLine(QChart *chart, QAbstractAxis *xAxis, QAbstractAxis *yAxis,
                                               double levelValue, const QString &color, const QString &label) {
    QLineSeries *complianceLine = new QLineSeries();
    complianceLine->setColor(QColor(color));
    complianceLine->setName(label);

    // Add horizontal line using x-axis range
    if (xAxis && xAxis->type() == QAbstractAxis::AxisTypeDateTime) {
        const QDateTimeAxis *dateTimeAxis = qobject_cast<QDateTimeAxis *>(xAxis);
        if (dateTimeAxis) {
            qint64 minX = dateTimeAxis->min().toMSecsSinceEpoch();
            qint64 maxX = dateTimeAxis->max().toMSecsSinceEpoch();
            complianceLine->append(minX, levelValue);
            complianceLine->append(maxX, levelValue);
        }
    }

    chart->addSeries(complianceLine);
    complianceLine->attachAxis(xAxis);
    complianceLine->attachAxis(yAxis);
}

// *UI Job* msg boxes for pollutant categories

void PollutantOverview::moreInfoMsgBox() {
    QMessageBox::information(this, "PCB Info", "more info about PCBs");
}

void PollutantOverview::viewListMsgBox() {
    QMessageBox::information(this, "List of Persistent Organic Pollutants", "List of PollutantOverview");
}


void PollutantOverview::heavyMetalsInfoMsgBox() {
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

void PollutantOverview::organicChemicalsInfoMsgBox() {
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

void PollutantOverview::inorganicChemicalsInfoMsgBox() {
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

void PollutantOverview::nutrientsInfoMsgBox() {
    QMessageBox::information(this, "Nutrients Info",
                             "Excess nutrients, particularly nitrogen and phosphorus, can lead to eutrophication in water bodies. "
                             "This process depletes oxygen levels, causing algal blooms and fish kills, and disrupting aquatic ecosystems.\n\n"
                             "Pollutants in this category:\n"
                             "- Nitrate\n"
                             "- Phosphate");
}

void PollutantOverview::volatileOrganicCompoundsInfoMsgBox() {
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
