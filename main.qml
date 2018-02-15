import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3


ApplicationWindow {
    id: mainWindow

    visible: true
    width: 430
    height: 740
    title: qsTr("Duck sighting")

    //The background color
    Rectangle {
        id: background
        //color: "lightsteelblue"
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightgrey" }
            GradientStop { position: 1.0; color: "grey" }
        }

        visible: true
        border.width: 0
        anchors.fill: parent
    }

    TabBar {
        id: bar
        width: parent.width
        contentHeight: 170
        z: 3

        TabButton {
            text: qsTr("Add a new sighting")
        }
        TabButton {
            text: qsTr("Sightings")
        }
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: bar.currentIndex
        Item {
            id: addNew_tab
            AddNewSighting_Page {}
        }
        Item {
            id: sightings_tab
            Sightings_Page {}
        }
    }
}

