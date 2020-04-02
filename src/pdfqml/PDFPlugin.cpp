#include "PDFPlugin.h"
#include "QPDFView.h"
#include "qpdfpagenavigation.h"
PDFPlugin::PDFPlugin() {}
void PDFPlugin::registerTypes(const char *uri) {
    Q_ASSERT(uri == QLatin1String("ro.wesell.qmlpdf"));
    qmlRegisterType<QPDFView>(uri, 1, 0, "QPDFView");
    qRegisterMetaType<QPdfPageNavigation *>("QPdfPageNavigation*");
}
