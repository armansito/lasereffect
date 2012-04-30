QT += core gui opengl

TARGET = lasertest
TEMPLATE = app

CONFIG += debug

QMAKE_CFLAGS += -O3
QMAKE_CFLAGS -= -O2
QMAKE_CXXFLAGS += -O3
QMAKE_CXXFLAGS -= -O2

SOURCES += main.cpp \
     mainwindow.cpp \
       glwidget.cpp \
         camera.cpp \
    lasereffect.cpp

HEADERS += mainwindow.h \ 
             glwidget.h \
               camera.h \
               vector.h \
          lasereffect.h
