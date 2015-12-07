#include "livestockman.h"
#include "ui_livestockman.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qpixmap.h>
#include "submodules/cropyield.h"
#include "submodules/labourallocation.h"
#include "modulesubscreen.h"
#include "moduleglobal.h"
#include <QMenu>

livestockman::livestockman(QWidget *parent,QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::livestockman)
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

livestockman::~livestockman()
{
    //qDebug() << "Before destroy livestockman UI";
    delete ui;
    //qDebug() << "After destroy livestockman UI";
}

void livestockman::loadAndroidEffects()
{
    cropcharm.activateOn(ui->AdvListView1);
    actcharm.activateOn(ui->AdvListView4);
}

void livestockman::loadForm()
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


    //Creates and loads activity model
    m_activityModel = new managementModel(this);
    m_actSortModel = new managementSortModel(this);
    connect(m_activityModel,SIGNAL(modeldDataChanged()),this,SLOT(activityChanged()));


    m_activityModel->setDatabase(dataBase);
    //m_activityModel->setImpactModule(this);

    QList<TmoduleFieldDef> cpmankeys;
    key.code = "SYS_COD";
    cpmankeys.append(key);
    key.code = "LIVE_ICOD";
    cpmankeys.append(key);
    key.code = "GROUP_COD";
    cpmankeys.append(key);


    m_activityModel->addMasterTable("livemangrp");
    m_activityModel->setKeysFields("livemangrp",cpmankeys);
    m_activityModel->setLookUpLinkedField("livemangrp","ACTY_COD");
    m_activityModel->setPeriodTable("livemangrp","livemangrptim");
    key.code = "ACTY_COD";
    cpmankeys.append(key);
    m_activityModel->setPeriodKeyFields("livemangrp",cpmankeys);
    m_activityModel->setPeriodField("livemangrp","NMONTH");
    m_activityModel->setPeriodStatusField("livemangrp","CHECKED");
    m_activityModel->setPeriodDefaultValue("livemangrp","N");



    m_activityModel->setlkUpTable("livemangrp","activity");
    m_activityModel->setlkUpCodeFields("livemangrp","ACTY_COD");
    m_activityModel->setlkUpDescFields("livemangrp","ACTY_DES");
    m_activityModel->setlkUpWhereClause("livemangrp"," ACTY_TYP = 'L'");
    m_activityModel->setlkUpColDescription("Activities");
    m_activityModel->setlkUpUnChkColor("livemangrp",QColor(230,255,255));
    m_activityModel->setlkUpChkColor("livemangrp",QColor(199,255,255));

    m_actSortModel->setSourceModel(m_activityModel);
    m_activityModel->loadItems();
    m_actSortModel->sort(1);
    ui->AdvListView4->setModel(m_actSortModel);
    //m_activityModel->distributeColums();

    if (m_parentModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_parentModel->index(0,0);
        parentViewActivated(start);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
    {
        ui->AdvListView4->setEnabled(false);
    }

    ui->AdvListView4->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->AdvListView4,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(actMenuRequested(QPoint)));

    for (int rsize = 0; rsize <= m_parentModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

}

void livestockman::constructCustomDelegator()
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
    ckhdelegate->setManagementModel(m_activityModel);
    ckhdelegate->setSortModel(m_actSortModel);
    connect(ckhdelegate,SIGNAL(requestPopUpMenu(QPoint,QModelIndex)),this,SLOT(constructActPopUpMenu(QPoint,QModelIndex)));


    int pos;
    for (pos = 0; pos <= m_actSortModel->columnCount()-1;pos++)
    {
        if (pos > 0)
        {
            ui->AdvListView4->setItemDelegateForColumn(pos,ckhdelegate);
        }
    }

    for (pos =0; pos <= m_actSortModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView4->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView4->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

}

void livestockman::constructHeader1(QList<TmoduleFieldDef> keys)
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

void livestockman::parentViewActivated(const QModelIndex &index)
{
    QList<TmoduleFieldDef> keys;
    keys = m_parentModel->getRowKeyValues(index.row());
    constructHeader1(keys);
    if (keys.count() > 0)
    {
        m_activityModel->chkitems(keys);
        m_actSortModel->sort(1);
        applyAcyData = false;


        for (int rsize = 0; rsize <= m_actSortModel->rowCount()-1;rsize++)
            ui->AdvListView4->setRowHeight(rsize,listViewRowSize);
    }
}

void livestockman::activityChanged()
{
    ui->AdvListView1->setEnabled(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn5->setEnabled(true);
    applyAcyData = true;
}

void livestockman::on_BitBtn5_clicked()
{
    if (applyAcyData)
    {
        if (m_activityModel->applyData())
        {
            ui->BitBtn1->setEnabled(false);
            ui->BitBtn5->setEnabled(false);
            applyAcyData = false;
            m_actSortModel->sort(1);
            ui->AdvListView4->scrollToTop();
            ui->AdvListView1->setEnabled(true);
        }
    }
}

void livestockman::on_BitBtn1_clicked()
{
    if (applyAcyData)
    {
        m_activityModel->cancelData();
        m_actSortModel->sort(1);
        ui->BitBtn1->setEnabled(false);
        ui->BitBtn5->setEnabled(false);
        applyAcyData = false;
        ui->AdvListView1->setEnabled(true);
    }
}

void livestockman::constructActPopUpMenu(QPoint mousepos, QModelIndex index)
{
    QModelIndex idx;
    idx = m_actSortModel->mapToSource(index);

    if (idx.column() == 0)
    {
        if (m_activityModel->getCheckedStatusForRow(idx.row()) == true)
        {
            QMenu menu(this);
            subModuleKeyFields = m_activityModel->getMasterKeyFields(idx.row());
            flabourTable = m_activityModel->getMasterTable(idx.row());
            mlabourTable = "livemangrplabourB";
            menu.addAction(showLabourAct);
            menu.exec(mousepos);
        }
    }
    else
    {
        if (((m_activityModel->getPeriodValue(idx).toString() == "T") ||
             (m_activityModel->getPeriodValue(idx).toString() == "Y")) &&
            (m_activityModel->getCheckedStatusForRow(idx.row()) == true))
        {
            QMenu menu(this);
            header2 = m_activityModel->getPeriodDescription(idx);
            header2 = "Activity: " + header2;

            subModuleKeyFields = m_activityModel->getPeriodKeyFields(idx);
            flabourTable = m_activityModel->getPeriodTable(idx.row());
            mlabourTable = "livemangrplabour";
            menu.addAction(showLabourAct);            
            menu.exec(mousepos);
        }
    }
}

void livestockman::cropMenuRequested(QPoint mousepos)
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

void livestockman::actMenuRequested(QPoint mousepos)
{
    //qWarning() << "In here!";
    QModelIndex pos;
    pos = ui->AdvListView4->indexAt(mousepos);
    if (pos.isValid())
    {
        //qWarning() << "Is valid!";
        pos = m_actSortModel->mapToSource(pos);
        if (pos.column() == 0)
        {
            //qWarning() << "is 0";
            if (m_activityModel->getCheckedStatusForRow(pos.row()) == true)
            {
                QMenu menu(this);
                header2 = "Activity: " + pos.data().toString();
                subModuleKeyFields.clear();
                subModuleKeyFields.append(m_activityModel->getItemKeyFields(pos.row()));
                flabourTable = "livemangrp";
                mlabourTable = "livemangrplabourB";
                menu.addAction(showLabourAct);
                menu.exec(ui->AdvListView4->mapToGlobal(mousepos));
            }
        }
    }
}

void livestockman::createActions()
{
    showYieldAct = new QAction(tr("Yield allocation"), this);
    connect(showYieldAct,SIGNAL(triggered()),this,SLOT(loadYield()));
    showLabourAct = new QAction(tr("Labour allocation"), this);
    connect(showLabourAct,SIGNAL(triggered()),this,SLOT(loadLabour()));
}

void livestockman::loadYield()
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

void livestockman::loadLabour()
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




void livestockman::on_cmddetails_clicked()
{
    if (ui->AdvListView4->currentIndex().isValid())
        constructActPopUpMenu(QCursor::pos(),ui->AdvListView4->currentIndex());
}
