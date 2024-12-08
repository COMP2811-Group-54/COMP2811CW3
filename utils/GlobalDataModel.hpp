// GlobalDataModel.hpp
#pragma once
#include "./DataModel.hpp"
#include "Dataset.hpp"

class GlobalDataModel {
public:
    // Access the singleton instance
    static GlobalDataModel& instance();

    // Getter for the DataModel - the model to be used in different UI components
    DataModel& getDataModel();

    // Getter for the Dataset - for data loading and accessing raw data
    Dataset& getDataset();

    // Functions to load and update dataset
    void loadData(const std::string& filePath);

private:
    GlobalDataModel() = default;
    ~GlobalDataModel() = default;

    // Delete copy constructor and assignment operator to prevent copying
    GlobalDataModel(const GlobalDataModel&) = delete;
    GlobalDataModel& operator=(const GlobalDataModel&) = delete;

    DataModel dataModel;  // Instance of DataModel managed globally
    Dataset dataset;      // Instance of Dataset managed globally

    // Method to update DataModel from Dataset
    void updateDataModel();
};