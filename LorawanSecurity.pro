include(../config.pri)

TEMPLATE = lib

CONFIG -= qt
CONFIG += c++11
CONFIG += staticlib

DEFINES += LORAWANSECURITY_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = lorawan-security

INCLUDEPATH += $${LIB_SDK}/libtins/include
DEPENDPATH += $${LIB_SDK}/libtins/include

SOURCES += \
    Examples/tester.cpp \
    Packet/datapacket.cpp \
    Packet/joinacceptpacket.cpp \
    Packet/joinrequestpacket.cpp \
    Packet/lorawanpacket.cpp \
    Device/lorawandevice.cpp \
    Device/lorawandevice1_0_2.cpp \
    Packet/packetcontroller.cpp \
    Packet/uplinkcontroller.cpp \
    Test/bruteforcing_mic.cpp \
    Test/testparams.cpp \
    TestPrerequisite/mitmattack.cpp \
    TestPrerequisite/testprerequisite.cpp \
    Test/lorawantest.cpp \
    Utils/common.cpp \
    Utils/jsonparser.cpp \
    lorawansecurity.cpp \
    packetstorage.cpp

HEADERS += \
    Examples/tester.h \
    Packet/packetcontroller.h \
    Packet/uplinkcontroller.h \
    Test/bruteforcing_mic.h \
    Test/testparams.h \
    Utils/common.h \
    Utils/jsonparser.h \
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
    CURRENT_BUILD = desktop
    message(Building local linux library....)

    INCLUDEPATH += /usr/local/ssl/include #openssl version 1.1.1d
    DEPENDPATH += /usr/local/ssl/include
    LIBS +=  /usr/local/ssl/lib/ -lssl -lcrypto
    INCLUDEPATH += /home/kriss/PROJECTS/SDK/rapidjson/include
    DEPENDPATH += /home/kriss/PROJECTS/SDK/rapidjson/include



}
else:contains(CONFIG,rpi) {
    CURRENT_BUILD = arm
    message(Building RPi ARM library....)

    INCLUDEPATH += $${RPI_FS}/usr/local/ssl/include #openssl version 1.1.1d
    DEPENDPATH += $${RPI_FS}/usr/local/ssl/include
    LIBS += $${RPI_FS}/usr/local/ssl/lib/ -lssl -lcrypto

    INCLUDEPATH += $${RPI_FS}/home/ttn/THESIS/rapidjson/include
    DEPENDPATH += $${RPI_FS}/home/ttn/THESIS/rapidjson/include
    QMAKE_LFLAGS += -Wl,-rpath,"$${RPI_FS}/usr/lib/arm-linux-gnueabihf/" #libpcap

}

!contains(CURRENT_BUILD, default){

    LIBTINS_DIR = $${LIB_SDK}/libtins/build-$${CURRENT_BUILD}

   message($${LIBTINS_DIR})
   LIBS += $${LIBTINS_DIR}/lib/ -ltins
   LIBS += -lpcap

   LIBTINS_OBJ_DIR = $${LIBTINS_DIR}/$${TINS_DIR}

   QMAKE_AR_CMD = ar rc $(TARGET) $(OBJECTS) $${LIBTINS_OBJ_DIR}/*.o \
                                                  $${LIBTINS_OBJ_DIR}/detail/*.o \
                                                     $${LIBTINS_OBJ_DIR}/dot11/*.o \
                                                     $${LIBTINS_OBJ_DIR}/tcp_ip/*.o \
                                                     $${LIBTINS_OBJ_DIR}/utils/*.o
   contains(CURRENT_BUILD, desktop){

        QMAKE_AR_CMD += $${CRYPTO_OBJ_DIR}/*.o $${SSL_OBJ_DIR}/*.o
   }
   else:contains(CURRENT_BUILD, arm){
        QMAKE_AR_CMD += $${RPI_FS}$${CRYPTO_OBJ_DIR}/*.o $${RPI_FS}$${SSL_OBJ_DIR}/*.o
   }
    #message($${QMAKE_AR_CMD})

}
else:message(Error chosing build)

QMAKE_CLEAN += *.a Makefile


