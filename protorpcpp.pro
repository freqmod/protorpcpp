# -------------------------------------------------
# Project created by QtCreator 2009-01-15T12:27:13
# -------------------------------------------------
QT += network
QT -= gui
TARGET = protorpcpp
TEMPLATE = lib
DEFINES += PROTORPCPP_LIBRARY
SOURCES += protorpcpp.cpp \
    twowaystream.cpp \
    threadtool.cpp \
    callentry.cpp \
    streamcallbackinfo.cpp \
    simplerpccontroller.cpp \
    twowayrpccontroller.cpp \
    prototools.cpp
HEADERS += protorpcpp.h \
    protorpcpp_global.h \
    twowaystream.h \
    threadtool.h \
    callentry.h \
    streamcallbackinfo.h \
    simplerpccontroller.h \
    twowayrpccontroller.h \
    prototools.h
