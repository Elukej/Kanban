import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import MainSingleton 1.0

ScrollView {
    id: scroll
    clip: true
    signal submit(user : string)

    Item {
        id: content
        anchors.centerIn: parent // iz nekog razloga ne radi ako se ovde stavi root umesto parent. ne znam zasto
        implicitHeight: childrenRect.height + login.y + 50 // 50 is scroll offset. childrenRect has size properties of all item children summed
        //color: "transparent"
        
        Text {
            id: login
            text: "<b>LOGIN</b>"
            anchors.horizontalCenter: parent.horizontalCenter
           // color: "#B76D68" //"#f2f718"
            y: scroll.parent.height * 0.2 //ovaj scroll.parent.height vredi razmisliti da li je dobro staviti, ali mi se cini da je komponenta nezavisnija na ovaj nacin od komponent u koju se ubaci
            font.pointSize: 90
        }

        TextFieldWithWarning {
            id: username
            textWidth: login.width
            anchors.top: login.bottom
            anchors.topMargin: 50
            anchors.horizontalCenter: content.horizontalCenter
            placeholderText: "username"
            label: "Username:"
            warning: "Wrong username or password!"
            onAccepted: signin.clicked() // when enter is pressed sign in user
        }
        
        TextFieldWithWarning {
            id: password
            textWidth: login.width
            anchors.top: username.bottom
            anchors.topMargin: 30
            anchors.horizontalCenter: content.horizontalCenter
            placeholderText: "password"
            label: "Password:"
            echoMode: TextInput.Password         
            onAccepted: signin.clicked() // when enter is pressed sign in user
        }  
        
        RowLayout {
            id: checkBoxRow
            spacing: 2
            anchors.top: password.bottom
            anchors.left: signin.left
            anchors.leftMargin: -12
            anchors.topMargin: 5
            CheckBox {
                id: seePassword
                checked: false
                font.pointSize: 14
                onClicked: {
                    password.echoMode = checked ? TextInput.Normal : TextInput.Password
                }
            }
            Label {
                text: "Show password"
                color: "#B76D68"
                font.pointSize: 13
            }
        }

        Button {
            id: signin
            text: "Sign In"
            width: checkBoxRow.width
            anchors.top: checkBoxRow.bottom
            anchors.horizontalCenter: content.horizontalCenter
            anchors.topMargin: 20
            Material.elevation: 4
            onClicked: {
                let user = username.text
                let pass = password.text
                password.text = ""
                if (MainSingleton.loginInit(user, pass)) {
                    username.warningVisible = false
                    submit(user);
                    username.text = ""
                }
                else username.warningVisible = true
            }
        }
    }
}