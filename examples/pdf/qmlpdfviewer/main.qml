import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ro.wesell.qmlpdf 1.0
ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World 4")
    ColumnLayout{
        anchors.fill: parent
        QPDFView{
            id: pdfViewer
            Layout.fillWidth: true
            Layout.fillHeight: true
            url: "file:///Users/cmgeorge/Downloads/printpreview.pdf"
            Component.onCompleted: {
                update();
            }
        }
        RowLayout{
            Button{
                text: "Back"
                enabled: pdfViewer.pageNavigation.currentPage!==0
                onClicked: pdfViewer.pageNavigation.goToPreviousPage()
            }

            Label{
                text: "Page: "
            }
            Label{
                text: pdfViewer.pageNavigation.currentPage+1
            }
            Label {
                text: " from "
            }
            Label{
                text: pdfViewer.pageNavigation.pageCount;
            }
            Button{
                text: "Forward"
                enabled: pdfViewer.pageNavigation.currentPage<pdfViewer.pageNavigation.pageCount
                onClicked: pdfViewer.pageNavigation.goToNextPage()
            }
        }
    }

}
