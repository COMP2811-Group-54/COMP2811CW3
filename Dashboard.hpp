#pragma once

#include <QtWidgets>
#include <QtCharts>
#include <QTabWidget>

class QLabel;
class QPushButton;
class QComboBox;
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QTabWidget;
class QStackedWidget;

class Dashboard: public QWidget
{
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
        
        QVBoxLayout* mainLayout;
        QHBoxLayout* topLayout;
        QVBoxLayout* sideLayout;
        QGridLayout* gridLayout;
        QHBoxLayout* bottomLayout;
        QVBoxLayout* stackedLayout;

        
        QLabel* title;

        QStackedWidget* pages;

        QPushButton* BtnPFA;
        QPushButton* BtnPOP;
        QPushButton* BtnLitter;
        QPushButton* BtnHelp;
        QPushButton* BtnDS;
        QPushButton* BtnDashboard;
        QPushButton* BtnPO;
        QPushButton* BtnCD;

        QComboBox* language; 

    private slots:
        void goToOverviewCards()
        {
            pages->setCurrentIndex(0);
        }

        void goToPOPs()
        {
            pages->setCurrentIndex(1);
        }

        void goToPFAs()
        {
            pages->setCurrentIndex(2);
        }
        
};