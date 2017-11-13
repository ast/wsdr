#-------------------------------------------------
#
# Project created by QtCreator 2017-10-21T14:46:34
#
#-------------------------------------------------

QT       += core gui network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gnuradiotest
TEMPLATE = app

QT += remoteobjects
REPC_REPLICA += ../sdrserver/radio.rep

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
        mainwindow.cpp\
        panel.cpp \
        wopenglshaderprogram.cpp \
    wdgramsocket.cpp \
    glwaterfall.cpp \
    glspectrum.cpp

HEADERS  += mainwindow.h\
    panel.h \
    wopenglshaderprogram.h \
    wdgramsocket.h \
    glwaterfall.h \
    glspectrum.h

FORMS    += mainwindow.ui \
    panel.ui

DISTFILES += \
    shaders/fft_line.frag \
    shaders/fft_line.vert \
    shaders/h_grid.frag \
    shaders/h_grid.vert \
    shaders/textured_quad.frag \
    shaders/textured_quad.vert \
    shaders/colored_line.frag \
    shaders/colored_line.vert

