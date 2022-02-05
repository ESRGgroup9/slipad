QT       += core gui
QT       += widgets webview
#positioning
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets positioning

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addLamp.cpp \
    comms.cpp \
    consult.cpp \
    main.cpp \
    menu.cpp \
    repairLamp.cpp \
    slipad.cpp \
    tcp/CCommunication.cpp \
    tcp/CTCPclient.cpp \
    tcp/CTCPcomm.cpp \
    tcp/parser.cpp

HEADERS += \
    addLamp.h \
    comms.h \
    consult.h \
    lamppost.h \
    menu.h \
    repairLamp.h \
    slipad.h \
    tcp/CCommunication.h \
    tcp/CTCPclient.h \
    tcp/CTCPcomm.h \
    tcp/debug.h \
    tcp/defs.h \
    tcp/parser.h \
    tcp/utils.h

FORMS += \
    addLamp.ui \
    consult.ui \
    menu.ui \
    repairLamp.ui \
    slipad.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources.qrc
