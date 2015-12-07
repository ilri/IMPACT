#include "grazingman.h"
#include "ui_grazingman.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>

grazingman::grazingman(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::grazingman)
{
    ui->setupUi(this);
    dataBase = db;
    currentSystem = systemID;

    QSqlQuery sysinfo(dataBase);
    QString sql;
    sql = "SELECT count(*) FROM periods ";
    sysinfo.exec(sql);
    sysinfo.first();
    numseasons = sysinfo.value(0).toInt();

    loadForm();
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

grazingman::~grazingman()
{
    //qDebug() << "Before destroy grazingman UI";
    delete ui;
    //qDebug() << "After destroy grazingman UI";
}

void grazingman::loadAndroidEffects()
{
    charmprod.activateOn(ui->AdvListView1);
    charmmarks.activateOn(ui->AdvListView2);
    charmdetail.activateOn(ui->AdvListView3);
    connect(ui->AdvListView3,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadDetCalc(QModelIndex)));
}

void grazingman::loadForm()
{

    m_paddocksModel = new fieldInColModel(this);
    m_paddocksModel->setDatabase(dataBase);
    //m_paddocksModel->setImpactModule(this);
    m_paddocksModel->setTableName("plotpaddspe");

    m_paddocksModel->insertField("SYS_COD","System Code","000","000",true,false,false);
    m_paddocksModel->insertField("PLOT_COD","Plot Code","000","000",true,false,true);
    m_paddocksModel->insertField("NUMPADD","Paddock","000","000",true,false,true);
    m_paddocksModel->insertField("CROP_COD","Species","000","000",true,false,true,true);
    m_paddocksModel->setReadOnly(true);
    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    key.code = "CUTCARRY";
    key.value = "N";
    keys.append(key);
    m_paddocksModel->loadData(keys);

    ui->AdvListView1->setModel(m_paddocksModel);
    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadFeedGData(QModelIndex)));
    //m_paddocksModel->distributeColums();

    //Feeding groups model

    m_groupsModel = new grazingTableModel(this);
    m_groupsSortModel = new grazingTableSortModel(this);
    m_groupsModel->setDatabase(dataBase);

    connect(m_groupsModel,SIGNAL(modelDataChanged()),this,SLOT(marketDataChanged()));
    connect(m_groupsModel,SIGNAL(afterInsert(QList<TmoduleInsertedKeys>)),this,SLOT(afterApply(QList<TmoduleInsertedKeys>)));

    m_groupsModel->loadItems(currentSystem);
    m_groupsSortModel->setSourceModel(m_groupsModel);
    ui->AdvListView2->setModel(m_groupsSortModel);
    connect(ui->AdvListView2,SIGNAL(clicked(QModelIndex)),this,SLOT(loadCalendarData(QModelIndex)));


    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);



    m_periodDataModel = new periodTableModel(this);
    m_periodDataModel->setDatabase(dataBase);
    //m_periodDataModel->setImpactModule(this);

    m_periodDataModel->setTableName("grazingmanperiods");
    m_periodDataModel->addField("tim","Timing","N","");
    m_periodDataModel->addField("rot","Grazing periods (days)","0","");
    m_periodDataModel->addField("ncd","Restinf period (days)","0","");
    m_periodDataModel->addField("pia","Instant availability (kg DM/paddock)","0","");
    m_periodDataModel->addField("mnr","Manure deposition (kg DM/paddock)","0","");
    connect(m_periodDataModel,SIGNAL(modeldDataChanged()),this,SLOT(calendarDataChanged()));
    ui->AdvListView3->setModel(m_periodDataModel);

   if (m_paddocksModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_paddocksModel->index(0,0);
        loadFeedGData(start);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
   {
        setMarketStatus(false);
        setCalendarStatus(false);
    }

   for (int rsize = 0; rsize <= m_paddocksModel->rowCount()-1;rsize++)
       ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

}

void grazingman::constructCustomDelegator()
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
    ui->AdvListView1->setItemDelegateForColumn(2,cdelegate);

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate->addIgnoredColumn(0);
    ui->AdvListView3->setItemDelegateForRow(0,ckhdelegate);

    int pos;
    for (pos =0; pos <= m_periodDataModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView3->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView3->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

}


void grazingman::loadFeedGData(QModelIndex index)
{
    if (!index.isValid())
        return;

    m_groupsModel->checkItems(m_paddocksModel->getRowKeyValues(index.row()));
    m_groupsSortModel->sort(1);

    if (m_groupsSortModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_groupsSortModel->index(0,0);
        loadCalendarData(start);
        ui->AdvListView2->setCurrentIndex(start);

        for (int rsize = 0; rsize <= m_groupsSortModel->rowCount()-1;rsize++)
            ui->AdvListView2->setRowHeight(rsize,listViewRowSize);
    }
    else
    {
        setCalendarStatus(false);
    }
}

void grazingman::loadCalendarData(QModelIndex index)
{
    QModelIndex idx;
    idx = m_groupsSortModel->mapToSource(index);
    if (!idx.isValid())
        return;
    if ((m_groupsModel->getItemStatus(idx.row()) != "N") &&
        (m_groupsModel->getItemStatus(idx.row()) != "D"))
    {
        m_periodDataModel->loadData(m_groupsModel->getRowKeyValues(idx.row()));
        //m_periodDataModel->distributeColums();
        ui->AdvListView3->setEnabled(true);
    }
    else
    {
        m_periodDataModel->loadData(m_groupsModel->getRowKeyValues(idx.row()));
        //m_periodDataModel->distributeColums();
        ui->AdvListView3->setEnabled(false);
    }

    for (int rsize = 0; rsize <= m_periodDataModel->rowCount()-1;rsize++)
        ui->AdvListView3->setRowHeight(rsize,listViewRowSize);

}

void grazingman::setProductsStatus(bool status)
{
    ui->AdvListView1->setEnabled(status);
}

void grazingman::setCalendarStatus(bool status)
{
    ui->AdvListView3->setEnabled(status);
    ui->BitBtn1->setEnabled(status);
    ui->BitBtn2->setEnabled(status);
}

void grazingman::setMarketStatus(bool status)
{
    ui->AdvListView2->setEnabled(status);
    ui->BitBtn3->setEnabled(status);
    ui->BitBtn4->setEnabled(status);
}

void grazingman::calendarDataChanged()
{
    setProductsStatus(false);
    setMarketStatus(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn2->setEnabled(true);
}

void grazingman::marketDataChanged()
{
   setProductsStatus(false);
   ui->BitBtn3->setEnabled(true);
   ui->BitBtn4->setEnabled(true);
}

void grazingman::afterApply(QList<TmoduleInsertedKeys> insertedKeys)
{
    QSqlQuery table(dataBase);
    QString sql;
    int pos;
    int pos2;
    int periods;
    for (pos =0; pos <= insertedKeys.count()-1;pos++)
    {
        for (periods = 1; periods <= numseasons; periods++)
        {
            sql = "INSERT INTO grazingmanperiods (";
            for (pos2 = 0; pos2 <= insertedKeys[pos].keys.count()-1;pos2++)
            {
                sql = sql + insertedKeys[pos].keys[pos2].code + ",";
            }
            sql = sql + "period_id) VALUES (";
            for (pos2 = 0; pos2 <= insertedKeys[pos].keys.count()-1;pos2++)
            {
                sql = sql + "'" +insertedKeys[pos].keys[pos2].value.toString() + "',";
            }
            sql = sql + "'" + QString::number(periods) + "'";
            sql = sql + ")";
            if (!table.exec(sql))
            {
                qWarning() << "Error: " << table.lastError().databaseText();
                qWarning() << "Executing: " << sql;
            }
        }
    }
}

void grazingman::on_BitBtn3_clicked()
{
    if (m_groupsModel->applyChanges())
    {
        setProductsStatus(true);
        ui->BitBtn3->setEnabled(false);
        ui->BitBtn4->setEnabled(false);
        ui->BitBtn1->setEnabled(false);
        ui->BitBtn2->setEnabled(false);
        QModelIndex idx;
        idx = ui->AdvListView2->currentIndex();
        loadCalendarData(idx);
    }
}

void grazingman::on_BitBtn4_clicked()
{
    m_groupsModel->cancelChanges();
    setProductsStatus(true);
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn4->setEnabled(false);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
    QModelIndex idx;
    idx = ui->AdvListView2->currentIndex();
    loadCalendarData(idx);
}

void grazingman::on_BitBtn1_clicked()
{
    m_periodDataModel->applyData();
    setProductsStatus(true);
    setMarketStatus(true);
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn4->setEnabled(false);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
}

void grazingman::on_BitBtn2_clicked()
{
    m_periodDataModel->cancelData();
    setProductsStatus(true);
    setMarketStatus(true);
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn4->setEnabled(false);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
}


void grazingman::loadDetCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = false;
    colOk = false;

    if (index.column() > 0)
        colOk = true;

    if (index.row() > 0)
        rowOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(dataBase);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_periodDataModel->setData(index,calc.getCurrentDisplay());
    }
}
