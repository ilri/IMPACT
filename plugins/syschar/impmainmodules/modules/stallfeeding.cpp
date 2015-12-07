#include "stallfeeding.h"
#include "ui_stallfeeding.h"
#include "delegates/imagecheckdelegate.h"
#include "delegates/fixcombodelegate.h"
#include "submodules/feedmngqty.h"
#include "submodules/feedingothprods.h"
#include "modulesubscreen.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <stdlib.h>
#include "moduleglobal.h"
#include <QMenu>


stallfeeding::stallfeeding(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::stallfeeding)
{
    ui->setupUi(this);


    connect(ui->BitBtn5,SIGNAL(clicked()),this,SLOT(applyProducts()));
    connect(ui->BitBtn1,SIGNAL(clicked()),this,SLOT(cancelProducts()));

    connect(ui->cmdothprods,SIGNAL(clicked()),this,SLOT(loadOtherProducts()));

    database = db;

    QSqlQuery sysinfo(database);
    QString sql;
    sql = "SELECT count(*) FROM periods ";
    sysinfo.exec(sql);
    sysinfo.first();
    numseasons = sysinfo.value(0).toInt();

    currentSystem = systemID;
    //currentSystem = "KE20061206BMZEMBMZ011";
    //currentSystem = "KE20061205BMZEMBMZCTB";

    m_feedGroupsModel = new fieldInColModel(this);
    m_feedGroupsModel->setDatabase(database);
    //m_feedGroupsModel->setImpactModule(this);
    m_feedGroupsModel->setTableName("feedgroups");



    m_feedGroupsModel->insertField("SYS_COD","Sys Cod","000","000",true,false,false);
    m_feedGroupsModel->insertField("LIVE_ICOD","Livestock","000","000",true,true,true);
    m_feedGroupsModel->insertField("GROUP_COD","Livestock","000","000",true,true,false);
    m_feedGroupsModel->insertField("GROUP_DES","Feeding group","HEA","HEA",false,false,true);

    m_feedGroupsModel->setReadOnly(true);

    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_feedGroupsModel->loadData(keys);
    ui->AdvListView1->setModel(m_feedGroupsModel);

    //m_feedGroupsModel->distributeColums();

    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadChildData(QModelIndex)));

    m_feedingModel = new feedingTableModel(this);
    //m_feedingModel->setImpactModule(this);

    m_feedingModel->setDatabase(database);

    QList<TmoduleFieldDef> masterKeys;
    //Add crop products
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "LIVE_ICOD";
    masterKeys.append(key);
    key.code = "GROUP_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);
    m_feedingModel->addMasterTable("crpprodmanfeed",masterKeys);
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);
    m_feedingModel->addLookUpTable("crpprodmanfeed","crpprodman","Crop Product",masterKeys);
    m_feedingModel->addYieldTable("crpprodmanfeed","crpprodmantim","YIELD","PORLF");

    //Add crop forages
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "LIVE_ICOD";
    masterKeys.append(key);
    key.code = "GROUP_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "FORA_COD";
    masterKeys.append(key);
    m_feedingModel->addMasterTable("crpforamanfeed",masterKeys);
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "FORA_COD";
    masterKeys.append(key);
    m_feedingModel->addLookUpTable("crpforamanfeed","crpforaman","Crop Forage",masterKeys);
    m_feedingModel->addYieldTable("crpforamanfeed","crpforamantim","YIELD","PORLF");

    //Pastures

    //Add Pastures products
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "LIVE_ICOD";
    masterKeys.append(key);
    key.code = "GROUP_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMPADD";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);
    m_feedingModel->addMasterTable("grassprodmanfeed",masterKeys);
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMPADD";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);
    m_feedingModel->addLookUpTable("grassprodmanfeed","grassprodman","Pasture Product",masterKeys);
    m_feedingModel->addYieldTable("grassprodmanfeed","grassprodmantim","YIELD","PORLF");

    //Add pasture forages
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "LIVE_ICOD";
    masterKeys.append(key);
    key.code = "GROUP_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMPADD";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "FORA_COD";
    masterKeys.append(key);
    m_feedingModel->addMasterTable("grassforamanfeed",masterKeys);
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMPADD";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "FORA_COD";
    masterKeys.append(key);
    m_feedingModel->addLookUpTable("grassforamanfeed","grassforaman","Crop Forage",masterKeys);
    m_feedingModel->addYieldTable("grassforamanfeed","grassforamantim","YIELD","PORLF");

    //Vegetables

    //Add vegetable products
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "LIVE_ICOD";
    masterKeys.append(key);
    key.code = "GROUP_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "VEG_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);
    m_feedingModel->addMasterTable("vegprodmanfeed",masterKeys);
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);
    m_feedingModel->addLookUpTable("vegprodmanfeed","vegprodman","Vegetable Product",masterKeys);
    m_feedingModel->addYieldTable("vegprodmanfeed","vegprodmantim","YIELD","PORLF");

    //Add vegetable forages
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "LIVE_ICOD";
    masterKeys.append(key);
    key.code = "GROUP_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "VEG_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "FORA_COD";
    masterKeys.append(key);
    m_feedingModel->addMasterTable("vegforamanfeed",masterKeys);
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "FORA_COD";
    masterKeys.append(key);
    m_feedingModel->addLookUpTable("vegforamanfeed","vegforaman","Vegetable Forage",masterKeys);
    m_feedingModel->addYieldTable("vegforamanfeed","vegforamantim","YIELD","PORLF");






    //Add purchased products
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "LIVE_ICOD";
    masterKeys.append(key);
    key.code = "GROUP_COD";
    masterKeys.append(key);
    key.code = "FEED_COD";
    masterKeys.append(key);
    m_feedingModel->addMasterTable("stallfeeprch",masterKeys);
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "FEED_COD";
    masterKeys.append(key);
    m_feedingModel->addLookUpTable("stallfeeprch","liveavailres","Purchased",masterKeys);
    m_feedingModel->addYieldTable("stallfeeprch","liveavailresperiods","QTY","NULL");

    //Add Livestock products
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "FLIVE_ICOD";
    masterKeys.append(key);
    key.code = "FGROUP_COD";
    masterKeys.append(key);
    key.code = "LIVE_ICOD";
    masterKeys.append(key);
    key.code = "GROUP_COD";
    masterKeys.append(key);
    key.code = "LIVE_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);
    m_feedingModel->addMasterTable("livehvstfeed",masterKeys);
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "LIVE_ICOD";
    masterKeys.append(key);
    key.code = "GROUP_COD";
    masterKeys.append(key);
    key.code = "LIVE_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);

    m_feedingModel->addLookUpTable("livehvstfeed","livehvst","Livestock product",masterKeys);
    m_feedingModel->addYieldTable("livehvstfeed","livehvsttim","YIELD","PORLF");

    showYieldAct = new QAction(tr("Quantity allocation"), this);
    connect(showYieldAct,SIGNAL(triggered()),this,SLOT(loadYield()));

    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    connect(ckhdelegate,SIGNAL(requestPopUpMenu(QPoint,QModelIndex)),this,SLOT(requestPopUpMenu(QPoint,QModelIndex)));
    //m_feedingModel->setYielddelegator(ckhdelegate,numseasons);

    connect(m_feedingModel,SIGNAL(modeldDataChanged()),this,SLOT(productsDataChanged()));
    //Loads the model
    masterKeys.clear();
    key.code = "SYS_COD";
    key.value = currentSystem;
    masterKeys.append(key);
    m_feedingModel->loadItems(masterKeys);
    ui->AdvListView2->setModel(m_feedingModel);

    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);

    if (m_feedGroupsModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_feedGroupsModel->index(0,0);
        loadChildData(start);
        start = m_feedGroupsModel->index(0,1);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
        setChildStatus(false);

    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();

    for (int rsize = 0; rsize <= m_feedGroupsModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

    for (int rsize = 0; rsize <= m_feedingModel->rowCount()-1;rsize++)
        ui->AdvListView2->setRowHeight(rsize,listViewRowSize);
}

stallfeeding::~stallfeeding()
{
    //qDebug() << "Before destroy stallfeeding UI";
    delete ui;
    //qDebug() << "After destroy stallfeeding UI";
}

void stallfeeding::loadAndroidEffects()
{
    charmUnits.activateOn(ui->AdvListView1);
    charmProds.activateOn(ui->AdvListView2);
}


void stallfeeding::setChildStatus(bool status)
{
    ui->AdvListView2->setEnabled(status);
    ui->BitBtn1->setEnabled(status);
    ui->BitBtn5->setEnabled(status);
}

void stallfeeding::constructCustomDelegator()
{   

    fixComboDelegate *pdelegate = new fixComboDelegate(this);
    pdelegate->setReadOnly(true);
    QSqlQuery livestock(database);
    QString sql;
    QString desc;
    sql = "SELECT lvstosys.live_icod,livestoc.LIVE_DES FROM lvstosys,livestoc";
    sql = sql + " WHERE lvstosys.LIVE_COD = livestoc.live_cod AND lvstosys.sys_cod = '" + currentSystem + "'";
    livestock.exec(sql);
    while(livestock.next())
    {
        desc = livestock.value(1).toString();
        pdelegate->insertItem(livestock.value(0).toString(),desc);
    }

    ui->AdvListView1->setItemDelegateForColumn(0,pdelegate);


    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->addIgnoredColumn(0);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate->setManagementModel(m_feedingModel);
    //ckhdelegate->setSortModel(m_actSortModel);
    //connect(ckhdelegate,SIGNAL(requestPopUpMenu(QPoint,QModelIndex)),this,SLOT(requestPopUpMenu(QPoint,QModelIndex)));

    int pos;
    for (pos = 0; pos <= m_feedingModel->columnCount()-1;pos++)
    {
        if (pos > 2)
        {
            ui->AdvListView2->setItemDelegateForColumn(pos,ckhdelegate);
        }
    }

    for (pos =0; pos <= m_feedingModel->columnCount()-1;pos++)
    {
        if (pos <= 2)
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

}

void stallfeeding::loadYield()
{
    feedmngqty *qtyassing = new feedmngqty(0);
    qtyassing->setTableName(yieldTable);
    for (int pos =0; pos <= subModuleKeyFields.count()-1;pos++)
    {
        if (subModuleKeyFields[pos].code.toLower() != "period_id")
            qtyassing->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,true);
        else
            qtyassing->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,false);
    }
    qtyassing->setFieldsToTable(yieldTable);
    qtyassing->loadMaintenance(database);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(qtyassing);
    m_dialogWindow.setWindowTitle(tr("Quantity allocation"));
    m_dialogWindow.exec();
}

void stallfeeding::requestPopUpMenu(QPoint pos,QModelIndex index)
{
    if (m_feedingModel->getCurrentStatusForRow(index.row()) == "C")
    {
        yieldTable = m_feedingModel->getMasterTable(index.row());
        yieldTable = yieldTable + "periods";
        yieldPeriod = index.column()-2;

        if (m_feedingModel->getPeriodValueForRow(index.row(),yieldPeriod) == "T")
        {
            subModuleKeyFields.clear();
            subModuleKeyFields.append(m_feedingModel->getLookUpFields(index));
            QModelIndex idx;
            idx = ui->AdvListView1->currentIndex();
            subModuleKeyFields.append(m_feedGroupsModel->getRowKeyValues(idx.row()));
            TmoduleFieldDef periodkey;
            periodkey.code = "period_id";
            periodkey.value = QString::number(yieldPeriod);
            subModuleKeyFields.append(periodkey);

            QMenu menu(this);
            menu.addAction(showYieldAct);
            menu.exec(pos);
        }
    }
}

void stallfeeding::setParentStatus(bool status)
{
    ui->AdvListView1->setEnabled(status);
}


void stallfeeding::productsDataChanged()
{
    setParentStatus(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn5->setEnabled(true);
}

void stallfeeding::loadChildData(QModelIndex index)
{
    if (index.isValid())
    {
        setChildStatus(true);
        ui->BitBtn1->setEnabled(false);
        ui->BitBtn5->setEnabled(false);
        if (ui->AdvListView2->isEnabled())
        {
            if (!index.isValid())
                return;
            if ((m_feedGroupsModel->getRowStatus(index.row()) != "N") ||
                (m_feedGroupsModel->getRowStatus(index.row()) != "D"))
            {
                m_feedingModel->chkitems(m_feedGroupsModel->getRowKeyValues(index.row()));
                //m_feedingModel->distributeColums();
            }
        }
    }
    else
    {
        setChildStatus(false);
    }
}

void stallfeeding::applyProducts()
{
    if (m_feedingModel->applyData())
    {
        setParentStatus(true);
        ui->BitBtn1->setEnabled(false);
        ui->BitBtn5->setEnabled(false);
    }
}

void stallfeeding::cancelProducts()
{
    m_feedingModel->cancelData();
    setParentStatus(true);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn5->setEnabled(false);
}

void stallfeeding::loadOtherProducts()
{
    feedingothprods *othfeedprods = new feedingothprods(0,database,currentSystem);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(othfeedprods);
    m_dialogWindow.setWindowTitle("Purchased products for manure management");
    m_dialogWindow.exec();
    if (othfeedprods->changes)
    {
        QList<TmoduleFieldDef> keys;
        TmoduleFieldDef key;
        key.code = "SYS_COD";
        key.value = currentSystem;
        keys.append(key);
        m_feedingModel->loadItems(keys);

        QModelIndex idx;
        idx = ui->AdvListView1->currentIndex();
        loadChildData(idx);
    }
}

void stallfeeding::on_cmddetails_clicked()
{
    if (ui->AdvListView2->currentIndex().isValid())
        requestPopUpMenu(QCursor::pos(),ui->AdvListView2->currentIndex());
}
