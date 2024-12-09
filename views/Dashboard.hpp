#pragma once

#include <iostream>
#include <QtWidgets>
#include <QtCharts>
#include <QShortcut>

#include "PFAs.hpp"
#include "POPs.hpp"

class QLabel;
class QPushButton;
class QComboBox;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QStackedWidget;
class QFrame;
class QTranslator;
class QShortcut;

class Dashboard : public QWidget {
    Q_OBJECT

public:
    Dashboard();

private:
    void createMainLayout();

    void createTopLayout();

    void createLeftLayout();

    void createGridLayout();

    void createBottomLayout();

    void createStackedWidget();

    void combineLayouts();

    void onLanguageChanged(int index);

    void retranslateUI();

    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;
    QVBoxLayout *sideLayout;
    QGridLayout *gridLayout;
    QHBoxLayout *bottomLayout;
    QVBoxLayout *stackedLayout;
    QVBoxLayout *stackedLayoutFrame;
    QFrame *pagesFrame;

    QLabel *title;

    QStackedWidget *pages;

    QPushButton *BtnPFA;
    QPushButton *BtnPOP;
    QPushButton *BtnHelp;
    QPushButton *BtnDS;
    QPushButton *BtnDashboard;
    QPushButton *BtnPO;
    QPushButton *BtnCD;
    QPushButton *BtnDP;
    QPushButton *BtnGH;

    QComboBox *language;
    QTranslator *translator;

    QMap<QWidget *, QString> originalTexts;

private slots:
    void goToOverviewCards() {
        pages->setCurrentIndex(0);
    }

    void goToPOPs() {
        pages->setCurrentIndex(1);

        retranslateUI();

        auto page = qobject_cast<PersistentOrganicPollutants *>(pages->currentWidget());
        if (page) {
            std::cout << "retranslating" << std::endl;
            page->retranslateUI();
        }
    }

    void goToPFAs() {
        pages->setCurrentIndex(2);

        retranslateUI();

        auto page = qobject_cast<PFApage *>(pages->currentWidget());
        if (page) {
            std::cout << "retranslating" << std::endl;
            page->retranslateUI();
            page->initializeWithData();
        }
    }

    void goToPO() {
        pages->setCurrentIndex(3);
    }

    void goToDP() {
        pages->setCurrentIndex(4);
    }

    void goToCD() {
        pages->setCurrentIndex(5);
    }

    void goToGH() {
        pages->setCurrentIndex(6);
    }

    void helpMsgBox();

    void dataSourcesMsgBox();
};
