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
        center: QtPositioning.coordinate(53.8008, -1.5491) // Default center
        zoomLevel: 10
        minimumZoomLevel: 3
        maximumZoomLevel: 20

        property var dynamicCircles: [] // Keep references to dynamically created circles

        function updateCircles() {
            // Remove any previous circles before creating new ones
            for (let i = 0; i < dynamicCircles.length; ++i) {
                map.removeMapItem(dynamicCircles[i])
            }
            dynamicCircles = []

            // Add new circles from samplingPointModel
            for (let i = 0; i < samplingPointModel.points.length; ++i) {
                let point = samplingPointModel.points[i]
                let lat = point.lat
                let lon = point.lon
                let color = point.color
                if (lat !== undefined && lon !== undefined && color !== undefined) {
                    try {
                        let createdCircle = Qt.createQmlObject(
                            'import QtQuick 2.15; \
                             import QtLocation 5.15; \
                             import QtPositioning 5.15; \
                             MapCircle { \
                                 center: QtPositioning.coordinate(' + lat + ', ' + lon + '); \
                                 radius: 1000; \
                                 color: "' + color + '"; \
                                 border.width: 3; \
                                 MouseArea { \
                                     anchors.fill: parent; \
                                     onClicked: { \
                                         map.center = QtPositioning.coordinate(' + lat + ', ' + lon + '); \
                                         console.log("Focus on: " + ' + lat + ' + ", " + ' + lon + '); \
                                     } \
                                 } \
                             }',
                            map,  // Parent is map
                            "dynamicCircle"
                        )
                        map.addMapItem(createdCircle)
                        dynamicCircles.push(createdCircle)
                    } catch (error) {
                        console.error("Error creating circle:", error)
                    }
                } else {
                    console.warn("Skipping invalid sampling point:", point)
                }
            }
        }

        Component.onCompleted: {
            updateCircles()
        }
Component {
    id: circularSpinner
    Rectangle {
        width: 48
        height: 48
        color: "transparent"
        radius: width / 2
        border.color: "gray"
        border.width: 3
        layer.enabled: true
        SequentialAnimation {
            running: true
            loops: Animation.Infinite
            NumberAnimation {
                target: spinnerRectangle
                property: "rotation"
                from: 0
                to: 360
                loops: Animation.Infinite
                duration: 1000
            }
        }
        Rectangle {
            id: spinnerRectangle
            width: 48
            height: 48
            radius: width / 2
            border.color: "gray"
            border.width: 3
            anchors.centerIn: parent
            rotation: 0
        }

        RotationAnimation on rotation {
            from: 0; to: 360
            running: true
            loops: Animation.Infinite
            duration: 1000
        }
    }
}
        PinchHandler {
            id: pinch
            target: null
            onActiveChanged: if (active) {
                map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
            }
            onScaleChanged: (delta) => {
                map.zoomLevel += Math.log2(delta)
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
            }
            onRotationChanged: (delta) => {
                map.bearing -= delta
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
            }
            grabPermissions: PointerHandler.TakeOverForbidden
        }
        WheelHandler {
            id: wheel
            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                ? PointerDevice.Mouse | PointerDevice.TouchPad
                : PointerDevice.Mouse
            property real zoomIncrement: 1.2
            onWheel: {
                let zoomFactor = wheel.angleDelta.y > 0 ? zoomIncrement : (1 / zoomIncrement)
                map.zoomLevel = Math.max(map.minimumZoomLevel, Math.min(map.maximumZoomLevel, map.zoomLevel * zoomFactor))
            }
        }
        DragHandler {
            id: drag
            target: null
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }
        Shortcut {
            enabled: map.zoomLevel < map.maximumZoomLevel
            sequence: StandardKey.ZoomIn
            onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
        }
        Shortcut {
            enabled: map.zoomLevel > map.minimumZoomLevel
            sequence: StandardKey.ZoomOut
            onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
        }
    }

    Connections {
        target: samplingPointModel

        function onPointsChanged() {
            map.updateCircles()
        }
    }

Item {
    anchors.centerIn: parent
    visible: !samplingPointModel || samplingPointModel.points.length === 0

    Loader {
        anchors.centerIn: parent
        active: true
        sourceComponent: circularSpinner
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: 40
        font.pixelSize: 24
        font.bold: true
        text: "Loading water sampling data..."
        color: "gray"
    }
}
}