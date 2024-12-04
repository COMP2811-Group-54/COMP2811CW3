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

class PFApage: public QWidget
{
        Q_OBJECT
    public:
        explicit PFApage(QWidget *parent = nullptr);

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

        QChartView* pfaChartView;

        QLabel* pfas;
        QLabel* otherPfas;
        QPushButton* moreInfo;
        QPushButton* viewList;

    private slots:
        void moreInfoMsgBox();
        void viewListMsgBox();
};