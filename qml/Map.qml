import QtQuick 2.15
import QtPositioning 5.15
import QtLocation 5.15

Rectangle {
    width: 800
    height: 600

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(53.8008, -1.5491)
        zoomLevel: 10
        minimumZoomLevel: 3
        maximumZoomLevel: 20

        function updateCircles() {
            for (var i = 0; i < samplingPointModel.points.length; ++i) {
                let point = samplingPointModel.points[i]
                let lat = point.lat
                let lon = point.lon
                let color = point.color
                if (lat !== undefined && lon !== undefined) {
                    let createdCircle = Qt.createQmlObject(
                        'import QtLocation 5.15; \
                         import QtPositioning 5.15; \
                         import QtQuick 2.15; \
                         MapCircle { \
                             id: dynamicCircle; \
                             center: QtPositioning.coordinate(' + lat + ', ' + lon + '); \
                             radius: 1000; \
                             color: "' + color + '"; \
                             border.width: 3; \
                             MouseArea { \
                                 anchors.fill: parent; \
                                 onClicked: { \
                                     map.center = dynamicCircle.center; \
                                     console.log("Focus on: " + dynamicCircle.center.latitude + ", " + dynamicCircle.center.longitude); \
                                 } \
                             } \
                         }',
                        map,  // Parent is map
                        "dynamicCircle"
                    )
                    map.addMapItem(createdCircle)
                }
            }
        }

        Component.onCompleted: {
            updateCircles()
        }

        // WheelHandler for zoom functionality with trackpad and mouse wheel
        WheelHandler {
            target: map
            onWheel: {
                if (wheel.angleDelta.y !== 0) {
                    let zoomChange = wheel.angleDelta.y > 0 ? 1 : -1;
                    map.zoomLevel = Math.max(
                        map.minimumZoomLevel,
                        Math.min(map.maximumZoomLevel, map.zoomLevel + zoomChange)
                    )
                }
            }
        }
    }

    Connections {
        target: samplingPointModel
        function onPointsChanged() {
            map.updateCircles()
        }
    }

    Text {
        anchors.centerIn: parent
        text: "Loading data..."
        visible: !samplingPointModel || samplingPointModel.points.length === 0
    }
}