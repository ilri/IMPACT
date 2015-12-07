#include "grassharvest.h"
#include "ui_grassharvest.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qpixmap.h>
#include "submodules/cropyield.h"
#include "submodules/labourallocation.h"
#include "submodules/fertiliserallocation.h"
#include "submodules/inputsallocation.h"
#include "submodules/manureallocation.h"
#include "modulesubscreen.h"
#include "moduleglobal.h"
#include <QMenu>


grassManagementModel::grassManagementModel(QObject *parent)
    :managementModel(parent)
{

}

void grassManagementModel::beforeInsert(QString table, QList<TmoduleFieldDef> keyFields)
{
    if ((table.toLower() == "grassprodman") || (table.toLower() == "grassforaman"))
    {
        QString sql;
        QSqlQuery mtable(database);
        int pos;
        sql = "INSERT INTO grassman (";
        for (pos = 0; pos <= keyFields.count()-1;pos++)
        {
            sql = sql + keyFields[pos].code + ",";
        }
        sql = sql.left(sql.length()-1) + ") VALUES (";
        for (pos = 0; pos <= keyFields.count()-1;pos++)
        {
            sql = sql + "'" + keyFields[pos].value.toString() + "',";
        }
        sql = sql.left(sql.length()-1) + ")";
        mtable.exec(sql);
    }
}

//**********************************************************************

grassharvest::grassharvest(QWidget *parent,QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::grassharvest)
{
    dataBase = db;
    currentSystem = systemID;
    ui->setupUi(this);
    loadForm();
    createActions();
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

grassharvest::~grassharvest()
{
    //qDebug() << "Before destroy grassman UI";
    delete ui;
    //qDebug() << "After destroy grassman UI";
}

void grassharvest::loadAndroidEffects()
{
    cropcharm.activateOn(ui->AdvListView1);
    actcharm.activateOn(ui->AdvListView5);
}

void grassharvest::loadForm()
{
    connect(ui->AdvListView1,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(cropMenuRequested(QPoint)));

    ui->AdvListView1->setContextMenuPolicy(Qt::CustomContextMenu);

    //Creates and load the parent model
    m_parentModel = new fieldInColModel(this);
    m_parentModel->setReadOnly(true);
    m_parentModel->setDatabase(dataBase);
    //m_parentModel->setImpactModule(this);
    m_parentModel->setTableName("plotpaddspe");


    m_parentModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_parentModel->insertField("PLOT_COD","Plot ID","000","000",true,false,true,true);
    m_parentModel->insertField("NUMPADD","Paddock","000","000",true,false,true);
    m_parentModel->insertField("CROP_COD","Pasture (only cut and carry)","000","000",true,false,true,true);
    m_parentModel->setWhereClause(" cutcarry = 'Y'");

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
    key.code = "PLOT_COD";
    cpmankeys.append(key);
    key.code = "NUMPADD";
    cpmankeys.append(key);
    key.code = "CROP_COD";
    cpmankeys.append(key);
    key.code = "ACTY_COD";
    cpmankeys.append(key);
    //Load the harvest model
    m_harvestModel = new grassManagementModel(this);
    connect(m_harvestModel,SIGNAL(modeldDataChanged()),this,SLOT(harvestingChanged()));


    m_hvrstSortModel = new managementSortModel(this);

    m_harvestModel->setDatabase(dataBase);
    //m_harvestModel->setImpactModule(this);

    m_harvestModel->setlkUpColDescription("Products / Forages");

    //Add the harvesting of products
    m_harvestModel->addMasterTable("grassprodman");
    m_harvestModel->setKeysFields("grassprodman",cpmankeys);
    m_harvestModel->setLookUpLinkedField("grassprodman","PROD_COD");
    m_harvestModel->setPeriodTable("grassprodman","grassprodmantim");
    key.code = "PROD_COD";
    cpmankeys.append(key);
    m_harvestModel->setPeriodKeyFields("grassprodman",cpmankeys);
    m_harvestModel->setPeriodField("grassprodman","NMONTH");
    m_harvestModel->setPeriodStatusField("grassprodman","CHECKED");
    m_harvestModel->setPeriodDefaultValue("grassprodman","N");

    // We don't use a standard lookup table as the products change
    // between crops
    m_harvestModel->setlkUpUnChkColor("grassprodman",QColor(255,222,222));
    m_harvestModel->setlkUpChkColor("grassprodman",QColor(255,191,191));
    m_harvestModel->setlkUpColOrder("grassprodman","1P");
    cpmankeys.removeLast();
    //Add the harvesting of forages
    m_harvestModel->addMasterTable("grassforaman");
    m_harvestModel->setKeysFields("grassforaman",cpmankeys);
    m_harvestModel->setLookUpLinkedField("grassforaman","FORA_COD");
    m_harvestModel->setPeriodTable("grassforaman","grassforamantim");

    key.code = "FORA_COD";
    cpmankeys.append(key);
    m_harvestModel->setPeriodKeyFields("grassforaman",cpmankeys);
    m_harvestModel->setPeriodField("grassforaman","NMONTH");
    m_harvestModel->setPeriodStatusField("grassforaman","CHECKED");
    m_harvestModel->setPeriodDefaultValue("grassforaman","N");

    // We don't use a standard lookup table as the products change
    // between crops
    m_harvestModel->setlkUpUnChkColor("grassforaman",QColor(222,255,222));
    m_harvestModel->setlkUpChkColor("grassforaman",QColor(191,255,191));
    m_harvestModel->setlkUpColOrder("grassforaman","2F");

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

void grassharvest::constructCustomDelegator()
{

    fixComboDelegate *pdelegate = new fixComboDelegate(this);
    pdelegate->setReadOnly(true);
    QSqlQuery plots(dataBase);
    QString sql;
    QString desc;
    sql = "SELECT PLOT_COD,PLOT_ID,AREA FROM spadstown WHERE sys_cod = '" + currentSystem + "'";
    plots.exec(sql);
    while(plots.next())
    {
        desc = plots.value(1).toString() + " (" + plots.value(2).toString() + "ha)";
        pdelegate->insertItem(plots.value(0).toString(),desc);
    }
    ui->AdvListView1->setItemDelegateForColumn(0,pdelegate);


    fixComboDelegate *cdelegate = new fixComboDelegate(this);
    cdelegate->setReadOnly(true);
    sql = "SELECT CROP_COD,CROP_DES FROM pastures";
    plots.exec(sql);
    while(plots.next())
    {
        desc = plots.value(1).toString();
        cdelegate->insertItem(plots.value(0).toString(),desc);
    }
    ui->AdvListView1->setItemDelegateForColumn(2,cdelegate);


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
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

}

void grassharvest::constructHeader1(QList<TmoduleFieldDef> keys)
{
    header1 = "";
    QString plot = "";
    QString season = "";
    QString paddock = "";
    QString crop = "";
    QString vege = "";
    QSqlQuery query(dataBase);
    QString sql;
    int pos;
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        if (keys[pos].code.toUpper() == "PLOT_COD")
            plot = keys[pos].value.toString();
        if (keys[pos].code.toUpper() == "NUMSEASON")
            season = keys[pos].value.toString();
        if (keys[pos].code.toUpper() == "NUMPADD")
            paddock = keys[pos].value.toString();
        if (keys[pos].code.toUpper() == "CROP_COD")
            crop = keys[pos].value.toString();
        if (keys[pos].code.toUpper() == "VEG_COD")
            vege = keys[pos].value.toString();
    }
    sql = "SELECT PLOT_ID,AREA FROM spadstown WHERE sys_cod = '" + currentSystem + "'";
    sql = sql + " AND PLOT_COD = '" + plot + "'";
    if (query.exec(sql))
    {
        if (query.first())
        {
            header1 = "Plot: " + query.value(0).toString() + "(";
            header1 = header1 + query.value(1).toString() + " ha)";
        }
    }
    if (!vege.isEmpty())
    {
        header1 = header1 + " Season: " + season;
        sql = "SELECT CROP_COD,CROP_DES FROM crop WHERE CROP_COD = '" + vege + "'";
        if (query.exec(sql))
        {
            if (query.first())
            {
                header1 = header1 + " Vegetable: " + query.value(1).toString();
            }
        }
        return;
    }
    if (!paddock.isEmpty())
    {
        header1 = header1 + " Paddock: " + paddock;
        sql = "SELECT CROP_COD,CROP_DES FROM pastures WHERE CROP_COD = '" + crop + "'";
        if (query.exec(sql))
        {
            if (query.first())
            {
                header1 = header1 + " Crop: " + query.value(1).toString();
            }
        }
        return;
    }
    header1 = header1 + " Season: " + season;
    sql = "SELECT CROP_COD,CROP_DES FROM crop WHERE CROP_COD = '" + crop + "'";
    if (query.exec(sql))
    {
        if (query.first())
        {
            header1 = header1 + " Crop: " + query.value(1).toString();
        }
    }
}

void grassharvest::parentViewActivated(const QModelIndex &index)
{
    QList<TmoduleFieldDef> keys;
    keys = m_parentModel->getRowKeyValues(index.row());
    constructHeader1(keys);
    //qWarning() << keys.count();
    if (keys.count() > 0)
    {
        m_harvestModel->clearItems();
        for (int pos =0; pos <= keys.count()-1;pos++)
        {
            if (keys[pos].code.toUpper() == "CROP_COD")
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



void grassharvest::harvestingChanged()
{
    ui->AdvListView1->setEnabled(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn5->setEnabled(true);
    appyHarvestData = true;
}

void grassharvest::on_BitBtn5_clicked()
{
    if (appyHarvestData)
    {
        QList<TmoduleFieldDef> keys;
        TmoduleFieldDef key;
        key.code = "ACTY_COD";
        key.value = "006";
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

void grassharvest::on_BitBtn1_clicked()
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

void grassharvest::loadProdandFora(QString crop)
{
    QSqlQuery lktable(dataBase);
    QString sql;

    sql = "SELECT cpprods.prod_cod,cpprods.prod_des FROM cpprods,pastprods WHERE ";
    sql = sql + "cpprods.prod_cod = pastprods.prod_cod AND ";
    sql = sql + "pastprods.crop_cod = '" + crop + "'";
    lktable.exec(sql);
    while(lktable.next())
    {
        m_harvestModel->insertItem("grassprodman",lktable.value(0).toString(),
                                   lktable.value(1).toString());
    }

    sql = "SELECT forages.fora_cod,forages.fora_des FROM forages,pastfora WHERE ";
    sql = sql + "forages.fora_cod = pastfora.fora_cod AND ";
    sql = sql + "pastfora.crop_cod = '" + crop + "'";
    lktable.exec(sql);
    while(lktable.next())
    {
        m_harvestModel->insertItem("grassforaman",lktable.value(0).toString(),
                                   lktable.value(1).toString());
    }

}

void grassharvest::constructPorFPopUpMenu(QPoint mousepos, QModelIndex index)
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
            header2 = "Harvesting: " + header2;
            subModuleKeyFields = m_harvestModel->getPeriodKeyFields(idx);
            flabourTable = yieldTable;
            if (yieldTable.contains("fora",Qt::CaseInsensitive))
                mlabourTable = "grassforalabour";
            else
                mlabourTable = "grassprodlabour";
            menu.addAction(showLabourAct);            
            menu.exec(mousepos);
        }
    }
}

void grassharvest::cropMenuRequested(QPoint mousepos)
{
    QModelIndex pos;
    pos = ui->AdvListView1->currentIndex();
    if (pos.isValid())
    {
        QMenu menu(this);
        subModuleKeyFields = m_parentModel->getRowKeyValues(pos.row());
        flabourTable = "plotpaddspe";
        mlabourTable = "plotpaddspelabour";
        menu.addAction(showLabourAct);
        menu.exec(this->ui->AdvListView1->mapToGlobal(mousepos));
    }
}

void grassharvest::createActions()
{
    showYieldAct = new QAction(tr("Yield allocation"), this);
    connect(showYieldAct,SIGNAL(triggered()),this,SLOT(loadYield()));
    showLabourAct = new QAction(tr("Labour allocation"), this);
    connect(showLabourAct,SIGNAL(triggered()),this,SLOT(loadLabour()));
}

void grassharvest::loadYield()
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
    yieldDialog->loadMaintenance(dataBase);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(yieldDialog);
    m_dialogWindow.setWindowTitle(tr("Yield allocation"));
    m_dialogWindow.exec();
}

void grassharvest::loadLabour()
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

//void grassharvest::beforeInsert(QString table, QList<TmoduleFieldDef> keyFields)
//{

//}

void grassharvest::on_cmddetails_clicked()
{
    if (ui->AdvListView5->currentIndex().isValid())
        constructPorFPopUpMenu(QCursor::pos(),ui->AdvListView5->currentIndex());
}
