# -------------------------------------------------
# Project created by QtCreator 2009-01-16T18:49:17
# -------------------------------------------------
QT += network
QT -= gui
TARGET = Protorpcpp
TEMPLATE = lib
DEFINES += PROTORPCPP_LIBRARY
SOURCES += twowaystream.cpp \
    threadtool.cpp \
    simplerpccontroller.cpp \
    prototools.cpp \
    protorpcpp.cpp \
    Message.pb.cc \
    callentry.cpp \
    streamcallbackinfo.cpp \
    twowayrpccontroller.cpp \
    socketserver.cpp \
    testserversideservice.cpp
HEADERS += Protorpcpp_global.h \
    twowaystream.h \
    threadtool.h \
    simplerpccontroller.h \
    prototools.h \
    protorpcpp_global.h \
    protorpcpp.h \
    Message.pb.h \
    callentry.h \
    streamcallbackinfo.h \
    twowayrpccontroller.h \
    socketserver.h \
    testserversideservice.h
