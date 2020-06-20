include(../config.pri)

TEMPLATE = lib

CONFIG -= qt
CONFIG += c++11
CONFIG += staticlib

DEFINES += LORAWANSECURITY_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = lorawan-security


SOURCES += \
    lorawansecurity.cpp

HEADERS += \
    LorawanSecurity_global.h \
    lorawansecurity.h

contains(CONFIG,desktop) {

}
else:contains(CONFIG,rpi) {

}
