#pragma once

#include <iostream>
#include <QtWidgets>


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

    void adjustButtonTextToFit(QPushButton *button); // Add this line

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

    bool dataLoadedPOPs = false;
    bool dataLoadedPFAs = false;

private slots:
    void goToOverviewCards();

    void goToPOPs();

    void goToPFAs();

    void goToPO();

    void goToDP();

    void goToCD();

    void goToGH();

    void helpMsgBox();

    void dataSourcesMsgBox();

    void resizeEvent(QResizeEvent *event);

    void onDataLoaded();
};
