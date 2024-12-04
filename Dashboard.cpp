#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "Dashboard.hpp"
#include "POPs.hpp"
#include "PFAs.hpp"
#include "OverviewCards.hpp"

Dashboard::Dashboard(): QWidget()
{
    createMainLayout();
    createTopLayout();
    createLeftLayout();
    createBottomLayout();
    createStackedWidget();
    combineLayouts();

    setWindowTitle("Dashboard");
}

void Dashboard::createMainLayout()
{
    auto centralWidget = new QWidget();

    mainLayout = new QVBoxLayout(centralWidget);
}

void Dashboard::createTopLayout()
{
    topLayout = new QHBoxLayout();
    
    //Add title and adjust font
    title = new QLabel("Water Quality Monitor");
    
    QFont titleFont("Arial", 30, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    title->setMinimumSize(450, 100);

    //Add buttons and adjust font
    BtnPFA = new QPushButton("PFAs");
    BtnPOP = new QPushButton("POPs");
    BtnLitter = new QPushButton("Litter");

    QFont topBtnFont("Arial", 22, QFont::Normal);
    BtnPFA->setFont(topBtnFont);
    BtnPOP->setFont(topBtnFont);
    BtnLitter->setFont(topBtnFont);

    BtnPFA->setMinimumSize(150, 50);
    BtnPOP->setMinimumSize(150, 50);
    BtnLitter->setMinimumSize(150, 50);

    //Connect buttons to slots
    connect(BtnPOP, &QPushButton::clicked, this, &Dashboard::goToPOPs);
    connect(BtnPFA, &QPushButton::clicked, this, &Dashboard::goToPFAs);

    //Add ComboBox and adjust font
    language = new QComboBox();
    language->addItem("English");
    language->addItem("Arabic");
    language->setFont(topBtnFont);


    //Add widgets to the layout
    topLayout->addWidget(title);    // Add title on the far left
    topLayout->addSpacing(20);  

    topLayout->addWidget(BtnPFA);
    topLayout->addSpacing(20);
    topLayout->addWidget(BtnPOP);
    topLayout->addSpacing(20);
    topLayout->addWidget(BtnLitter);
    topLayout->addSpacing(20);
    topLayout->addWidget(language);
}

void Dashboard::createLeftLayout()
{
    sideLayout = new QVBoxLayout();

    //Add buttons and adjust size and font
    BtnDashboard = new QPushButton("Dashboard");
    BtnPO = new QPushButton("Pollutant Overview");
    BtnCD = new QPushButton("Compliance Dashboard");

    BtnDashboard->setMinimumSize(200, 100);
    BtnPO->setMinimumSize(200, 100);
    BtnCD->setMinimumSize(200, 100);

    QFont sideBtnFont("Arial", 13, QFont::Bold);
    BtnDashboard->setFont(sideBtnFont);
    BtnPO->setFont(sideBtnFont);
    BtnCD->setFont(sideBtnFont);

    //Add widgets to the layout
    sideLayout->addStretch();
    sideLayout->addWidget(BtnDashboard);
    sideLayout->addWidget(BtnPO);
    sideLayout->addWidget(BtnCD);
    sideLayout->addStretch(); // Push everything up
    sideLayout->setSpacing(30);

    //Connect buttons to slots
    connect(BtnDashboard, &QPushButton::clicked, this, &Dashboard::goToOverviewCards);
}

void Dashboard::createBottomLayout()
{
    bottomLayout = new QHBoxLayout();

    //Add buttons and adjust font
    BtnHelp = new QPushButton("Help");
    BtnDS = new QPushButton("Data Sources");

    QFont bottomBtnFont("Arial", 10, QFont::Normal);
    BtnHelp->setFont(bottomBtnFont);
    BtnDS->setFont(bottomBtnFont);

    //Add widgets to layout
    bottomLayout->addStretch(); // Push buttons to the right
    bottomLayout->addWidget(BtnHelp);
    bottomLayout->addWidget(BtnDS);
    bottomLayout->addStretch();
}

void Dashboard::createStackedWidget()
{
    stackedLayout = new QVBoxLayout();
    QWidget *page1 = new OverviewCards();
    QWidget *page2 = new PersistentOrganicPollutants();
    QWidget *page3 = new PFApage();

    pages = new QStackedWidget();
    pages->addWidget(page1);
    pages->addWidget(page2);
    pages->addWidget(page3);

    pages->setMinimumSize(800, 800);

    stackedLayout->addWidget(pages);
}

void Dashboard::combineLayouts()
{
    //Combine side and grid layout into one content layout
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->addLayout(sideLayout, 1);
    contentLayout->addLayout(stackedLayout, 6);

    //Add the top, content, and bottom layouts to the main layout
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
}