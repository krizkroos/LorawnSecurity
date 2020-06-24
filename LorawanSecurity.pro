include(../config.pri)

TEMPLATE = lib

CONFIG -= qt
CONFIG += c++11
CONFIG += staticlib

DEFINES += LORAWANSECURITY_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = lorawan-security

INCLUDEPATH += /home/kriss/PROJECTS/SDK/libtins/include
DEPENDPATH += /home/kriss/PROJECTS/SDK/libtins/include

SOURCES += \
    datapacket.cpp \
    joinacceptpacket.cpp \
    joinrequestpacket.cpp \
    lorawandevice.cpp \
    lorawandevice1_0_2.cpp \
    lorawanpacket.cpp \
    lorawansecurity.cpp \
    lorawantest.cpp \
    mitmattack.cpp \
    packetstorage.cpp \
    testprerequisite.cpp

HEADERS += \
    LorawanSecurity_global.h \
    datapacket.h \
    defineLorawan.h \
    joinacceptpacket.h \
    joinrequestpacket.h \
    lorawandevice.h \
    lorawandevice1_0_2.h \
    lorawanpacket.h \
    lorawansecurity.h \
    lorawantest.h \
    mitmattack.h \
    packetstorage.h \
    testprerequisite.h

CURRENT_BUILD = build-desktop
contains(CONFIG,desktop) {
    CURRENT_BUILD = build-desktop
}
else:contains(CONFIG,rpi) {
    CURRENT_BUILD = build-arm
}

LIBS += $${LIB_SDK}/libtins/$${CURRENT_BUILD}/lib/ -ltins
PRE_TARGETDEPS += $${LIB_SDK}/libtins/$${CURRENT_BUILD}/lib/libtins.a
