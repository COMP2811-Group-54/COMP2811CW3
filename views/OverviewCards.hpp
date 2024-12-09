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

    signals:
      void goToPOPs();
    void goToPFAs();
    void goToPO();
    void goToCD();

private:
    // void createCards();
    void createPOP();
    void createPFA();
    void createPO();
    void createCD();
    void arrangeWidgets();

    QGridLayout* gridLayout;

    // QLabel* CrdPOP;
    // QLabel* CrdPFA;
    // QLabel* CrdPO;
    // QLabel* CrdLitter;
    // QLabel* CrdCD;

    QFrame* FramePOP;
    QVBoxLayout* CrdPOP;
    QLabel* TitlePOP;
    QLabel* ExPOP1;
    QLabel* ExPOP2;
    QLabel* ExPOP3;
    QPushButton* POPsDetails;

    QFrame* FramePO;
    QVBoxLayout* CrdPO;
    QLabel* TitlePO;
    QLabel* ExPO1;
    QLabel* ExPO2;
    QLabel* ExPO3;
    QPushButton* PODetails;

    QFrame* FramePFA;
    QVBoxLayout* CrdPFA;
    QLabel* TitlePFA;
    QLabel* ExPFA1;
    QLabel* ExPFA2;
    QLabel* ExPFA3;
    QPushButton* PFAsDetails;

    QFrame* FrameCD;
    QVBoxLayout* CrdCD;
    QLabel* TitleCD;
    QLabel* ExCD1;
    QLabel* ExCD2;
    QLabel* ExCD3;
    QPushButton* CDDetails;
};