#pragma once
#ifndef DATASET_HPP
#define DATASET_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "Measurement.hpp"

class Dataset : public QObject {
    Q_OBJECT

public:
    // Constructors
    Dataset() = default;

    explicit Dataset(const std::string &filePath) { loadDataset(filePath); }

    explicit Dataset(const std::vector<Measurement> &data) : data(data) {
    }

    // Load dataset from file
    void loadDataset(const std::string &path);

    // Measurement Access
    std::vector<Measurement> getMeasurementsByDeterminand(const std::string &determinandLabel) const;

    std::vector<Measurement> getMeasurementsBySamplingPoint(const std::string &samplingPoint) const;

    Measurement getMeasurement(int determinand);

    // Dataset utility methods
    Q_INVOKABLE size_t size() const { return data.size(); }
    bool isEmpty() const { return data.empty(); }

    // Operators
    Measurement operator[](const int index) const { return data.at(index); }
    std::vector<Measurement>::iterator begin() { return data.begin(); }
    std::vector<Measurement>::iterator end() { return data.end(); }
    std::vector<Measurement>::const_iterator begin() const { return data.begin(); }
    std::vector<Measurement>::const_iterator end() const { return data.end(); }

    // Sampling point-related methods
    std::unordered_map<std::string, std::pair<int, std::string> > getSamplingPointCounts() const;

    std::unordered_map<std::string, std::pair<double, double> > samplingPointCoordinates;

    // Data analysis methods
    std::vector<Measurement> sortByTimestamp();

    std::vector<std::string> getHighDataPointLocations();

    std::vector<std::string> getHighDataPointLocations(const std::string &compound);

    Q_INVOKABLE int getPendingRequestsCount() const { return pendingRequests; };

public slots:
    void fetchLatLonForSamplingPoints();

private slots:
    void handleNetworkData(QNetworkReply *reply, const std::string &locationNotationStr);

signals:
    void pendingRequestsChanged();

    void fetchesComplete();

private:
    // Internal methods
    void checkDataExists() const;

    // Member variables
    std::string filePath;
    std::vector<Measurement> data;

    // Maps for efficient grouping and lookup
    std::unordered_map<std::string, std::vector<Measurement> > measurementsByDeterminand;
    std::unordered_map<std::string, std::vector<Measurement> > measurementsBySamplingPoint;

    // Network-related members
    QNetworkAccessManager networkManager;
    int pendingRequests = 0;
};

#endif // DATASET_HPP
