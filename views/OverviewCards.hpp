#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include <QGridLayout>

class OverviewCards : public QWidget
{
    Q_OBJECT

public:
    explicit OverviewCards(QWidget *parent = nullptr);
    void retranslateUI();

    signals:
        void goToPOPs();
    void goToPFAs();
    void goToPO();
    void goToCD();

    public slots:
        void updateDataDisplays();  // Now declared as a public slot

private:
    void createPOP();
    void createPFA();
    void createPO();
    void createCD();
    void arrangeWidgets();

    QGridLayout* gridLayout;

    QFrame* FramePOP;
    QVBoxLayout* CrdPOP;
    QLabel* TitlePOP;
    QLabel* ExPOP1;
    QLabel* ExPOP2;
    QLabel* ExPOP3;
    QPushButton* POPsDetails;

    QFrame* FramePFA;
    QVBoxLayout* CrdPFA;
    QLabel* TitlePFA;
    QLabel* ExPFA1;
    QLabel* ExPFA2;
    QLabel* ExPFA3;
    QPushButton* PFAsDetails;

    QFrame* FramePO;
    QVBoxLayout* CrdPO;
    QLabel* TitlePO;
    QLabel* ExPO1;
    QLabel* ExPO2;
    QLabel* ExPO3;
    QPushButton* PODetails;

    QFrame* FrameCD;
    QVBoxLayout* CrdCD;
    QLabel* TitleCD;
    QLabel* ExCD1;
    QLabel* ExCD2;
    QLabel* ExCD3;
    QPushButton* CDDetails;
};