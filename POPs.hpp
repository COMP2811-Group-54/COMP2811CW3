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
  public:
    PersistentOrganicPollutants();

  private:
    void createChart();
    void createButtons();
    void createBoxes();
    void createFilters();
    // void createScale();
    void arrangeWidgets();

    QLabel* locationLabel;
    QLabel* timeRangeLabel;
    QComboBox* location;
    QComboBox* timeRange;

    // QToolTip* red;
    // QToolTip* orange;
    // QToolTip* green;

    QChartView* popChartView;

    QLabel* pcbs;
    QLabel* otherPops;
    QPushButton* moreInfo;
    QPushButton* viewList;
};