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
        return int(Qt::AlignRight | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole) {
        const Measurement q = dataset[index.row()];
        switch (index.column()) {
            case 0: return QVariant(QString::fromStdString(q.getCompoundName()));  // Convert std::string to QString
            case 1: {
                QDateTime datetime = q.getDatetime();
                return QVariant(datetime.toString("yyyy-MM-dd HH:mm:ss"));  // Convert QDateTime to QString
            }
            case 2: return QVariant(QString::fromStdString(q.getDescription()));  // Convert std::string to QString
            case 3: return QVariant(q.getValue());
            case 4: return QVariant(QString::fromStdString(q.getUnit()));  // Convert std::string to QString
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
}

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