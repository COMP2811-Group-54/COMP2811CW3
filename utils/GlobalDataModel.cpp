#include "GlobalDataModel.hpp"

// Returns the singleton instance of GlobalDataset
GlobalDataset& GlobalDataset::instance() {
    static GlobalDataset instance;
    return instance;
}

// Getter for dataset
Dataset& GlobalDataset::getDataset() {
    return dataset;
}