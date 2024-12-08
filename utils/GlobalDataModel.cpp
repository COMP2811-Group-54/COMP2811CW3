// GlobalDataModel.cpp
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

void GlobalDataModel::loadData(const std::string& filePath) {
    // Load data into the Dataset
    dataset.loadDataset(filePath);

    // Update the DataModel accordingly
    updateDataModel();
}

void GlobalDataModel::updateDataModel() {}