#pragma once

#include <complex.h>
#include <QtWidgets>
#include <QtCharts>

#include "../utils/Measurement.hpp"


class QComboBox;
class QLabel;
class QPushButton;
class QChartView;
class QToolTip;
class QTextEdit;
class QLabel;
class QLineEdit;

class PollutantOverview : public QWidget {
    Q_OBJECT

public:
    explicit PollutantOverview(QWidget *parent = nullptr);

    void retranslateUI();

private:
    void createTitle();

    void createSearchBar();

    void createChart(const std::vector<Measurement> &filteredData);


    void addComplianceLevelLine(QChart *chart, QAbstractAxis *xAxis, QAbstractAxis *yAxis, double levelValue,
                                const QString &color,
                                const QString &label);

    void createButtons();

    void createFilters();

    void createComplianceLabels();

    void arrangeWidgets();

    QLabel *title;

    QLineEdit *pollutantSearchBar;


    QLabel *locationLabel;
    QLabel *timeRangeLabel;
    QLabel *pollutantLabel;
    QComboBox *location;
    QComboBox *timeRange{};
    QComboBox *pollutant;

    QLabel *red;
    QLabel *orange;
    QLabel *green;

    QChartView *overviewChartView;

    QLabel *heavyMetals;
    QPushButton *heavyMetalsInfo;

    QLabel *organicChemicals;
    QPushButton *organicChemicalsInfo;

    QLabel *inorganicChemicals;
    QPushButton *inorganicChemicalsInfo;

    QLabel *nutrients;
    QPushButton *nutrientsInfo;

    QLabel *volatileOrganicCompounds;
    QPushButton *volatileOrganicCompoundsInfo;

private slots:
    void heavyMetalsInfoMsgBox();

    void organicChemicalsInfoMsgBox();

    void inorganicChemicalsInfoMsgBox();

    void nutrientsInfoMsgBox();

    void volatileOrganicCompoundsInfoMsgBox();

    void updateChart();
};
