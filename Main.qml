import QtQuick

Window {
    minimumWidth: 260
    minimumHeight: 380
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    visible: true
    title: qsTr("Guitar Pedal")

    FontLoader{
        id: russoFontLoader
        source: "fonts/RussoOne-Regular.ttf"
    }

    FontLoader{
        id: prismaFontLoader
        source: "fonts/Prisma.ttf"
    }

    Image {
        id: pedalBackground
        source: "assets/Guitar-Pedal-Background.png"
        anchors.fill: parent
    }

    Item{
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        anchors.topMargin: 17
        anchors.bottomMargin: 17

        component ScrewImage: Image{
            source: "assets/Screw.png"
        }

        ScrewImage {
            anchors.top: parent.top
            anchors.left: parent.left
        }

        ScrewImage {
            anchors.top: parent.top
            anchors.right: parent.right
        }

        ScrewImage {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }

        ScrewImage {
            anchors.bottom: parent.bottom
            anchors.right: parent.right
        }

        component DeviceText: Text{
            color: "#191919"
            font.family: russoFontLoader.font.family
            font.weight: russoFontLoader.font.weight
            font.pixelSize: 9
        }

        component InfoText: Column{
            id: infoLabel
            spacing: 5

            property alias text: label.text
            property alias font: label.font
            property int lineWidth: 200
            property int lineHeight: 2
            property color lineColor: "#191919"

            Rectangle{
                width: infoLabel.lineWidth
                height: infoLabel.lineHeight
                color: infoLabel.lineColor
            }

            Text {
                id: label
                text: qsTr("TIME BLANDER")
                font.pixelSize: 18
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Rectangle{
                width: infoLabel.lineWidth
                height: infoLabel.lineHeight
                color: infoLabel.lineColor
            }
        }

        InfoText {
            text:  qsTr("TIME BLANDER")
            anchors.top: parent.verticalCenter
            anchors.topMargin: 16
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            font.family: prismaFontLoader.font.family
            font.pixelSize: 18
        }

        InfoText {
            text:  qsTr("IN")
            anchors.top: parent.top
            anchors.topMargin: 60
            anchors.right: parent.right
            lineWidth: 30
            lineHeight: 2
        }

        InfoText {
            text:  qsTr("OUT")
            anchors.top: parent.top
            anchors.topMargin: 60
            anchors.left: parent.left
            lineWidth: 45
            lineHeight: 2
        }

    }

    component SwitchImage : Image {
        required property string sourceBaseName
        property bool checked

        source: `assets/${sourceBaseName}${checked ? "-Checked" : ""}.png`
    }

    component DeviceSwitch: SwitchImage {
        property alias topMargin: tapHandler.margin

        TapHandler {
            id: tapHandler
            onTapped: parent.checked = !parent.checked
        }
    }

    SwitchImage {
        x: parent.width * 0.33 - width / 2
        y: 14
        sourceBaseName: "LED"
        checked: footPedal.checked

        DeviceText{
            text: qsTr("CHECKED")
            anchors.top: parent.bottom
            anchors.topMargin: 4
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    DeviceSwitch {
        x: parent.width * 0.66 - width / 2
        y: 14
        sourceBaseName: "Switch"
        topMargin : 16

        DeviceText{
            text: qsTr("MODE")
            anchors.top: parent.bottom
            anchors.topMargin: 4
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    DeviceSwitch {
        id: footPedal
        sourceBaseName: "Button-Pedal"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 17
        checked: false
    }

    component DeviceDial: Image {
        id: dial
        source: "assets/Knob-Markings.png"

        property alias text: dialLabel.text

        property int value
        property real angle

        readonly property int minimumValue: 0
        readonly property int maximumValue: 100
        readonly property int range: dial.maximumValue - dial.minimumValue

        DragHandler {
            target: null
            onCentroidChanged: updateValueAndRotation()

            function updateValueAndRotation() {
                if(centroid.pressedButtons !== Qt.LeftButton){
                    return
                }

                const startAngle = -140
                const endAngle = 140

                const yy = dial.height / 2.0 - centroid.position.y
                const xx = centroid.position.x - dial.width / 2.0

                const radianAngle = Math.atan2(yy, xx)
                let newAngle = (-radianAngle / Math.PI * 180) + 90

                newAngle = ((newAngle - dial.angle + 180) % 360) + dial.angle - 180

                dial.angle = Math.max(startAngle, Math.min(newAngle,endAngle))
                dial.value = (dial.angle - startAngle) / (endAngle - startAngle) * dial.range

                console.log("angle: ", dial.angle, "value: ", dial.value)
            }
        }

        Image {
            source: "assets/Knob-Dial.png"
            anchors.centerIn: parent
            rotation: dial.angle
        }

        DeviceText {
            id: dialLabel
            anchors.horizontalCenter: dial.horizontalCenter
            anchors.top: dial.bottom
        }

        DeviceText {
            text: qsTr("MIN")
            anchors.left: dial.left
            anchors.bottom: dial.bottom
            font.pixelSize: 6
        }

        DeviceText {
            text: qsTr("MAX")
            anchors.right: dial.right
            anchors.bottom: dial.bottom
            font.pixelSize: 6
        }
    }

    DeviceDial {
        anchors.left: footPedal.left
        y: 147 - height / 2
        text: qsTr("TIME")
    }

    DeviceDial {
        anchors.right: footPedal.right
        y: 147 - height / 2
        text: qsTr("FEEDBACK")
    }

    DeviceDial {
        anchors.horizontalCenter: parent.horizontalCenter
        y: 67 - height / 2
        text: qsTr("LEVEL")
    }
}
