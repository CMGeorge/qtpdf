QT =  qml quick
TARGET  = qmlpdf
TEMPLATE = lib
CONFIG += plugin
#CONFIG += staticlib

#LIBS += -L$$OUT_PWD/../../lib/ -lqt5pdf
#INCLUDEPATH += $$OUT_PWD/../../include/QtPdf
#warning("Set Include Path" $$OUT_PWD/../../include);
CONFIG += c++11
DEFINES += Q_PDF_EXPORT
MODULE_BASE_OUTDIR = $$OUT_PWD/../../
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../pdf/
include($$PWD/../pdf/pdf.pri)
SOURCES += \
    PDFPlugin.cpp \
    QPDFView.cpp

HEADERS += \
    PDFPlugin.h \
    QPDFView.h

# Default rules for deployment.
#unix {
#    target.path = $$[QT_INSTALL_PLUGINS]/generic
#}
#!isEmpty(target.path): INSTALLS += target
#qml.files = plugins.qml
#qml.path += $$[QT_INSTALL_EXAMPLES]/qml/qmlextensionplugins
#target.path += $$[QT_INSTALL_QML]/qml/$${TARGET}
#pluginfiles.path += $$[QT_INSTALL_EXAMPLES]/qml/qmlextensionplugins/imports/TimeExample
TARGETPATH = ro/wesell/$${TARGET}
#INSTALLS += target #qml pluginfiles

CONFIG += install_ok  # Do not cargo-cult this!
load(qml_plugin)

DISTFILES += \
    qmldir
