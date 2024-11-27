// COMP2811 Coursework 2 sample solution: dialog

#include <QtWidgets>
#include "stats.hpp"


StatsDialog::StatsDialog(QWidget* parent): QDialog(parent)
{
  createWidgets();
  arrangeWidgets();
  setWindowTitle("Quake Stats");
}


// void StatsDialog::update(const QString& newCompoundName, const QString& newDatetime, const QString& newDescription, const double newValue, const QString& newUnit)
// {
//   compoundName->setText(newCompoundName);
//   dateTime->setText(newDatetime);
//   description->setText(newDescription);
//   value->setText(QString::number(newValue));
//   unit->setText(newUnit);
// }


void StatsDialog::createWidgets()
{


  compoundName = new QLineEdit("?");
  compoundName->setMaxLength(20);
  compoundName->setReadOnly(true);

  dateTime = new QLineEdit("?");
  dateTime->setMaxLength(20);
  dateTime->setReadOnly(true);

  description = new QLineEdit("?");
  description->setMaxLength(20);
  description->setReadOnly(true);

  value = new QLineEdit("?");
  value->setMaxLength(20);
  value->setReadOnly(true);

  unit = new QLineEdit("?");
  unit->setMaxLength(20);
  unit->setReadOnly(true);


  closeButton = new QPushButton("Close");
  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}


void StatsDialog::arrangeWidgets()
{
  auto* form = new QFormLayout();
  // form->addRow("Mean depth (km):", meanDepth);
  // form->addRow("Mean magnitude:", meanMagnitude);

  auto* box = new QVBoxLayout();
  box->addLayout(form);
  box->addWidget(closeButton);

  setLayout(box);
}
