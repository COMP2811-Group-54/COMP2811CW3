#pragma once

#include <QtWidgets>
#include <QtCharts>
#include "PO.hpp"
#include "../utils/Measurement.hpp"

class PFApage : public QWidget {
    Q_OBJECT

public:
    explicit PFApage(QWidget *parent = nullptr);

    void retranslateUI();


    void addComplianceLevelLine(QChart *chart, QDateTimeAxis *xAxis, QValueAxis *yAxis, double level, QColor color,
                                const QString &label);

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

    QLabel *title{};
    QLabel *locationLabel{};
    QLabel *timeRangeLabel{};
    QLabel *pollutantLabel{};
    QComboBox *location{};
    QComboBox *timeRange{};
    QComboBox *pollutant{};

    QLabel *red{};
    QLabel *orange{};
    QLabel *green{};
    QChartView *pfaChartView;

    QLabel *pfas{};
    QLabel *otherPfas{};
    QPushButton *moreInfo{};
    QPushButton *viewList{};
};
