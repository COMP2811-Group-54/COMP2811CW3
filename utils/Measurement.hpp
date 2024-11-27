//
// Created by Josh Mundray on 27/11/2024.
//
#pragma once
#ifndef MEASUREMENT_HPP
#define MEASUREMENT_HPP

#include <string>


class Measurement {
public:
    Measurement(const std::string &id, const std::string &samplingPoint, const std::string &label,
                const std::string &datetime,
                const std::string &compoundName, const std::string &description, const std::string &unit, const double value, const int determinand)
        : id(id), samplingPoint(samplingPoint), label(label), datetime(datetime),
          compoundName(compoundName), description(description), unit(unit), value(value), compoundDeterminand(determinand) {
    }

    std::string getId() const { return id; };
    std::string getSamplingPoint() const { return samplingPoint; };
    std::string getLabel() const { return label; }
    std::string getDatetime() const { return datetime; };
    std::string getCompoundName() const { return compoundName; };
    std::string getDescription() const { return description; };
    std::string getUnit() const { return unit; };

    double getValue() const;
    int getCompoundDeterminand() const;

private:
    std::string id;
    std::string samplingPoint;
    std::string label;
    std::string datetime;
    std::string compoundName;
    std::string description;
    std::string unit;
    double value{};
    int compoundDeterminand;
};


#endif //MEASUREMENT_HPP
