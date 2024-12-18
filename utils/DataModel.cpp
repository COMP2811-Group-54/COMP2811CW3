#include "DataModel.hpp"

/* Derived from template code provided by Nick Efford */

// Provide header labels for the table view
QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }

    // Column headers
    switch (section) {
        case 0: return QString("Compound Name");
        case 1: return QString("Sample DateTime");
        case 2: return QString("Description");
        case 3: return QString("Value");
        case 4: return QString("Unit");
        case 5: return QString("Sample Location");

        default: return QVariant();
    }
}

// Provide data for each cell in the table view
QVariant DataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !dataset) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        const Measurement &m = (*dataset)[index.row()];

        switch (index.column()) {
            case 0: return QString::fromStdString(m.getCompoundName());
            case 1: return m.getDatetime().toString("yyyy-MM-dd HH:mm:ss");
            case 2: return QString::fromStdString(m.getDescription());
            case 3: return m.getValue();
            case 4: return QString::fromStdString(m.getUnit());
            case 5: return QString::fromStdString(m.getLabel());
            default: return QVariant();
        }
    }

    return QVariant();
}
