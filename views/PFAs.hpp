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

    private slots:
        void updateChart();
    void onLocationSelected(int index);
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
    searchableComboBox *location{};
    QComboBox *timeRange{};
    searchableComboBox *pollutant{};

    QLabel *red{};
    QLabel *orange{};
    QLabel *green{};
    QChartView *pfaChartView;

    QLabel *pfas{};
    QLabel *otherPfas{};
    QPushButton *moreInfo{};
    QPushButton *viewList{};
};