#pragma once
#ifndef MEASUREMENT_HPP
#define MEASUREMENT_HPP

#include <string>
#include <QDateTime>
#include <utility>

class Measurement {
public:
    Measurement(std::string id, std::string samplingPoint, std::string notation, std::string label,
                const std::string &datetimeStr,
                std::string compoundName, std::string description, std::string unit, const double value,
                const int determinand)
        : id(std::move(id)), samplingPoint(std::move(samplingPoint)), notation(std::move(notation)),
          label(std::move(label)),
          datetime(QDateTime::fromString(QString::fromStdString(datetimeStr), "yyyy-MM-dd'T'HH:mm:ss")),
          compoundName(std::move(compoundName)), description(std::move(description)), unit(std::move(unit)),
          value(value), compoundDeterminand(determinand) {
    }

    std::string getId() const { return id; }
    std::string getSamplingPoint() const { return samplingPoint; }
    std::string getSamplingPointNotation() const { return notation; }
    std::string getLabel() const { return label; }
    QDateTime getDatetime() const { return datetime; }
    std::string getCompoundName() const { return compoundName; }
    std::string getDescription() const { return description; }
    std::string getUnit() const { return unit; }

    double getValue() const;

    int getCompoundDeterminand() const;

private:
    std::string id;
    std::string samplingPoint;
    std::string label;
    std::string notation;
    QDateTime datetime;
    std::string compoundName;
    std::string description;
    std::string unit;
    double value{};
    int compoundDeterminand;
};

#endif // MEASUREMENT_HPP
