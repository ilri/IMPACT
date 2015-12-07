#-------------------------------------------------
#
# Project created by QtCreator 2011-08-08T10:59:54
#
#-------------------------------------------------

QT += core widgets sql svg

TARGET = $$qtLibraryTarget(impmainmodules)
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../..
INCLUDEPATH += ../../../plugins/interfaces ../../designer/impwidgets ../../designer/impwidgets/widgets/calculator

SOURCES += \
    impmodules.cpp \
    modules/plots.cpp \
    moduleclasses.cpp \
    delegates/labeldelegate.cpp \
    delegates/fixcombodelegate.cpp \
    modulesubscreen.cpp \
    submodules/plotssoil.cpp \
    modules/climate.cpp \
    delegates/imagecombodelegate.cpp \
    modules/cropman.cpp \
    submodules/cropyield.cpp \
    submodules/manureallocation.cpp \
    submodules/inputsallocation.cpp \
    submodules/fertiliserallocation.cpp \
    submodules/labourallocation.cpp \
    delegates/imagecheckdelegate.cpp \
    submodules/plotsgps.cpp \
    submodules/plotsotherinfo.cpp \
    modules/housesize.cpp \
    submodules/hhsotherinfo.cpp \
    submodules/hhswellbeing.cpp \
    submodules/lvotherinfo.cpp \
    modules/livestock.cpp \
    submodules/calvpattern.cpp \
    modules/herdstr.cpp \
    submodules/fdglvallocation.cpp \
    modules/feedgroups.cpp \
    submodules/mnrmannkpc.cpp \
    submodules/mnrmanyieldalloc.cpp \
    submodules/mnrmanothprods.cpp \
    modules/manureman.cpp \
    submodules/feedmngqty.cpp \
    modules/stallfeeding.cpp \
    submodules/feedingothprods.cpp \
    modules/soldprods.cpp \
    modules/prchinscrops.cpp \
    modules/grassman.cpp \
    modules/vegman.cpp \
    modules/livestockman.cpp \
    modules/ohtlivelabour.cpp \
    modules/ohtlabour.cpp \
    modules/prchinslvexpenses.cpp \
    modules/otherincome.cpp \
    modules/otherexpenses.cpp \
    modules/hhconscrops.cpp \
    covers/climatecover.cpp \
    modules/cropplots.cpp \
    delegates/seasondelegate.cpp \
    modules/grassplots.cpp \
    modules/grazingman.cpp \
    modules/livesold.cpp \
    modules/hhconslive.cpp \
    modules/hhconsother.cpp \
    modules/farmassets.cpp \
    modules/npkcconscrops.cpp \
    modules/npkcconspastures.cpp \
    modules/npkcconslive.cpp \
    tabledescriptor.cpp \
    modules/cropharvest.cpp \
    modules/grassharvest.cpp \
    modules/vegharvest.cpp \
    modules/livestockproduction.cpp \
    modules/plotlabour.cpp \
    flickcharm.cpp \
    androideffects.cpp \
    submodules/mnrmanyielddsp.cpp \
    submodules/hhconsprodslkp.cpp \
    covers/householdcover.cpp \
    covers/landcover.cpp \
    covers/landusecover.cpp \
    covers/landmanagementcover.cpp \
    covers/livestockcover.cpp \
    covers/liveactcover.cpp \
    covers/labourcover.cpp \
    covers/iocover.cpp \
    covers/cropsalescover.cpp \
    covers/prchinscover.cpp \
    covers/otherinscover.cpp \
    covers/dietcover.cpp \
    covers/hhcover.cpp \
    covers/nkpccover.cpp


HEADERS += \
    impmodules.h \
    modules/plots.h \
    moduletypes.h \
    moduleclasses.h \
    delegates/labeldelegate.h \
    delegates/fixcombodelegate.h \
    modulesubscreen.h \
    submodules/plotssoil.h \
    modules/climate.h \
    delegates/imagecombodelegate.h \
    modules/cropman.h \
    submodules/cropyield.h \
    submodules/manureallocation.h \
    submodules/inputsallocation.h \
    submodules/fertiliserallocation.h \
    submodules/labourallocation.h \
    delegates/imagecheckdelegate.h \
    submodules/plotsgps.h \
    submodules/plotsotherinfo.h \
    modules/housesize.h \
    submodules/hhsotherinfo.h \
    submodules/hhswellbeing.h \
    submodules/lvotherinfo.h \
    modules/livestock.h \
    submodules/calvpattern.h \
    modules/herdstr.h \
    submodules/fdglvallocation.h \
    modules/feedgroups.h \
    submodules/mnrmannkpc.h \
    submodules/mnrmanyieldalloc.h \
    submodules/mnrmanothprods.h \
    modules/manureman.h \
    submodules/feedmngqty.h \
    modules/stallfeeding.h \
    submodules/feedingothprods.h \
    modules/soldprods.h \
    modules/prchinscrops.h \
    modules/grassman.h \
    modules/vegman.h \
    modules/livestockman.h \
    modules/ohtlivelabour.h \
    modules/ohtlabour.h \
    modules/prchinslvexpenses.h \
    modules/otherincome.h \
    modules/otherexpenses.h \
    modules/hhconscrops.h \
    covers/climatecover.h \
    modules/cropplots.h \
    delegates/seasondelegate.h \
    modules/grassplots.h \
    modules/grazingman.h \
    modules/livesold.h \
    modules/hhconslive.h \
    modules/hhconsother.h \
    modules/farmassets.h \
    modules/npkcconscrops.h \
    modules/npkcconspastures.h \
    modules/npkcconslive.h \
    tabledescriptor.h \
    modules/cropharvest.h \
    modules/grassharvest.h \
    modules/vegharvest.h \
    modules/livestockproduction.h \
    moduleglobal.h \
    modules/plotlabour.h \
    flickcharm.h \
    androideffects.h \
    submodules/mnrmanyielddsp.h \
    submodules/hhconsprodslkp.h \
    covers/householdcover.h \
    covers/landcover.h \
    covers/landusecover.h \
    covers/landmanagementcover.h \
    covers/livestockcover.h \
    covers/liveactcover.h \
    covers/labourcover.h \
    covers/iocover.h \
    covers/cropsalescover.h \
    covers/prchinscover.h \
    covers/otherinscover.h \
    covers/dietcover.h \
    covers/hhcover.h \
    covers/nkpccover.h


FORMS += \
    modules/plots.ui \
    modulesubscreen.ui \
    submodules/plotssoil.ui \
    modules/climate.ui \
    modules/cropman.ui \
    submodules/cropyield.ui \
    submodules/manureallocation.ui \
    submodules/inputsallocation.ui \
    submodules/fertiliserallocation.ui \
    submodules/labourallocation.ui \
    submodules/plotsgps.ui \
    submodules/plotsotherinfo.ui \
    modules/housesize.ui \
    submodules/hhsotherinfo.ui \
    submodules/hhswellbeing.ui \
    submodules/lvotherinfo.ui \
    modules/livestock.ui \
    modules/herdstr.ui \
    submodules/calvpattern.ui \
    modules/feedgroups.ui \
    submodules/fdglvallocation.ui \
    modules/manureman.ui \
    submodules/mnrmannkpc.ui \
    submodules/mnrmanyieldalloc.ui \
    submodules/mnrmanothprods.ui \
    modules/stallfeeding.ui \
    submodules/feedmngqty.ui \
    submodules/feedingothprods.ui \
    modules/soldprods.ui \
    modules/prchinscrops.ui \
    modules/grassman.ui \
    modules/vegman.ui \
    modules/livestockman.ui \
    modules/ohtlivelabour.ui \
    modules/ohtlabour.ui \
    modules/prchinslvexpenses.ui \
    modules/otherincome.ui \
    modules/otherexpenses.ui \
    modules/hhconscrops.ui \
    covers/climatecover.ui \
    modules/cropplots.ui \
    modules/grassplots.ui \
    modules/grazingman.ui \
    modules/livesold.ui \
    modules/hhconslive.ui \
    modules/hhconsother.ui \
    modules/farmassets.ui \
    modules/npkcconscrops.ui \
    modules/npkcconspastures.ui \
    modules/npkcconslive.ui \
    modules/cropharvest.ui \
    modules/grassharvest.ui \
    modules/vegharvest.ui \
    modules/livestockproduction.ui \
    modules/plotlabour.ui \
    submodules/mnrmanyielddsp.ui \
    submodules/hhconsprodslkp.ui \
    covers/householdcover.ui \
    covers/landcover.ui \
    covers/landusecover.ui \
    covers/landmanagementcover.ui \
    covers/livestockcover.ui \
    covers/liveactcover.ui \
    covers/labourcover.ui \
    covers/iocover.ui \
    covers/cropsalescover.ui \
    covers/prchinscover.ui \
    covers/otherinscover.ui \
    covers/dietcover.ui \
    covers/hhcover.ui \
    covers/nkpccover.ui

RESOURCES += images.qrc

unix:LIBS += -L/home/cquiros/data/projects/ILRI-MARIO/software/impact/crossimpact/ -limpwidgetsplugin
win32:LIBS += -LC:\ilri\crossimpact -limpwidgetsplugin





























































