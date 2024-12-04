#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "OverviewCards.hpp"

OverviewCards::OverviewCards(QWidget *parent): QWidget(parent)
{
    createCards();
    setWindowTitle("Overview Cards");
}

void OverviewCards::createCards()
{
    gridLayout = new QGridLayout();

    //Add cards and adjust font and style
    CrdPO = new QLabel("Pollutant Overview Card");
    CrdPFA = new QLabel("Fluorinated Compounds Card");
    CrdLitter = new QLabel("Environmental Litter Card");
    CrdPOP = new QLabel("Persistent Organic Pollutants Card");
    CrdCD = new QLabel("Compliance Dashboard Card");

    QFont cardFont("Arial", 12, QFont::Normal);
    CrdPO->setFont(cardFont);
    CrdPFA->setFont(cardFont);
    CrdLitter->setFont(cardFont);
    CrdPOP->setFont(cardFont);
    CrdCD->setFont(cardFont);

    CrdPO->setStyleSheet("border: 1px solid black; background-color: white;");
    CrdPFA->setStyleSheet("border: 1px solid black; background-color: white;");
    CrdLitter->setStyleSheet("border: 1px solid black; background-color: white;");
    CrdPOP->setStyleSheet("border: 1px solid black; background-color: white;");
    CrdCD->setStyleSheet("border: 1px solid black; background-color: white;");

    //Add widgets to layout
    gridLayout->addWidget(CrdPO, 0, 0);
    gridLayout->addWidget(CrdPFA, 0, 1);
    gridLayout->addWidget(CrdCD, 1, 0);
    gridLayout->addWidget(CrdLitter, 1, 1);
    gridLayout->addWidget(CrdPOP, 2, 0, 1, 2); 

    setLayout(gridLayout);
}