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
    mainwindow.cpp \
    communicationsetting.cpp \
    helpwindow.cpp \
    instructionwindow.cpp \
    burn_data.cpp \
    edid_data.cpp \
    hdcp_data.cpp \
    isp_iic.cpp \
    burnsetting.cpp \
    ddc_protocol.cpp \
    transfer.cpp \
    ddc_aps.cpp

HEADERS  += \
    cvt_global.h \
    lib/FTCI2C.h \
    lib/ftd2xx.h \
    mainwindow.h \
    communicationsetting.h \
    helpwindow.h \
    instructionwindow.h \
    burn_data.h \
    edid_data.h \
    hdcp_data.h \
    isp_iic.h \
    burnsetting.h \
    ddc_protocol.h \
    transfer.h \
    ddc_aps.h

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
    communicationsetting.ui \
    helpwindow.ui \
    instructionwindow.ui

