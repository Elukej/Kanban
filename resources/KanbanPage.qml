import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import MainSingleton 1.0

Item {
    id: root
    Row {
        anchors.fill: parent
        spacing: 1
        TaskListView {
            id: toDo
            headerText: "TO DO"
            status: "To Do"
            borderColor: Material.color(Material.Red)
        }
        TaskListView {
            id: inProgress
            headerText: "IN PROGRESS"
            status: "In Progress"
            borderColor: Material.color(Material.Green)
        }
        TaskListView {
            id: blocked
            headerText: "BLOCKED"
            status: "Blocked"
            borderColor: Material.color(Material.Teal)
        }
        TaskListView {
            id: done
            headerText: "DONE"
            status: "Done"
            borderColor: Material.color(Material.Orange)
        }

    } 
}