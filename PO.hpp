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
class QLineEdit;

class PollutantOverview: public QWidget
{
        Q_OBJECT
    public:
        explicit PollutantOverview(QWidget *parent = nullptr);

    private:
        void createTitle();
        void createSearchBar();
        void createChart();
        void createButtons();
        void createBoxes();
        void createFilters();
        void createComplianceLabels();
        void arrangeWidgets();

        QLabel* title;

        QLineEdit* searchBar;

        QLabel* locationLabel;
        QLabel* timeRangeLabel;
        QLabel* pollutantLabel;
        QComboBox* location;
        QComboBox* timeRange;
        QComboBox* pollutant;

        QLabel* red;
        QLabel* orange;
        QLabel* green;

        QChartView* overviewChartView;

        QLabel* pcbs;
        QLabel* otherPops;
        QPushButton* moreInfo;
        QPushButton* viewList;

    private slots:
        void moreInfoMsgBox();
        void viewListMsgBox();
        QString searchQuery();
};