// GeographicalHotspotsPage.hpp
#pragma once

#include <QWidget>
#include <QQuickWidget>

class GeographicalHotspotsPage : public QWidget {
    Q_OBJECT

public:
    explicit GeographicalHotspotsPage(QWidget *parent = nullptr);

    void initializeMapWithData();

private:
    QQuickWidget *mapView;
};
