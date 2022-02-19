import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id: main_window
    width: 800
    height: 600
    visible: true
    title: qsTr("OpenCascadeQML")
    Item {
        id: main_item
        Item {
            id: occt_viewer
        }
        MouseArea {
            id: viewer_mouse_area
        }
    }
}
