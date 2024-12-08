// ComplianceDashboard.hpp

#pragma once

#include <QMainWindow>
#include "./utils/DataModel.hpp"
#include "./utils/Compliance.hpp" // Include necessary header

class QString;
class QComboBox;
class QLabel;
class QPushButton;
class QTableView;

class ComplianceDashboard : public QWidget {
    Q_OBJECT

public:
    explicit ComplianceDashboard(QWidget *parent = nullptr);

private slots:
    void filterDataByCompliance(); // Slot to filter data

private:
    void createTitle();

    void createTable();

    void createFilters();

    void createPollutantIndicator();

    void setMainLayout();

    QLabel *title;
    DataModel &model; // Use a reference to the global DataModel
    ComplianceChecker complianceChecker; // Add a ComplianceChecker instance

    QLabel *locationLabel;
    QLabel *complianceLabel;
    QLabel *pollutantLabel;
    QComboBox *location;
    QComboBox *compliance;
    QComboBox *pollutant;
    QLabel *currentPollutant;
    QLabel *currentPollutantCompliance;
    QTableView *table;
    QLabel *fileInfo;
};
