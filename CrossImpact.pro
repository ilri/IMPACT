#-------------------------------------------------
#
# Project created by QtCreator 2011-07-01T11:31:29
#
#-------------------------------------------------

QT       += core sql xml webkit widgets webkitwidgets

TARGET = crossimpact
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/documentwindow.cpp \
    src/errordialog.cpp \
    src/countries.cpp \
    src/crops.cpp \
    src/syscharloader.cpp \
    src/impclasses.cpp \
    src/imppluginloader.cpp \
    src/navtree.cpp \
    src/prjsettings.cpp \
    src/pluginnotfound.cpp \
    src/plugintree.cpp \
    src/moduletree.cpp \
    src/genmodels.cpp \
    src/gendelegates.cpp \
    src/general.cpp \
    src/systemsdialog.cpp \    
    src/systypes.cpp \
    src/crpprods.cpp \
    src/crpforages.cpp \
    src/units.cpp \
    src/methods.cpp \
    src/mntdialog.cpp \
    src/activity.cpp \
    src/drainage.cpp \
    src/markoutlets.cpp \
    src/fertilisers.cpp \
    src/soil.cpp \
    src/prchfeeds.cpp \
    src/hhconsprods.cpp \
    src/pastures.cpp \
    src/livestock.cpp \
    src/lvprods.cpp \
    src/prjmanager.cpp \
    src/prjmanadddb.cpp \
    src/prjmancreatedb.cpp \
    src/ecnomics.cpp \
    src/foodsecurity.cpp \
    src/nutrientflows.cpp \
    src/about.cpp

HEADERS  += mainwindow.h \
    src/documentwindow.h \
    src/errordialog.h \
    src/countries.h \
    src/crops.h \
    src/syscharloader.h \
    src/impclasses.h \
    plugins/interfaces/impinterface.h \
    src/imppluginloader.h \
    plugins/interfaces/imptypes.h \
    src/navtree.h \
    src/prjsettings.h \
    src/pluginnotfound.h \
    src/plugintree.h \
    src/moduletree.h \
    src/genmodels.h \
    src/gendelegates.h \
    src/general.h \
    src/systemsdialog.h \
    IMPACTGlobal.h \    
    src/systypes.h \
    src/crpprods.h \
    src/crpforages.h \
    src/units.h \
    src/methods.h \
    src/mntdialog.h \
    src/activity.h \
    src/drainage.h \
    src/markoutlets.h \
    src/fertilisers.h \
    src/soil.h \
    src/prchfeeds.h \
    src/hhconsprods.h \
    src/pastures.h \
    src/livestock.h \
    src/lvprods.h \
    src/prjmanager.h \
    src/prjmanadddb.h \
    src/prjmancreatedb.h \
    src/ecnomics.h \
    src/foodsecurity.h \
    src/nutrientflows.h \
    src/about.h

FORMS    += mainwindow.ui \
    src/documentwindow.ui \
    src/errordialog.ui \
    src/test.ui \
    src/countries.ui \
    src/crops.ui \
    src/navtree.ui \
    src/syscharloader.ui \
    src/prjsettings.ui \
    src/pluginnotfound.ui \
    src/general.ui \
    src/systemsdialog.ui \
    src/systypes.ui \
    src/crpprods.ui \
    src/crpforages.ui \
    src/units.ui \
    src/methods.ui \
    src/mntdialog.ui \
    src/activity.ui \
    src/drainage.ui \
    src/markoutlets.ui \
    src/fertilisers.ui \
    src/soil.ui \
    src/prchfeeds.ui \
    src/hhconsprods.ui \
    src/pastures.ui \
    src/livestock.ui \
    src/lvprods.ui \
    src/prjmanager.ui \
    src/prjmanadddb.ui \
    src/prjmancreatedb.ui \
    src/ecnomics.ui \
    src/foodsecurity.ui \
    src/nutrientflows.ui \
    src/about.ui

INCLUDEPATH += plugins/designer/impwidgets src plugins/interfaces

unix:LIBS += -L/home/cquiros/data/projects/ILRI-MARIO/software/impact/crossimpact -limpwidgetsplugin
win32:LIBS += -LC:\ilri\crossimpact -limpwidgetsplugin


RESOURCES += \
    images/images.qrc \
    database.qrc
