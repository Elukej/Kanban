import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import MainSingleton 1.0
import "DialogChecker.js" as DialogChecker

Dialog {
    id: dialog 
    property bool editMode: false
    property int editIndex: 0
    property alias titleText: dialogTitle.text
    property alias descriptionText: dialogDescription.text
    property alias startDateText: dialogStartDate.text
    property alias estimationText: dialogEstimation.text
    property alias statusText: dialogStatus.text 

    height: (parent.height < 1000) ? 550 : ((parent.height < 1250) ? parent.height / 2: 625)
    width: 400
    modal: true
    focus: true
    title: qsTr("Inserting a Task")
    anchors.centerIn: parent
    header: Rectangle {
        color: Material.color(Material.Blue)
        height: 20
        Text { 
            anchors.horizontalCenter:parent.horizontalCenter
            text: "Insert Task"
        }
    }
    background: Rectangle { color: Material.color(Material.Blue)}
    standardButtons: Dialog.Apply | Dialog.Cancel
    footer: DialogButtonBox {
        background: Rectangle {
            color: Material.color(Material.Blue)
            }
    }

    onApplied: {
        let temp1 = dialogStartDate.text.split('-')
        let temp2 = dialogEstimation.text.split('-')
        let task = {  
                      "title" : dialogTitle.text,
                      "description" : dialogDescription.text,
                      "startDate" : new Date(temp1[0],temp1[1] - 1,temp1[2]),
                      "estimation" : new Date(temp2[0],temp2[1] - 1,temp2[2]),
                      "status" : dialogStatus.text
                   };
        
        let messages = DialogChecker.checkDialog(columnLayout);
        let hasErrors = false;
        let i = -1;

        for (var mes in messages) {
            i++;
            if (messages[mes] === true) {
                columnLayout.children[i].warningVisible = false;  
                continue;
            }
            columnLayout.children[i].warning = messages[mes];
            columnLayout.children[i].warningVisible = true;
            hasErrors = true;
        }

        if (task != {} && !hasErrors) {
            if (!editMode) MainSingleton.model.append(task)
            else MainSingleton.model.update(editIndex, task)
            dialog.accept()
            DialogChecker.resetDialog(columnLayout)
        }
    }
    onRejected: DialogChecker.resetDialog(columnLayout)


    Component.onCompleted: dialog.standardButton(Dialog.Apply).text = "Ok" 
    // easier to change just text of apply button then to mess with signals for ok


    contentItem: ScrollView {
        clip: true
        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            spacing: 6   

            TextFieldWithWarning {
                id: dialogTitle
                objectName: "dialogTitle"
                Layout.topMargin: 50
                Layout.fillWidth: true
                label: "Title:"
                placeholderText: "title"
                onAccepted: dialog.applied()    
            } 

            TextFieldWithWarning {
                id: dialogDescription
                objectName: "dialogDescription"
                Layout.fillWidth: true
                label: "Description:"
                placeholderText: "description"
                onAccepted: dialog.applied()
            } 
                
            TextFieldWithWarning {
                id: dialogStartDate
                objectName: "dialogStartDate"
                Layout.fillWidth: true
                label: "StartDate:"
                placeholderText: "year-month-day"
                onAccepted: dialog.applied()
            } 
                
            TextFieldWithWarning {
                id: dialogEstimation
                objectName: "dialogEstimation"
                Layout.fillWidth: true
                label: "Estimation:"
                placeholderText: "year-month-day"
                onAccepted: dialog.applied()
            }
                
            TextFieldWithWarning {
                id: dialogStatus
                objectName: "dialogStatus"
                Layout.fillWidth: true
                label: "Status:"
                placeholderText: "status"
                onAccepted: dialog.applied()
            }
        }
    }
}