#pragma once

/* Derived from template code provided by Nick Efford */

#include <QAbstractTableModel>
#include "Dataset.hpp"

class DataModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit DataModel(QObject *parent = nullptr)
        : QAbstractTableModel(parent), dataset(nullptr) {
    }

    // Function to set the dataset and reset the model
    void setDataset(Dataset *ds) {
        beginResetModel(); // Notify the view that the model is about to change
        dataset = ds;
        endResetModel();   // Notify the view that the model has been updated
    }

    // Row count: Number of rows in the dataset
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid() || !dataset) {
            return 0;
        }
        return static_cast<int>(dataset->size());
    }

    // Column count: Fixed at 5 columns (Compound Name, Sample DateTime, Description, Value, Unit)
    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return 6;
    }

    // Header data for table view
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // Data retrieval for the table view
    QVariant data(const QModelIndex &index, int role) const override;

    // Check if the dataset has data
    bool hasData() const { return dataset && !dataset->isEmpty(); }

private:
    Dataset *dataset; // Pointer to the dataset
};
