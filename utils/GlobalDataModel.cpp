#include "GlobalDataModel.hpp"

GlobalDataModel& GlobalDataModel::instance() {
    static GlobalDataModel instance;
    return instance;
}

DataModel& GlobalDataModel::getDataModel() {
    return dataModel;
}

Dataset& GlobalDataModel::getDataset() {
    return dataset;
}

void GlobalDataModel::loadData(const std::string &filePath) {
    dataset.loadDataset(filePath);
    dataModel.setDataset(&dataset); // Set loaded dataset to dataModel

}

void GlobalDataModel::emitFetchesComplete() {
    emit allRequestsComplete();
}
