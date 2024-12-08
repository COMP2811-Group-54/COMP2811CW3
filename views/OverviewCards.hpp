#pragma once

#include <QtWidgets>
#include <QtCharts>
#include <QTabWidget>

class QLabel;
class QStackedWidget;
class QGridLayout;

class OverviewCards: public QWidget
{
    Q_OBJECT
  public:
    explicit OverviewCards(QWidget *parent = nullptr);
    
  private:
    void createCards();

    QGridLayout* gridLayout;

    QLabel* CrdPOP;
    QLabel* CrdPFA;
    QLabel* CrdPO;
    QLabel* CrdLitter;
    QLabel* CrdCD;
};