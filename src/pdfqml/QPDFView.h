#ifndef QPDFVIEW_H
#define QPDFVIEW_H

#include <QImage>
#include <QQuickItem>
#include <QQuickPaintedItem>
class QPdfDocument;
class QPdfPageNavigation;
class QPdfPageRenderer;
class QPDFView : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(bool rightAligned READ isRightAligned WRITE setRightAligned
               NOTIFY rightAlignedChanged)
    Q_PROPERTY(QPdfPageNavigation *pageNavigation READ pageNavigation WRITE
               setPageNavigation NOTIFY pageNavigationChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

    struct DocumentLayout {
        QSize documentSize;
        QHash<int, QRect> pageGeometries;
    };

public:
    enum PageMode { SinglePage, MultiPage };
    Q_ENUM(PageMode)

    enum ZoomMode { CustomZoom, FitToWidth, FitInView };
    Q_ENUM(ZoomMode)

    QPDFView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);

    bool isRightAligned();
    void setRightAligned(bool rightAligned);

    QString url() const;

    QPdfPageNavigation *pageNavigation() const { return m_pageNavigation; }

private:
    DocumentLayout calculateDocumentLayout() const;
    void pageRendered(int pageNumber, QSize imageSize, const QImage &image,
                      quint64 requestId);
    void invalidateDocumentLayout();
    void updateDocumentLayout();
    void invalidatePageCache();
    bool rightAligned;
    QPdfDocument *m_document;
    // private
    DocumentLayout m_documentLayout;
    //    QPdfPageNavigation *m_pageNavigation;
    PageMode m_pageMode;
    ZoomMode m_zoomMode;
    qreal m_zoomFactor;
    qreal m_screenResolution; // pixels per point
    QRect m_viewport;
    QHash<int, QImage> m_pageCache;
    QVector<int> m_cachedPagesLRU;
    int m_pageCacheLimit;

    int m_pageSpacing;
    QMargins m_documentMargins;
    QPdfPageRenderer *m_pageRenderer;
    // End private
    QString m_url;

    QPdfPageNavigation *m_pageNavigation;

signals:
    void rightAlignedChanged();
    void urlChanged(QString url);

    void pageNavigationChanged(QPdfPageNavigation *pageNavigation);

public slots:
    void setUrl(const QString &url);

    void setPageNavigation(QPdfPageNavigation *pageNavigation) {
        if (m_pageNavigation == pageNavigation)
            return;

        m_pageNavigation = pageNavigation;
        emit pageNavigationChanged(m_pageNavigation);
    }

private slots:
    void documentStatusChanged();
    void currentPageChanged(int currentPage);
};

#endif // QPDFVIEW_H
