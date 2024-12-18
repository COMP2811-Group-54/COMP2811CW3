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
    // Derived from: https://stackoverflow.com/questions/51708003/qt-q-propertys-notify-signal-not-emited-about-member-change
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
        if (count > 10) return QColor("red");
        else if (count > 5) return QColor("orange");
        else return QColor("green");
    }

signals:
    void pointsChanged();

public slots:
    void updateModel();

private:
    QVariantList m_points;
};
