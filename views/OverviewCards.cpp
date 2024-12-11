#include "OverviewCards.hpp"
#include "../utils/Compliance.hpp"
#include "../utils/GlobalDataModel.hpp"

OverviewCards::OverviewCards(QWidget *parent) : QWidget(parent) {
    createPOP();
    createPFA();
    createPO();
    createCD();
    arrangeWidgets();

    // Connect the dataReady signal to the updateDataDisplays slot
    connect(&GlobalDataModel::instance(), &GlobalDataModel::dataReady, this, &OverviewCards::updateDataDisplays);

    updateDataDisplays(); // Initial call to populate data once construction is complete
}

void OverviewCards::createPO() {
    FramePO = new QFrame();
    FramePO->setFrameShape(QFrame::Box);
    FramePO->setLineWidth(1);
    FramePO->setFixedSize(500, 250);

    CrdPO = new QVBoxLayout(FramePO);

    TitlePO = new QLabel("<h2>Metals and Volatile Organic Compounds<h2>");
    TitlePO->setAlignment(Qt::AlignCenter);

    QHBoxLayout *ComponentsPO = new QHBoxLayout();
    QVBoxLayout *ExamplesPO = new QVBoxLayout();

    ExPO1 = new QLabel("<h2>Number of green:<h2>");
    ExPO2 = new QLabel("<h2>Number of orange:<h2>");
    ExPO3 = new QLabel("<h2>Number of red:<h2>");

    QFont buttonsFont("Arial", 16, QFont::Normal);
    PODetails = new QPushButton("Show Detailed Page");
    PODetails->setMinimumSize(50, 50);
    PODetails->setFont(buttonsFont);

    connect(PODetails, &QPushButton::clicked, this, &OverviewCards::goToPO);

    // Arrange the widgets in the layout
    ComponentsPO->addLayout(ExamplesPO);
    ComponentsPO->addWidget(PODetails);

    ExamplesPO->addWidget(ExPO1);
    ExamplesPO->addWidget(ExPO2);
    ExamplesPO->addWidget(ExPO3);

    CrdPO->addWidget(TitlePO);
    CrdPO->addLayout(ComponentsPO);
}

void OverviewCards::createPOP() {
    FramePOP = new QFrame();
    FramePOP->setFrameShape(QFrame::Box);
    FramePOP->setLineWidth(1);
    FramePOP->setFixedSize(500, 250);

    CrdPOP = new QVBoxLayout(FramePOP);

    TitlePOP = new QLabel("<h2>Persistent Organic Pollutants<h2>");
    TitlePOP->setAlignment(Qt::AlignCenter);

    QHBoxLayout *ComponentsPOP = new QHBoxLayout();
    QVBoxLayout *ExamplesPOP = new QVBoxLayout();

    ExPOP1 = new QLabel("<h2>Number of green:<h2>");
    ExPOP2 = new QLabel("<h2>Number of orange:<h2>");
    ExPOP3 = new QLabel("<h2>Number of red:<h2>");

    QFont buttonsFont("Arial", 16, QFont::Normal);
    POPsDetails = new QPushButton("Show Detailed Page");
    POPsDetails->setMinimumSize(50, 50);
    POPsDetails->setFont(buttonsFont);

    connect(POPsDetails, &QPushButton::clicked, this, &OverviewCards::goToPOPs);

    // Arrange the widgets in the layout
    ComponentsPOP->addLayout(ExamplesPOP);
    ComponentsPOP->addWidget(POPsDetails);

    ExamplesPOP->addWidget(ExPOP1);
    ExamplesPOP->addWidget(ExPOP2);
    ExamplesPOP->addWidget(ExPOP3);

    CrdPOP->addWidget(TitlePOP);
    CrdPOP->addLayout(ComponentsPOP);
}

void OverviewCards::createPFA() {
    FramePFA = new QFrame();
    FramePFA->setFrameShape(QFrame::Box);
    FramePFA->setLineWidth(1);
    FramePFA->setFixedSize(500, 250);

    CrdPFA = new QVBoxLayout(FramePFA);

    TitlePFA = new QLabel("<h2>Per- and Polyfluoroalkyl Substances<h2>");
    TitlePFA->setAlignment(Qt::AlignCenter);

    QHBoxLayout *ComponentsPFA = new QHBoxLayout();
    QVBoxLayout *ExamplesPFA = new QVBoxLayout();

    ExPFA1 = new QLabel("<h2>Number of green:<h2>");
    ExPFA2 = new QLabel("<h2>Number of orange:<h2>");
    ExPFA3 = new QLabel("<h2>Number of red:<h2>");

    QFont buttonsFont("Arial", 16, QFont::Normal);
    PFAsDetails = new QPushButton("Show Detailed Page");
    PFAsDetails->setMinimumSize(50, 50);
    PFAsDetails->setFont(buttonsFont);

    connect(PFAsDetails, &QPushButton::clicked, this, &OverviewCards::goToPFAs);

    // Arrange the widgets in the layout
    ComponentsPFA->addLayout(ExamplesPFA);
    ComponentsPFA->addWidget(PFAsDetails);

    ExamplesPFA->addWidget(ExPFA1);
    ExamplesPFA->addWidget(ExPFA2);
    ExamplesPFA->addWidget(ExPFA3);

    CrdPFA->addWidget(TitlePFA);
    CrdPFA->addLayout(ComponentsPFA);
}

void OverviewCards::createCD() {
    FrameCD = new QFrame();
    CrdCD = new QVBoxLayout(FrameCD);
    TitleCD = new QLabel("Location Compliance");
    ExCD1 = new QLabel();
    ExCD2 = new QLabel();
    ExCD3 = new QLabel();
    CDDetails = new QPushButton("Show Detailed Page");

    connect(CDDetails, &QPushButton::clicked, this, &OverviewCards::goToCD);

    CrdCD->addWidget(TitleCD);
    CrdCD->addWidget(ExCD1);
    CrdCD->addWidget(ExCD2);
    CrdCD->addWidget(ExCD3);
    CrdCD->addWidget(CDDetails);
}

void OverviewCards::arrangeWidgets() {
    gridLayout = new QGridLayout(this);
    gridLayout->addWidget(FramePOP, 0, 0);
    gridLayout->addWidget(FramePFA, 0, 1);
    gridLayout->addWidget(FramePO, 1, 0);
    gridLayout->addWidget(FrameCD, 1, 1);
    setLayout(gridLayout);
}

void OverviewCards::updateDataDisplays() {
    auto &dataset = GlobalDataModel::instance().getDataset();
    ComplianceChecker complianceChecker;

    int popGreen = 0, popOrange = 0, popRed = 0;
    int pfaGreen = 0, pfaOrange = 0, pfaRed = 0;
    int metalGreen = 0, metalOrange = 0, metalRed = 0;
    int vocGreen = 0, vocOrange = 0, vocRed = 0;

    // Fetch compound data only once, to avoid calling the getter each time in the loop
    auto pops = ComplianceChecker::getPOPs();
    auto pfas = ComplianceChecker::getPFAs();
    auto metals = ComplianceChecker::getMetals();
    auto vocs = ComplianceChecker::getVOCs();

    auto locations = ComplianceChecker::getLocations();

    // This map holds each location and its assigned tier (r/o/g, 3/2/1)
    unordered_map<string, int> locationTiers;

    for (const auto &measurement: dataset) {
        std::string compoundName = measurement.getCompoundName();
        double value = measurement.getValue();

        std::string locationName = measurement.getLabel();

        int complianceStatus = complianceChecker.complianceCheck(compoundName, value);

        // Check the compliance of the measurement at the location
        if (std::find(locations.begin(), locations.end(), locationName) != locations.end()) {

            // Locations sorted into tiers (correspponding to r/o/g)
            if (complianceStatus == 3) {
                locationTiers[locationName] = 3;
            }
            else if (complianceStatus == 2 && locationTiers[locationName] < 3) {
                locationTiers[locationName] = 2;
            }
            else if (complianceStatus == 1 && locationTiers[locationName] < 2) {
                locationTiers[locationName] = 1;
            }
        }

        // Check compliance for POPs
        if (std::find(pops.begin(), pops.end(), compoundName) != pops.end()) {
            if (complianceStatus == 1)
                popGreen++;
            else if (complianceStatus == 2)
                popOrange++;
            else if (complianceStatus == 3)
                popRed++;
        }
        // Check compliance for PFAs
        else if (std::find(pfas.begin(), pfas.end(), compoundName) != pfas.end()) {
            if (complianceStatus == 1)
                pfaGreen++;
            else if (complianceStatus == 2)
                pfaOrange++;
            else if (complianceStatus == 3)
                pfaRed++;
        }
        // Check compliance for Metals
        else if (std::find(metals.begin(), metals.end(), compoundName) != metals.end()) {
            if (complianceStatus == 1)
                metalGreen++;
            else if (complianceStatus == 2)
                metalOrange++;
            else if (complianceStatus == 3)
                metalRed++;
        }
        // Check compliance for VOCs
        else if (std::find(vocs.begin(), vocs.end(), compoundName) != vocs.end()) {
            if (complianceStatus == 1)
                vocGreen++;
            else if (complianceStatus == 2)
                vocOrange++;
            else if (complianceStatus == 3)
                vocRed++;
        }
    }

    // Count number of locations in each category (r/o/g)
    int greenLocations = 0;
    int orangeLocations = 0;
    int redLocations = 0;

    for (const auto& [key, value] : locationTiers) {
        if (value == 1) {
            greenLocations++;
        }
        else if (value == 2) {
            orangeLocations++;
        }
        else if (value == 3) {
            redLocations++;
        }
    }

    // Update text for POP
    ExPOP1->setText(QString("Number of Green: %1").arg(popGreen));
    ExPOP2->setText(QString("Number of Orange: %1").arg(popOrange));
    ExPOP3->setText(QString("Number of Red: %1").arg(popRed));

    // Update text for PFA
    ExPFA1->setText(QString("Number of Green: %1").arg(pfaGreen));
    ExPFA2->setText(QString("Number of Orange: %1").arg(pfaOrange));
    ExPFA3->setText(QString("Number of Red: %1").arg(pfaRed));

    // Update text for PO (metals and VOCs combined metrics)
    ExPO1->setText(QString("Number of Green: %1").arg(metalGreen + vocGreen));
    ExPO2->setText(QString("Number of Orange: %1").arg(metalOrange + vocOrange));
    ExPO3->setText(QString("Number of Red: %1").arg(metalRed + vocRed));

    // Update text for locations
    ExCD1->setText(QString("Green Locations: %1").arg(greenLocations));
    ExCD2->setText(QString("Orange Locations: %1").arg(orangeLocations));
    ExCD3->setText(QString("Red Locations: %1").arg(redLocations));
}
