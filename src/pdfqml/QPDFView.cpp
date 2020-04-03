#include "QPDFView.h"
#include <QBrush>
#include <QGuiApplication>

#include <QPainter>
#include "qpdfdocument.h"
#include "qpdfpagenavigation.h"
#include "qpdfpagerenderer.h"

//#include <QPdfDocument>
//#include <QPdfPageNavigation>
//#include <QPdfPageRenderer>
#include <QScreen>

QPDFView::QPDFView(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_document(nullptr),
      m_pageMode(SinglePage),
      m_zoomMode(CustomZoom),
      m_zoomFactor(1.0),

      m_screenResolution(
          QGuiApplication::primaryScreen()->logicalDotsPerInch() / 72.0
          ),
      m_pageCacheLimit(20),
      m_pageRenderer(nullptr),
      m_pageNavigation(new QPdfPageNavigation){
//    setSize(QSizeF(1,1));
//    setWidth(400);
    setImplicitWidth(1);
    setImplicitHeight(1);
    m_pageRenderer = new QPdfPageRenderer;
    m_pageRenderer->setRenderMode(QPdfPageRenderer::MultiThreadedRenderMode);
    //    m_pageRenderer->setRenderMode(QPdfPageRenderer::SingleThreadedRenderMode);


//    m_pageRenderer = new QPdfPageRenderer;
//    m_pageRenderer->setRenderMode(QPdfPageRenderer::MultiThreadedRenderMode);
    connect(m_pageRenderer, &QPdfPageRenderer::pageRendered, this,
            [=](int pageNumber, QSize imageSize, const QImage &image,
            QPdfDocumentRenderOptions, quint64 requestId) {
        qDebug() << "pageRendered CALLED";
        pageRendered(pageNumber, imageSize, image, requestId);
        update();
    });

    //    d->calculateViewport();
}

void QPDFView::paint(QPainter *painter) {
    qDebug() << "In Paint... 2";
//    calculateDocumentLayout();
//    QSize _explicitPageSize = m_documentLayout.pageGeometries.take(0).size();
//    setImplicitSize(_explicitPageSize.width(),_explicitPageSize.height());
//    qDebug()<<"Set size to explicit size: "<<_explicitPageSize;
//    setSize(_explicitPageSize);
    QRect _paintRect(0, 0,
                     size().width(),
                     size().height()
                     );
    painter->fillRect(_paintRect,
                      "white");
    qDebug()<<"Will Fill "<<_paintRect;
    for (auto it = m_documentLayout.pageGeometries.cbegin();
         it != m_documentLayout.pageGeometries.cend(); ++it) {
        const QRect pageGeometry = it.value();
        //        if (pageGeometry.intersects(d->m_viewport)) { // page needs to
        //        be painted
        painter->fillRect(pageGeometry, Qt::white);
        const int page = it.key();
        qDebug() << "QPDFView::paint  =  m_pageCache " << m_pageCache;

        const auto pageIt = m_pageCache.constFind(page);
        if (pageIt != m_pageCache.cend()) {
            qDebug() << "Should precess page... "<<pageIt.value();
            const QImage &img = pageIt.value();
            setImplicitSize(pageIt.value().size().width(),
                            pageIt.value().size().height());
//            setSize(pageIt.value().size());
            painter->drawImage(pageGeometry.topLeft(), img);
        } else {
            qDebug()<<"Render new page";
            QPdfDocumentRenderOptions options;
            quint64 returnArg;
            qRegisterMetaType<quint64>("quint64");
            //            QMetaObject::invokeMethod(m_pageRenderer,
            //                                      "requestPage",
            //                                      Qt::DirectConnection,
            //                                      Q_RETURN_ARG(quint64,returnArg),
            //                                      Q_ARG(int,page),
            //                                      Q_ARG(QSize,pageGeometry.size()),
            //                                      Q_ARG(QPdfDocumentRenderOptions,options)
            //                                      );
            m_pageRenderer->requestPage(page, pageGeometry.size());
        }
        //}
    }
}
bool QPDFView::isRightAligned() { return this->rightAligned; }

void QPDFView::setRightAligned(bool rightAligned) {
    this->rightAligned = rightAligned;
}

QString QPDFView::url() const { return m_url; }

QPDFView::DocumentLayout QPDFView::calculateDocumentLayout() const {
    // The DocumentLayout describes a virtual layout where all pages are
    // positioned inside
    //    - For SinglePage mode, this is just an area as large as the current
    //    page surrounded
    //      by the m_documentMargins.
    //    - For MultiPage mode, this is the area that is covered by all pages
    //    which are placed
    //      below each other, with m_pageSpacing inbetween and surrounded by
    //      m_documentMargins

    DocumentLayout documentLayout;
    //    qDebug() << "m_document " << m_document->status();
    if (!m_document || m_document->status() != QPdfDocument::Ready)
        return documentLayout;
    QHash<int, QRect> pageGeometries;

    const int pageCount = m_document->pageCount();

    int totalWidth = 0;

    const int startPage =
            (m_pageMode == QPDFView::SinglePage ? m_pageNavigation->currentPage()
                                                : 0);
    const int endPage = (m_pageMode == QPDFView::SinglePage
                         ? m_pageNavigation->currentPage() + 1
                         : pageCount);

    //    return DocumentLayout();
    // calculate page sizes
    for (int page = startPage; page < endPage; ++page) {
        QSize pageSize;

        qDebug() << pageSize;
        if (m_zoomMode == QPDFView::CustomZoom) {
            pageSize = QSizeF(m_document->pageSize(page) * m_screenResolution *
                              m_zoomFactor)
                    .toSize();
            qDebug() << pageSize;
        } else if (m_zoomMode == QPDFView::FitToWidth) {
            pageSize = QSizeF(m_document->pageSize(page) * m_screenResolution)
                    .toSize();
            const qreal factor =
                    (qreal(m_viewport.width() - m_documentMargins.left() -
                           m_documentMargins.right()) /
                     qreal(pageSize.width()));
            pageSize *= factor;
        } else if (m_zoomMode == QPDFView::FitInView) {
            const QSize viewportSize(
                        m_viewport.size() +
                        QSize(-m_documentMargins.left() - m_documentMargins.right(),
                              -m_pageSpacing));

            pageSize = QSizeF(m_document->pageSize(page) * m_screenResolution)
                    .toSize();
            pageSize = pageSize.scaled(viewportSize, Qt::KeepAspectRatio);
        }
        pageSize = QSize(pageSize.width() * 2, pageSize.height() * 2);
        totalWidth = qMax(totalWidth, pageSize.width());

        pageGeometries[page] = QRect(QPoint(0, 0), pageSize);
    }

    totalWidth += m_documentMargins.left() + m_documentMargins.right();

    int pageY = m_documentMargins.top();

    // calculate page positions
    for (int page = startPage; page < endPage; ++page) {
        const QSize pageSize = pageGeometries[page].size();

        // center horizontal inside the viewport
        const int pageX =
                (qMax(totalWidth, m_viewport.width()) - pageSize.width()) / 2;

        pageGeometries[page].moveTopLeft(QPoint(pageX, pageY));

        pageY += pageSize.height() + m_pageSpacing;
    }

    pageY += m_documentMargins.bottom();

    documentLayout.pageGeometries = pageGeometries;

    // calculate overall document size
    documentLayout.documentSize = QSize(totalWidth, pageY);

    return documentLayout;
}

void QPDFView::pageRendered(int pageNumber, QSize imageSize,
                            const QImage &image, quint64 requestId) {
    Q_UNUSED(imageSize)
    Q_UNUSED(requestId)
    qDebug() << "hould render page " << pageNumber;

    if (!m_cachedPagesLRU.contains(pageNumber)) {
        if (m_cachedPagesLRU.length() > m_pageCacheLimit)
            m_pageCache.remove(m_cachedPagesLRU.takeFirst());

        m_cachedPagesLRU.append(pageNumber);
    }

    m_pageCache.insert(pageNumber, image);
    qDebug() << "Cash size is: " << m_pageCache.count();
    //    q->viewport()->update();
}

void QPDFView::setUrl(const QString &url) {

    if (m_url == url)
        return;
    qDebug() << "Shoudl 2 open:: " << url;

    //        m_pageRenderer = new QPdfPageRenderer(q);
    //        m_pageRenderer->setRenderMode(QPdfPageRenderer::MultiThreadedRenderMode);
//    if (m_pageRenderer) {
//        m_pageRenderer->deleteLater();
//        m_document->deleteLater();
//    }
    if (m_document) m_document->deleteLater();
    m_document = new  QPdfDocument(this);
    connect(m_document, &QPdfDocument::statusChanged, this,
            [=]() {
        qDebug()<<"QPdfDocument::statusChanged ";
        documentStatusChanged();
    });
    connect(m_pageNavigation, &QPdfPageNavigation::currentPageChanged, this,
            [=](int page) {
        qDebug()<<"QPdfDocument::currentPageChanged "<<page;
        currentPageChanged(page);
    });
    m_pageRenderer->setDocument(nullptr);
    m_pageRenderer->setDocument(m_document);
    if (url.startsWith("file:/")){
        QString _url(url);
#ifdef Q_OS_WIN
        _url.replace("file:///","");
        m_document->load(_url);
        m_url = _url;
#else
        _url.replace("file://","");
        m_document->load(_url);
        m_url = _url;
#endif

    }else{
        m_document->load(url);
        m_url = url;

    }
    qDebug()<<"Set LOad UEL :"<<m_url;
    emit urlChanged(m_url);
}

void QPDFView::documentStatusChanged() {
    qDebug() << "Document status changed "<<m_document;
    m_pageNavigation->setDocument(m_document);
    m_pageRenderer->setDocument(m_document);
    m_documentLayout = calculateDocumentLayout();

    //        QQuickPaintedItem::updatePolish(); // QRect(0, 0, 1000, 1000));

    //updatePaintNode();
            invalidatePageCache();
}

void QPDFView::currentPageChanged(int currentPage) {
    qDebug() << "Page changd to " << currentPage;
    if (m_pageMode == SinglePage)
        invalidateDocumentLayout();

    qDebug() << "m_documentLayout = " << m_documentLayout.pageGeometries;


    //    calculateDocumentLayout();
    //    update();
    //    if (m_blockPageScrolling)
    //        return;

    //    if (m_pageMode == SinglePage)
    //        invalidateDocumentLayout();
}
void QPDFView::invalidateDocumentLayout() {
    updateDocumentLayout();
    invalidatePageCache();
}

void QPDFView::updateDocumentLayout() {
    m_documentLayout = calculateDocumentLayout();
    qCritical()<<"m_documentLayout ??  "<<m_documentLayout.pageGeometries;
}

void QPDFView::invalidatePageCache() {
    m_pageCache.clear();
    update();
}
