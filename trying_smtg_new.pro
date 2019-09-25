#-------------------------------------------------
#
# Project created by QtCreator 2019-08-16T03:17:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = trying_smtg_new
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        Components/capacitor.cpp \
        Components/tools/component.cpp \
        Components/resistor.cpp \
        Toolbar_elements/m_button.cpp \
        custom_scene.cpp \
        m_view.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        Components/capacitor.h \
        Components/tools/component.h \
        Components/resistor.h \
        Toolbar_elements/m_button.h \
        custom_scene.h \
        m_view.h \
        mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    icons_rsc.qrc
