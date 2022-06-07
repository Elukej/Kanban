import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import MainSingleton 1.0

ScrollView {
  /*signal signout()
    signal draw()

    Button {
        text: qsTr("Sign Out")
        anchors.right: parent.right
        anchors.top: parent.top
        onClicked: signout()
    } */ //This code is very good way to propagate information into upper qml files(main.qml) but i dont need it
        // since signout will happen through header, and draw also. I leave it as reminder for me

    RoundButton {
        text: "+"
        anchors.left: parent.left
        anchors.bottom: view.top
        onClicked: {
            dialog.editMode = false
            dialog.open()
        }
    }

    ListView {
        id: view
        anchors.centerIn: parent
        width: parent.width
        y: 45
        spacing: 5
        height: contentHeight
        model: MainSingleton.model
        delegate: ItemDelegate {
            text: model.title + "     " + model.description + "   " + 
                  model.startDate.toLocaleDateString(Qt.locale(), "yyyy-MM-dd") + "    " +  
                  model.estimation.toLocaleDateString(Qt.locale(), "yyyy-MM-dd") + "    " +
                  model.status
            font.pointSize: 10
            height: 30
            width: 200//ListView.width // nikako parent width, ne radi nista, mora ovaj attachedProperty
            onClicked: {
                dialog.editMode = true
                dialog.editIndex = model.index
                dialog.open()
                dialog.titleText = model.title
                dialog.descriptionText = model.description
                dialog.startDateText = model.startDate.toLocaleDateString(Qt.locale(), "yyyy-MM-dd")
                dialog.estimationText = model.estimation.toLocaleDateString(Qt.locale(), "yyyy-MM-dd")
                dialog.statusText = model.status
            }
            HoverHandler {
                onHoveredChanged: parent.width = (hovered) ? parent.ListView.width : 200
            }
        }
        
        ScrollBar.vertical: ScrollBar {
                           active: true
        }
    }
    
    TaskInsertDialog {
        id: dialog
    }

}  