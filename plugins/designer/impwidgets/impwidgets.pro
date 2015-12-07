QT       += core sql widgets designer
TARGET      = $$qtLibraryTarget(impwidgetsplugin)
TEMPLATE    = lib
CONFIG     += plugin
DEFINES += IMPWIDGETS_LIBRARY

HEADERS     = impeditplugin.h impcheckboxplugin.h impgroupboxplugin.h impradiobuttonplugin.h impcomboboxplugin.h impwidgets.h impgenmaintplugin.h \
    ../../../src/errordialog.h \
    maintclases.h \
    widgets/calculator/calculator.h \
    widgets/calculator/button.h \
    widgets/calendar/calendardialog.h \
    widgets/calculator/calcdialog.h \
    export.h
SOURCES     = impeditplugin.cpp impcheckboxplugin.cpp impgroupboxplugin.cpp impradiobuttonplugin.cpp impcomboboxplugin.cpp impwidgets.cpp impgenmaintplugin.cpp \
    ../../../src/errordialog.cpp \
    maintclases.cpp \
    widgets/calculator/calculator.cpp \
    widgets/calculator/button.cpp \
    widgets/calendar/calendardialog.cpp \
    widgets/calculator/calcdialog.cpp
RESOURCES   = icons.qrc
#LIBS        += -L.

INCLUDEPATH += ../../../src ../../../plugins/interfaces

DESTDIR = ../../..

include(impedit.pri)
include(impgroupbox.pri)
include(impcombobox.pri)
include(impcheckbox.pri)
include(impradiobutton.pri)
include(impgenmaint.pri)

FORMS += \
    ../../../src/errordialog.ui \
    widgets/calendar/calendardialog.ui \
    widgets/calculator/calcdialog.ui


#DEFINES += QT_NO_PRINTER






