// COMP2811 Coursework 2: data model

#include "DataModel.hpp"


void DataModel::updateFromFile(const QString &filename) {
    beginResetModel();
    dataset.loadDataset(filename.toStdString());
    endResetModel();
}


QVariant DataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        return int (Qt::AlignRight | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole) {
        const Measurement q = dataset[index.row()];
        switch (index.column()) {
            case 0: return QVariant(q.getCompoundName().c_str());
            case 1: return QVariant(q.getDatetime().c_str());
            case 2: return QVariant(q.getDescription().c_str());
            case 3: return QVariant(q.getValue());
            case 4: return QVariant(q.getUnit().c_str());
            default: return QVariant("0");
        }
    }

    return QVariant();
}


void DataModel::Query(const std::string &query) {
    Dataset queries = dataset.queryDeterminand(query);
    beginResetModel();
    dataset = queries;
    endResetModel();
};

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }

    switch (section) {
        case 0: return QString("Compound Name");
        case 1: return QString("Time");
        case 2: return QString("Description");
        case 3: return QString("Value");
        case 4: return QString("Unit");
        default: return QVariant();
    }
}