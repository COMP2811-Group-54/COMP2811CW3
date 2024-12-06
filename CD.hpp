#pragma once

#include <QMainWindow>
#include "./utils/DataModel.hpp"

class QString;
class QComboBox;
class QLabel;
class QPushButton;
class QTableView;
class StatsDialog;

class ComplianceDashboard : public QWidget 
{
    Q_OBJECT

public:
    explicit ComplianceDashboard(QWidget *parent = nullptr);

private:
    void createTitle();
    void createTable();
    void onTextChanged(const QString &text);
    void createFilters();
    void createPollutantIndicator();
    void setMainLayout();

    QLabel* title;
    DataModel model; // data model used by table
    QString dataLocation; // location of CSV data files
    QLabel* locationLabel;
    QLabel* complianceLabel;
    QLabel* pollutantLabel;
    QComboBox* location;
    QComboBox* compliance;
    QComboBox* pollutant;
    QLabel* currentPollutant;
    QLabel* currentPollutantCompliance;
    QTableView *table; // table of quake data
    QLabel *fileInfo; // status bar info on current file
    StatsDialog *statsDialog; // dialog to display stats

private slots:

};
