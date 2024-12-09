#pragma once

#include <QtWidgets>
#include <QtCharts>
#include "PO.hpp"


class QComboBox;
class QLabel;
class QPushButton;
class QChartView;
class QToolTip;
class QTextEdit;
class QLabel;

class PFApage : public QWidget {
    Q_OBJECT

public:
    explicit PFApage(QWidget *parent = nullptr);

    void retranslateUI();

    void initializeWithData();

private:
    void createTitle();

    void createChart();

    void createButtons();

    void createBoxes();

    void createFilters();

    void createComplianceLabels();

    void arrangeWidgets();

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

    private slots:
        void moreInfoMsgBox();

    void viewListMsgBox();
};