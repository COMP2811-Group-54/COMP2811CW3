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

    // Property to track the total number of initial requests
    property int totalInitialRequests: 0

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
                                 opacity: 0.5; \
                                 border.width: 0.5; \
                                 MouseArea { \
                                     anchors.fill: parent; \
                                     onClicked: { \
                                         map.center = QtPositioning.coordinate(' + lat + ', ' + lon + '); \
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

        DragHandler {
            id: drag
            target: null
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }
    }

    // Circular Progress Indicator
    Item {
        id: progressContainer
        anchors.centerIn: parent
        width: 200
        height: 130

        // Initially hidden, will show when there are pending requests
        visible: false
        opacity: 0

        Canvas {
            id: progressCanvas
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 100
            height: 100

            onPaint: {
                var ctx = getContext("2d");
                ctx.reset(); // Clear previous drawing

                // Background circle
                ctx.beginPath();
                ctx.arc(width/2, height/2, 40, 0, 2 * Math.PI);
                ctx.lineWidth = 10;
                ctx.strokeStyle = "rgba(224, 224, 224, 0.5)"; // Translucent gray background
                ctx.stroke();

                // Check total and pending requests
                var totalRequests = totalInitialRequests;
                var pendingRequests = dataset.getPendingRequestsCount();

                // Progress arc (starts full and empties)
                var angle = ((totalRequests - pendingRequests) / totalRequests) * 2 * Math.PI;

                ctx.beginPath();
                ctx.arc(width/2, height/2, 40, -Math.PI / 2, -Math.PI / 2 + angle);
                ctx.lineWidth = 10;
                ctx.strokeStyle = "#4CAF50"; // Green progress
                ctx.stroke();
            }
        }

        Text {
            id: progressText
            anchors.top: progressCanvas.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            font.pixelSize: 16
            font.weight: Font.Bold
            text: "Fetching data..."
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }
    }

    // Connections to update progress indicator
    Connections {
        target: dataset
        function onPendingRequestsChanged() {
            var pendingRequests = dataset.getPendingRequestsCount();

            // Cache the total initial requests when first fetch starts
            if (totalInitialRequests === 0 && pendingRequests > 0) {
                totalInitialRequests = pendingRequests;
            }

            console.log("Remaining fetches: " + pendingRequests);

            // Show progress container when there are pending requests
            progressContainer.visible = pendingRequests > 0;
            progressContainer.opacity = pendingRequests > 0 ? 1 : 0;

            // Update progress text
            progressText.text = pendingRequests + " fetches remaining";

            // Reset total requests when all fetches are done
            if (pendingRequests === 0) {
                totalInitialRequests = 0;
            }

            // Update progress canvas
            progressCanvas.requestPaint();
        }
    }

    Connections {
        target: samplingPointModel
        function onPointsChanged() {
            map.updateCircles()
        }
    }
}