CONFIG += qml_build
TARGET = QtPdfWidgets

QT = core gui widgets widgets-private pdf
SOURCES += \
    qpdfview.cpp

HEADERS += \
    qpdfview.h \
    qpdfview_p.h \
    qtpdfwidgetsglobal.h
qml_build{
DEFINES += QML_BUILD
QT =  qml quick quick-private pdf qml-private
TARGET  = qmlpdf
SOURCES += \
    PDFPlugin.cpp
HEADERS += \
    PDFPlugin.h
target.path += $$[QT_INSTALL_QML]/qml/$${TARGET}
TARGETPATH = ro/wesell/$${TARGET}
load(qml_plugin)

DISTFILES += \
    qmldir
}else{
load(qt_module)
}


