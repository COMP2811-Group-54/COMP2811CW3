/*
To implement:
- for POP, PFA and PO cards, relay the number of pollutants that on average are red, green or orange for each
- for CD card, relay the number of sites that contain only greens, some oranges or some reds (or some kind of average if a better classifier thought of)
*/

#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "OverviewCards.hpp"

OverviewCards::OverviewCards(QWidget *parent): QWidget(parent)
{
    // createCards();
    createPOP();
    createPFA();
    createPO();
    createCD();
    arrangeWidgets();
}

void OverviewCards::createPOP()
{
    FramePOP = new QFrame();
    FramePOP->setFrameShape(QFrame::Box);
    FramePOP->setLineWidth(1);
    FramePOP->setFixedSize(500, 250);

    CrdPOP = new QVBoxLayout(FramePOP);

    TitlePOP = new QLabel("<h2>Persistent Organic Pollutants<h2>");
    TitlePOP->setAlignment(Qt::AlignCenter);

    QHBoxLayout* ComponentsPOP = new QHBoxLayout();
    QVBoxLayout* ExamplesPOP = new QVBoxLayout();

    ExPOP1 = new QLabel("<h2>Number of green:<h2>");
    ExPOP2 = new QLabel("<h2>Number of orange:<h2>");
    ExPOP3 = new QLabel("<h2>Number of red:<h2>");

    QFont buttonsFont("Arial", 16, QFont::Normal);
    POPsDetails = new QPushButton("Show Detailed Page");
    POPsDetails->setMinimumSize(50, 50);
    POPsDetails->setFont(buttonsFont);
    connect(POPsDetails, &QPushButton::clicked, this, &OverviewCards::goToPOPs);

    ExamplesPOP->addWidget(ExPOP1);
    ExamplesPOP->addWidget(ExPOP2);
    ExamplesPOP->addWidget(ExPOP3);

    ComponentsPOP->addLayout(ExamplesPOP);
    ComponentsPOP->addWidget(POPsDetails);

    CrdPOP->addWidget(TitlePOP);
    CrdPOP->addLayout(ComponentsPOP);
}

void OverviewCards::createPFA()
{
    FramePFA = new QFrame();
    FramePFA->setFrameShape(QFrame::Box);
    FramePFA->setLineWidth(1);
    FramePFA->setFixedSize(500, 250);

    CrdPFA = new QVBoxLayout(FramePFA);

    TitlePFA = new QLabel("<h2>Fluorinated Compounds<h2>");
    TitlePFA->setAlignment(Qt::AlignCenter);

    QHBoxLayout* ComponentsPFA = new QHBoxLayout();
    QVBoxLayout* ExamplesPFA = new QVBoxLayout();

    ExPFA1 = new QLabel("<h2>Number of green:<h2>");
    ExPFA2 = new QLabel("<h2>Number of orange:<h2>");
    ExPFA3 = new QLabel("<h2>Number of red:<h2>");

    QFont buttonsFont("Arial", 16, QFont::Normal);
    PFAsDetails = new QPushButton("Show Detailed Page");
    PFAsDetails->setMinimumSize(50, 50);
    PFAsDetails->setFont(buttonsFont);
    connect(PFAsDetails, &QPushButton::clicked, this, &OverviewCards::goToPFAs);

    ExamplesPFA->addWidget(ExPFA1);
    ExamplesPFA->addWidget(ExPFA2);
    ExamplesPFA->addWidget(ExPFA3);

    ComponentsPFA->addLayout(ExamplesPFA);
    ComponentsPFA->addWidget(PFAsDetails);

    CrdPFA->addWidget(TitlePFA);
    CrdPFA->addLayout(ComponentsPFA);
}

void OverviewCards::createPO()
{
    FramePO = new QFrame();
    FramePO->setFrameShape(QFrame::Box);
    FramePO->setLineWidth(1);
    FramePO->setFixedSize(500, 250);

    CrdPO = new QVBoxLayout(FramePO);

    TitlePO = new QLabel("<h2>Pollutant Overview<h2>");
    TitlePO->setAlignment(Qt::AlignCenter);

    QHBoxLayout* ComponentsPO = new QHBoxLayout();
    QVBoxLayout* ExamplesPO = new QVBoxLayout();

    ExPO1 = new QLabel("<h2>Number of green:<h2>");
    ExPO2 = new QLabel("<h2>Number of orange:<h2>");
    ExPO3 = new QLabel("<h2>Number of red:<h2>");

    QFont buttonsFont("Arial", 16, QFont::Normal);
    PODetails = new QPushButton("Show Detailed Page");
    PODetails->setMinimumSize(50, 50);
    PODetails->setFont(buttonsFont);
    connect(PODetails, &QPushButton::clicked, this, &OverviewCards::goToPO);

    ExamplesPO->addWidget(ExPO1);
    ExamplesPO->addWidget(ExPO2);
    ExamplesPO->addWidget(ExPO3);

    ComponentsPO->addLayout(ExamplesPO);
    ComponentsPO->addWidget(PODetails);

    CrdPO->addWidget(TitlePO);
    CrdPO->addLayout(ComponentsPO);
}

void OverviewCards::createCD()
{
    FrameCD = new QFrame();
    FrameCD->setFrameShape(QFrame::Box);
    FrameCD->setLineWidth(1);
    FrameCD->setFixedSize(500, 250);

    CrdCD = new QVBoxLayout(FrameCD);

    TitleCD = new QLabel("<h2>Compliance Dashboard<h2>");
    TitleCD->setAlignment(Qt::AlignCenter);

    QHBoxLayout* ComponentsCD = new QHBoxLayout();
    QVBoxLayout* ExamplesCD = new QVBoxLayout();

    ExCD1 = new QLabel("<h2>Number of green sites<h2>");
    ExCD2 = new QLabel("<h2>Number of orange sites<h2>");
    ExCD3 = new QLabel("<h2>Number of red sites<h2>");

    QFont buttonsFont("Arial", 16, QFont::Normal);
    CDDetails = new QPushButton("Show Detailed Page");
    CDDetails->setMinimumSize(50, 50);
    CDDetails->setFont(buttonsFont);
    connect(CDDetails, &QPushButton::clicked, this, &OverviewCards::goToCD);

    ExamplesCD->addWidget(ExCD1);
    ExamplesCD->addWidget(ExCD2);
    ExamplesCD->addWidget(ExCD3);

    ComponentsCD->addLayout(ExamplesCD);
    ComponentsCD->addWidget(CDDetails);

    CrdCD->addWidget(TitleCD);
    CrdCD->addLayout(ComponentsCD);
}


void OverviewCards::arrangeWidgets()
{
    gridLayout = new QGridLayout();

    gridLayout->addWidget(FramePOP, 0, 0);
    gridLayout->addWidget(FramePFA, 0, 1);
    gridLayout->addWidget(FramePO, 1, 0);
    gridLayout->addWidget(FrameCD, 1, 1);


    setLayout(gridLayout);
}

// void OverviewCards::createCards()
// {
//     gridLayout = new QGridLayout();

//     //Add cards and adjust font and style
//     CrdPO = new QLabel("Pollutant Overview Card");
//     CrdPFA = new QLabel("Fluorinated Compounds Card");
//     CrdLitter = new QLabel("Environmental Litter Card");
//     CrdPOP = new QLabel("Persistent Organic Pollutants Card");
//     CrdCD = new QLabel("Compliance Dashboard Card");

//     QFont cardFont("Arial", 12, QFont::Normal);
//     CrdPO->setFont(cardFont);
//     CrdPFA->setFont(cardFont);
//     CrdLitter->setFont(cardFont);
//     CrdPOP->setFont(cardFont);
//     CrdCD->setFont(cardFont);

//     CrdPO->setStyleSheet("border: 1px solid black; background-color: white;");
//     CrdPFA->setStyleSheet("border: 1px solid black; background-color: white;");
//     CrdLitter->setStyleSheet("border: 1px solid black; background-color: white;");
//     CrdPOP->setStyleSheet("border: 1px solid black; background-color: white;");
//     CrdCD->setStyleSheet("border: 1px solid black; background-color: white;");

//     //Add widgets to layout
//     gridLayout->addWidget(CrdPO, 0, 0);
//     gridLayout->addWidget(CrdPFA, 0, 1);
//     gridLayout->addWidget(CrdCD, 1, 0);
//     gridLayout->addWidget(CrdLitter, 1, 1);
//     gridLayout->addWidget(CrdPOP, 2, 0, 1, 2);

//     setLayout(gridLayout);
// }