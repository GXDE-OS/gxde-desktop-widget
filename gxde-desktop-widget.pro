QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutprogram.cpp \
    infoutils.cpp \
    main.cpp \
    mainwindow.cpp \
    settingwindow.cpp \
    weatherinformation.cpp

HEADERS += \
    aboutprogram.h \
    infoutils.h \
    mainwindow.h \
    settingwindow.h \
    weatherinformation.h

FORMS += \
    aboutprogram.ui \
    mainwindow.ui \
    settingwindow.ui

TRANSLATIONS += \
    gxde-desktop-widget_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
