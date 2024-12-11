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

class searchableComboBox : public QComboBox {
    Q_OBJECT

public:
    explicit searchableComboBox(QWidget *parent = nullptr) : QComboBox(parent), allOptions() {
        setEditable(true);

        this->lineEdit()->setPlaceholderText("Enter to reset");

        connect(this->lineEdit(), &QLineEdit::returnPressed, this, &searchableComboBox::filterOptions);
    }

    void setOptions(const QStringList &options) {
        allOptions = options;
        clear();
        addItems(allOptions);
    }

private slots:
    void filterOptions() {
        QString text = this->lineEdit()->text();

        clear();
        for (const QString &item: allOptions) {
            if (item.contains(text, Qt::CaseInsensitive)) {
                addItem(item);
            }
        }
        showPopup();
    }

private:
    QStringList allOptions;
};

class PollutantOverview : public QWidget {
    Q_OBJECT

public:
    explicit PollutantOverview(QWidget *parent = nullptr);

private:
    void createTitle();

    void createSearchBar();

    void createChart(const std::vector<Measurement> &filteredData);


    void addComplianceLevelLine(QChart *chart, QAbstractAxis *xAxis, QAbstractAxis *yAxis, double levelValue, const QString &color,
                                const QString &label);

    void createButtons();

    void createBoxes();

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

    QLabel *pcbs;
    QLabel *otherPops;
    QPushButton *moreInfo;
    QPushButton *viewList;

private slots:
    void moreInfoMsgBox();

    void viewListMsgBox();

    void heavyMetalsInfoMsgBox();

    void organicChemicalsInfoMsgBox();

    void inorganicChemicalsInfoMsgBox();

    void nutrientsInfoMsgBox();

    void volatileOrganicCompoundsInfoMsgBox();

    void updateChart();
};
