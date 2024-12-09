#pragma once
#include <iostream>
#include <ostream>
#include <QColor>
#include <QObject>
#include <QVariantList>

#include "Dataset.hpp"
#include "GlobalDataModel.hpp"

class SamplingPointModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList points READ points NOTIFY pointsChanged)

public:
    explicit SamplingPointModel(QObject *parent = nullptr) : QObject(parent) {
        connect(&GlobalDataModel::instance(), &GlobalDataModel::allRequestsComplete,
                this, &SamplingPointModel::updateModel);
    }

    QVariantList points() const {
        return m_points;
    }

    // Function to calculate color based on count
    QColor getColorForCount(int count) {
        // Here you can define your color logic
        if (count > 10) return QColor("red");
        else if (count > 5) return QColor("orange");
        else return QColor("green");
    }

signals:
    void pointsChanged();

public slots:
    void updateModel() {
        m_points.clear();
        Dataset &dataset = GlobalDataModel::instance().getDataset();
        auto counts = dataset.getSamplingPointCounts();

        for (const auto &pair: counts) {
            QVariantMap pointData;
            // Fetch the coordinates
            auto coords = dataset.samplingPointCoordinates.find(pair.first);
            if (coords != dataset.samplingPointCoordinates.end()) {
                pointData["lat"] = coords->second.first;
                pointData["lon"] = coords->second.second;
                std::cout << coords->second.first << " " << coords->second.second << std::endl;
            } else {
                pointData["lat"] = QVariant(); // or some default value
                pointData["lon"] = QVariant(); // or some default value
            }

            pointData["count"] = pair.second;
            pointData["color"] = getColorForCount(pair.second).name();
            m_points.append(pointData);
        }
        emit pointsChanged();
    }

private:
    QVariantList m_points;
};
