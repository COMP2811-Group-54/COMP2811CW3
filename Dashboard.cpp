#include <QtWidgets>
#include <QtCharts>
#include <QtCore>
#include <QTranslator>
#include "Dashboard.hpp"

#include <iostream>

#include "POPs.hpp"
#include "PFAs.hpp"
#include "OverviewCards.hpp"
#include "PO.hpp"
#include "DataPage.hpp"
#include "CD.hpp"

Dashboard::Dashboard() : QWidget(), translator(new QTranslator(this)) {
    createMainLayout();
    createTopLayout();
    createLeftLayout();
    createBottomLayout();
    createStackedWidget();
    combineLayouts();

    setWindowTitle(tr("DASHBOARD_TITLE"));
}

void Dashboard::createMainLayout() {
    auto centralWidget = new QWidget();
    mainLayout = new QVBoxLayout(centralWidget);
}

void Dashboard::createTopLayout() {
    topLayout = new QHBoxLayout();

    // Add title and adjust font
    title = new QLabel(tr("DASHBOARD_TITLE"));

    QFont titleFont("Arial", 30, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    title->setMinimumSize(300, 100);

    // Add buttons and adjust font
    BtnPFA = new QPushButton(tr("DASHBOARD_PFAS"));
    BtnPOP = new QPushButton(tr("DASHBOARD_POPS"));

    QFont topBtnFont("Arial", 22, QFont::Normal);
    BtnPFA->setFont(topBtnFont);
    BtnPOP->setFont(topBtnFont);

    BtnPFA->setMinimumSize(150, 50);
    BtnPOP->setMinimumSize(150, 50);

    // Connect buttons to slots
    connect(BtnPOP, &QPushButton::clicked, this, &Dashboard::goToPOPs);
    connect(BtnPFA, &QPushButton::clicked, this, &Dashboard::goToPFAs);

    // Add ComboBox and adjust font
    language = new QComboBox();
    language->addItem("English", "en_GB");
    language->addItem("French", "fr_FR");
    language->setFont(topBtnFont);

    // Connect dropdown to language change function
    connect(language, &QComboBox::currentIndexChanged, this, &Dashboard::onLanguageChanged);

    // Add widgets to the layout
    topLayout->addWidget(title); // Add title on the far left
    topLayout->addSpacing(20);

    topLayout->addWidget(BtnPFA);
    topLayout->addSpacing(20);
    topLayout->addWidget(BtnPOP);
    topLayout->addSpacing(20);
    topLayout->addWidget(language);
}

void Dashboard::createLeftLayout() {
    sideLayout = new QVBoxLayout();

    // Add buttons and adjust size and font
    BtnDashboard = new QPushButton(tr("DASHBOARD_BUTTON"));
    BtnPO = new QPushButton(tr("DASHBOARD_PO"));
    BtnCD = new QPushButton(tr("DASHBOARD_CD"));
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

    // Add widgets to the layout
    sideLayout->addStretch();
    sideLayout->addWidget(BtnDashboard);
    sideLayout->addWidget(BtnDP);
    sideLayout->addWidget(BtnPO);
    sideLayout->addWidget(BtnCD);
    sideLayout->addStretch(); // Push everything up
    sideLayout->setSpacing(30);

    // Connect buttons to slots
    connect(BtnDashboard, &QPushButton::clicked, this, &Dashboard::goToOverviewCards);
    connect(BtnPO, &QPushButton::clicked, this, &Dashboard::goToPO);
    connect(BtnDP, &QPushButton::clicked, this, &Dashboard::goToDP);
    connect(BtnCD, &QPushButton::clicked, this, &Dashboard::goToCD);
}

void Dashboard::createBottomLayout() {
    bottomLayout = new QHBoxLayout();

    // Add buttons and adjust font
    BtnHelp = new QPushButton(tr("DASHBOARD_HELP"));
    BtnDS = new QPushButton(tr("DASHBOARD_DS"));

    QFont bottomBtnFont("Arial", 10, QFont::Normal);
    BtnHelp->setFont(bottomBtnFont);
    BtnDS->setFont(bottomBtnFont);

    // Add widgets to layout
    bottomLayout->addStretch(); // Push buttons to the right
    bottomLayout->addWidget(BtnHelp);
    bottomLayout->addWidget(BtnDS);
    bottomLayout->addStretch();
}

void Dashboard::createStackedWidget() {
    stackedLayout = new QVBoxLayout();
    stackedLayout->setSizeConstraint(QLayout::SetMinimumSize);
    QWidget *page1 = new OverviewCards();
    QWidget *page2 = new PersistentOrganicPollutants();
    QWidget *page3 = new PFApage();
    QWidget *page4 = new PollutantOverview();
    QWidget *page5 = new DataPage();
    QWidget *page6 = new ComplianceDashboard();

    pages = new QStackedWidget();
    pages->addWidget(page1);
    pages->addWidget(page2);
    pages->addWidget(page3);
    pages->addWidget(page4);
    pages->addWidget(page5);
    pages->addWidget(page6);

    pages->setMinimumSize(1250, 600);

    pagesFrame = new QFrame();
    pagesFrame->setFrameShape(QFrame::Box);
    pagesFrame->setLineWidth(2);

    stackedLayoutFrame = new QVBoxLayout(pagesFrame);
    stackedLayoutFrame->addWidget(pages);

    stackedLayout->addWidget(pagesFrame);
}

void Dashboard::combineLayouts() {
    // Combine side and grid layout into one content layout
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSizeConstraint(QLayout::SetMinimumSize);
    contentLayout->addLayout(sideLayout, 1);
    contentLayout->addLayout(stackedLayout, 6);

    // Add the top, content, and bottom layouts to the main layout
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
}

void Dashboard::onLanguageChanged(int index) {
    // Load the appropriate translation file for the language
    QString languageCode = language->currentData().toString();

    std::cout << "Selected language: " << languageCode.toStdString() << std::endl;

    // Load the appropriate translation file
    if (translator->load("languages/" + languageCode + ".qm")) {
        qApp->installTranslator(translator); // Install the translator to the application
        std::cout << "Translator loaded and installed." << std::endl;
    } else {
        std::cerr << "Failed to load translation file for: " << languageCode.toStdString() << std::endl;
    }

    // Retranslate both the dashboard and the current page
    retranslateUI();
    auto page = qobject_cast<PFApage *>(pages->currentWidget());
    if (page) {
        page->retranslateUI(); // Retranslate the current page
    }
}

void Dashboard::retranslateUI() {
    // Force re-translate of all UI components (this ensures that UI updates even if translator is installed after initial load)
    title->setText(tr("DASHBOARD_TITLE"));
    BtnPFA->setText(tr("DASHBOARD_PFAS"));
    BtnPOP->setText(tr("DASHBOARD_POPS"));
    BtnDashboard->setText(tr("DASHBOARD_BUTTON"));
    BtnPO->setText(tr("DASHBOARD_PO"));
    BtnCD->setText(tr("DASHBOARD_CD"));
    BtnHelp->setText(tr("DASHBOARD_HELP"));
    BtnDS->setText(tr("DASHBOARD_DS"));
    language->setItemText(0, tr("English"));
    language->setItemText(1, tr("French"));

    // Retranslate the current page as well (will be updated to be more efficient)
    auto page = qobject_cast<PFApage *>(pages->currentWidget());
    if (page) {
        page->retranslateUI();
    }

    auto page_1 = qobject_cast<PersistentOrganicPollutants *>(pages->currentWidget());
    if (page_1) {
        std::cout << "retranslating" << std::endl;
        page_1->retranslateUI();
    }
}
