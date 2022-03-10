import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import OpenCascade 7.6

Window {
    id: mainWindow
    visible: true
    width: 1040
    height: 680
    minimumWidth: 640
    minimumHeight: 480

    title: "OpenCascade Qt Viewer Demo"
    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 2
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
    }
    Text {
        id: label
        color: "black"
        text: "This render Open Cascade under QML"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }
    OCCT {
        id: openCascade
        anchors.fill: parent
        onOccViewChanged: mainWindow.update()
    }
    MouseArea {
        width: mainWindow.width
        height: mainWindow.height
        acceptedButtons:  Qt.AllButtons

        onPressed:(mouse)=> {
            openCascade.mousePress(mouse.x, mouse.y, mouse.buttons)
        }
        onReleased:(mouse)=> {
            openCascade.mouseRelease(mouse.x, mouse.y, mouse.buttons)
        }
        onWheel:(wheel)=> {
            openCascade.mouseWheel(wheel.x, wheel.y, wheel.angleDelta.y, wheel.buttons)
        }
        onPositionChanged:(mouse)=> {
            openCascade.mouseMove(mouse.x, mouse.y, mouse.buttons)
        }
    }
}
