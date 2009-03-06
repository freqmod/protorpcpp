# -------------------------------------------------
# Project created by QtCreator 2009-01-15T12:27:13
# -------------------------------------------------
QT += network
QT -= gui
TARGET = protorpcpp
TEMPLATE = lib
DEFINES += PROTORPCPP_LIBRARY
SOURCES += twowaystream.cpp \
    callentry.cpp \
    streamcallbackinfo.cpp \
    simplerpccontroller.cpp \
    twowayrpccontroller.cpp \
    prototools.cpp \
    testserversideservice.cpp \
    socketserver.cpp \
    responsewaiterprivate.cpp \
    responsewaiter.cpp
HEADERS += protorpcpp_global.h \
    twowaystream.h \
    callentry.h \
    streamcallbackinfo.h \
    simplerpccontroller.h \
    twowayrpccontroller.h \
    prototools.h \
    testserversideservice.h \
    socketserver.h \
    responsewaiterprivate.h \
    responsewaiter.h
