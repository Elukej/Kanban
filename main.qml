import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "resources"
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    id: root
    width: 640
    height: 600
    visible: true
    title: qsTr("Task viewer app")
    Material.theme: Material.Light
    Material.accent: "#B76D68"

    background: Rectangle {
        anchors.fill: parent
    }

    header: ToolBar {
        id: menuBar
        Material.background: Material.Blue
        visible: false
        ToolButton {
            id: drawerButton
            anchors { left: parent.left; verticalCenter: parent.verticalCenter }
            icon.source: "resources/baseline-menu-24px.svg"
            onClicked: drawer.open()
        }
        Rectangle {
            id: circle
            width: 20
            height: 20
            radius: 180
            anchors { right: userLabel.left; verticalCenter: parent.verticalCenter; rightMargin:10 }
        }
        Label {
            id: pageName
            anchors { left: drawerButton.right; verticalCenter: parent.verticalCenter }
            font.pointSize: 15
        }
        Label {
            id: userLabel
            anchors { right: parent.right; verticalCenter: parent.verticalCenter; rightMargin: 5; }
            font.pointSize: 15
        }
    }     

    Drawer {
        id: drawer
        width: root.width < 600 ? 0.6 * root.width : 400
        height: root.height
        Material.background: Material.Orange
        ListView {
            id:view
            focus: true
            currentIndex: -1
            anchors.fill: parent
            delegate: ItemDelegate {
                width: parent.width
                text: model.text
                font.pointSize: 14
                highlighted: ListView.isCurrentItem
                onClicked: {
                    if (text !== stackView.state) {
                        switch(text) {
                            case "Tasks":
                                stackView.push(tasksPage)     
                                break;                    
                            case "Kanban":
                                stackView.push(kanbanPage)
                                break;
                        }
                        pageName.text = text
                        stackView.state = text
                        drawer.close()     
                    }
                }
            }
            model: ListModel {
                ListElement {
                    text: qsTr("Tasks")
                }
                ListElement {
                    text: qsTr("Kanban")
                }
            }
            ItemDelegate {
                text: qsTr("Sign Out")
                anchors.bottom: parent.bottom
                width: drawer.width
                font.pointSize: 14
                onClicked: {
                    drawer.close()
                    header.visible = false
                    stackView.pop(null)
                    stackView.state = "login"
                }
            }
        }          
    }
 
    StackView{
        id: stackView
        anchors.fill: parent
        state: "login"
        states: [ //presumption is that state names mirror the drawer texts that lead to them (Tasks or Kanban)
            State { name: "login" },
            State { name: "Tasks" },
            State { name: "Kanban"}
        ]
        initialItem: LoginPage {
            onSubmit: { //signal is submit(user : string)
                userLabel.text = user
                pageName.text = "Tasks"
                stackView.push(tasksPage)
                stackView.state = "Tasks"
                circle.color =  "#" + (Math.floor(0xffffff * Math.random())).toString(16)
                header.visible = true
            }
        }
    }

    Component {
        id: tasksPage

        TasksPage {
        /*  onSignout: stackView.pop(null)
            onDraw: drawer.open() */ //usage of signals from bottom file. I leave it as a reminder
        }
    }

    Component {
        id: kanbanPage

        KanbanPage {
        /*  onSignout: stackView.pop(null)
            onDraw: drawer.open() */ //usage of signals from bottom file. I leave it as a reminder
        }
    }
}