//
// Created by Josh Mundray on 27/11/2024.
//
#pragma once
#ifndef DATASET_HPP
#define DATASET_HPP

#include <string>
#include <vector>
#include "Measurement.hpp"

class Dataset {
public:
    Dataset() = default;

    explicit Dataset(const std::string &filePath) { loadDataset(filePath); };
    explicit Dataset(const std::vector<Measurement> &data) { this->data = data; };

    // int loadFile(string filePath);

    void loadDataset(const std::string &path);

    Measurement getMeasurement(int determinand);

    Dataset queryDeterminand(const std::string &query) const;;
    size_t size() const { return data.size(); }
    Measurement operator[](const int index) const { return data.at(index); }

private:
    std::string filePath;
    std::vector<Measurement> data;

    void checkDataExists() const;
};


#endif //DATASET_HPP
