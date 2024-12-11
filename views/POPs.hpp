#pragma once

#include <QtWidgets>
#include <QtCharts>
#include "PO.hpp"
#include "../utils/Measurement.hpp"

class PersistentOrganicPollutants : public QWidget {
    Q_OBJECT

public:
    explicit PersistentOrganicPollutants(QWidget *parent = nullptr);

    void retranslateUI();

private slots:
    void updateChart();

    void moreInfoMsgBox();

    void viewListMsgBox();

private:
    void createTitle();

    void createButtons();

    void createBoxes();

    void createFilters();

    void createComplianceLabels();

    void arrangeWidgets();

    void createChart(const std::vector<Measurement> &filteredData);

    void addComplianceLevelLine(QChart *chart, QDateTimeAxis *xAxis, QValueAxis *yAxis, double level, QColor color,
                                const QString &label);

    QLabel *title;
    QLabel *locationLabel;
    QLabel *timeRangeLabel;
    QLabel *pollutantLabel;
    QComboBox *location;
    QComboBox *timeRange;
    QComboBox *pollutant;

    QLabel *red;
    QLabel *orange;
    QLabel *green;
    QChartView *popChartView;

    QLabel *pops;
    QLabel *otherPops;
    QPushButton *moreInfo;
    QPushButton *viewList;
};
