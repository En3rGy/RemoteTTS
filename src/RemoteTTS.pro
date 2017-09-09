#-------------------------------------------------
#
# Project created by QtCreator 2017-05-08T06:15:59
#
#-------------------------------------------------

QT       += core gui texttospeech multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(debug, debug|release) {
  CONFIG  -= debug release
  CONFIG  += debug
  DEFINES += _DEBUG

  TARGET  = RemoteTTSD
}

CONFIG(release) {
  CONFIG -= debug release
  CONFIG += release

  TARGET  = RemoteTTS
}

win32:DEFINES += win32

DESTDIR  = ../bin

TARGET = RemoteTTS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = myappico.ico

SOURCES += main.cpp\
        remotetts.cpp \
    udpmanager.cpp \
    systemstatus.cpp \
    configform.cpp \
    tcpmanager.cpp

HEADERS  += remotetts.h \
    udpmanager.h \
    systemstatus.h \
    configform.h \
    tcpmanager.h

FORMS    += remotetts.ui \
    configform.ui
