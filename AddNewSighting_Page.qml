import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: item1
    anchors.fill: parent

    GridLayout {
        id: speciesGrid
        anchors.fill: parent
        anchors.topMargin: 200
        anchors.bottomMargin: mainWindow.height / 2

        rows: server_object.giveSpeciesVectorSize
        columns: 1

        ButtonGroup {
            id: btnGroup
        }

        Repeater {
            id: repeater
            model: server_object.giveSpeciesVectorSize


            Button {
                id: speciesButton
                checked: true
                checkable: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.rightMargin: 20
                Layout.leftMargin: 20
                opacity: !speciesButton.pressed ? 1 : 0.40

                property string species: speciesLabel.text

                ButtonGroup.group: btnGroup

                Rectangle {
                    id: oldentrance_rect
                    anchors.fill: parent
                    color: "lightgrey"
                    border.color: "white"
                    border.width: parent.checked ? 7 : 0
                    Label {
                        id: speciesLabel
                        text: server_object.giveNextSpecies
                        width: parent.width
                        wrapMode: Text.WordWrap
                        font.pointSize: 16
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.centerIn: parent
                        z: 1
                    }
                }
                onClicked: {
                    checked: true
                }
            }

        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.topMargin: mainWindow.height / 2 + 20
        id: sighting_grid
        rows: 4
        columns: 1

        Label {
            id: label
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            Text {
                id: count
                anchors.centerIn: parent
                text: qsTr("Count:")
                font.pointSize: 18
                color: "white"
            }
        }

        SpinBox {
            id: control
            height: 150
            Layout.fillWidth: true
            Layout.rightMargin: 180
            Layout.leftMargin: 180
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            value: 1
            from: 1
            to: 100
            editable: true

            contentItem: TextInput {
                z: 2
                text: control.textFromValue(control.value, control.locale)
                font.pointSize: 16
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                readOnly: !control.editable
                validator: control.validator
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }

            up.indicator: Rectangle {
                x: control.mirrored ? 0 : parent.width - width
                height: parent.height
                implicitWidth: 100
                implicitHeight: parent.height
                color: control.up.pressed ? "#e4e4e4" : "#f6f6f6"
                border.color: "grey"

                Text {
                    text: "+"
                    font.pixelSize: control.font.pixelSize * 1.5
                    anchors.fill: parent
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            down.indicator: Rectangle {
                id: rectangle
                x: control.mirrored ? parent.width - width : 0
                height: parent.height
                implicitWidth: 100
                implicitHeight: parent.height
                color: control.down.pressed ? "#e4e4e4" : "#f6f6f6"
                border.color: "grey"

                Text {
                    text: "-"
                    font.pixelSize: control.font.pixelSize * 1.5
                    anchors.fill: parent
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            background: Rectangle {
                implicitWidth: 140
                border.color: "#bdbebf"
            }
        }

        TextField {
            id: textField
            height: 80
            Layout.fillWidth: true
            Layout.rightMargin: 180
            Layout.leftMargin: 180
            placeholderText: qsTr("description")
        }


        Button {
            id: button

            text: qsTr("Submit sighting")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: {
                server_object.postSighting(btnGroup.checkedButton.species, textField.text, control.value)
                server_object.fetch()
            }
        }
    }
}
