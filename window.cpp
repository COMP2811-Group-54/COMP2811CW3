// COMP2811 Coursework 2 sample solution: main window
// aidan branch test
#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "window.hpp"
#include "stats.hpp"
#include "./utils/DataModel.hpp"

static const int MIN_WIDTH = 620;


DataPage::DataPage(QWidget *parent): QWidget(parent), statsDialog(nullptr)
{
  createMainWidget();
  // createFileSelectors();
  createButtons();
  // createToolBar();
  setMainLayout();
  // createStatusBar();
  // addFileMenu();
  // addHelpMenu();

  setMinimumWidth(MIN_WIDTH);
}


void DataPage::createMainWidget()
{
  table = new QTableView();
  table->setModel(&model);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);
}

// void DataPage::onTextChanged(const QString &text) {
//   std::string stdText = text.toStdString();
//   std::cout << "Querying: " << stdText << std::endl;
//   model.Query(stdText);
// }


// void DataPage::createFileSelectors()
// {
//   QStringList periodOptions;
//   periodOptions << "hour" << "day" << "week" << "month";
//   period = new QComboBox();
//   period->addItems(periodOptions);
// }


void DataPage::createButtons()
{
  setLocationButton = new QPushButton("Set Data Location");
  loadButton = new QPushButton("Load");
  statsButton = new QPushButton("Stats");

  connect(loadButton, SIGNAL(clicked()), this, SLOT(openCSV()));
  connect(setLocationButton, SIGNAL(clicked()), this, SLOT(setDataLocation()));
  // connect(statsButton, SIGNAL(clicked()), this, SLOT(displayStats()));
}

void DataPage::setMainLayout()
{
  QVBoxLayout* buttons = new QVBoxLayout();
  buttons->addWidget(setLocationButton);
  buttons->addWidget(loadButton);
  buttons->addWidget(statsButton);

  QHBoxLayout* dataPage = new QHBoxLayout();
  dataPage->addLayout(buttons, 1);
  dataPage->addWidget(table, 9);

  setLayout(dataPage);
}


// void DataPage::createToolBar() {
//   QToolBar* toolBar = new QToolBar();

//   auto significanceSearch = new QLineEdit(this);

//   toolBar->addWidget(significanceSearch);

  // loadButton = new QPushButton("Load");
  // toolBar->addWidget(loadButton);
  // connect(loadButton, &QPushButton::clicked, this, &DataPage::openCSV);

//   this->addToolBar(toolBar);

//   connect(significanceSearch, &QLineEdit::textChanged, this, [this](const QString & text) { onTextChanged(text); });
// }

// void DataPage::createStatusBar()
// {
//   fileInfo = new QLabel("Current file: <none>");
//   QStatusBar* status = statusBar();
//   status->addWidget(fileInfo);
// }


// void DataPage::addFileMenu()
// {
//   QAction* locAction = new QAction("Set Data &Location", this);
//   locAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
//   connect(locAction, SIGNAL(triggered()), this, SLOT(setDataLocation()));

//   QAction* closeAction = new QAction("Quit", this);
//   closeAction->setShortcut(QKeySequence::Close);
//   connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

//   QMenu* fileMenu = menuBar()->addMenu("&File");
//   fileMenu->addAction(locAction);
//   fileMenu->addAction(closeAction);
// }


// void DataPage::addHelpMenu()
// {
//   QAction* aboutAction = new QAction("&About", this);
//   connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

//   QAction* aboutQtAction = new QAction("About &Qt", this);
//   connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

//   QMenu* helpMenu = menuBar()->addMenu("&Help");
//   helpMenu->addAction(aboutAction);
//   helpMenu->addAction(aboutQtAction);
// }


void DataPage::setDataLocation()
{
  QString directory = QFileDialog::getExistingDirectory(
    this, "Data Location", ".",
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (directory.length() > 0) {
    dataLocation = directory;
  }
}


void DataPage::openCSV()
{
  if (dataLocation == "") {
    QMessageBox::critical(this, "Data Location Error",
      "Data location has not been set!\n\n"
      "You can specify this via the File menu."
    );
    return;
  }

  // auto filename = QString("%1-%2.csv")
  //   .arg(significance->currentText()).arg(period->currentText());

  auto filename = QString("Y-2024.csv");

  auto path = dataLocation + "/" + filename;

  try {
    model.updateFromFile(path);
  }
  catch (const std::exception& error) {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }

  fileInfo->setText(QString("Current file: <kbd>%1</kbd>").arg(filename));
  table->resizeColumnsToContents();

  if (statsDialog != nullptr && statsDialog->isVisible()) {
    statsDialog->update();
  }
}


// void DataPage::DisplayData()
// {
//   if (model.hasData()) {
//     for (int i = 0; i < model.rowCount(); i++) {
//       QList<QStandardItem *> newRow;
//       newRow.append(new QStandardItem("Data 1")); // Replace "Data 1" with actual data
//       newRow.append(new QStandardItem("Data 2")); // Replace "Data 2" with actual data
//       newRow.append(new QStandardItem("Data 3")); // Replace "Data 3" with actual data
//       model.appendRow(newRow);
//     }
//   }
// }


// void DataPage::about()
// {
//   QMessageBox::about(this, "About Quake Tool",
//     "Quake Tool displays and analyzes earthquake data loaded from"
//     "a CSV file produced by the USGS Earthquake Hazards Program.\n\n"
//     "(c) 2024 Nick Efford");
// }
