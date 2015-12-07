#include "moduletree.h"
#include <QDomDocument>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <qglobal.h>
#include <QMimeData>

moduletree::moduletree(QWidget *parent) :
    QTreeWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);

    setDragEnabled(true);
    setAcceptDrops(true);
    cntModules = 0;
}

void moduletree::setPlugins(QList<impPluginInstance *> lstPlugins)
{
    //qWarning() << "Start: setPlugins";
    plugins.append(lstPlugins);
     //qWarning() << "End: setPlugins";
}


void moduletree::createStandardTree()
{
    this->clear();
    items.clear();
    int parent;
    int parent2;

    QString suffix;
    QString prefix;
#if defined(Q_OS_WIN)
    suffix = "dll";
    prefix = "";
#else
    suffix = "so";
    prefix = "lib";
#endif

    cntModules = 0;


    parent = insertNodes(-1,tr("Climate"),prefix + "impmainmodules." + suffix,"ClimateCover");
    insertNodes(parent,tr("Climatic characteristics"),prefix + "impmainmodules." + suffix,"Climate");

    parent = insertNodes(-1,tr("Household"),prefix + "impmainmodules." + suffix,"HouseholdCover");
    parent = insertNodes(parent,tr("Household size"),prefix + "impmainmodules." + suffix,"HouseSize");

    parent = insertNodes(-1,tr("Land"),prefix + "impmainmodules." + suffix,"LandCover");
    parent2 = insertNodes(parent,tr("Land use"),prefix + "impmainmodules." + suffix,"LandUseCover");
    insertNodes(parent2,tr("Plot areas and distribution"),prefix + "impmainmodules." + suffix,"Plots");
    insertNodes(parent2,tr("Plot Management"),prefix + "impmainmodules." + suffix,"PlotMan");
    insertNodes(parent2,tr("Crop plots"),prefix + "impmainmodules." + suffix,"CropPlots");
    insertNodes(parent2,tr("Pasture plots"),prefix + "impmainmodules." + suffix,"PasturePlots");

    parent2 = insertNodes(parent,tr("Land management"),prefix + "impmainmodules." + suffix,"LandManagementCover");
    insertNodes(parent2,tr("Crop management"),prefix + "impmainmodules." + suffix,"CropMan");
    insertNodes(parent2,tr("Crop haverst management"),prefix + "impmainmodules." + suffix,"CropHarvest");
    insertNodes(parent2,tr("Vegetable management"),prefix + "impmainmodules." + suffix,"VegMan");
    insertNodes(parent2,tr("Vegetable harvest management"),prefix + "impmainmodules." + suffix,"VegHarvest");
    insertNodes(parent2,tr("Pasture management"),prefix + "impmainmodules." + suffix,"GrassMan");
    insertNodes(parent2,tr("Pasture harvest management"),prefix + "impmainmodules." + suffix,"GrassHarvest");

    parent = insertNodes(-1,tr("Livestock"),prefix + "impmainmodules." + suffix,"LivestockCover");
    insertNodes(parent,tr("Species and purpose"),prefix + "impmainmodules." + suffix,"Livestock");
    insertNodes(parent,tr("Herd structure"),prefix + "impmainmodules." + suffix,"HerdStructure");
    insertNodes(parent,tr("Feeding groups"),prefix + "impmainmodules." + suffix,"FeedGroups");

    parent2 = insertNodes(parent,tr("Livestock Activities"),prefix + "impmainmodules." + suffix,"LiveActCover");
    insertNodes(parent2,tr("Livestock management"),prefix + "impmainmodules." + suffix,"LivestockMan");
    insertNodes(parent2,tr("Livestock production"),prefix + "impmainmodules." + suffix,"LivestockProduction");
    insertNodes(parent2,tr("Manure management"),prefix + "impmainmodules." + suffix,"ManureMan");
    insertNodes(parent2,tr("Grazing management"),prefix + "impmainmodules." + suffix,"GrazingMan");
    insertNodes(parent2,tr("Feeding management"),prefix + "impmainmodules." + suffix,"StallFeeding");

    parent = insertNodes(-1,tr("Labour"),prefix + "impmainmodules." + suffix,"LabourCover");
    insertNodes(parent,tr("Other livestock labour"),prefix + "impmainmodules." + suffix,"OthLvstLabour");
    insertNodes(parent,tr("Other labour"),prefix + "impmainmodules." + suffix,"OthLabour");

    parent = insertNodes(-1,tr("Farm inputs and outputs"),prefix + "impmainmodules." + suffix,"IOCover");
    parent2 = insertNodes(parent,tr("Sale of products"),prefix + "impmainmodules." + suffix,"CropSalesCover");
    insertNodes(parent2,tr("Crops, vegetables and pastures"),prefix + "impmainmodules." + suffix,"SoldCropProds");
    insertNodes(parent2,tr("Livestock"),prefix + "impmainmodules." + suffix,"SoldLiveProds");

    parent2 = insertNodes(parent,tr("Purchased inputs"),prefix + "impmainmodules." + suffix,"PrchInsCover");
    insertNodes(parent2,tr("Crops, vegetables and pastures"),prefix + "impmainmodules." + suffix,"PrchInsCrops");
    insertNodes(parent2,tr("Livestock"),prefix + "impmainmodules." + suffix,"OthLvstExpenses");

    parent2 = insertNodes(parent,tr("Other household income and expeditures"),prefix + "impmainmodules." + suffix,"OtherInsCover");
    insertNodes(parent2,tr("Other household income"),prefix + "impmainmodules." + suffix,"OthIncome");
    insertNodes(parent2,tr("Farm assets"),prefix + "impmainmodules." + suffix,"farmAssets");
    insertNodes(parent2,tr("Other household expenses"),prefix + "impmainmodules." + suffix,"OthExpenses");

    parent = insertNodes(-1,tr("Household dietary pattern"),prefix + "impmainmodules." + suffix,"DietCover");
    parent2 = insertNodes(parent,tr("Household consumption"),prefix + "impmainmodules." + suffix,"HHCover");
    insertNodes(parent2,tr("Products from crop, vegetables and forages"),prefix + "impmainmodules." + suffix,"HHConsCrops");
    insertNodes(parent2,tr("Livestock products"),prefix + "impmainmodules." + suffix,"HHConsLive");
    insertNodes(parent2,tr("Other products"),prefix + "impmainmodules." + suffix,"HHConsOther");

    parent = insertNodes(-1,tr("Organic mineral information"),prefix + "impmainmodules." + suffix,"NKPCCover");
    insertNodes(parent,tr("Crop and vegetable products"),prefix + "impmainmodules." + suffix,"NKPCCrops");
    insertNodes(parent,tr("Pasture products"),prefix + "impmainmodules." + suffix,"NKPCPastures");
    insertNodes(parent,tr("Livestock products"),prefix + "impmainmodules." + suffix,"NKPCLivestock");

    this->insertTopLevelItems(0, items);
    this->expandAll();
    emit treeChanged();

}

void moduletree::dragEnterEvent (QDragEnterEvent * event )
{
    if (event->mimeData()->hasFormat("object/x-IMPACT-plugin"))
         event->accept();
     else
         event->ignore();
}

void moduletree::dragMoveEvent (QDragMoveEvent * event)
{
    if (event->mimeData()->hasFormat("object/x-IMPACT-plugin"))
    {
        event->setDropAction(Qt::MoveAction);

        QTreeWidgetItem *item;
        item = this->itemAt(event->pos());
        if (item)
        {
            this->clearSelection();
            item->setSelected(true);
        }

        event->accept();
    }
    else
        event->ignore();
}

QString moduletree::getModuleCode(int cnt)
{
    QString res;
    res = QString::number(cnt);
    int size;
    size = res.length();
    for (int pos = 1; pos <= 3-size;pos++)
    {
        res = "0" + res;
    }
    return res;
}

QString moduletree::getModuleDescription(QString library, QString interface, QString plugin)
{
    //qWarning() << "Start: getModuleDescription";
    int pos;
    for (pos=0;pos <=plugins.count()-1;pos++)
    {
        if (plugins[pos]->getLibraryName() == library)
        {
            if (plugins[pos]->getInstanceName() == interface)
            {
                impInterface *iinstance = qobject_cast<impInterface *>(plugins[pos]->getInstance());
                return iinstance->getPluginDescription(plugin);
            }
        }
    }
    //qWarning() << "End: getModuleDescription";
    return QString();
}

int moduletree::insertNodes(int parent, QString description, QString library, QString pluginCode)
{
    cntModules++;
    QString code;
    QString parentCode;
    code = getModuleCode(cntModules);
    if (parent == -1)
        parentCode = "NULL";
    else
        parentCode = items[parent]->data(0,Qt::UserRole+3).toString();
    return insertNodes(parent,description,library,pluginCode,code,parentCode,cntModules);
}

int moduletree::insertNodes(int parent, QString description, QString library, QString pluginCode, QString code, QString parentCode, int order)
{
    //qDebug() << caption;
    int res;
    QTreeWidgetItem *newItem;
    if (parent == -1)
    {
        newItem = new QTreeWidgetItem(QStringList(description));

        newItem->setText(0, description);
        newItem->setData(0,Qt::UserRole,library);
        newItem->setData(0,Qt::UserRole+1,pluginCode);
        newItem->setData(0,Qt::UserRole+2,description);
        newItem->setData(0,Qt::UserRole+3,code);
        newItem->setData(0,Qt::UserRole+4,parentCode);
        newItem->setData(0,Qt::UserRole+5,order);
        if (getModuleDescription(library,"impInterface",pluginCode).isEmpty())
            newItem->setTextColor(0,QColor(Qt::red));
        items.append(newItem);
    }
    else
    {
        newItem = new QTreeWidgetItem(items[parent],QStringList(description));
        newItem->setText(0, description);
        newItem->setData(0,Qt::UserRole,library);
        newItem->setData(0,Qt::UserRole+1,pluginCode);
        newItem->setData(0,Qt::UserRole+2,description);
        newItem->setData(0,Qt::UserRole+3,code);
        newItem->setData(0,Qt::UserRole+4,parentCode);
        newItem->setData(0,Qt::UserRole+5,order);
        if (getModuleDescription(library,"impInterface",pluginCode).isEmpty())
            newItem->setTextColor(0,QColor(Qt::red));
        items.append(newItem);
    }
    res = items.count()-1; //We get here the index of the last item
    //Change some flags and set the state of the node of unchecked
    items[res]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return res; //We return the new node so can be used to set childs
}

void moduletree::loadTree(QSqlDatabase db)
{
    QSqlQuery modules(db);
    QString sql;
    sql = "SELECT module_id,module_library,";
    sql = sql + "module_plugin,module_pluginDesc,module_order,";
    sql = sql + "parent_module FROM moduletree ORDER BY module_order";
    if (modules.exec(sql))
    {
        while (modules.next())
        {
            cntModules++;
            if (modules.value(5).toString().isEmpty())
            {
                insertNodes(-1,modules.value(3).toString(),
                            modules.value(1).toString(),modules.value(2).toString(),
                            modules.value(0).toString(),"NULL",
                            modules.value(4).toInt());
            }
            else
            {
                insertNodes(modules.value(5).toInt()-1,modules.value(3).toString(),
                            modules.value(1).toString(),modules.value(2).toString(),
                            modules.value(0).toString(),modules.value(5).toString(),
                            modules.value(4).toInt());
            }
        }
        if (items.count() > 0)
        {
            this->insertTopLevelItems(0, items);
            this->expandAll();
        }
        else
        {
            createStandardTree();
        }

       //qWarning() << "Count: " << cntModules;
    }
    else
    {
        qWarning() << "Error: " << modules.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

void moduletree::dropEvent (QDropEvent * event)
{
    if (event->mimeData()->hasFormat("object/x-IMPACT-plugin"))
    {
       QString draggedLibrary;
       QString draggedPlugin;
       QString draggedPluginDesc;

       QDomDocument doc("IMPMODULE");
       QByteArray objectData = event->mimeData()->data("object/x-IMPACT-plugin");
       doc.setContent(objectData,true);
       QDomElement docElem = doc.documentElement();
       QDomElement element;
       QDomNode node = docElem.firstChild();
       element = node.toElement();
       draggedLibrary = element.text();

       node = node.nextSibling();
       element = node.toElement();
       draggedPlugin = element.text();

       node = node.nextSibling();
       element = node.toElement();
       draggedPluginDesc = element.text();

       QModelIndex index;
       index = this->indexAt(event->pos());

       QTreeWidgetItem *item;
       item = this->itemAt(event->pos());
       cntModules++;
       if (!item)
       {
           QTreeWidgetItem *root = new QTreeWidgetItem(this);
           root->setText(0, draggedPluginDesc);
           root->setData(0,Qt::UserRole,draggedLibrary);
           root->setData(0,Qt::UserRole+1,draggedPlugin);
           root->setData(0,Qt::UserRole+2,draggedPluginDesc);
           root->setData(0,Qt::UserRole+3,getModuleCode(cntModules));
           root->setData(0,Qt::UserRole+4,"NULL");
           root->setData(0,Qt::UserRole+5,cntModules);
       }
       else
       {
           QTreeWidgetItem *child = new QTreeWidgetItem(item);
           child->setText(0, draggedPluginDesc);
           child->setData(0,Qt::UserRole,draggedLibrary);
           child->setData(0,Qt::UserRole+1,draggedPlugin);
           child->setData(0,Qt::UserRole+2,draggedPluginDesc);
           child->setData(0,Qt::UserRole+3,getModuleCode(cntModules));
           child->setData(0,Qt::UserRole+4,item->data(0,Qt::UserRole+3));
           child->setData(0,Qt::UserRole+5,cntModules);
           item->setExpanded(true);
       }
       emit treeChanged();
    }
}
