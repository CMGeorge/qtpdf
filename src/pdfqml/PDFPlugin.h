#ifndef PDFPLUGIN_H
#define PDFPLUGIN_H
#include <QDebug>
#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>
class PDFPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    PDFPlugin();
    void registerTypes(const char *uri) override;
};

#endif // PDFPLUGIN_H
