#include "manureman.h"
#include "ui_manureman.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/labeldelegate.h"
#include "submodules/mnrmannkpc.h"
#include "submodules/mnrmanyieldalloc.h"
#include "submodules/mnrmanothprods.h"
#include "submodules/mnrmanyielddsp.h"
#include "modulesubscreen.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <stdlib.h>
#include <calcdialog.h>
#include "moduleglobal.h"


manureman::manureman(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::manureman)
{
    ui->setupUi(this);

    connect(ui->BitBtn4,SIGNAL(clicked()),this,SLOT(addUnit()));
    connect(ui->BitBtn5,SIGNAL(clicked()),this,SLOT(deleteUnit()));
    connect(ui->BitBtn6,SIGNAL(clicked()),this,SLOT(applyUnits()));
    connect(ui->BitBtn7,SIGNAL(clicked()),this,SLOT(cancelUnits()));
    connect(ui->BitBtn1,SIGNAL(clicked()),this,SLOT(applyProducts()));
    connect(ui->BitBtn2,SIGNAL(clicked()),this,SLOT(cancelProducts()));

    database = db;
    currentSystem = systemID;
    //currentSystem = "KE20061206BMZEMBMZ011";

    m_heapModel = new fieldInColModel(this);
    m_heapModel->setDatabase(database);
    //m_heapModel->setImpactModule(this);
    m_heapModel->setTableName("mnrheap");


    m_heapModel->insertExtraColumn("STS","  ","","","","");
    m_heapModel->insertField("SYS_COD","Sys Cod","000","000",true,false,false);
    m_heapModel->insertField("HEAP_COD","Unit","000","000",true,true,true);
    m_heapModel->insertField("HEAP_TYP","Type","HEA","HEA",false,false,true,true);
    m_heapModel->insertField("QTYASS","Quantity \n (kg)","0","0",false,false,true);
    m_heapModel->insertField("PERCLOSS","Store loss \n (%)","0","0",false,false,true);
    m_heapModel->insertField("YIELD","Yield \n (kg)","0","0",false,false,true);
    m_heapModel->insertExtraColumn("YLDUSES","Yield \n uses","CS","AS","AS","TD");
    m_heapModel->insertExtraColumn("MINERAL","Mineral \n composition","CS","AS","AS","TD");
    m_heapModel->addReadOnlyField("STS","ALL");
    m_heapModel->addReadOnlyField("HEAP_COD","ALL");
    m_heapModel->addReadOnlyField("QTYASS","ALL");
    m_heapModel->addReadOnlyField("YIELD","ALL");
    connect(m_heapModel,SIGNAL(modeldDataChanged()),this,SLOT(unitsDataChanged()));
    connect(m_heapModel,SIGNAL(modeldDataChanged(int,int)),this,SLOT(unitsDataChanged(int,int)));



    m_heapModel->addDistributionException(0);

    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_heapModel->loadData(keys);
    ui->AdvListView1->setModel(m_heapModel);

    //m_heapModel->distributeColums();

    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadChildData(QModelIndex)));


    m_mnrprodsModel = new mnrManTableModel(this);
    //m_mnrprodsModel->setImpactModule(this);

    m_mnrprodsModel->setDatabase(database);
    QList<TmoduleFieldDef> masterKeys;

    //Add crop products
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "HEAP_COD";
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
    m_mnrprodsModel->addMasterTable("mnrheapcrpprod",masterKeys);
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
    m_mnrprodsModel->addLookUpTable("mnrheapcrpprod","crpprodman","Crop Product",masterKeys);
    m_mnrprodsModel->addYieldTable("mnrheapcrpprod","crpprodmantim","YIELD","PORMM");

    //Add crop forages
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "HEAP_COD";
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
    m_mnrprodsModel->addMasterTable("mnrheapcrpfora",masterKeys);
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
    m_mnrprodsModel->addLookUpTable("mnrheapcrpfora","crpforaman","Crop Forage",masterKeys);
    m_mnrprodsModel->addYieldTable("mnrheapcrpfora","crpforamantim","YIELD","PORMM");

    //Pastures

    //Add Pastures products
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "HEAP_COD";
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
    m_mnrprodsModel->addMasterTable("mnrheapgrassprod",masterKeys);
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
    m_mnrprodsModel->addLookUpTable("mnrheapgrassprod","grassprodman","Pasture Product",masterKeys);
    m_mnrprodsModel->addYieldTable("mnrheapgrassprod","grassprodmantim","YIELD","PORMM");

    //Add pasture forages
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "HEAP_COD";
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
    m_mnrprodsModel->addMasterTable("mnrheapgrassfora",masterKeys);
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
    m_mnrprodsModel->addLookUpTable("mnrheapgrassfora","grassforaman","Crop Forage",masterKeys);
    m_mnrprodsModel->addYieldTable("mnrheapgrassfora","grassforamantim","YIELD","PORMM");

    //Vegetables

    //Add vegetable products
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "HEAP_COD";
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
    m_mnrprodsModel->addMasterTable("mnrheapvegprod",masterKeys);
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
    m_mnrprodsModel->addLookUpTable("mnrheapvegprod","vegprodman","Vegetable Product",masterKeys);
    m_mnrprodsModel->addYieldTable("mnrheapvegprod","vegprodmantim","YIELD","PORMM");

    //Add pasture forages
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "HEAP_COD";
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
    m_mnrprodsModel->addMasterTable("mnrheapvegfora",masterKeys);
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
    m_mnrprodsModel->addLookUpTable("mnrheapvegfora","vegforaman","Vegetable Forage",masterKeys);
    m_mnrprodsModel->addYieldTable("mnrheapvegfora","vegforamantim","YIELD","PORMM");


    //Add purchased products
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "HEAP_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);
    m_mnrprodsModel->addMasterTable("mnrheapaddprods",masterKeys);
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);
    m_mnrprodsModel->addLookUpTable("mnrheapaddprods","mnrheapprods","Purchased",masterKeys);
    m_mnrprodsModel->addYieldTable("mnrheapaddprods","mnrheapprodsperiods","QTY","NULL");

    //Add Livestock products
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "HEAP_COD";
    masterKeys.append(key);
    key.code = "LIVE_ICOD";
    masterKeys.append(key);
    key.code = "GROUP_COD";
    masterKeys.append(key);
    key.code = "LIVE_COD";
    masterKeys.append(key);
    key.code = "PROD_COD";
    masterKeys.append(key);
    m_mnrprodsModel->addMasterTable("mnrheaplvprod",masterKeys);
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



    m_mnrprodsModel->addLookUpTable("mnrheaplvprod","livehvst","Livestock product",masterKeys);
    m_mnrprodsModel->addYieldTable("mnrheaplvprod","livehvsttim","YIELD","PORMM");

    labelDelegate *ldelegate = new labelDelegate(this);
    ldelegate->insertItem("CE","DblClick to edit");
    ldelegate->insertItem("AE","Apply to edit");
    ldelegate->insertItem("TD","To be deleted");
    ldelegate->addNotDialogValue("AE");
    ldelegate->addNotDialogValue("TD");
    ldelegate->addNoDrawValue("NA");
    connect(ldelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadNPKC(QModelIndex)));
    //m_mnrprodsModel->setNKPCdelegator(ldelegate);

    labelDelegate *ydelegate = new labelDelegate(this);
    ydelegate->insertItem("CV","DblClick to view");
    ydelegate->insertItem("AV","Apply to view");
    ydelegate->insertItem("TD","To be deleted");
    ydelegate->addNotDialogValue("AV");
    ydelegate->addNotDialogValue("TD");
    ydelegate->addNoDrawValue("NA");
    connect(ydelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadYield(QModelIndex)));
    //m_mnrprodsModel->setYielddelegator(ydelegate);


    connect(m_mnrprodsModel,SIGNAL(modeldDataChanged()),this,SLOT(productsDataChanged()));
    //Loads the model
    masterKeys.clear();
    key.code = "SYS_COD";
    key.value = currentSystem;
    masterKeys.append(key);
    m_mnrprodsModel->loadItems(masterKeys);
    ui->AdvListView2->setModel(m_mnrprodsModel);


    if (m_heapModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_heapModel->index(0,0);
        loadChildData(start);
        start = m_heapModel->index(0,2);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
        setChildStatus(false);

    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();

    for (int rsize = 0; rsize <= m_heapModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);
}

void manureman::loadAndroidEffects()
{
    charmUnits.activateOn(ui->AdvListView1);
    charmProds.activateOn(ui->AdvListView2);
    connect(ui->AdvListView1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadUnitsCalc(QModelIndex)));
    connect(ui->AdvListView2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadProdsCalc(QModelIndex)));
}

manureman::~manureman()
{
    //qDebug() << "Before destroy manureman UI";
    delete ui;
    //qDebug() << "After destroy manureman UI";
}


void manureman::constructCustomDelegator()
{

    fixComboDelegate *ldelegate = new fixComboDelegate(this);
    ldelegate->insertItem("HEA","Heap");
    ldelegate->insertItem("PIT","Pit");
    ldelegate->insertItem("INS","In situ");
    ldelegate->insertItem("OTH","Other");

    ui->AdvListView1->setItemDelegateForColumn(2,ldelegate);

    labelDelegate *pdelegate = new labelDelegate(this);
    pdelegate->insertItem("CS","Dblclick to edit");
    pdelegate->insertItem("AS","Apply to edit");
    pdelegate->insertItem("TD","To be deleted");
    pdelegate->addNotDialogValue("AS");
    pdelegate->addNotDialogValue("TD");
    connect(pdelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadYieldAlloc(QModelIndex)));

    ui->AdvListView1->setItemDelegateForColumn(6,pdelegate);


    labelDelegate *sdelegate = new labelDelegate(this);
    sdelegate->insertItem("CS","Dblclick to edit");
    sdelegate->insertItem("AS","Apply to edit");
    sdelegate->insertItem("TD","To be deleted");
    sdelegate->addNotDialogValue("AS");
    sdelegate->addNotDialogValue("TD");
    connect(sdelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadNPKC(QModelIndex)));
    ui->AdvListView1->setItemDelegateForColumn(7,sdelegate);

    labelDelegate *vdelegate = new labelDelegate(this);
    vdelegate->insertItem("CE","Dblclick to edit");
    vdelegate->insertItem("AE","Apply to edit");
    vdelegate->insertItem("TD","To be deleted");
    vdelegate->addNotDialogValue("AE");
    vdelegate->addNotDialogValue("TD");
    connect(vdelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadNPKCAlloc(QModelIndex)));
    ui->AdvListView2->setItemDelegateForColumn(2,vdelegate);


    labelDelegate *xdelegate = new labelDelegate(this);
    xdelegate->insertItem("CV","Dblclick to view");
    xdelegate->insertItem("AV","Apply to edit");
    xdelegate->insertItem("TD","To be deleted");
    xdelegate->addNotDialogValue("AV");
    xdelegate->addNotDialogValue("TD");
    connect(xdelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadYield(QModelIndex)));
    ui->AdvListView2->setItemDelegateForColumn(6,xdelegate);

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);

    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);


}



void manureman::setChildStatus(bool status)
{
    ui->BitBtn3->setEnabled(status);
    ui->BitBtn1->setEnabled(status);
    ui->BitBtn2->setEnabled(status);
    ui->AdvListView2->setEnabled(status);
}

void manureman::setParentStatus(bool status)
{
    ui->BitBtn4->setEnabled(status);
    ui->BitBtn5->setEnabled(status);
    ui->BitBtn6->setEnabled(status);
    ui->BitBtn7->setEnabled(status);
    ui->AdvListView1->setEnabled(status);
}

void manureman::unitsDataChanged(int row, int column)
{
    if (column == 4)
    {
        float qtyas;
        qtyas = m_heapModel->getDataFromField(row,"QTYASS").toFloat();
        float loss;
        loss = m_heapModel->getDataFromField(row,"PERCLOSS").toFloat();
        float yield;
        yield= qtyas - ((loss/100) * qtyas);
        m_heapModel->setDataToField(row,"YIELD",QString::number(yield));
    }
}

void manureman::unitsDataChanged()
{
    setChildStatus(false);
    ui->BitBtn6->setEnabled(true);
    ui->BitBtn7->setEnabled(true);
}

void manureman::productsDataChanged()
{
    setParentStatus(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn2->setEnabled(true);
    ui->BitBtn3->setEnabled(false);
}

void manureman::loadChildData(QModelIndex index)
{
    if (ui->AdvListView2->isEnabled())
    {
        if (!index.isValid())
            return;
        if ((m_heapModel->getRowStatus(index.row()) != "N") ||
            (m_heapModel->getRowStatus(index.row()) != "D"))
        {
            m_mnrprodsModel->chkitems(m_heapModel->getRowKeyValues(index.row()));
            //m_mnrprodsModel->distributeColums();
        }

        for (int rsize = 0; rsize <= m_mnrprodsModel->rowCount()-1;rsize++)
            ui->AdvListView2->setRowHeight(rsize,listViewRowSize);
    }
}

void manureman::loadNPKC(QModelIndex index)
{
    if (m_mnrprodsModel->getCurrentStatusForRow(index.row()) != "D")
    {
        mnrmannkpc *nkpc = new mnrmannkpc(0);
        nkpc->setTableName(m_mnrprodsModel->getLookUpTable(index.row()));
        nkpc->setFieldsToTable(m_mnrprodsModel->getLookUpTable(index.row()));

        for (int pos=0; pos <= m_mnrprodsModel->getRowKeyValues(index.row()).count()-1;pos++)
        {
            nkpc->insertKeyField(m_mnrprodsModel->getRowKeyValues(index.row())[pos].code,
                                 m_mnrprodsModel->getRowKeyValues(index.row())[pos].value);
        }
        nkpc->loadMaintenance(database);
        moduleSubScreen m_dialogWindow;
        m_dialogWindow.loadSubScreen(nkpc);
        m_dialogWindow.setWindowTitle("Nutrient information");
        m_dialogWindow.exec();
    }
}

void manureman::loadYield(QModelIndex index)
{
    if (index.isValid())
    {
        if (m_mnrprodsModel->getCurrentStatusForRow(index.row()) != "D")
        {
            mnrmanyielddsp yieldalloc;
            yieldalloc.setDataBase(database);
            yieldalloc.setWindowTitle("Yield distribution");
            if (m_mnrprodsModel->getLookUpTable(index.row()) != "mnrheapprods")
                yieldalloc.loadData(m_mnrprodsModel->getLookUpTable(index.row()) + "tim",m_mnrprodsModel->getRowKeyValues(index.row()));
            else
                yieldalloc.loadData(m_mnrprodsModel->getLookUpTable(index.row()) + "periods",m_mnrprodsModel->getRowKeyValues(index.row()));

            yieldalloc.exec();
            /* mnrmanyielddsp *yieldalloc = new mnrmanyielddsp(0);

            if (m_mnrprodsModel->getLookUpTable(index.row()) != "mnrheapprods")
            {
                yieldalloc->setTableName(m_mnrprodsModel->getLookUpTable(index.row()) + "tim");
                yieldalloc->setFieldName("yield");
            }
            else
            {
                yieldalloc->setTableName(m_mnrprodsModel->getLookUpTable(index.row()) + "periods");
                yieldalloc->setFieldName("qty");
            }
            QList <TmoduleFieldDef> keys;
            keys = m_mnrprodsModel->getRowKeyValues(index.row());
            if (m_mnrprodsModel->getLookUpTable(index.row()) != "mnrheapprods")
            {
                for (int pos = 0; pos <= keys.count()-1;pos++)
                {
                    if (keys[pos].code == "period_id")
                        keys[pos].code = "nmonth";
                }
            }
            yieldalloc->setKeysFields(keys);
            yieldalloc->loadDistro(database);
            moduleSubScreen m_dialogWindow;
            m_dialogWindow.loadSubScreen(yieldalloc);
            m_dialogWindow.setWindowTitle("Yield distribution");
            m_dialogWindow.exec();*/
        }
    }
}

void manureman::loadYieldAlloc(QModelIndex index)
{
    if (m_heapModel->getRowStatus(index.row()) != "D")
    {
        mnrmanyieldalloc *yield = new mnrmanyieldalloc();


        for (int pos=0; pos <= m_heapModel->getRowKeyValues(index.row()).count()-1;pos++)
        {
            yield->insertKeyField(m_heapModel->getRowKeyValues(index.row())[pos].code,
                                 m_heapModel->getRowKeyValues(index.row())[pos].value);
        }
        yield->loadMaintenance(database);
        moduleSubScreen m_dialogWindow;
        m_dialogWindow.loadSubScreen(yield);
        m_dialogWindow.setWindowTitle("Yield allocation");
        m_dialogWindow.exec();
    }
}

void manureman::loadNPKCAlloc(QModelIndex index)
{
    if (m_heapModel->getRowStatus(index.row()) != "D")
    {
        mnrmannkpc *nkpc = new mnrmannkpc(0);
        nkpc->setTableName("mnrheap");
        nkpc->setFieldsToTable("mnrheap");

        for (int pos=0; pos <= m_heapModel->getRowKeyValues(index.row()).count()-1;pos++)
        {
            nkpc->insertKeyField(m_heapModel->getRowKeyValues(index.row())[pos].code,
                                 m_heapModel->getRowKeyValues(index.row())[pos].value);
        }
        nkpc->loadMaintenance(database);
        moduleSubScreen m_dialogWindow;
        m_dialogWindow.loadSubScreen(nkpc);
        m_dialogWindow.setWindowTitle("Nutrient information");
        m_dialogWindow.exec();
    }
}

void manureman::applyUnits()
{
    if (m_heapModel->applyChanges())
    {
        setChildStatus(true);
        ui->BitBtn6->setEnabled(false);
        ui->BitBtn7->setEnabled(false);
        ui->BitBtn1->setEnabled(false);
        ui->BitBtn2->setEnabled(false);
        ui->BitBtn3->setEnabled(true);
        QModelIndex index;
        index = ui->AdvListView1->currentIndex();
        loadChildData(index);
    }
}

void manureman::cancelUnits()
{
    m_heapModel->cancelChanges();
    setChildStatus(true);
    ui->BitBtn6->setEnabled(false);
    ui->BitBtn7->setEnabled(false);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
    ui->BitBtn3->setEnabled(true);
    QModelIndex index;
    index = ui->AdvListView1->currentIndex();
    loadChildData(index);
}

void manureman::addUnit()
{
    m_heapModel->insertNewItem();
}

void manureman::deleteUnit()
{
    QModelIndex index;
    index = ui->AdvListView1->currentIndex();
    if (index.isValid())
    {
        m_heapModel->deleteItem(index.row());
    }
}


void manureman::applyProducts()
{
    if (m_mnrprodsModel->applyData())
    {
        QModelIndex index;
        index = ui->AdvListView1->currentIndex();
        float total;
        total = m_mnrprodsModel->getTotalUsage();
        m_heapModel->setDataToField(index.row(),"QTYASS",total);
        float loss;
        loss = m_heapModel->getDataFromField(index.row(),"PERCLOSS").toFloat();
        total = total - (loss/100) * total;
        m_heapModel->setDataToField(index.row(),"YIELD",total);
        applyUnits();

        setParentStatus(true);
        ui->BitBtn6->setEnabled(false);
        ui->BitBtn7->setEnabled(false);
        ui->BitBtn3->setEnabled(true);
        ui->BitBtn1->setEnabled(false);
        ui->BitBtn2->setEnabled(false);
    }
}

void manureman::cancelProducts()
{
    m_mnrprodsModel->cancelData();
    setParentStatus(true);
    ui->BitBtn6->setEnabled(false);
    ui->BitBtn7->setEnabled(false);
    ui->BitBtn3->setEnabled(true);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
}

void manureman::on_BitBtn3_clicked()
{
    mnrmanothprods *otherprods = new mnrmanothprods(0,database,currentSystem);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(otherprods);
    m_dialogWindow.setWindowTitle("Purchased products for manure management");
    m_dialogWindow.exec();
    if (otherprods->changes)
    {
        QList<TmoduleFieldDef> keys;
        TmoduleFieldDef key;
        key.code = "SYS_COD";
        key.value = currentSystem;
        keys.append(key);
        m_mnrprodsModel->loadItems(keys);

        QModelIndex idx;
        idx = ui->AdvListView1->currentIndex();
        loadChildData(idx);
    }
}

void manureman::loadUnitsCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if (index.column() == 4)
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(database);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_heapModel->setData(index,calc.getCurrentDisplay());
    }
}

void manureman::loadProdsCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if (index.column() == 4)
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(database);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_mnrprodsModel->setData(index,calc.getCurrentDisplay());
    }
}
