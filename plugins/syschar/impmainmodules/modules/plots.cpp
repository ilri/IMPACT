#include "plots.h"
#include "ui_plots.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/labeldelegate.h"
#include "modulesubscreen.h"
#include "submodules/plotssoil.h"
#include "modulesubscreen.h"
#include "submodules/plotsotherinfo.h"
#include "submodules/plotsgps.h"
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>

plots::plots(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::plots)
{
    ui->setupUi(this);
    dataBase = db;
    currentSystem = systemID;
    loadForm();
    ui->tableView->setMouseTracking(true);
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();

    qDebug() << "Some";
}

plots::~plots()
{
    //qDebug() << "Before destroy plots UI";
    delete ui;
    //qDebug() << "After destroy plots UI";
}

void plots::loadAndroidEffects()
{
    charm.activateOn(ui->tableView);
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalc(QModelIndex)));
}

void plots::constructCustomDelegator()
{

    fixComboDelegate *ldelegate = new fixComboDelegate(this);
    ldelegate->insertItem("CROP","Crops");
    ldelegate->insertItem("PASTURE","Pasture");
    ui->tableView->setItemDelegateForColumn(4,ldelegate);


    fixComboDelegate *pdelegate = new fixComboDelegate(this);
    pdelegate->insertItem("OWNED","Owned");
    pdelegate->insertItem("RENTED","Rented");
    pdelegate->insertItem("COMMUNAL","Communal");
    ui->tableView->setItemDelegateForColumn(3,pdelegate);



    labelDelegate *hdelegate = new labelDelegate(this);
    hdelegate->insertItem("CS","Click to edit");
    hdelegate->insertItem("AS","Apply to edit");
    hdelegate->insertItem("TD","To be deleted");
    hdelegate->addNotDialogValue("AS");
    hdelegate->addNotDialogValue("TD");
    connect(hdelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadSoil(QModelIndex)));
    ui->tableView->setItemDelegateForColumn(5,hdelegate);



    labelDelegate *cdelegate = new labelDelegate(this);
    cdelegate->insertItem("CS","Click to edit");
    cdelegate->insertItem("AS","Apply to edit");
    cdelegate->insertItem("TD","To be deleted");
    cdelegate->addNotDialogValue("AS");
    cdelegate->addNotDialogValue("TD");
    connect(cdelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadExtra(QModelIndex)));
    ui->tableView->setItemDelegateForColumn(7,cdelegate);


    labelDelegate *rdelegate = new labelDelegate(this);
    rdelegate->insertItem("CS","Click to edit");
    rdelegate->insertItem("AS","Apply to edit");
    rdelegate->insertItem("TD","To be deleted");
    rdelegate->addNotDialogValue("AS");
    rdelegate->addNotDialogValue("TD");
    connect(rdelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadGPS(QModelIndex)));
    ui->tableView->setItemDelegateForColumn(6,rdelegate);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
}

void plots::loadForm()
{
    m_colModel = new fieldInColModel(this);
    m_colModel->setDatabase(dataBase);
    //m_colModel->setImpactModule(this);
    m_colModel->setTableName("spadstown");


    m_colModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_colModel->insertField("PLOT_COD","Plot ID","000","000",true,true,false);

    m_colModel->insertField("PLOT_ID","Plot ID","000","000",false,false,true);
    m_colModel->insertField("AREA","Area\n(km)","0","0",false,false,true);
    m_colModel->insertField("DISTHH","Dist. to \n Hld. (km)","0","0",false,false,true);
    m_colModel->insertField("STATUS","Land\nTenure","000","OWNED",false,false,true,true);
    m_colModel->insertField("TYPE","Crop/ \nPasture","000","CROP",false,false,true,true);
    m_colModel->insertExtraColumn("SOIL","Soil \ninformation","CS","AS","AS","TD");
    m_colModel->insertExtraColumn("GPS","GPS information \n(optional)","CS","AS","AS","TD");
    m_colModel->insertExtraColumn("OPT","Optional \ndata","CS","AS","AS","TD");

    connect(m_colModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(dataChanged(QModelIndex,QModelIndex)));
    connect(ui->cmdapply,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->cmdcancel,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->cmdnew,SIGNAL(clicked()),this,SLOT(insertItem()));
    connect(ui->cmddelete,SIGNAL(clicked()),this,SLOT(deleteItem()));
    connect(m_colModel,SIGNAL(modeldDataChanged()),this,SLOT(valueChanged()));
    connect(m_colModel,SIGNAL(afterInsert(int)),this,SLOT(valueInserted(int)));



    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_colModel->loadData(keys);
    ui->tableView->setModel(m_colModel);

    //m_colModel->distributeColums();

    for (int rsize = 0; rsize <= m_colModel->rowCount()-1;rsize++)
        ui->tableView->setRowHeight(rsize,listViewRowSize);
}

void plots::dataChanged(const QModelIndex &, const QModelIndex &)
{
    //m_colModel->distributeColums();
}

void plots::deleteItem()
{
    if (ui->tableView->currentIndex().isValid())
    {
        m_colModel->deleteItem(ui->tableView->currentIndex().row());
    }
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void plots::valueInserted(int row)
{
    QString value;
    value = m_colModel->getDataFromField(row,"PLOT_COD").toString();
    m_colModel->setDataToField(row,"PLOT_ID",value);
}

void plots::insertItem()
{
    m_colModel->insertNewItem();
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void plots::applyChanges()
{
    if (m_colModel->applyChanges())
    {
        ui->cmdapply->setEnabled(false);
        ui->cmdcancel->setEnabled(false);
    }
}

void plots::cancelChanges()
{
    m_colModel->cancelChanges();
    ui->cmdapply->setEnabled(false);
    ui->cmdcancel->setEnabled(false);
}
void plots::valueChanged()
{
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}


void plots::loadSoil(QModelIndex index)
{
    if ((m_colModel->getRowStatus(index.row()) != "N") ||
        (m_colModel->getRowStatus(index.row()) != "D"))
    {
        plotssoil *soil = new plotssoil(0);
        soil->setTableName("spadstown");
        for (int pos=0; pos <= m_colModel->getRowKeyValues(index.row()).count()-1;pos++)
        {
            if (m_colModel->getRowKeyValues(index.row())[pos].code.toUpper() == "PLOT_COD")
                soil->insertKeyField(m_colModel->getRowKeyValues(index.row())[pos].code,
                                     m_colModel->getRowKeyValues(index.row())[pos].value,false);
            else
                soil->insertKeyField(m_colModel->getRowKeyValues(index.row())[pos].code,
                                     m_colModel->getRowKeyValues(index.row())[pos].value);

        }
        soil->loadMaintenance(dataBase);
        moduleSubScreen m_dialogWindow;
        m_dialogWindow.loadSubScreen(soil);
        m_dialogWindow.setWindowTitle(tr("Soil information"));
        m_dialogWindow.exec();        
    }
}

void plots::loadGPS(QModelIndex index)
{
    if ((m_colModel->getRowStatus(index.row()) != "N") ||
        (m_colModel->getRowStatus(index.row()) != "D"))
    {
        plotsgps *gpsinfo = new plotsgps(0);
        gpsinfo->setKeyValues(m_colModel->getRowKeyValues(index.row()));

        gpsinfo->loadModel(dataBase);
        moduleSubScreen m_dialogWindow;
        m_dialogWindow.loadSubScreen(gpsinfo);
        m_dialogWindow.exec();        
    }
}

void plots::loadExtra(QModelIndex index)
{
    if ((m_colModel->getRowStatus(index.row()) != "N") ||
        (m_colModel->getRowStatus(index.row()) != "D"))
    {
        plotsotherinfo *othinfo = new plotsotherinfo(0);
        othinfo->setTableName("spadstown");
        for (int pos=0; pos <= m_colModel->getRowKeyValues(index.row()).count()-1;pos++)
        {
            if (m_colModel->getRowKeyValues(index.row())[pos].code.toUpper() == "PLOT_COD")
                othinfo->insertKeyField(m_colModel->getRowKeyValues(index.row())[pos].code,
                                     m_colModel->getRowKeyValues(index.row())[pos].value,false);
            else
                othinfo->insertKeyField(m_colModel->getRowKeyValues(index.row())[pos].code,
                                     m_colModel->getRowKeyValues(index.row())[pos].value);

        }
        othinfo->loadMaintenance(dataBase);
        moduleSubScreen m_dialogWindow;
        m_dialogWindow.loadSubScreen(othinfo);
        m_dialogWindow.exec();        
    }
}

void plots::loadCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if ((index.column() > 0) && (index.column() < 3))
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(dataBase);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_colModel->setData(index,calc.getCurrentDisplay());
    }
}
