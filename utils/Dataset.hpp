#pragma once
#ifndef DATASET_HPP
#define DATASET_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <QObject>
#include <QNetworkAccessManager>
#include <unordered_set>

#include "Measurement.hpp"

class Dataset : public QObject {
    Q_OBJECT

public:
    Dataset() = default;

    explicit Dataset(const std::string &filePath) { loadDataset(filePath); }
    explicit Dataset(const std::vector<Measurement> &data) { this->data = data; }

    void loadDataset(const std::string &path);

    std::vector<Measurement> getMeasurementsByDeterminand(const std::string &determinandLabel) const;

    std::vector<Measurement> getMeasurementsBySamplingPoint(const std::string &samplingPoint) const;

    Measurement getMeasurement(int determinand);

    size_t size() const { return data.size(); }

    bool isEmpty() const {
        return data.empty();
    }

    Measurement operator[](const int index) const { return data.at(index); }

    std::vector<Measurement>::iterator begin() { return data.begin(); }
    std::vector<Measurement>::iterator end() { return data.end(); }

    std::vector<Measurement>::const_iterator begin() const { return data.begin(); }
    std::vector<Measurement>::const_iterator end() const { return data.end(); }

    std::unordered_map<std::string, int> getSamplingPointCounts() const;


    std::unordered_map<std::string, std::pair<double, double> > samplingPointCoordinates;

    std::vector<Measurement> sortByTimestamp();

    std::vector<std::string> getHighDataPointLocations();

public slots:
    void fetchLatLonForSamplingPoints();

private slots:
    void handleNetworkData(QNetworkReply *reply);

private:
    std::string filePath;
    std::vector<Measurement> data;
    QNetworkAccessManager networkManager;
    std::vector<std::string> highDataPointLocations;

    int pendingRequests;

    void checkDataExists() const;
};

#endif // DATASET_HPP
