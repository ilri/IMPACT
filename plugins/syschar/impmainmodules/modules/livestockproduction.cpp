#include "livestockproduction.h"
#include "ui_livestockproduction.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qpixmap.h>
#include "submodules/cropyield.h"
#include "submodules/labourallocation.h"
#include "modulesubscreen.h"
#include "moduleglobal.h"
#include <QDebug>
#include <QMenu>

livestockproduction::livestockproduction(QWidget *parent,QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::livestockproduction)
{
    dataBase = db;
    //currentSystem = "KE20061129ILREMBMZCTB";
    currentSystem = systemID;
    ui->setupUi(this);
    loadForm();
    createActions();
    currentLiveCod = "NULL";
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

livestockproduction::~livestockproduction()
{
    //qDebug() << "Before destroy livestockman UI";
    delete ui;
    //qDebug() << "After destroy livestockman UI";
}

void livestockproduction::loadAndroidEffects()
{
    cropcharm.activateOn(ui->AdvListView1);
    actcharm.activateOn(ui->AdvListView5);
}

void livestockproduction::loadForm()
{
    connect(ui->AdvListView1,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(cropMenuRequested(QPoint)));

    ui->AdvListView1->setContextMenuPolicy(Qt::CustomContextMenu);

    //Creates and load the parent model
    m_parentModel = new fieldInColModel(this);
    m_parentModel->setReadOnly(true);
    m_parentModel->setDatabase(dataBase);
    //m_parentModel->setImpactModule(this);
    m_parentModel->setTableName("feedgroups");


    m_parentModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_parentModel->insertField("LIVE_ICOD","Livestock","000","000",true,false,true,true);
    m_parentModel->insertField("GROUP_COD","Feeding group","000","000",true,false,false);
    m_parentModel->insertField("GROUP_DES","Feeding group","000","000",false,false,true);

    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_parentModel->loadData(keys);
    ui->AdvListView1->setModel(m_parentModel);
    //m_parentModel->distributeColums();
    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(parentViewActivated(QModelIndex)));
    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);



    QList<TmoduleFieldDef> cpmankeys;
    key.code = "SYS_COD";
    cpmankeys.append(key);
    key.code = "LIVE_ICOD";
    cpmankeys.append(key);
    key.code = "GROUP_COD";
    cpmankeys.append(key);
    key.code = "ACTY_COD";
    cpmankeys.append(key);

    //Load the harvest model
    m_harvestModel = new managementModel(this);
    connect(m_harvestModel,SIGNAL(modeldDataChanged()),this,SLOT(harvestingChanged()));


    m_hvrstSortModel = new managementSortModel(this);

    m_harvestModel->setDatabase(dataBase);
    //m_harvestModel->setImpactModule(this);

    m_harvestModel->setlkUpColDescription("Livestock products");
    cpmankeys.removeLast();
    key.code = "LIVE_COD";
    cpmankeys.append(key);
    //Add the harvesting of products
    m_harvestModel->addMasterTable("livehvst");
    m_harvestModel->setKeysFields("livehvst",cpmankeys);
    m_harvestModel->setLookUpLinkedField("livehvst","PROD_COD");
    m_harvestModel->setPeriodTable("livehvst","livehvsttim");


    key.code = "PROD_COD";
    cpmankeys.append(key);
    m_harvestModel->setPeriodKeyFields("livehvst",cpmankeys);
    m_harvestModel->setPeriodField("livehvst","NMONTH");
    m_harvestModel->setPeriodStatusField("livehvst","CHECKED");
    m_harvestModel->setPeriodDefaultValue("livehvst","N");

    // We don't use a standard lookup table as the products change
    // between crops
    m_harvestModel->setlkUpUnChkColor("livehvst",QColor(255,222,222));
    m_harvestModel->setlkUpChkColor("livehvst",QColor(255,191,191));
    m_harvestModel->setlkUpColOrder("livehvst","1P");

    m_hvrstSortModel->setSourceModel(m_harvestModel);
    ui->AdvListView5->setModel(m_hvrstSortModel);
    //m_harvestModel->distributeColums();

    if (m_parentModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_parentModel->index(0,0);
        parentViewActivated(start);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
    {
        ui->AdvListView5->setEnabled(false);
    }

    for (int rsize = 0; rsize <= m_parentModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);
}

void livestockproduction::constructCustomDelegator()
{
    fixComboDelegate *pdelegate = new fixComboDelegate(this);
    pdelegate->setReadOnly(true);
    QSqlQuery livestock(dataBase);
    QString sql;
    QString desc;
    sql = "SELECT lvstosys.live_icod,lvstosys.purpose,lvstosys.bredd,livestoc.LIVE_DES FROM lvstosys,livestoc";
    sql = sql + " WHERE lvstosys.LIVE_COD = livestoc.live_cod AND lvstosys.sys_cod = '" + currentSystem + "'";
    livestock.exec(sql);
    while(livestock.next())
    {
        desc = livestock.value(3).toString() + " - Purpose: " + livestock.value(1).toString();
        desc = desc + " - Breed: " + livestock.value(2).toString();
        pdelegate->insertItem(livestock.value(0).toString(),desc);
    }
    ui->AdvListView1->setItemDelegateForColumn(0,pdelegate);


    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->addIgnoredColumn(0);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate->setManagementModel(m_harvestModel);
    ckhdelegate->setSortModel(m_hvrstSortModel);
    connect(ckhdelegate,SIGNAL(requestPopUpMenu(QPoint,QModelIndex)),this,SLOT(constructPorFPopUpMenu(QPoint,QModelIndex)));


    int pos;
    for (pos = 0; pos <= m_hvrstSortModel->columnCount()-1;pos++)
    {
        if (pos > 0)
        {
            ui->AdvListView5->setItemDelegateForColumn(pos,ckhdelegate);
        }
    }

    for (pos =0; pos <= m_hvrstSortModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView5->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView5->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void livestockproduction::constructHeader1(QList<TmoduleFieldDef> keys)
{
    header1 = "";
    QString livestock = "";
    QString group = "";

    QSqlQuery query(dataBase);
    QString sql;
    int pos;
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        if (keys[pos].code.toUpper() == "LIVE_ICOD")
            livestock = keys[pos].value.toString();
        if (keys[pos].code.toUpper() == "GROUP_COD")
            group = keys[pos].value.toString();
    }
    sql = "SELECT feedgroups.grop_des,lvstosys.purpose,lvstosys.bredd,livestoc.LIVE_DES";
    sql = sql + " FROM lvstosys,livestoc,feedgroups";
    sql = sql + " WHERE lvstosys.LIVE_COD = livestoc.LIVE_COD";
    sql = sql + " AND lvstosys.LIVE_ICOD = feedgroups.LIVE_ICOD";
    sql = sql + " AND lvstosys.LIVE_ICOD = '" + livestock + "'";
    sql = sql + " AND feedgroups.group_cod = '" + group + "'";
    if (query.exec(sql))
    {
        if (query.first())
        {
            header1 = query.value(3).toString() + " - Purpose: " + query.value(1).toString();
            header1 = header1 + " - Breed: " + query.value(2).toString();
            header1 = header1 + " - Feeding group: " + query.value(0).toString();
        }
    }
}

void livestockproduction::parentViewActivated(const QModelIndex &index)
{
    QList<TmoduleFieldDef> keys;
    keys = m_parentModel->getRowKeyValues(index.row());
    constructHeader1(keys);
    if (keys.count() > 0)
    {
        m_harvestModel->clearItems();
        for (int pos =0; pos <= keys.count()-1;pos++)
        {
            if (keys[pos].code.toUpper() == "LIVE_ICOD")
            {
                loadProdandFora(keys[pos].value.toString());
                break;
            }
        }
        m_harvestModel->resetData();
        m_harvestModel->chkitems(keys);
        m_hvrstSortModel->sort(1);
        appyHarvestData = false;

        for (int rsize = 0; rsize <= m_hvrstSortModel->rowCount()-1;rsize++)
            ui->AdvListView5->setRowHeight(rsize,listViewRowSize);
    }
}

void livestockproduction::harvestingChanged()
{
    ui->AdvListView1->setEnabled(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn5->setEnabled(true);
    appyHarvestData = true;
}

void livestockproduction::on_BitBtn5_clicked()
{
    if (appyHarvestData)
    {
        QList<TmoduleFieldDef> keys;
        TmoduleFieldDef key;
        key.code = "LIVE_COD";
        key.value = currentLiveCod;
        keys.append(key);
        if (m_harvestModel->applyData(keys))
        {
            ui->BitBtn1->setEnabled(false);
            ui->BitBtn5->setEnabled(false);
            appyHarvestData = false;
            m_hvrstSortModel->sort(1);
            ui->AdvListView5->scrollToTop();
            ui->AdvListView1->setEnabled(true);
        }
    }
}

void livestockproduction::on_BitBtn1_clicked()
{
    if (appyHarvestData)
    {
        m_harvestModel->cancelData();
        m_hvrstSortModel->sort(1);
        ui->AdvListView5->scrollToTop();
        ui->BitBtn1->setEnabled(false);
        ui->BitBtn5->setEnabled(false);
        appyHarvestData = false;
        ui->AdvListView1->setEnabled(true);
    }
}

void livestockproduction::loadProdandFora(QString liveIcod)
{
    QSqlQuery lktable(dataBase);
    QString sql;

    sql = "SELECT live_cod FROM lvstosys WHERE ";
    sql = sql + " live_icod = '" + liveIcod + "'";
    sql = sql + " AND sys_cod = '" + currentSystem + "'";
    if (lktable.exec(sql))
    {
        if (lktable.first())
            currentLiveCod = lktable.value(0).toString();
    }
    else
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    sql = "SELECT products.prod_cod,products.prod_des FROM products,lvproducts,lvstosys WHERE ";
    sql = sql + "products.prod_cod = lvproducts.prod_cod AND ";
    sql = sql + "lvproducts.live_cod = lvstosys.live_cod AND ";
    sql = sql + "lvstosys.live_icod = '" + liveIcod + "'";
    sql = sql + " AND lvstosys.sys_cod = '" + currentSystem + "'";    
    if (lktable.exec(sql))
    {
        while(lktable.next())
        {
            m_harvestModel->insertItem("livehvst",lktable.value(0).toString(),
                                       lktable.value(1).toString());
        }
    }
    else
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

void livestockproduction::constructPorFPopUpMenu(QPoint mousepos, QModelIndex index)
{
    QModelIndex idx;
    idx = m_hvrstSortModel->mapToSource(index);
    if (idx.column() > 0)
    {
        if (((m_harvestModel->getPeriodValue(idx).toString() == "T") ||
             (m_harvestModel->getPeriodValue(idx).toString() == "Y")) &&
            (m_harvestModel->getCheckedStatusForRow(idx.row()) == true))
        {

            QMenu menu(this);
            menu.addAction(showYieldAct);
            yieldTable = m_harvestModel->getPeriodTable(idx.row());
            header2 = m_harvestModel->getPeriodDescription(idx);
            header2 = "Producing: " + header2;
            subModuleKeyFields = m_harvestModel->getPeriodKeyFields(idx);
            flabourTable = yieldTable;
            mlabourTable = "livehvstlabour";
            menu.addAction(showLabourAct);
            menu.exec(mousepos);
        }
    }
}

void livestockproduction::cropMenuRequested(QPoint mousepos)
{
    QModelIndex pos;
    pos = ui->AdvListView1->currentIndex();
    if (pos.isValid())
    {
        QMenu menu(this);
        subModuleKeyFields = m_parentModel->getRowKeyValues(pos.row());
        flabourTable = "feedgroups";
        mlabourTable = "feedgroupslabour";
        menu.addAction(showLabourAct);
        menu.exec(this->ui->AdvListView1->mapToGlobal(mousepos));
    }
}

void livestockproduction::createActions()
{
    showYieldAct = new QAction(tr("Yield allocation"), this);
    connect(showYieldAct,SIGNAL(triggered()),this,SLOT(loadYield()));
    showLabourAct = new QAction(tr("Labour allocation"), this);
    connect(showLabourAct,SIGNAL(triggered()),this,SLOT(loadLabour()));
}

void livestockproduction::loadYield()
{
    cropyield *yieldDialog = new cropyield(0);
    yieldDialog->setTableName(yieldTable);
    yieldDialog->setHeader2Description(header2);
    yieldDialog->setHeader1Description(header1);

    for (int pos =0; pos <= subModuleKeyFields.count()-1;pos++)
    {
        if (subModuleKeyFields[pos].code.toLower() != "nmonth")
            yieldDialog->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,true);
        else
            yieldDialog->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,false);
    }
    yieldDialog->setFieldsToTable(yieldTable);
    yieldDialog->disableField(yieldTable,"st");
    yieldDialog->disableField(yieldTable,"porst");
    yieldDialog->loadMaintenance(dataBase);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(yieldDialog);
    m_dialogWindow.setWindowTitle(tr("Yield allocation"));
    m_dialogWindow.exec();
}

void livestockproduction::loadLabour()
{
    labourallocation *labourDialog = new labourallocation(0);
    labourDialog->setTableName(mlabourTable);
    labourDialog->setHeader2Description(header2);
    labourDialog->setHeader1Description(header1);
    labourDialog->setModelTable(mlabourTable);
    labourDialog->setTableName(flabourTable);
    labourDialog->setFieldsToTable(flabourTable);
    labourDialog->setKeyFields(subModuleKeyFields);
    for (int pos =0; pos <= subModuleKeyFields.count()-1;pos++)
    {
        if (subModuleKeyFields[pos].code.toLower() != "nmonth")
            labourDialog->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,true);
        else
            labourDialog->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,false);
    }
    labourDialog->loadMaintenance(dataBase);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(labourDialog);
    m_dialogWindow.setWindowTitle(tr("Labour allocation"));
    m_dialogWindow.exec();
}




void livestockproduction::on_cmddetails_clicked()
{
    if (ui->AdvListView5->currentIndex().isValid())
        constructPorFPopUpMenu(QCursor::pos(),ui->AdvListView5->currentIndex());
}
