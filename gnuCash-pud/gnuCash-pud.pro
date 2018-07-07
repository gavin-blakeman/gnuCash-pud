##**********************************************************************************************************************************
#
## PROJECT:							gnuCash-pud (gnuCash-Price Upload Daemon)
## FILE:								main application
## SUBSYSTEM:						main(...)
## LANGUAGE:						C++
## TARGET OS:						UNIX/LINUX/WINDOWS/MAC
## LIBRARY DEPENDANCE:	Qt
## NAMESPACE:						WS
## AUTHOR:							Gavin Blakeman (GGB)
## LICENSE:             GPLv2
##
##                      Copyright 2018 Gavin Blakeman.
##                      This file is part of the gnuCash-pud (gnuCash-Price Upload Daemon)
##
##                      gnuCash-pud is free software: you can redistribute it and/or modify it under the terms of the GNU General
##                      Public License as published by the Free Software Foundation, either version 2 of the License, or (at your
##                      option) any later version.
##
##                      gnuCash-pud is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
##                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
##                      for more details.
##
##                      You should have received a copy of the GNU General Public License along with gnuCash-pud.  If not,
##                      see <http://www.gnu.org/licenses/>.
##
## OVERVIEW:            Project File.
##
## HISTORY:             2018-06-28/GGB - Development of gnuCash-pud
##
##**********************************************************************************************************************************



TEMPLATE = app
TARGET = gnuCash-pud

CONFIG   += console qt thread static debug

QT       += core network sql
QT       -= gui

QMAKE_CXXFLAGS += -std=c++17 -static -static-libgcc #-save-temps=obj

#DESTDIR = ""
OBJECTS_DIR = "objects"
UI_DIR = "objects/ui"
MOC_DIR = "objects/moc"
RCC_DIR = "objects/rcc"

DEFINES += BOOST_THREAD_USE_LIB
DEFINES += QT_CORE_LIB
DEFINES += QXT_STATIC
DEFINES += BOOST_CHRONO_DONT_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0

INCLUDEPATH +=  \
  "../boost 1.62" \
  "../gnuCash-lib" \
  "../GCL" \
  "../QCL" \
  "../qt-solutions"

SOURCES += \
    source/service.cpp \
    source/statemachine.cpp \
    ../../Library/QtSolutions/qtservice/src/qtunixsocket.cpp \
    ../../Library/QtSolutions/qtservice/src/qtunixserversocket.cpp \
    ../../Library/QtSolutions/qtservice/src/qtservice_unix.cpp \
    ../../Library/QtSolutions/qtservice/src/qtservice.cpp \
    source/tcp.cpp \
    ../../Library/WeatherLink/Source/GeneralFunctions.cpp \
    ../../Library/WeatherLink/Source/database.cpp \
    ../../Library/WeatherLink/Source/error.cpp \
    ../../Library/WeatherLink/Source/settings.cpp \
    source/gnuCash-pud.cpp

HEADERS += \
    include/service.h \
    include/statemachine.h \
    include/tcp.h \
    ../../Library/QtSolutions/qtservice/src/qtunixsocket.h \
    ../../Library/QtSolutions/qtservice/src/qtunixserversocket.h \
    ../../Library/QtSolutions/qtservice/src/qtservice_p.h \
    ../../Library/QtSolutions/qtservice/src/qtservice.h \
    ../../Library/WeatherLink/Include/WeatherLinkIP.h \
    ../../Library/WeatherLink/Include/GeneralFunctions.h \
    ../../Library/WeatherLink/Include/database.h \
    ../../Library/WeatherLink/Include/error.h \
    ../../Library/WeatherLink/Include/settings.h \
    include/settings.h

win32:CONFIG(release, debug|release) {
  LIBS += -L../../Library/Library/win32/debug -lGCL
  LIBS += -L../../Library/Library/win32/debug -lMCL
  LIBS += -L../../Library/Library/win32/debug -lSCL
  LIBS += -L../../Library/Library/win32/debug -lQCL
  LIBS += -L../../Library/Library/win32/debug -lboost_filesystem
  LIBS += -L../../Library/Library/win32/debug -lboost_system
  LIBS += -L../../Library/Library/win32/debug -lboost_thread
  LIBS += -L../../Library/Library/win32/debug -lboost_chrono
  LIBS += -L../../Library/Library/unix/debug -lboost_program_options
}
else:win32:CONFIG(debug, debug|release) {
  LIBS += -L../../Library/Library/win32/debug -lGCL
  LIBS += -L../../Library/Library/win32/debug -lMCL
  LIBS += -L../../Library/Library/win32/debug -lSCL
  LIBS += -L../../Library/Library/win32/debug -lQCL
  LIBS += -L../../Library/Library/win32/debug -lboost_filesystem
  LIBS += -L../../Library/Library/win32/debug -lboost_system
  LIBS += -L../../Library/Library/win32/debug -lboost_thread
  LIBS += -L../../Library/Library/win32/debug -lboost_chrono
  LIBS += -L../../Library/Library/unix/debug -lboost_program_options
}
else:unix:CONFIG(debug, debug|release) {
  LIBS += -L../GCL -lGCL
  LIBS += -L../MCL -lMCL
  LIBS += -L../QCL -lQCL
  LIBS += -L../SCL -lSCL
  LIBS += -L../../../Library/Library -lboost_filesystem
  LIBS += -L../../../Library/Library/unix/debug -lboost_system
  LIBS += -L../../Library/Library/unix/debug -lboost_thread
  LIBS += -L../../Library/Library/unix/debug -lboost_chrono
  LIBS += -L../../Library/Library/unix/debug -lboost_program_options
}
else:unix:CONFIG(release, debug|release) {
  LIBS += -L../GCL -lGCL
  LIBS += -L../MCL -lMCL
  LIBS += -L../QCL -lQCL
  LIBS += -L../SCL -lSCL
  LIBS += -L../../../Library/Library -lboost_filesystem
  LIBS += -L../../../Library/Library/unix/debug -lboost_system
  LIBS += -L../../Library/Library/unix/debug -lboost_thread
  LIBS += -L../../Library/Library/unix/debug -lboost_chrono
  LIBS += -L../../Library/Library/unix/debug -lboost_program_options
}



DISTFILES += \
    ../README

