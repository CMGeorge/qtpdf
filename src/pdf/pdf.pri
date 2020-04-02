QT += core-private

CONFIG += c++11
CONFIG -= precompile_header # Not supported by upstream header files
win32: DEFINES += NOMINMAX
INCLUDEPATH += $$QTPDF_ROOT/src/3rdparty/pdfium/fpdfsdk/include
INCLUDEPATH += $$QTPDF_ROOT/src/3rdparty/pdfium
INCLUDEPATH += $$QTPDF_ROOT/src/3rdparty/pdfium/third_party/freetype/include

LIBS_PRIVATE += -L$$MODULE_BASE_OUTDIR/lib -lqtpdfium$$qtPlatformTargetSuffix()

#load(qt_module)

#QMAKE_DOCS = $$PWD/doc/qtpdf.qdocconf

gcc {
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
}

msvc {
    QMAKE_CXXFLAGS_WARN_ON += -wd"4100"
}

SOURCES += \
    $$PWD/jsbridge.cpp \
    $$PWD/qpdfbookmarkmodel.cpp \
    $$PWD/qpdfdocument.cpp \
    $$PWD/qpdfpagenavigation.cpp \
    $$PWD/qpdfpagerenderer.cpp

HEADERS += \
    $$PWD/qpdfbookmarkmodel.h \
    $$PWD/qpdfdocument.h \
    $$PWD/qpdfdocument_p.h \
    $$PWD/qpdfdocumentrenderoptions.h \
    $$PWD/qpdfnamespace.h \
    $$PWD/qpdfpagenavigation.h \
    $$PWD/qpdfpagerenderer.h \
    $$PWD/qtpdfglobal.h
