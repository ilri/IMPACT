#include "impmodules.h"
#include "modules/plots.h"
#include "modules/climate.h"
#include "modules/cropman.h"
#include "modules/cropharvest.h"
#include "modules/grassman.h"
#include "modules/grassharvest.h"
#include "modules/vegman.h"
#include "modules/vegharvest.h"
#include "modules/housesize.h"
#include "modules/livestock.h"
#include "modules/herdstr.h"
#include "modules/feedgroups.h"
#include "modules/manureman.h"
#include "modules/stallfeeding.h"
#include "modules/soldprods.h"
#include "modules/prchinscrops.h"
#include "modules/livestockman.h"
#include "modules/livestockproduction.h"
#include "modules/ohtlivelabour.h"
#include "modules/ohtlabour.h"
#include "modules/prchinslvexpenses.h"
#include "modules/otherincome.h"
#include "modules/otherexpenses.h"
#include "modules/hhconscrops.h"
#include "modules/cropplots.h"
#include "modules/grassplots.h"
#include "modules/grazingman.h"
#include "modules/livesold.h"
#include "modules/hhconslive.h"
#include "modules/hhconsother.h"
#include "modules/farmassets.h"
#include "modules/npkcconscrops.h"
#include "modules/npkcconspastures.h"
#include "modules/npkcconslive.h"
#include "modules/plotlabour.h"

#include "covers/climatecover.h"
#include "covers/householdcover.h"
#include "covers/landcover.h"
#include "covers/landusecover.h"
#include "covers/landmanagementcover.h"
#include "covers/livestockcover.h"
#include "covers/liveactcover.h"
#include "covers/labourcover.h"
#include "covers/iocover.h"
#include "covers/cropsalescover.h"
#include "covers/prchinscover.h"
#include "covers/otherinscover.h"
#include "covers/dietcover.h"
#include "covers/hhcover.h"
#include "covers/nkpccover.h"



QStringList impMainModules::pluginNames()
{
    QStringList res;
    res << tr("Plots");
    res << tr("Climate");
    res << tr("CropMan");
    res << tr("CropHarvest");
    res << tr("GrassMan");
    res << tr("GrassHarvest");
    res << tr("VegMan");
    res << tr("VegHarvest");
    res << tr("HouseSize");
    res << tr("Livestock");
    res << tr("HerdStructure");
    res << tr("FeedGroups");
    res << tr("ManureMan");
    res << tr("StallFeeding");
    res << tr("SoldCropProds");
    res << tr("PrchInsCrops");
    res << tr("LivestockMan");
    res << tr("LivestockProduction");
    res << tr("OthLvstLabour");
    res << tr("OthLabour");
    res << tr("OthLvstExpenses");
    res << tr("OthIncome");
    res << tr("OthExpenses");
    res << tr("HHConsCrops");
    res << tr("CropPlots");
    res << tr("PasturePlots");
    res << tr("GrazingMan");
    res << tr("SoldLiveProds");
    res << tr("HHConsLive");
    res << tr("HHConsOther");
    res << tr("farmAssets");
    res << tr("NKPCCrops");
    res << tr("NKPCPastures");
    res << tr("NKPCLivestock");
    res << tr("PlotMan");

    res << tr("ClimateCover");
    res << tr("HouseholdCover");
    res << tr("LandCover");
    res << tr("LandUseCover");
    res << tr("LandManagementCover");
    res << tr("LivestockCover");
    res << tr("LiveActCover");
    res << tr("LabourCover");
    res << tr("IOCover");
    res << tr("CropSalesCover");
    res << tr("PrchInsCover");
    res << tr("OtherInsCover");
    res << tr("DietCover");
    res << tr("HHCover");
    res << tr("NKPCCover");


    return res;
}

QString impMainModules::getPluginDescription(QString pluginName)
{
    QString res;
    res = "";

    if (pluginName == "Plots")
        res = tr("Plot areas and distribution");
    if (pluginName == "Climate")
        res = tr("Climatic characteristics");
    if (pluginName == "CropMan")
        res = tr("Crop activity management");
    if (pluginName == "PlotMan")
        res = tr("Plot activity management");
    if (pluginName == "CropHarvest")
        res = tr("Crop harvesting management");
    if (pluginName == "GrassMan")
        res = tr("Pasture management");
    if (pluginName == "GrassHarvest")
        res = tr("Pasture harvesting management");
    if (pluginName == "VegMan")
        res = tr("Vegetable management");
    if (pluginName == "VegHarvest")
        res = tr("Vegetable harvesting management");
    if (pluginName == "HouseSize")
        res = tr("Household characteristics");
    if (pluginName == "Livestock")
        res = tr("Livestock");
    if (pluginName == "HerdStructure")
        res = tr("Herd structucture");
    if (pluginName == "FeedGroups")
        res = tr("Feeding groups");
    if (pluginName == "ManureMan")
        res = tr("Manure management");
    if (pluginName == "StallFeeding")
        res = tr("Stall feeding");
    if (pluginName == "SoldCropProds")
        res = tr("Sales of crops, pastures and vegetables");
    if (pluginName == "PrchInsCrops")
        res = tr("Purchased inputs for crop, pastures and vegetables");
    if (pluginName == "LivestockMan")
        res = tr("Livestock management");
    if (pluginName == "LivestockProduction")
        res = tr("Livestock production management");
    if (pluginName == "OthLvstLabour")
        res = tr("Other livestock labour");
    if (pluginName == "OthLabour")
        res = tr("Other labour");
    if (pluginName == "OthLvstExpenses")
        res = tr("Other livestock expenses");
    if (pluginName == "OthIncome")
        res = tr("Other income");
    if (pluginName == "OthExpenses")
        res = tr("Other expenses");
    if (pluginName == "HHConsCrops")
        res = tr("Household consumption of crops");
    if (pluginName == "CropPlots")
        res = tr("Crop plots");
    if (pluginName == "PasturePlots")
        res = tr("Pasture plots");
    if (pluginName == "GrazingMan")
        res = tr("Grazing management");
    if (pluginName == "SoldLiveProds")
        res = tr("Sales of livestock products");
    if (pluginName == "HHConsLive")
        res = tr("Household consumption of livestock products");
    if (pluginName == "HHConsOther")
        res = tr("Other products consumed by the household");
    if (pluginName == "farmAssets")
        res = tr("Household assets");
    if (pluginName == "NKPCCrops")
        res = tr("NPKC in crop and vegetable products");
    if (pluginName == "NKPCPastures")
        res = tr("NPKC in pasture products");
    if (pluginName == "NKPCLivestock")
        res = tr("NPKC in livestock products");

    //
    //Covers
    if (pluginName == "ClimateCover")
        res = tr("Climate");
    if (pluginName == "HouseholdCover")
        res = tr("Household");
    if (pluginName == "LandCover")
        res = tr("Land");
    if (pluginName == "LandUseCover")
        res = tr("Land use");
    if (pluginName == "LandManagementCover")
        res = tr("Land management");
    if (pluginName == "LivestockCover")
        res = tr("Livestock");
    if (pluginName == "LiveActCover")
        res = tr("Livestock activities");
    if (pluginName == "LabourCover")
        res = tr("Labour");
    if (pluginName == "IOCover")
        res = tr("Farm inputs and outputs");
    if (pluginName == "CropSalesCover")
        res = tr("Sale of products");
    if (pluginName == "PrchInsCover")
        res = tr("Purchased inputs");
    if (pluginName == "OtherInsCover")
        res = tr("Other household income and expenditures");
    if (pluginName == "DietCover")
        res = tr("Household dietary pattern");
    if (pluginName == "HHCover")
        res = tr("Household consumption");
    if (pluginName == "NKPCCover")
        res = tr("Organic mineral information");

    return res;
}

QWidget* impMainModules::loadScreen(QString pluginName,  QSqlDatabase dataBase, QString systemID)
{
    QWidget *result;
    result = 0;
    if (pluginName == tr("Plots"))
    {
        result = new plots(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("Climate"))
    {
        result = new climate(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("CropMan"))
    {
        result = new cropman(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("PlotMan"))
    {
        result = new plotlabour(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("CropHarvest"))
    {
        result = new cropharvest(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("GrassMan"))
    {
        result = new grassman(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("GrassHarvest"))
    {
        result = new grassharvest(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("VegMan"))
    {
        result = new vegman(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("VegHarvest"))
    {
        result = new vegharvest(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("HouseSize"))
    {
        result = new housesize(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("Livestock"))
    {
        result = new livestock(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("HerdStructure"))
    {
        result = new herdstr(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("FeedGroups"))
    {
        result = new feedgroups(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("ManureMan"))
    {
        result = new manureman(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("StallFeeding"))
    {
        result = new stallfeeding(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("SoldCropProds"))
    {
        result = new soldprods(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("PrchInsCrops"))
    {
        result = new prchinscrops(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("LivestockMan"))
    {
        result = new livestockman(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("LivestockProduction"))
    {
        result = new livestockproduction(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("OthLvstLabour"))
    {
        result = new ohtlivelabour(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("OthLabour"))
    {
        result = new ohtlabour(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("OthLvstExpenses"))
    {
        result = new prchinslvexpenses(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("OthIncome"))
    {
        result = new otherincome(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("OthExpenses"))
    {
        result = new otherexpenses(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("HHConsCrops"))
    {
        result = new hhconscrops(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("CropPlots"))
    {
        result = new cropplots(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("PasturePlots"))
    {
        result = new grassplots(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("GrazingMan"))
    {
        result = new grazingman(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("SoldLiveProds"))
    {
        result = new livesold(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("HHConsLive"))
    {
        result = new hhconslive(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("HHConsOther"))
    {
        result = new hhconsother(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("farmAssets"))
    {
        result = new farmassets(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("NKPCCrops"))
    {
        result = new NPKCConsCrops(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("NKPCPastures"))
    {
        result = new NPKCConsPastures(0,dataBase,systemID);
        return result;
    }
    if (pluginName == tr("NKPCLivestock"))
    {
        result = new NPKCConsLive(0,dataBase,systemID);
        return result;
    }

//
    //Covers
    if (pluginName == tr("ClimateCover"))
    {
        result = new climateCover(0);
        return result;
    }
    if (pluginName == tr("HouseholdCover"))
    {
        result = new householdCover(0);
        return result;
    }
    if (pluginName == tr("LandCover"))
    {
        result = new landCover(0);
        return result;
    }
    if (pluginName == tr("LandUseCover"))
    {
        result = new landUseCover(0);
        return result;
    }
    if (pluginName == tr("LandManagementCover"))
    {
        result = new landManagementCover(0);
        return result;
    }
    if (pluginName == tr("LivestockCover"))
    {
        result = new livestockCover(0);
        return result;
    }
    if (pluginName == tr("LiveActCover"))
    {
        result = new liveActCover(0);
        return result;
    }
    if (pluginName == tr("LabourCover"))
    {
        result = new labourCover(0);
        return result;
    }
    if (pluginName == tr("IOCover"))
    {
        result = new ioCover(0);
        return result;
    }
    if (pluginName == tr("CropSalesCover"))
    {
        result = new cropSalesCover(0);
        return result;
    }
    if (pluginName == tr("PrchInsCover"))
    {
        result = new prchInsCover(0);
        return result;
    }
    if (pluginName == tr("OtherInsCover"))
    {
        result = new otherInsCover(0);
        return result;
    }
    if (pluginName == tr("DietCover"))
    {
        result = new dietCover(0);
        return result;
    }
    if (pluginName == tr("HHCover"))
    {
        result = new hhCover(0);
        return result;
    }
    if (pluginName == tr("NKPCCover"))
    {
        result = new NKPCCover(0);
        return result;
    }
    return result;
}

//Q_EXPORT_PLUGIN2(imp_mainModules, impMainModules)
