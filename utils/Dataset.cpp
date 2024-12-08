//
// Created by Josh Mundray on 27/11/2024.
//
#include <iostream>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <unordered_map>
#include <vector>

#include "Dataset.hpp"
#include "csv.hpp"

#include "Compliance.hpp"
#include <unordered_set>

// Dataset.cpp


const std::string ID_COLUMN = "@id";
const std::string SAMPLE_POINT_COLUMN = "sample.samplingPoint";
const std::string SAMPLING_POINT_LABEL_COLUMN = "sample.samplingPoint.label";
const std::string SAMPLE_DATE_TIME_COLUMN = "sample.sampleDateTime";
const std::string DETERMINAND_LABEL_COLUMN = "determinand.label";
const std::string DETERMINAND_DEFINITION_COLUMN = "determinand.definition";
const std::string DETERMINAND_UNIT_LABEL_COLUMN = "determinand.unit.label";
const std::string RESULT_COLUMN = "result";
const std::string DETERMINAND_NOTATION_COLUMN = "determinand.notation";

// Additional index maps
std::unordered_map<std::string, std::vector<Measurement> > measurementsByDeterminand;
std::unordered_map<std::string, std::vector<Measurement> > measurementsBySamplingPoint;

void Dataset::loadDataset(const std::string &path) {
    csv::CSVReader csvReader(path);
    data.clear();
    measurementsByDeterminand.clear();
    measurementsBySamplingPoint.clear();

    ComplianceChecker complianceChecker;
    std::unordered_set<std::string> validCompounds;

    // Fill the set with valid determinand labels from the complianceThresholds map
    for (const auto &pair: complianceChecker.complianceThresholds) {
        validCompounds.insert(pair.first);
    }

    // Iterate through CSV rows
    for (const auto &row: csvReader) {
        const std::string determinandLabel = row[DETERMINAND_LABEL_COLUMN].get<>();
        const std::string samplingPoint = row[SAMPLING_POINT_LABEL_COLUMN].get<>();

        if (validCompounds.find(determinandLabel) != validCompounds.end()) {
            Measurement measurement{
                row[ID_COLUMN].get<>(),
                row[SAMPLE_POINT_COLUMN].get<>(),
                row[SAMPLING_POINT_LABEL_COLUMN].get<>(),
                row[SAMPLE_DATE_TIME_COLUMN].get<>(),
                determinandLabel,
                row[DETERMINAND_DEFINITION_COLUMN].get<>(),
                row[DETERMINAND_UNIT_LABEL_COLUMN].get<>(),
                row[RESULT_COLUMN].get<double>(),
                row[DETERMINAND_NOTATION_COLUMN].get<int>()
            };
            data.push_back(measurement);

            // Populate index maps
            measurementsByDeterminand[determinandLabel].push_back(measurement);
            measurementsBySamplingPoint[samplingPoint].push_back(measurement);
        }
    }
}

// https://cplusplus.com/reference/algorithm/find/
std::vector<Measurement> Dataset::getMeasurementsByDeterminand(const std::string &determinandLabel) const {
    auto it = measurementsByDeterminand.find(determinandLabel);
    if (it != measurementsByDeterminand.end()) {
        return it->second;
    }
    return {}; // Return an empty vector if not found
}

std::vector<Measurement> Dataset::getMeasurementsBySamplingPoint(const std::string &samplingPoint) const {
    auto it = measurementsBySamplingPoint.find(samplingPoint);
    if (it != measurementsBySamplingPoint.end()) {
        return it->second;
    }
    return {}; // Return an empty vector if not found
}


Dataset Dataset::queryDeterminand(const std::string &query) const {
    if (query.empty()) {
        return *this;
    }

    std::unordered_map<std::string, int> distanceCache;
    std::vector<std::pair<Measurement, int> > distances;

    for (const auto &m: data) {
        const std::string compoundName = m.getCompoundName();

        if (distanceCache.find(compoundName) == distanceCache.end()) {
            int distance = levenshteinDist(query, compoundName);
            distanceCache[compoundName] = distance;
        }

        distances.emplace_back(m, distanceCache[compoundName]);
    }

    std::sort(distances.begin(), distances.end(),
              [](const std::pair<Measurement, int> &a, const std::pair<Measurement, int> &b) {
                  return a.second < b.second;
              });

    std::vector<Measurement> sorted_measurements;
    for (const auto &[measurement, distance]: distances) {
        if (distance <= 5) {
            sorted_measurements.push_back(measurement);
        }
    }
    return Dataset(sorted_measurements);
}

void Dataset::checkDataExists() const {
    if (size() == 0) {
        throw std::runtime_error("Dataset is empty!");
    }
}


// https://github.com/guilhermeagostinelli/levenshtein/blob/master/levenshtein.cpp
// Returns the Levenshtein distance between word1 and word2.
int Dataset::levenshteinDist(const std::string &word1, const std::string &word2) {
    int size1 = word1.size();
    int size2 = word2.size();
    std::vector<std::vector<int> > verif(size1 + 1, std::vector<int>(size2 + 1)); // Use dynamic allocation with vector

    // If one of the words has zero length, the distance is equal to the size of the other word.
    if (size1 == 0)
        return size2;
    if (size2 == 0)
        return size1;

    // Sets the first row and the first column of the verification matrix with the numerical order from 0 to the length of each word.
    for (int i = 0; i <= size1; i++)
        verif[i][0] = i;
    for (int j = 0; j <= size2; j++)
        verif[0][j] = j;

    // Verification step / matrix filling.
    for (int i = 1; i <= size1; i++) {
        for (int j = 1; j <= size2; j++) {
            // Sets the modification cost.
            // 0 means no modification (i.e. equal letters) and 1 means that a modification is needed (i.e. unequal letters).
            int cost = (word2[j - 1] == word1[i - 1]) ? 0 : 1;

            // Sets the current position of the matrix as the minimum value between a (deletion), b (insertion) and c (substitution).
            // a = the upper adjacent value plus 1: verif[i - 1][j] + 1
            // b = the left adjacent value plus 1: verif[i][j - 1] + 1
            // c = the upper left adjacent value plus the modification cost: verif[i - 1][j - 1] + cost
            verif[i][j] = std::min(
                std::min(verif[i - 1][j] + 1, verif[i][j - 1] + 1),
                verif[i - 1][j - 1] + cost
            );
        }
    }

    // The last position of the matrix will contain the Levenshtein distance.
    return verif[size1][size2];
}
