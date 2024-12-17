#include "GH.hpp"
#include <iostream>
#include <QQmlContext>
#include <QtWidgets>
#include <QQuickWidget>
#include "../utils/SamplingPoint.hpp"

GeographicalHotspotsPage::GeographicalHotspotsPage(QWidget *parent) : QWidget(parent) {
    mapView = new QQuickWidget(this);

    std::cout << "DISPLAYING MAP" << std::endl;

    // Delay setting source until model is ready
    mapView->setResizeMode(QQuickWidget::SizeRootObjectToView);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(mapView);
    setLayout(layout);

    connect(&GlobalDataModel::instance(), &GlobalDataModel::dataReady,
            this, &GeographicalHotspotsPage::initializeMapWithData);
}

void GeographicalHotspotsPage::initializeMapWithData() {
    std::cout << "MAP INITIALISED MAP" << std::endl;

    auto model = new SamplingPointModel(this);
    mapView->rootContext()->setContextProperty("samplingPointModel", model);
    mapView->rootContext()->setContextProperty("dataset", &GlobalDataModel::instance().getDataset());


    // Set the source after the model is ready
    mapView->setSource(QUrl(QStringLiteral("qml/Map.qml")));
}
