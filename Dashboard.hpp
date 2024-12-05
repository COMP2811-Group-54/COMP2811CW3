#pragma once

#include <QtWidgets>
#include <QtCharts>

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
    QPushButton *BtnLitter;
    QPushButton *BtnHelp;
    QPushButton *BtnDS;
    QPushButton *BtnDashboard;
    QPushButton *BtnPO;
    QPushButton *BtnCD;

    QComboBox *language;
    QTranslator *translator;

    QMap<QWidget *, QString> originalTexts;

private slots:
    void goToOverviewCards() {
        pages->setCurrentIndex(0);
    }

    void goToPOPs() {
        pages->setCurrentIndex(1);
    }

    void goToPFAs() {
        pages->setCurrentIndex(2);
    }
};
