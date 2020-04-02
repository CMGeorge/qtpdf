TARGET = QtPdf
QT += gui core
QT_PRIVATE += network
TEMPLATE = lib

#CONFIG -= precompile_header # Not supported by upstream header files
#win32: DEFINES += NOMINMAX
#INCLUDEPATH += $$QTPDF_ROOT/src/3rdparty/pdfium/fpdfsdk/include
#INCLUDEPATH += $$QTPDF_ROOT/src/3rdparty/pdfium
#INCLUDEPATH += $$QTPDF_ROOT/src/3rdparty/pdfium/third_party/freetype/include
load(qt_module)

QMAKE_DOCS = $$PWD/doc/qtpdf.qdocconf

include(pdf.pri)
#gcc {
#    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#}

#msvc {
#    QMAKE_CXXFLAGS_WARN_ON += -wd"4100"
#}

#SOURCES += \
#    jsbridge.cpp \
#    qpdfbookmarkmodel.cpp \
#    qpdfdocument.cpp \
#    qpdfpagenavigation.cpp \
#    qpdfpagerenderer.cpp

#HEADERS += \
#    qpdfbookmarkmodel.h \
#    qpdfdocument.h \
#    qpdfdocument_p.h \
#    qpdfdocumentrenderoptions.h \
#    qpdfnamespace.h \
#    qpdfpagenavigation.h \
#    qpdfpagerenderer.h \
#    qtpdfglobal.h
