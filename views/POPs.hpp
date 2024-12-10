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
    QChartView *popChartView;

    QLabel *pcbs;
    QLabel *otherPops{};
    QPushButton *moreInfo{};
    QPushButton *viewList{};
};