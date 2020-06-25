include(../config.pri)

TEMPLATE = lib

CONFIG -= qt
CONFIG += c++11
CONFIG += staticlib

DEFINES += LORAWANSECURITY_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = lorawan-security


INCLUDEPATH += ./Packet \
               ./Device \
               ./Test \
               ./TestPrerequisite
DEPENDPATH += ./Packet \
               ./Device \
               ./Test \
               ./TestPrerequisite

INCLUDEPATH += /home/kriss/PROJECTS/SDK/libtins/include
DEPENDPATH += /home/kriss/PROJECTS/SDK/libtins/include

SOURCES += \
    Packet/datapacket.cpp \
    Packet/joinacceptpacket.cpp \
    Packet/joinrequestpacket.cpp \
    Packet/lorawanpacket.cpp \
    Device/lorawandevice.cpp \
    Device/lorawandevice1_0_2.cpp \
    Test/bruteforcing_mic.cpp \
    TestPrerequisite/mitmattack.cpp \
    TestPrerequisite/testprerequisite.cpp \
    Test/lorawantest.cpp \
    lorawansecurity.cpp \
    packetstorage.cpp

HEADERS += \
    Test/bruteforcing_mic.h \
    defineLorawan.h \
    LorawanSecurity_global.h \
    Packet/datapacket.h \
    Packet/lorawanpacket.h \
    Packet/joinacceptpacket.h \
    Packet/joinrequestpacket.h \
    Device/lorawandevice.h \
    Device/lorawandevice1_0_2.h \
    TestPrerequisite/testprerequisite.h \
    TestPrerequisite/mitmattack.h \
    Test/lorawantest.h \
    lorawansecurity.h \

    packetstorage.h


CURRENT_BUILD = default #default
contains(CONFIG,desktop) {
    CURRENT_BUILD = build-desktop
}
else:contains(CONFIG,rpi) {
    CURRENT_BUILD = build-arm
}

!contains(CURRENT_BUILD, default){

LIBS += $${LIB_SDK}/libtins/$${CURRENT_BUILD}/lib/ -ltins
PRE_TARGETDEPS += $${LIB_SDK}/libtins/$${CURRENT_BUILD}/lib/libtins.a

}


