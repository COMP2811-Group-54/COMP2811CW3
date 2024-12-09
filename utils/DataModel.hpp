#pragma once

#include <QAbstractTableModel>
#include "Dataset.hpp"

class DataModel : public QAbstractTableModel {
public:
    explicit DataModel(QObject *parent = nullptr)
        : QAbstractTableModel(parent), dataset(nullptr) {
    }

    void setDataset(Dataset *ds) { dataset = ds; }

    int rowCount(const QModelIndex &index) const override {
        return dataset ? static_cast<int>(dataset->size()) : 0;
    }

    int columnCount(const QModelIndex &index) const override {
        return 5;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QVariant data(const QModelIndex &, int) const override;

    bool hasData() const { return dataset && !dataset->isEmpty(); }

private:
    Dataset *dataset; // Pointer to dataset
};
