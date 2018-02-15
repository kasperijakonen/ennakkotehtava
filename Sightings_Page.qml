import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3


ListView {
    id: listView
    anchors.fill: parent
    anchors.topMargin: 180

    flickableDirection: Flickable.VerticalFlick
    headerPositioning: ListView.OverlayHeader

    header: Row {
        z: 2
        spacing: 1
        function itemAt(index) { return repeater.itemAt(index) }

        Repeater {
            id: repeater
            model: ["id", "species", "description", "time", "count"]

            Label {
                id: sighting_label
                text: modelData
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 16

                height: 100
                width: {
                    if (modelData == "id") {
                        60
                    }
                    else if (modelData == "count") {
                        170
                    }
                    else {
                        (mainWindow.width-230) / 3
                    }
                }
                background: Rectangle { color: "silver" }
            }
        }
    }
    // The number of sightings
    model: server_object.giveVectorSize

    delegate: Column {
        id: delegate
        property int row: index

        Row {
            height: 300
            spacing: 1

            Repeater {
                model: 5

                ItemDelegate {
                    property int column: index
                    width: listView.headerItem.itemAt(column).width
                    height: 200

                    Text {
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        height: 250
                        width: listView.headerItem.itemAt(column).width
                        text: qsTr("%1").arg(server_object.giveNextData)
                        font.pointSize: 12
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
        Rectangle {
            color: "white"
            width: parent.width
            height: 1
        }
    }

    ScrollIndicator.horizontal: ScrollIndicator { }
    ScrollIndicator.vertical: ScrollIndicator { }
}
