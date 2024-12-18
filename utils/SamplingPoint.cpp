//
// Created by Josh Mundray on 09/12/2024.
//

#include "SamplingPoint.hpp"

void SamplingPointModel::updateModel() {
    m_points.clear();
    Dataset &dataset = GlobalDataModel::instance().getDataset();
    auto counts = dataset.getSamplingPointCounts();

    for (const auto &pair: counts) {
        QVariantMap pointData;
        // Fetch the coordinates
        // Get the location notation (sampling point name)
        std::string locationNotation = pair.second.second;
        std::cout << locationNotation << std::endl;

        // Fetch the coordinates from the dataset
        auto coords = dataset.samplingPointCoordinates.find(locationNotation);
        // std::cout << "Sampling point: " << pair.second.first << std::endl;
        //
        if (coords != dataset.samplingPointCoordinates.end()) {
            pointData["lat"] = coords->second.first;
            pointData["lon"] = coords->second.second;
        } else {
            pointData["lat"] = QVariant(); // or some default value
            pointData["lon"] = QVariant(); // or some default value
        }

        pointData["count"] = pair.second.first;
        pointData["color"] = getColorForCount(pair.second.first).name();
        m_points.append(pointData);
    }
    emit pointsChanged();
}
