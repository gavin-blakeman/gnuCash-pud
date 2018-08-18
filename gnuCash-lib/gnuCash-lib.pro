#-------------------------------------------------
#
# Project file for Qt Class Library (QCL)
#
#-------------------------------------------------

TARGET = gnuCash-lib
TEMPLATE = lib
CONFIG += staticlib

#QT -= core gui
QT += sql

QMAKE_CXXFLAGS += -std=c++17 -static -static-libgcc #-save-temps=obj
QMAKE_LFLAGS += -fopenmp

DEFINES += BOOST_CHRONO_DONT_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0

OBJECTS_DIR = "objects"
UI_DIR = "objects/ui"
MOC_DIR = "objects/moc"
RCC_DIR = "objects/rcc"

win32:CONFIG(release, debug|release) {
  DESTDIR = "../Library/win32/release"
  OBJECTS_DIR = "../Library/win32/release/object/SCL"
}
else:win32:CONFIG(debug, debug|release) {
  DESTDIR = "../Library/win32/debug"
  OBJECTS_DIR = "../Library/win32/debug/object/SCL"
}
else:unix:CONFIG(release, debug|release) {
  DESTDIR = "../Library/unix/release"
  OBJECTS_DIR = "../Library/unix/release/object/SCL"
}
else:unix:CONFIG(debug, debug|release) {
  DESTDIR = ""
  OBJECTS_DIR = "objects"
}

INCLUDEPATH += \
  "../boost 1.62" \
  "../GCL" \
  "../QCL" \
  "../SCL"

SOURCES += \
    source/database.cpp \
    source/processCSV.cpp

HEADERS += \
    include/database.h \
    include/qt.h \
    include/processCSV.h

