// COMP2811 Coursework 2: data model

#pragma once

#include <QAbstractTableModel>
#include "dataset.hpp"

class DataModel final : public QAbstractTableModel
{
  public:
    explicit DataModel(QObject* parent = nullptr): QAbstractTableModel(parent) {}
    void updateFromFile(const QString&);
    bool hasData() const { return dataset.size() > 0; }

    int rowCount(const QModelIndex& index) const override { return dataset.size(); }
    int columnCount(const QModelIndex& index) const override { return 5; }
    QVariant data(const QModelIndex&, int) const override;

    void Query(const std::string &query);

    QVariant headerData(int, Qt::Orientation, int) const override;


  private:
    Dataset dataset;
};