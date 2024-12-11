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
#include "GH.hpp"

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

void Dashboard::createStackedWidget() {
    stackedLayout = new QVBoxLayout();
    stackedLayout->setSizeConstraint(QLayout::SetMinimumSize);
    OverviewCards *page1 = new OverviewCards();
    PersistentOrganicPollutants *page2 = new PersistentOrganicPollutants();
    PFApage *page3 = new PFApage();
    PollutantOverview *page4 = new PollutantOverview();
    DataPage *page5 = new DataPage();
    ComplianceDashboard *page6 = new ComplianceDashboard();
    GeographicalHotspotsPage *page7 = new GeographicalHotspotsPage();


    pages = new QStackedWidget();
    pages->addWidget(page1);
    pages->addWidget(page2);
    pages->addWidget(page3);
    pages->addWidget(page4);
    pages->addWidget(page5);
    pages->addWidget(page6);
    pages->addWidget(page7);

    connect(page1, &OverviewCards::goToPOPs, this, [this]() {
        pages->setCurrentIndex(1);
    });

    connect(page1, &OverviewCards::goToPFAs, this, [this]() {
        pages->setCurrentIndex(2);
    });

    connect(page1, &OverviewCards::goToPO, this, [this]() {
        pages->setCurrentIndex(3);
    });

    connect(page1, &OverviewCards::goToCD, this, [this]() {
        pages->setCurrentIndex(4);
    });


    pages->setMinimumSize(1250, 600);

    stackedLayout->addWidget(pages);
}

void Dashboard::createTopLayout() {
    topLayout = new QHBoxLayout();

    // Add title
    title = new QLabel(tr("DASHBOARD_TITLE"));
    title->setObjectName("title"); // Set object name for styling

    QFont titleFont("Arial", 30, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    title->setMinimumSize(300, 100);

    // Add buttons
    BtnPO = new QPushButton(tr("DASHBOARD_PO"));
    BtnPO->setObjectName("BtnPO");
    BtnPFA = new QPushButton(tr("DASHBOARD_PFAS"));
    BtnPOP = new QPushButton(tr("DASHBOARD_POPS"));
    BtnPFA->setObjectName("BtnPFA");
    BtnPOP->setObjectName("BtnPOP");
    QShortcut *POsc = new QShortcut(QKeySequence("2"), this);
    QShortcut *PFAsc = new QShortcut(QKeySequence("3"), this);
    QShortcut *POPsc = new QShortcut(QKeySequence("4"), this);


    QFont topBtnFont("Arial", 22, QFont::Normal);
    BtnPFA->setFont(topBtnFont);
    BtnPOP->setFont(topBtnFont);
    BtnPO->setFont(topBtnFont);

    BtnPO->setMinimumSize(150, 50);
    BtnPFA->setMinimumSize(150, 50);
    BtnPOP->setMinimumSize(150, 50);

    // Adjust text size to fit each top button
    adjustButtonTextToFit(BtnPO);
    adjustButtonTextToFit(BtnPFA);
    adjustButtonTextToFit(BtnPOP);

    // Add ComboBox
    language = new QComboBox();
    language->addItem("English", "en_GB");
    language->addItem("French", "fr_FR");
    language->setFont(topBtnFont);
    language->setFixedSize(150, 50);

    // Connect signals for buttons and language dropdown
    connect(BtnPO, &QPushButton::clicked, this, &Dashboard::goToPO);
    connect(POsc, &QShortcut::activated, BtnPO, &QPushButton::click);
    connect(BtnPFA, &QPushButton::clicked, this, &Dashboard::goToPFAs);
    connect(PFAsc, &QShortcut::activated, BtnPFA, &QPushButton::click);
    connect(BtnPOP, &QPushButton::clicked, this, &Dashboard::goToPOPs);
    connect(POPsc, &QShortcut::activated, BtnPOP, &QPushButton::click);
    connect(language, &QComboBox::currentIndexChanged, this, &Dashboard::onLanguageChanged);

    // Add widgets to the layout
    topLayout->addWidget(title); // Add title
    topLayout->addSpacing(20);
    topLayout->addWidget(BtnPO);
    topLayout->addSpacing(20);
    topLayout->addWidget(BtnPFA);
    topLayout->addSpacing(20);
    topLayout->addWidget(BtnPOP);
    topLayout->addSpacing(20);
    topLayout->addWidget(language);
}

void Dashboard::createLeftLayout() {
    sideLayout = new QVBoxLayout();

    // Add buttons
    BtnDashboard = new QPushButton(tr("DASHBOARD_BUTTON"));
    BtnDashboard->setObjectName("BtnDashboard"); // Set object name for styling
    BtnCD = new QPushButton(tr("DASHBOARD_CD"));
    BtnCD->setObjectName("BtnCD");
    BtnDP = new QPushButton(tr("DASHBOARD_DP"));
    BtnDP->setObjectName("BtnDP");
    BtnGH = new QPushButton(tr("DASHBOARD_GH"));
    BtnGH->setObjectName("BtnGH");

    QShortcut *DASHsc = new QShortcut(QKeySequence("1"), this);
    QShortcut *DPsc = new QShortcut(QKeySequence("5"), this);
    QShortcut *CDsc = new QShortcut(QKeySequence("6"), this);
    QShortcut *GHsc = new QShortcut(QKeySequence("7"), this);

    connect(DASHsc, &QShortcut::activated, BtnDashboard, &QPushButton::click);
    connect(DPsc, &QShortcut::activated, BtnDP, &QPushButton::click);
    connect(CDsc, &QShortcut::activated, BtnCD, &QPushButton::click);
    connect(GHsc, &QShortcut::activated, BtnGH, &QPushButton::click);

    BtnDashboard->setMinimumSize(200, 200);
    BtnCD->setMinimumSize(200, 100);
    BtnDP->setMinimumSize(200, 100);
    BtnGH->setMinimumSize(200, 100);

    QFont sideBtnFont("Arial", 13, QFont::Bold);
    BtnDashboard->setFont(sideBtnFont);
    BtnCD->setFont(sideBtnFont);
    BtnDP->setFont(sideBtnFont);
    BtnGH->setFont(sideBtnFont);

    // Adjust text size to fit each button
    adjustButtonTextToFit(BtnDashboard);
    adjustButtonTextToFit(BtnCD);
    adjustButtonTextToFit(BtnDP);
    adjustButtonTextToFit(BtnGH);

    // Connect signals for side buttons to the specific functions
    connect(BtnDashboard, &QPushButton::clicked, this, &Dashboard::goToOverviewCards);
    connect(BtnCD, &QPushButton::clicked, this, &Dashboard::goToCD);
    connect(BtnDP, &QPushButton::clicked, this, &Dashboard::goToDP);
    connect(BtnGH, &QPushButton::clicked, this, &Dashboard::goToGH);

    // Add widgets to the layout
    sideLayout->addSpacing(20);
    sideLayout->addStretch();
    sideLayout->addWidget(BtnDashboard);
    sideLayout->addSpacing(20);
    sideLayout->addWidget(BtnDP);
    sideLayout->addSpacing(20);
    sideLayout->addWidget(BtnCD);
    sideLayout->addSpacing(20);
    sideLayout->addWidget(BtnGH);
    sideLayout->addSpacing(20);
    sideLayout->addStretch();
}

void Dashboard::adjustButtonTextToFit(QPushButton *button) {
    QString text = button->text();
    QFont font = button->font();
    QFontMetrics metrics(font);

    int width = button->width();

    while (metrics.horizontalAdvance(text) > width && font.pointSize() > 1) {
        font.setPointSize(font.pointSize() - 1);
        metrics = QFontMetrics(font);
    }

    button->setFont(font);
}

void Dashboard::createBottomLayout() {
    bottomLayout = new QHBoxLayout();

    // Add buttons and adjust font
    BtnHelp = new QPushButton(tr("DASHBOARD_HELP"));
    connect(BtnHelp, &QPushButton::clicked, this, &Dashboard::helpMsgBox);
    BtnDS = new QPushButton(tr("DASHBOARD_DS"));
    connect(BtnDS, &QPushButton::clicked, this, &Dashboard::dataSourcesMsgBox);

    QFont bottomBtnFont("Arial", 10, QFont::Normal);
    BtnHelp->setFont(bottomBtnFont);
    BtnDS->setFont(bottomBtnFont);

    // Add widgets to layout
    bottomLayout->addStretch(); // Push buttons to the right
    bottomLayout->addWidget(BtnHelp);
    bottomLayout->addWidget(BtnDS);
    bottomLayout->addStretch();
}

void Dashboard::combineLayouts() {
    // Combine side and grid layout into one content layout
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSizeConstraint(QLayout::SetMinimumSize);
    contentLayout->addLayout(sideLayout, 1);
    contentLayout->addLayout(stackedLayout, 6);

    // Add the top, content, and bottom layouts to the main.qss layout
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
    auto page1 = qobject_cast<OverviewCards *>(pages->currentWidget());
    if (page1) {
        page1->retranslateUI(); // Retranslate the current page
    }

    auto page2 = qobject_cast<PersistentOrganicPollutants *>(pages->currentWidget());
    if (page2) {
        page2->retranslateUI(); // Retranslate the current page
    }

    auto page3 = qobject_cast<PFApage *>(pages->currentWidget());
    if (page3) {
        page3->retranslateUI(); // Retranslate the current page
    }

    auto page4 = qobject_cast<PollutantOverview *>(pages->currentWidget());
    if (page3) {
        page3->retranslateUI(); // Retranslate the current page
    }

    auto page5 = qobject_cast<DataPage *>(pages->currentWidget());
    if (page5) {
        page5->retranslateUI(); // Retranslate the current page
    }

    auto page6 = qobject_cast<ComplianceDashboard *>(pages->currentWidget());
    if (page6) {
        page6->retranslateUI(); // Retranslate the current page
    }
}

void Dashboard::retranslateUI() {
    // Force re-translate of all UI components (this ensures that UI updates even if translator is installed after initial load)
    title->setText(tr("DASHBOARD_TITLE"));
    BtnPFA->setText(tr("DASHBOARD_PFAS"));
    BtnDP->setText(tr("DASHBOARD_DP"));
    BtnGH->setText(tr("DASHBOARD_GH"));
    BtnPOP->setText(tr("DASHBOARD_POPS"));
    BtnDashboard->setText(tr("DASHBOARD_BUTTON"));
    BtnPO->setText(tr("DASHBOARD_PO"));
    BtnCD->setText(tr("DASHBOARD_CD"));
    BtnHelp->setText(tr("DASHBOARD_HELP"));
    BtnDS->setText(tr("DASHBOARD_DS"));
    language->setItemText(0, tr("English"));
    language->setItemText(1, tr("French"));

    // Retranslate the current page as well (will be updated to be more efficient)
    auto page1 = qobject_cast<OverviewCards *>(pages->currentWidget());
    if (page1) {
        page1->retranslateUI(); // Retranslate the current page
    }

    auto page2 = qobject_cast<PersistentOrganicPollutants *>(pages->currentWidget());
    if (page2) {
        page2->retranslateUI(); // Retranslate the current page
    }

    auto page3 = qobject_cast<PFApage *>(pages->currentWidget());
    if (page3) {
        page3->retranslateUI(); // Retranslate the current page
    }

    auto page4 = qobject_cast<PollutantOverview *>(pages->currentWidget());
    if (page4) {
        page4->retranslateUI(); // Retranslate the current page
    }

    auto page5 = qobject_cast<DataPage *>(pages->currentWidget());
    if (page5) {
        page5->retranslateUI(); // Retranslate the current page
    }

    auto page6 = qobject_cast<ComplianceDashboard *>(pages->currentWidget());
    if (page6) {
        page6->retranslateUI(); // Retranslate the current page
    }
}

void Dashboard::helpMsgBox() {
    QMessageBox::information(this, tr("DASHBOARD_HELP"), 
    tr("DASHBOARD_HELP_TEXT_BODY_1")+
    "\n"+
    tr("DASHBOARD_HELP_TEXT_BODY_2")+
    "\n"+
    tr("DASHBOARD_HELP_TEXT_BODY_3")+
    "\n"+
    tr("DASHBOARD_HELP_TEXT_BODY_4")+
    "\n"+
    tr("DASHBOARD_HELP_TEXT_BODY_5")+
    "\n"+
    tr("DASHBOARD_HELP_TEXT_BODY_6")+
    "\n"+
    tr("DASHBOARD_HELP_TEXT_BODY_7")+
    "\n"+
    tr("DASHBOARD_HELP_TEXT_BODY_8")+
    "\n"+
    tr("DASHBOARD_HELP_TEXT_BODY_9")+
    "\n"+
    tr("DASHBOARD_HELP_TEXT_BODY_10"));
}

void Dashboard::dataSourcesMsgBox() {
    QMessageBox::information(this, tr("DASHBOARD_DS"),
    tr("DASHBOARD_DATA_SOURCES_TEXT"));
}

void Dashboard::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    adjustButtonTextToFit(BtnDashboard);
    adjustButtonTextToFit(BtnCD);
    adjustButtonTextToFit(BtnDP);
    adjustButtonTextToFit(BtnGH);
    adjustButtonTextToFit(BtnPO);
    adjustButtonTextToFit(BtnPFA);
    adjustButtonTextToFit(BtnPOP);
}

void Dashboard::goToOverviewCards() {
    pages->setCurrentIndex(0);
    retranslateUI();

    auto page = qobject_cast<OverviewCards *>(pages->currentWidget());
    if (page) {
        std::cout << "retranslating" << std::endl;
        page->retranslateUI();
    }
}

void Dashboard::goToPOPs() {
    pages->setCurrentIndex(1);
    retranslateUI();

    auto page = qobject_cast<PersistentOrganicPollutants *>(pages->currentWidget());
    if (page) {
        std::cout << "retranslating" << std::endl;
        page->retranslateUI();
    }
}

void Dashboard::goToPFAs() {
    pages->setCurrentIndex(2);
    retranslateUI();

    auto page = qobject_cast<PFApage *>(pages->currentWidget());
    if (page) {
        std::cout << "retranslating" << std::endl;
        page->retranslateUI();
    }
}

void Dashboard::goToPO() {
    pages->setCurrentIndex(3);
    retranslateUI();

    auto page = qobject_cast<PollutantOverview *>(pages->currentWidget());
    if (page) {
        std::cout << "retranslating" << std::endl;
        page->retranslateUI();
    }
}

void Dashboard::goToDP() {
    pages->setCurrentIndex(4);
    retranslateUI();

    auto page = qobject_cast<DataPage *>(pages->currentWidget());
    if (page) {
        std::cout << "retranslating" << std::endl;
        page->retranslateUI();
    }
}

void Dashboard::goToCD() {
    pages->setCurrentIndex(5);
    retranslateUI();

    auto page = qobject_cast<ComplianceDashboard *>(pages->currentWidget());
    if (page) {
        std::cout << "retranslating" << std::endl;
        page->retranslateUI();
    }
}

void Dashboard::goToGH() {
    pages->setCurrentIndex(6);
}