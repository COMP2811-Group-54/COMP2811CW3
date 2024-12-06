#pragma once
#include "Dataset.hpp"

class GlobalDataset {
public:
    // Access the singleton instance
    static GlobalDataset& instance();

    // Getter for the Dataset
    Dataset& getDataset();

private:
    GlobalDataset() = default;  // Private constructor
    ~GlobalDataset() = default;  // Destructor

    // Delete copy constructor and assignment operator to prevent copying
    GlobalDataset(const GlobalDataset&) = delete;
    GlobalDataset& operator=(const GlobalDataset&) = delete;

    Dataset dataset;  // Instance of Dataset managed globally
};