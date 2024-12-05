#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include "Dashboard.hpp"
#include "POPs.hpp"
#include "PFAs.hpp"
#include "OverviewCards.hpp"
#include "PO.hpp"
#include "window.hpp"

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
    title->setMinimumSize(300, 100);

    //Add buttons and adjust font
    BtnPFA = new QPushButton("PFAs");
    BtnPOP = new QPushButton("POPs");

    QFont topBtnFont("Arial", 22, QFont::Normal);
    BtnPFA->setFont(topBtnFont);
    BtnPOP->setFont(topBtnFont);

    BtnPFA->setMinimumSize(150, 50);
    BtnPOP->setMinimumSize(150, 50);

    //Connect buttons to slots
    connect(BtnPOP, &QPushButton::clicked, this, &Dashboard::goToPOPs);
    connect(BtnPFA, &QPushButton::clicked, this, &Dashboard::goToPFAs);

    //Add ComboBox and adjust font
    language = new QComboBox();
    language->addItem("English");
    language->addItem("Arabic");
    language->setFont(topBtnFont);


    //Add widgets to the layout
    topLayout->addWidget(title);  // Add title on the far left
    topLayout->addSpacing(20);  

    topLayout->addWidget(BtnPFA);
    topLayout->addSpacing(20);
    topLayout->addWidget(BtnPOP);
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
    BtnDP = new QPushButton("Data Page");

    BtnDashboard->setMinimumSize(200, 100);
    BtnPO->setMinimumSize(200, 100);
    BtnCD->setMinimumSize(200, 100);
    BtnDP->setMinimumSize(200, 100);

    QFont sideBtnFont("Arial", 13, QFont::Bold);
    BtnDashboard->setFont(sideBtnFont);
    BtnPO->setFont(sideBtnFont);
    BtnCD->setFont(sideBtnFont);
    BtnDP->setFont(sideBtnFont);

    //Add widgets to the layout
    sideLayout->addStretch();
    sideLayout->addWidget(BtnDashboard);
    sideLayout->addWidget(BtnDP);
    sideLayout->addWidget(BtnPO);
    sideLayout->addWidget(BtnCD);
    sideLayout->addStretch(); // Push everything up
    sideLayout->setSpacing(30);

    //Connect buttons to slots
    connect(BtnDashboard, &QPushButton::clicked, this, &Dashboard::goToOverviewCards);
    connect(BtnPO, &QPushButton::clicked, this, &Dashboard::goToPO);
    connect(BtnDP, &QPushButton::clicked, this, &Dashboard::goToDP);
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
    stackedLayout->setSizeConstraint(QLayout::SetMinimumSize);
    QWidget *page1 = new OverviewCards();
    QWidget *page2 = new PersistentOrganicPollutants();
    QWidget *page3 = new PFApage();
    QWidget *page4 = new PollutantOverview();
    QWidget *page5 = new DataPage();

    pages = new QStackedWidget();
    pages->addWidget(page1);
    pages->addWidget(page2);
    pages->addWidget(page3);
    pages->addWidget(page4);
    pages->addWidget(page5);

    pages->setMinimumSize(1250, 600);

    pagesFrame = new QFrame();
    pagesFrame->setFrameShape(QFrame::Box);
    pagesFrame->setLineWidth(2);

    stackedLayoutFrame = new QVBoxLayout(pagesFrame);
    stackedLayoutFrame->addWidget(pages);

    stackedLayout->addWidget(pagesFrame);
}

void Dashboard::combineLayouts()
{
    //Combine side and grid layout into one content layout
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSizeConstraint(QLayout::SetMinimumSize);
    contentLayout->addLayout(sideLayout, 1);
    contentLayout->addLayout(stackedLayout, 6);

    //Add the top, content, and bottom layouts to the main layout
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
}