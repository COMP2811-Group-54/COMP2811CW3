#include "OverviewCards.hpp"

#include <iostream>
#include <ostream>

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
}

void OverviewCards::createPO() {
    FramePO = new QFrame();
    FramePO->setFrameShape(QFrame::Box);
    FramePO->setLineWidth(1);

    CrdPO = new QVBoxLayout(FramePO);

    TitlePO = new QLabel(tr("OC_PO_TITLE"));
    TitlePO->setAlignment(Qt::AlignCenter);


    QHBoxLayout *ComponentsPO = new QHBoxLayout();
    QVBoxLayout *ExamplesPO = new QVBoxLayout();

    ExPO1 = new QLabel(tr("OC_EX_PO1"));
    ExPO2 = new QLabel(tr("OC_EX_PO2"));
    ExPO3 = new QLabel(tr("OC_EX_PO3"));

    QFont buttonsFont("Arial", 16, QFont::Normal);
    PODetails = new QPushButton(tr("OC_PO_DETAILS"));
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

    CrdPOP = new QVBoxLayout(FramePOP);

    TitlePOP = new QLabel(tr("OC_POP_TITLE"));
    TitlePOP->setAlignment(Qt::AlignCenter);

    QHBoxLayout *ComponentsPOP = new QHBoxLayout();
    QVBoxLayout *ExamplesPOP = new QVBoxLayout();

    ExPOP1 = new QLabel(tr("OC_EX_POP1"));
    ExPOP2 = new QLabel(tr("OC_EX_POP2"));
    ExPOP3 = new QLabel(tr("OC_EX_POP3"));

    QFont buttonsFont("Arial", 16, QFont::Normal);
    POPsDetails = new QPushButton(tr("OC_POP_DETAILS"));
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

    CrdPFA = new QVBoxLayout(FramePFA);

    TitlePFA = new QLabel(tr("OC_PFA_TITLE"));
    TitlePFA->setAlignment(Qt::AlignCenter);

    QHBoxLayout *ComponentsPFA = new QHBoxLayout();
    QVBoxLayout *ExamplesPFA = new QVBoxLayout();

    ExPFA1 = new QLabel(tr("OC_EX_PFA1"));
    ExPFA2 = new QLabel(tr("OC_EX_PFA2"));
    ExPFA3 = new QLabel(tr("OC_EX_PFA3"));

    QFont buttonsFont("Arial", 16, QFont::Normal);
    PFAsDetails = new QPushButton(tr("OC_PFA_DETAILS"));
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
    FrameCD->setFrameShape(QFrame::Box);
    FrameCD->setLineWidth(1);

    CrdCD = new QVBoxLayout(FrameCD);

    TitleCD = new QLabel(tr("OC_CD_TITLE"));
    TitleCD->setAlignment(Qt::AlignCenter);

    QHBoxLayout *ComponentsCD = new QHBoxLayout();
    QVBoxLayout *ExamplesCD = new QVBoxLayout();

    ExCD1 = new QLabel(tr("OC_EX_PFA1"));
    ExCD2 = new QLabel(tr("OC_EX_PFA2"));
    ExCD3 = new QLabel(tr("OC_EX_PFA3"));

    QFont buttonsFont("Arial", 16, QFont::Normal);
    CDDetails = new QPushButton(tr("OC_CD_DETAILS"));
    CDDetails->setMinimumSize(50, 50);
    CDDetails->setFont(buttonsFont);

    connect(CDDetails, &QPushButton::clicked, this, &OverviewCards::goToCD);

    // Arrange the widgets in the layout
    ComponentsCD->addLayout(ExamplesCD);
    ComponentsCD->addWidget(CDDetails);

    ExamplesCD->addWidget(ExCD1);
    ExamplesCD->addWidget(ExCD2);
    ExamplesCD->addWidget(ExCD3);

    CrdCD->addWidget(TitleCD);
    CrdCD->addLayout(ComponentsCD);
}

void OverviewCards::arrangeWidgets() {
    gridLayout = new QGridLayout(this);
    gridLayout->addWidget(FramePOP, 0, 0);
    gridLayout->addWidget(FramePFA, 0, 1);
    gridLayout->addWidget(FramePO, 1, 0);
    gridLayout->addWidget(FrameCD, 1, 1);

    gridLayout->setHorizontalSpacing(50);
    gridLayout->setVerticalSpacing(50);
    gridLayout->setContentsMargins(50, 50, 50, 50);

    gridLayout->setRowStretch(0, 1);
    gridLayout->setRowStretch(1, 1);
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);

    setLayout(gridLayout);
}

void OverviewCards::retranslateUI() {
    TitlePO->setText(tr("OC_PO_TITLE"));
    PODetails->setText(tr("OC_PO_DETAILS"));
    TitlePOP->setText(tr("OC_POP_TITLE"));
    POPsDetails->setText(tr("OC_POP_DETAILS"));
    TitlePFA->setText(tr("OC_PFA_TITLE"));
    PFAsDetails->setText(tr("OC_PFA_DETAILS"));
    TitleCD->setText(tr("OC_CD_TITLE"));
    CDDetails->setText(tr("OC_CD_DETAILS"));

    // Update text for POP
    ExPOP1->setText(QString(tr("OC_EX_POP1") + " %1").arg(popGreen));
    ExPOP2->setText(QString(tr("OC_EX_POP2") + " %1").arg(popOrange));
    ExPOP3->setText(QString(tr("OC_EX_POP3") + " %1").arg(popRed));

    // Update text for PFA
    ExPFA1->setText(QString(tr("OC_EX_PFA1") + " %1").arg(pfaGreen));
    ExPFA2->setText(QString(tr("OC_EX_PFA2") + " %1").arg(pfaOrange));
    ExPFA3->setText(QString(tr("OC_EX_PFA3") + " %1").arg(pfaRed));

    // Update text for PO (metals and VOCs combined metrics)
    ExPO1->setText(QString(tr("OC_EX_PO1") + " %1").arg(metalGreen + vocGreen));
    ExPO2->setText(QString(tr("OC_EX_PO2") + " %1").arg(metalOrange + vocOrange));
    ExPO3->setText(QString(tr("OC_EX_PO3") + " %1").arg(metalRed + vocRed));

    // Update text for Locations
    ExCD1->setText(QString(tr("OC_EX_PFA1") + " %1").arg(locationGreen));
    ExCD2->setText(QString(tr("OC_EX_PFA1") + " %1").arg(locationGreen));
    ExCD3->setText(QString(tr("OC_EX_PFA1") + " %1").arg(locationRed));
}

void OverviewCards::updateDataDisplays() {
    auto &dataset = GlobalDataModel::instance().getDataset();
    ComplianceChecker complianceChecker;

    std::cout << "UPDATING DATA" << std::endl;

    popGreen = 0, popOrange = 0, popRed = 0;
    pfaGreen = 0, pfaOrange = 0, pfaRed = 0;
    metalGreen = 0, metalOrange = 0, metalRed = 0;
    vocGreen = 0, vocOrange = 0, vocRed = 0;
    locationGreen = 0, locationOrange = 0, locationRed = 0;

    // Fetch compound data only once, to avoid calling the getter each time in the loop
    auto pops = ComplianceChecker::getPOPs();
    auto pfas = ComplianceChecker::getPFAs();
    auto metals = ComplianceChecker::getMetals();
    auto vocs = ComplianceChecker::getVOCs();

    auto locations = GlobalDataModel::instance().getDataset().getHighDataPointLocations();

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
            } else if (complianceStatus == 2 && locationTiers[locationName] < 3) {
                locationTiers[locationName] = 2;
            } else if (complianceStatus == 1 && locationTiers[locationName] < 2) {
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

    for (const auto &[key, value]: locationTiers) {
        if (value == 1) {
            locationGreen++;
        } else if (value == 2) {
            locationOrange++;
        } else if (value == 3) {
            locationRed++;
        }
    }


    // Update text for POP
    ExPOP1->setText(QString(tr("OC_EX_POP1") + " %1").arg(popGreen));
    ExPOP2->setText(QString(tr("OC_EX_POP2") + " %1").arg(popOrange));
    ExPOP3->setText(QString(tr("OC_EX_POP3") + " %1").arg(popRed));

    // Update text for PFA
    ExPFA1->setText(QString(tr("OC_EX_PFA1") + " %1").arg(pfaGreen));
    ExPFA2->setText(QString(tr("OC_EX_PFA2") + " %1").arg(pfaOrange));
    ExPFA3->setText(QString(tr("OC_EX_PFA3") + " %1").arg(pfaRed));

    // Update text for PO (metals and VOCs combined metrics)
    ExPO1->setText(QString(tr("OC_EX_PO1") + " %1").arg(metalGreen + vocGreen));
    ExPO2->setText(QString(tr("OC_EX_PO2") + " %1").arg(metalOrange + vocOrange));
    ExPO3->setText(QString(tr("OC_EX_PO3") + " %1").arg(metalRed + vocRed));

    // Update text for Locations
    ExCD1->setText(QString(tr("OC_EX_PFA1") + " %1").arg(locationGreen));
    ExCD2->setText(QString(tr("OC_EX_PFA1") + " %1").arg(locationGreen));
    ExCD3->setText(QString(tr("OC_EX_PFA1") + " %1").arg(locationRed));
}
