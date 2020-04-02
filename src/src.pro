TEMPLATE = subdirs


SUBDIRS = lib \
#            src_pdf \
            pdfqml

#src_pdf.subdir = pdf
#src_pdf.depends = lib
pdfqml.depends = lib #src_pdf

# \
#    pdfqml

#qtHaveModule(widgets) {
#    src_pdfwidgets.subdir = pdfwidgets
#    src_pdfwidgets.depends = src_pdf

#    SUBDIRS += src_pdfwidgets
#}

#qtHaveModule(qml) {
#    src_pdfqml.subdir = pdfqml
#    src_pdfqml.depends = src_pdf

#    SUBDIRS += src_pdfqml
#}
