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
        QVBoxLayout* stackedLayoutFrame;
        QFrame* pagesFrame;
        
        QLabel* title;

        QStackedWidget* pages;

        QPushButton* BtnPFA;
        QPushButton* BtnPOP;
        QPushButton* BtnHelp;
        QPushButton* BtnDS;
        QPushButton* BtnDashboard;
        QPushButton* BtnPO;
        QPushButton* BtnCD;
        QPushButton* BtnDP;

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
        void goToPO()
        {
            pages->setCurrentIndex(3);
        }
        void goToDP()
        {
            pages->setCurrentIndex(4);
        }
        
};