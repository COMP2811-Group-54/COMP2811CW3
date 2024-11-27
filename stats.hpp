// COMP2811 Coursework 2 sample solution: dialog

#pragma once

#include <QDialog>

class QLineEdit;
class QPushButton;

class StatsDialog final : public QDialog {
public:
    explicit StatsDialog(QWidget *);

    // static void update(QString &newCompoundName, QString &time, QString &description, double, QString &unit);

private:
    QLineEdit *compoundName{};
    QLineEdit *dateTime{};
    QLineEdit *description{};
    QLineEdit *value{};
    QLineEdit *unit{};
    QPushButton *closeButton{};

    void createWidgets();

    void arrangeWidgets();
};
