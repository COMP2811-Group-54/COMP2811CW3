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

    std::vector<Measurement> getMeasurementsByDeterminand(const std::string &determinandLabel) const;

    std::vector<Measurement> getMeasurementsBySamplingPoint(const std::string &samplingPoint) const;

    Measurement getMeasurement(int determinand);

    Dataset queryDeterminand(const std::string &query) const;;

    size_t size() const { return data.size(); }
    Measurement operator[](const int index) const { return data.at(index); }

    std::vector<Measurement>::iterator begin() { return data.begin(); }
    std::vector<Measurement>::iterator end() { return data.end(); }

    std::vector<Measurement>::const_iterator begin() const { return data.begin(); }
    std::vector<Measurement>::const_iterator end() const { return data.end(); }

private:
    std::string filePath;
    std::vector<Measurement> data;

    void checkDataExists() const;

    static int levenshteinDist(const std::string &word1, const std::string &word2);
};


#endif //DATASET_HPP
