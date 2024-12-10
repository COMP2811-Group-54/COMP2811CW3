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

class searchableComboBox: public QComboBox
{
    Q_OBJECT

    public:
        explicit searchableComboBox(QWidget *parent = nullptr) : QComboBox(parent), allOptions()

        {
            setEditable(true);

            this->lineEdit()->setPlaceholderText("Enter to reset");

            connect(this->lineEdit(), &QLineEdit::returnPressed, this, &searchableComboBox::filterOptions);
        }

        void setOptions(const QStringList &options)
        {
            allOptions = options;
            clear();
            addItems(allOptions);
        }

    private slots:
        void filterOptions()
        {
            QString text = this->lineEdit()->text();

            clear();
            for (const QString &item : allOptions) {
                if (item.contains(text, Qt::CaseInsensitive)) {
                    addItem(item);
                }
            }
            showPopup();
        }

    private:
        QStringList allOptions;
};

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

        QLineEdit* pollutantSearchBar;

        QLabel* locationLabel;
        QLabel* timeRangeLabel;
        QLabel* pollutantLabel;
        searchableComboBox* location;
        QComboBox* timeRange;
        searchableComboBox* pollutant;

        QLabel* red;
        QLabel* orange;
        QLabel* green;

        QChartView* overviewChartView;

        QLabel* heavyMetals;
        QPushButton* heavyMetalsInfo;

        QLabel* organicChemicals;
        QPushButton* organicChemicalsInfo;

        QLabel* inorganicChemicals;
        QPushButton* inorganicChemicalsInfo;

        QLabel* nutrients;
        QPushButton* nutrientsInfo;

        QLabel* volatileOrganicCompounds;
        QPushButton* volatileOrganicCompoundsInfo;


    private slots:
        void volatileOrganicCompoundsInfoMsgBox();
        void nutrientsInfoMsgBox();
        void heavyMetalsInfoMsgBox();
        void organicChemicalsInfoMsgBox();
        void inorganicChemicalsInfoMsgBox();
};