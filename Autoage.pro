QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    extptr.cpp \
    functions.cpp \
    main.cpp \
    autoage.cpp \
    mobs.cpp \
    mobslist.cpp \
    player.cpp \
    radar.cpp

HEADERS += \
    autoage.h \
    constants.h \
    extptr.h \
    functions.h \
    mobs.h \
    mobslist.h \
    player.h \
    radar.h

FORMS += \
    autoage.ui \
    mobslist.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TODO \
    deadmob.png \
    friendlymob.png \
    mob.png \
    offsets.ini \
    player.png \
    settings.ini \
    target.png
