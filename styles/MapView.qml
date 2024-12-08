// MapView.qml
import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15

Rectangle {
    width: 800
    height: 600

    Plugin {
        id: mapPlugin
        name: "osm"  // OpenStreetMap
    }

    Map {
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(54.0, -2.0) // Center focusing on the UK
        zoomLevel: 6.0 // Adjust to your preferred level
    }
}