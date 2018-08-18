TEMPLATE = subdirs

#CONFIG += ordered
SUBDIRS += "GCL"
SUBDIRS += "MCL"
SUBDIRS += "SCL"
SUBDIRS += "QCL"
SUBDIRS += "QtSolutions/qtservice"
SUBDIRS += "gnuCash-lib"
SUBDIRS += "gnuCash-pud"

DEFINES += QWT_STATIC
