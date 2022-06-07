import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Item {
    id: root
    property alias textWidth: textField.width
    property alias text: textField.text
    property alias echoMode: textField.echoMode
    property alias placeholderText: textField.placeholderText
    property alias validator: textField.validator
    property alias label: label.text
    property alias warning: warning.text
    property alias warningVisible: warning.visible
    implicitHeight: warningVisible ? childrenRect.height : childrenRect.height - warning.height
    //if warning is invisible, treat it like it doesnt exist in item
    signal accepted()

    Label {
        id: label
        text: "Dummy text"
        anchors.bottom: textField.top
        anchors.left: textField.left
      //  color: "#B76D68"
        font.pointSize: 13
    }

    TextField {
        id: textField
        implicitWidth: 200
        anchors.horizontalCenter: parent.horizontalCenter
        //color: Material.color(Material.Amber)
        placeholderText: "dummy text"
        font.pointSize: 17
        Component.onCompleted: textField.accepted.connect(root.accepted) //odlicna fora za povezivanje vise signala
    } 

    Label {
        id: warning
        width: textField.width
        anchors.top: textField.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 15
        wrapMode: Text.Wrap
        text: "Invalid input for " + label.text
        color: Material.color(Material.Red)
        visible: false
    }     
}