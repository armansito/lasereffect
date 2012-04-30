QT += core gui opengl

TARGET = lasereffect 
TEMPLATE = app

CONFIG += debug

INCLUDEPATH += src
DEPENDPATH += src

QMAKE_CFLAGS += -O3
QMAKE_CFLAGS -= -O2
QMAKE_CXXFLAGS += -O3
QMAKE_CXXFLAGS -= -O2

SOURCES += main.cpp \
     mainwindow.cpp \
       glwidget.cpp \
         camera.cpp \
src/lasereffect.cpp

HEADERS += mainwindow.h \ 
             glwidget.h \
               camera.h \
           src/vector.h \
      src/lasereffect.h
