#pragma once

#include <QtWidgets>
#include <QtCharts>


class QComboBox;
class QLabel;
class QPushButton;
class QChartView;
class QToolTip;
class QTextEdit;
class QLabel;

class PersistentOrganicPollutants: public QWidget
{
        Q_OBJECT
    public:
        explicit PersistentOrganicPollutants(QWidget *parent = nullptr);
        void retranslateUI();


    private:
        void createTitle();
        void createChart();
        void createButtons();
        void createBoxes();
        void createFilters();
        void createComplianceLabels();
        void arrangeWidgets();

        QLabel* title;

        QLabel* locationLabel;
        QLabel* timeRangeLabel;
        QLabel* pollutantLabel;
        QComboBox* location;
        QComboBox* timeRange;
        QComboBox* pollutant;

        QLabel* red;
        QLabel* orange;
        QLabel* green;

        QChartView* popChartView;

        QLabel* pcbs;
        QLabel* otherPops;
        QPushButton* moreInfo;
        QPushButton* viewList;

    private slots:
        void moreInfoMsgBox();
        void viewListMsgBox();
};