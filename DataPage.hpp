#pragma once

#include <QMainWindow>
#include "./utils/DataModel.hpp"

class QString;
class QComboBox;
class QLabel;
class QPushButton;
class QTableView;
class StatsDialog;

class DataPage : public QWidget
{
    Q_OBJECT

public:
    explicit DataPage(QWidget *parent = nullptr);

private:
    void createTable();
    void onTextChanged(const QString &text);
    void createButtons();
    void setMainLayout();

    DataModel model; // data model used by table
    QString dataLocation; // location of CSV data files
    QPushButton *loadButton; // button to load a new CSV file
    QPushButton *setLocationButton;
    QPushButton *statsButton;
    QTableView *table; // table of quake data
    QLabel *fileInfo; // status bar info on current file
    StatsDialog *statsDialog; // dialog to display stats

    private slots:
        void setDataLocation();

    void openCSV();

    // void displayStats();
};