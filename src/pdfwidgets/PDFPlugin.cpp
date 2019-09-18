#include "PDFPlugin.h"
#include "qpdfview.h"
#include <QPdfPageNavigation>
PDFPlugin::PDFPlugin() {}
void PDFPlugin::registerTypes(const char *uri) {
    qDebug() << "URI IS: " << uri;
    Q_ASSERT(uri == QLatin1String("ro.wesell.qmlpdf"));
    qmlRegisterType<QPdfView>(uri, 1, 0, "QPDFView");
    qRegisterMetaType<QPdfPageNavigation *>("QPdfPageNavigation*");
}
