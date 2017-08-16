#-------------------------------------------------
#
# Project created by QtCreator 2017-06-17T11:50:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MonitorTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    cvt_burndata.cpp \
    cvt_burnsetting.cpp \
    cvt_dataTransfer.cpp \
    cvt_isp_iic.cpp \
    cvt_mainwindow.cpp \
    cvt_PraseEdid.cpp \
    cvt_PraseHdcp.cpp \
    cvt_protocol.cpp \
    cvt_debugtab.cpp \
    cvt_burntab.cpp \
    mainwindow.cpp \
    i2c_settingwindow.cpp \
    communicationsetting.cpp \
    helpwindow.cpp \
    instructionwindow.cpp

HEADERS  += \
    cvt_burndata.h \
    cvt_burnsetting.h \
    cvt_dataTransfer.h \
    cvt_global.h \
    cvt_isp_iic.h \
    cvt_mainwindow.h \
    cvt_PraseEdid.h \
    cvt_PraseHdcp.h \
    cvt_protocol.h \
    lib/FTCI2C.h \
    lib/ftd2xx.h \
    cvt_debugtab.h \
    cvt_burntab.h \
    global.h \
    mainwindow.h \
    i2c_settingwindow.h \
    communicationsetting.h \
    helpwindow.h \
    instructionwindow.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lFTCI2C
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lFTCI2C

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/libFTCI2C.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/libFTCI2C.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/FTCI2C.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/FTCI2C.lib

RESOURCES += \
    Images/images.qrc

FORMS += \
    mainwindow.ui \
    i2c_settingwindow.ui \
    communicationsetting.ui \
    helpwindow.ui \
    instructionwindow.ui

