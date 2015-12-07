#include "grassplots.h"
#include "ui_grassplots.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/labeldelegate.h"
#include "delegates/imagecheckdelegate.h"
#include "delegates/seasondelegate.h"
#include "modulesubscreen.h"
#include "submodules/hhsotherinfo.h"
#include <qsqlquery.h>
#include <QSqlRecord>
#include <QSqlError>
#include "moduleglobal.h"
#include <QDebug>

grassplots::grassplots(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::grassplots)
{
    ui->setupUi(this);
    dataBase = db;
    currentSystem = systemID;


    loadForm();
    ui->AdvListView1->setMouseTracking(true);

    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

grassplots::~grassplots()
{
    //qDebug() << "Before destroy grassplots UI";
    delete ui;
    //qDebug() << "After destroy grassplots UI";
}

void grassplots::loadAndroidEffects()
{
    charmprod.activateOn(ui->AdvListView1);
    charmmarks.activateOn(ui->AdvListView2);
}

void grassplots::constructCustomDelegator()
{


    fixComboDelegate *cdelegate = new fixComboDelegate(this);
    QSqlQuery plots(dataBase);
    QString sql;
    QString desc;
    sql = "SELECT CROP_COD,CROP_DES FROM pastures";
    plots.exec(sql);
    while(plots.next())
    {
        desc = plots.value(1).toString();
        cdelegate->insertItem(plots.value(0).toString(),desc);
    }

    ui->AdvListView2->setItemDelegateForColumn(1,cdelegate);

    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ui->AdvListView2->setItemDelegateForColumn(3,ckhdelegate);

    labelDelegate *hdelegate = new labelDelegate(this);
    hdelegate->insertItem("CS","Click to edit");
    hdelegate->insertItem("AS","Apply to edit");
    hdelegate->insertItem("TD","To be deleted");
    hdelegate->addNotDialogValue("AS");
    hdelegate->addNotDialogValue("TD");
    hdelegate->addNoDrawValue("NA");
    connect(hdelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadExtra(QModelIndex)));
    ui->AdvListView2->setItemDelegateForColumn(4,hdelegate);

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

}

void grassplots::loadForm()
{
    m_plotsModel = new fieldInColModel(this);
    m_plotsModel->setDatabase(dataBase);
    //m_plotsModel->setImpactModule(this);
    m_plotsModel->setTableName("spadstown");

    m_plotsModel->insertField("SYS_COD","System Code","000","000",true,false,false);
    m_plotsModel->insertField("PLOT_COD","Plot Code","000","000",true,false,false);
    m_plotsModel->insertField("PLOT_ID","Plot ID","000","000",false,false,true);
    m_plotsModel->insertField("AREA","Area","000","000",false,false,true);
    m_plotsModel->insertField("STATUS","Type","000","000",false,false,true,true);
    m_plotsModel->setReadOnly(true);
    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    key.code = "TYPE";
    key.value = "PASTURE";
    keys.append(key);
    m_plotsModel->loadData(keys);
    ui->AdvListView1->setModel(m_plotsModel);
    //m_plotsModel->distributeColums();
    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadPaddocks(QModelIndex)));

    //Seasons model;
    m_seasonsModel = new fieldInColModel(this);
    m_seasonsModel->setDatabase(dataBase);
    //m_seasonsModel->setImpactModule(this);
    m_seasonsModel->setTableName("plotpaddspe");

    //m_seasonsModel->setImpactModule(this);
    m_seasonsModel->insertField("SYS_COD","System Code","000","000",true,false,false);
    m_seasonsModel->insertField("PLOT_COD","Plot Code","000","000",true,false,false);
    m_seasonsModel->insertField("NUMPADD","Paddock","000","000",true,true,true);
    m_seasonsModel->insertField("CROP_COD","Species","001","001",true,false,true,true);
    m_seasonsModel->insertField("LANDCOVER","Land cover \n (%)","0","0",false,false,true,true);
    m_seasonsModel->insertField("CUTCARRY","Cut and carry","N","N",false,false,true,true);
    m_seasonsModel->insertExtraColumn("BOT","Botanical \n composition","CS","AS","AS","TD");
    m_seasonsModel->setMaxKeySize(2);
    m_seasonsModel->setBeforeInsertTable("plotpaddoks");
    m_seasonsModel->addBeforeInsertIgnoreField("CROP_COD");
    m_seasonsModel->addBeforeInsertIgnoreField("CUTCARRY");
    ui->AdvListView2->setModel(m_seasonsModel);
    ui->AdvListView2->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(m_seasonsModel,SIGNAL(afterDelete(QList<TmoduleInsertedKeys>)),this,SLOT(afterDelete(QList<TmoduleInsertedKeys>)));

    for (int rsize = 0; rsize <= m_plotsModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

    if (m_plotsModel->rowCount() > 0)
    {
        ui->AdvListView1->setCurrentIndex(m_plotsModel->index(0,0));
        loadPaddocks(ui->AdvListView1->currentIndex());
    }
}

void grassplots::loadPaddocks(QModelIndex index)
{
    QList<TmoduleFieldDef> keys;
    keys.append(m_plotsModel->getRowKeyValues(index.row()));
    m_seasonsModel->loadData(keys);

    for (int rsize = 0; rsize <= m_seasonsModel->rowCount()-1;rsize++)
        ui->AdvListView2->setRowHeight(rsize,listViewRowSize);

    //m_seasonsModel->distributeColums();
}

void grassplots::loadExtra(QModelIndex /*index*/)
{

}

void grassplots::on_BitBtn2_clicked()
{
    m_seasonsModel->insertNewItem();
}

void grassplots::on_BitBtn3_clicked()
{
    QModelIndex index;
    index = ui->AdvListView2->currentIndex();
    if (index.isValid())
    {
        m_seasonsModel->deleteItem(index.row());
    }
}

void grassplots::afterDelete(QList<TmoduleInsertedKeys> keys)
{
    QSqlQuery deltable(dataBase);
    QString sql;

    int pos;
    int pos2;
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        sql = "DELETE FROM plotpaddoks WHERE ";
        for (pos2 = 0; pos2 <= keys[pos].keys.count()-1;pos2++)
        {
            if (keys[pos].keys[pos2].code.toUpper() != "CROP_COD")
            {
                sql = sql + keys[pos].keys[pos2].code + " = ";
                sql = sql + "'" + keys[pos].keys[pos2].value.toString() + "' AND ";
            }
        }
        sql = sql.left(sql.length()-5);
        if (!deltable.exec(sql))
        {
            qWarning() << "Error: " << deltable.lastError().databaseText();
            qWarning() << "Executing : " << sql;
        }
    }
}

void grassplots::on_BitBtn4_clicked()
{
    if (m_seasonsModel->applyChanges())
    {

    }
}

void grassplots::on_BitBtn6_clicked()
{
    m_seasonsModel->cancelChanges();
}

