// DataPage.hpp
#pragma once

#include <QMainWindow>
#include "../utils/DataModel.hpp"

class QString;
class QComboBox;
class QLabel;
class QPushButton;
class QTableView;

class DataPage : public QWidget {
    Q_OBJECT

public:
    explicit DataPage(QWidget *parent = nullptr);

    void retranslateUI();

private:
    void createTable();

    void onTextChanged(const QString &text);

    void createButtons();

    void createLoadingSpinner();

    void createStatsLabel();

    void updateLoadingStats();

    void setMainLayout();

    DataModel &model; // Use a reference to DataModel instead of creating a new object
    QString dataLocation = "./data/";
    QPushButton *loadButton;
    QPushButton *setLocationButton;
    QPushButton *statsButton;
    QTableView *table;
    QLabel *fileInfo;

    QLabel *loadingSpinner;
    QMovie *loadingMovie;
    QLabel *loadingText;
    QLabel *statsLabel;

    QTimer *loadingTimer;

private slots:
    void setDataLocation();

    void openCSV();

    void onDataLoaded() const;

    void displayStats();
};
