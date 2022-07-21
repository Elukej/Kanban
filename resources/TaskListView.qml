import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import MainSingleton 1.0

DropArea {
    id: root
    property var headerText: "" //cant be aliased so the text in header will follow this value
    property var status: "" // value of status (To Do, In Progress, Blocked, Done)
    property var borderColor: ""
    width: parent.width/4 - 0.75 * parent.spacing // component meant to be inside row 
    height: parent.height
    onEntered: { // signal is entered(drag)
        console.log(headerText + " drop area entered")
       // console.log(Object.keys(drag.source))
       drag.source["background"]["color"] = borderColor
    }

    onDropped: { //signal is dropped(drop)
        console.log("item dropped in " + headerText + " drop area")
        drop.source["delegateStatus"] = root.status //delegateStatus is changed in order for its change handler to update the model
        //drop.source["background"]["color"] = drop.source.ListView.view.headerItem["color"] // i leave this a s a reminder how to access javascript objects
        // we return the color of the delegate to its list view creator so the colors dont get messed up, since qt is caching delegates
        //drop.source is delegate item as a js object
    }
    
    Rectangle {
        border.color: borderColor
        border.width: 2
        anchors.fill: parent
        color: "transparent"
        ListView {
            id: view
            anchors.fill: parent
            header: Label {
                id: label
                width: ListView.view.parent.width
                horizontalAlignment: Text.AlignHCenter
                text: headerText
                color: root.borderColor
                font { pointSize: 20; bold: true }
                wrapMode: Text.Wrap
            }
            model: MainSingleton.model
            delegate: ItemDelegate {
                id: itemDelegate
                property var delegateStatus: ""
                property var xBeforeDrag:;
                property var yBeforeDrag:;
                visible: model.status == root.status
                height: (visible) ? implicitHeight : 0
                width: ListView.view.width - 10
                bottomInset: 2
                x: (ListView.view.width - width)/2
                
                Component.onCompleted: delegateStatus = model.status
                onDelegateStatusChanged: model.status = delegateStatus

                Binding {
                    target: background
                    property: "color"
                    value: root.borderColor
                }
                contentItem: Label {
                    color: "white"
                    text: model.title
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 15
                    wrapMode: Text.Wrap
                }
                Drag.active: dragArea.drag.active //without this no drop events are sent to drop area
                Drag.source: itemDelegate
                Drag.hotSpot.x: width/2
                Drag.hotSpot.y: height/2
                MouseArea {
                    id: dragArea
                    anchors.fill: parent
                    onReleased: {
                        parent.Drag.drop() // This isnt written in documentation!!! This function needs to be manually called in order for the recipient of drop event to be notified
                        itemDelegate.x = itemDelegate.xBeforeDrag //we return delegate to its position in the listview in which it belongs where it wont be rendered if its status has changed
                        itemDelegate.y = itemDelegate.yBeforeDrag
                        itemDelegate.background.color = parent.ListView.view.headerItem["color"]
                    }
                    onPressed: {
                        xBeforeDrag = itemDelegate.x //we remember the coordinates of item when we start dragging it
                        yBeforeDrag = itemDelegate.y
                    }
                    drag.target: itemDelegate
                }
            }
        }
    }
}