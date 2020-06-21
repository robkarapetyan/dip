#-------------------------------------------------
#
# Project created by QtCreator 2019-08-16T03:17:37
#
#-------------------------------------------------

QT       += core gui svg xml

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
        Components/library.cpp \
        Components/tools/component.cpp \
        Components/resistor.cpp \
        Components/tools/port.cpp \
        ConnectionController/lineitems/flatlineitem.cpp \
        ConnectionController/lineitems/ortogonallineitem.cpp \
        ConnectionController/lines/flatline.cpp \
        ConnectionController/lines/iline.cpp \
        ConnectionController/lines/ortogonalline.cpp \
        Toolbar_elements/m_button.cpp \
        Tools/ActionController/actioncontroller.cpp \
        Tools/ActionController/actions/act.cpp \
        Tools/ActionController/actions/actionadd.cpp \
        Tools/ActionController/actions/actionmove.cpp \
        Tools/ActionController/actions/actionremove.cpp \
        Tools/LibraryEditor/libeditor.cpp \
        common_files/componentmenu.cpp \
        common_files/cyclicchache.cpp \
        common_files/m_spinbox.cpp \
        ConnectionController/connection_controller.cpp \
        custom_scene.cpp \
        m_view.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        Components/capacitor.h \
        Components/library.h \
        Components/tools/component.h \
        Components/resistor.h \
        Components/tools/port.h \
        ConnectionController/lineitems/flatlineitem.h \
        ConnectionController/lineitems/ortogonallineitem.h \
        ConnectionController/lines/flatline.h \
        ConnectionController/lines/iline.h \
        ConnectionController/lines/ortogonalline.h \
        Toolbar_elements/m_button.h \
        Tools/ActionController/actioncontroller.h \
        Tools/ActionController/actions/act.h \
        Tools/ActionController/actions/actionadd.h \
        Tools/ActionController/actions/actionmove.h \
        Tools/ActionController/actions/actionremove.h \
        Tools/LibraryEditor/libeditor.h \
        common_files/componentmenu.h \
        common_files/cyclicchache.h \
        common_files/m_spinbox.h \
        ConnectionController/connection_controller.h \
        custom_scene.h \
        m_view.h \
        mainwindow.h

FORMS += \
        Tools/LibraryEditor/libeditor.ui \
        mainwindow.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    icons_rsc.qrc

DISTFILES += \
    Tools/ActionController/aaa.cpp.txt
