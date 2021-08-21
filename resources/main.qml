import QtQuick 2.9
import QtCharts 2.0
import QtQuick.Dialogs 1.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: mainWindowID
    objectName: "MainWindow"

    visible: true
    minimumWidth: 640
    minimumHeight: 480
    title: qsTr("Words Counter")

    Connections {
        function toggleIndicatorVisibility() {
            busyIndicatorID.running = !busyIndicatorID.running
        }

        target: wordsModelController
        onFileBeingProcessed: toggleIndicatorVisibility()
        onFileProcessed: {
            barSeriesID.reFill()
            toggleIndicatorVisibility()
        }
    }

    BusyIndicator {
        id: busyIndicatorID
        objectName: "BusyIndicator"

        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        z: 1
        width: 100
        height: 100

        visible: running

        running: false
    }

    FileDialog {
        id: fileDialogID

        title: qsTr("Please choose a file")
        //folder: shortcuts.home
        nameFilters: [ "Text files (*.txt)" ]
        onAccepted: wordsModelController.ProcessFile(fileDialogID.fileUrl)
    }

    ColumnLayout {
        id: columnLayoutID

        anchors.fill: parent

        Text {
            id: mainTextID
            objectName: "MainText"

            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            text: qsTr("Find 15 most frequent words in a file")
        }

        Button {
            id: addFileButtonID
            objectName: "AddFileButton"

            text: qsTr("Open file")
            onClicked: fileDialogID.open()
        }

        ChartView {
            id: chartViewID
            objectName: "ChartView"

            title: qsTr("Words frequency")
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter

            legend.visible: false
            antialiasing: true

            BarSeries {
                id: barSeriesID
                objectName: "BarSeries"

                property alias categoryAxisX: categoryXAxisID
                property alias categoryAxisY: valueAxisYID

                function reFill() {
                    categoryAxisX.categories = wordsModelController.GetWords()
                    valueAxisYID.max = wordsModelController.GetYAxisMaxValue()
                    clear()
                    append("", wordsModelController.GetValues())
                }

                axisX: BarCategoryAxis { id: categoryXAxisID }
                axisY: ValueAxis { id: valueAxisYID }
            }
        }
    }
}
