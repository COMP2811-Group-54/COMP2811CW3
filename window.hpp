// COMP2811 Coursework 2 sample solution: main window

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
    void createMainWidget();

    void onTextChanged(const QString &text);

    // void createFileSelectors();

    void createButtons();

    void setMainLayout();

    // void createToolBar();

    // void createStatusBar();

    // void addFileMenu();

    // void addHelpMenu();

    DataModel model; // data model used by table
    QString dataLocation; // location of CSV data files
    // QComboBox *significance; // selector for quake feed significance level
    // QComboBox *period; // selector for quake feed time period
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
