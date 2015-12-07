#include "cropplots.h"
#include "ui_cropplots.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/labeldelegate.h"
#include "delegates/imagecheckdelegate.h"
#include "delegates/seasondelegate.h"
#include "modulesubscreen.h"
#include "submodules/hhsotherinfo.h"
#include <qsqlquery.h>
#include <QSqlRecord>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>
#include <QMessageBox>

cropplots::cropplots(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::cropplots)
{
    ui->setupUi(this);
    dataBase = db;
    currentSystem = systemID;

    QSqlQuery ptable(dataBase);
    QString sql;
    sql = "SELECT count(period_id) FROM periods";
    ptable.exec(sql);
    nperiods = 0;
    if (ptable.first())
    {
        nperiods = ptable.value(0).toInt();
    }
    loadForm();
    ui->AdvListView1->setMouseTracking(true);
    if (modules_use_Android == true)
        loadAndroidEffects();
}

cropplots::~cropplots()
{
    //qDebug() << "Before destroy cropplots UI";
    delete ui;
    //qDebug() << "After destroy cropplots UI";
}

void cropplots::loadAndroidEffects()
{
    plotcharm.activateOn(ui->AdvListView1);
    seasoncharm.activateOn(ui->AdvListView2);
    cropcharm.activateOn(ui->AdvListView4);
    connect(ui->AdvListView4,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalculator(QModelIndex)));
}

void cropplots::loadForm()
{
    m_plotsModel = new fieldInColModel(this);
    m_plotsModel->setDatabase(dataBase);
    //m_plotsModel->setImpactModule(this);
    m_plotsModel->setTableName("spadstown");

    m_plotsModel->insertField("SYS_COD","System Code","000","000",true,false,false);
    m_plotsModel->insertField("PLOT_COD","Plot Code","000","000",true,false,false);
    m_plotsModel->insertField("PLOT_ID","Plot ID","000","000",false,false,true);
    m_plotsModel->insertField("TYPE","Type","000","000",false,false,true,true);
    m_plotsModel->insertField("AREA","Area","000","000",false,false,true);

    m_plotsModel->setReadOnly(true);
    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    key.code = "TYPE";
    key.value = "CROP";
    keys.append(key);
    m_plotsModel->loadData(keys);
    ui->AdvListView1->setModel(m_plotsModel);
    //m_plotsModel->distributeColums();
    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadSeasons(QModelIndex)));

    //Seasons model;
    m_seasonsModel = new fieldInColModel(this);
    m_seasonsModel->setDatabase(dataBase);
    //m_seasonsModel->setImpactModule(this);
    m_seasonsModel->setTableName("plotseasons");

    m_seasonsModel->insertField("SYS_COD","System Code","000","000",true,false,false);
    m_seasonsModel->insertField("PLOT_COD","Plot Code","000","000",true,false,false);
    m_seasonsModel->insertField("NUMSEASON","Season","000","000",true,true,true);
    m_seasonsModel->insertExtraColumn("TYPE","Type","NA","AS","NA","TD");
    m_seasonsModel->setMaxKeySize(2);
    ui->AdvListView2->setModel(m_seasonsModel);
    ui->AdvListView2->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->AdvListView2,SIGNAL(clicked(QModelIndex)),this,SLOT(loadCalendar(QModelIndex)));

    seasonDelegate *sdelegate = new seasonDelegate(this);
    sdelegate->setDatabase(dataBase);
    sdelegate->setSeasonModel(m_seasonsModel);
    ui->AdvListView2->setItemDelegateForColumn(1,sdelegate);

    //Calendar model;
    m_calendarModel = new cropsTableModel(this);
    m_calendarModel->setDatabase(dataBase);

    m_calendarSortModel = new cropsTableSortModel(this);
    m_calendarSortModel->setSourceModel(m_calendarModel);
    ui->AdvListView4->setModel(m_calendarSortModel);

    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate->setManagementModel(m_calendarModel);
    ckhdelegate->setSortModel(m_calendarSortModel);

    int pos;
    for (pos = 1; pos <= nperiods; pos++)
    {
        ui->AdvListView4->setItemDelegateForColumn(pos+1,ckhdelegate);
    }

    //qDebug() << listViewRowSize;

    for (pos =0; pos <= m_plotsModel->columnCount()-1;pos++)
    {
        if (pos <= 1)
            ui->AdvListView1->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView1->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    if (m_plotsModel->rowCount() > 0)
    {
        ui->AdvListView1->setCurrentIndex(m_plotsModel->index(0,0));
        loadSeasons(ui->AdvListView1->currentIndex());
    }
}


void cropplots::seasonsChanged()
{

}

void cropplots::loadSeasons(QModelIndex index)
{
    QList<TmoduleFieldDef> keys;
    keys.append(m_plotsModel->getRowKeyValues(index.row()));
    m_seasonsModel->loadData(keys);

    int pos;
    for (pos =0; pos <= m_seasonsModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    if (m_seasonsModel->rowCount() > 0)
    {
        ui->AdvListView2->setCurrentIndex(m_seasonsModel->index(0,0));
        loadCalendar(ui->AdvListView2->currentIndex());
    }
}

void cropplots::loadCalendar(QModelIndex index)
{
    QList<TmoduleFieldDef> keys;
    keys.append(m_seasonsModel->getRowKeyValues(index.row()));
    if ((getSeasonType(keys) == "ES") ||
        (getSeasonType(keys) == "SC") ||
        (getSeasonType(keys) == "INT"))
    {
        m_calendarModel->setIsVegOrchad(false);
        m_calendarModel->setTableName("owneduse");
    }
    else
    {
        TmoduleFieldDef vegeCrop;
        vegeCrop.code = "crop_cod";
        vegeCrop.value = "(V1";
        keys.append(vegeCrop);
        m_calendarModel->setIsVegOrchad(true);
        m_calendarModel->setTableName("ownusedv");
    }
    m_calendarModel->loadItems();
    m_calendarModel->chkitems(keys);
    m_calendarSortModel->sort(1);

    for (int pos = 0; pos <= m_calendarModel->columnCount()-1;pos++)
    {
        if ((pos == 0) || (pos == 1))
            ui->AdvListView4->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView4->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }



    for (int rsize = 0; rsize <= m_calendarModel->rowCount()-1;rsize++)
        ui->AdvListView4->setRowHeight(rsize,listViewRowSize);

}


QString cropplots::getSeasonType(QList<TmoduleFieldDef> keys)
{
    QSqlQuery tquery(dataBase);
    QString sql;
    int pos;
    //Check for crops
    sql = "SELECT COUNT(*) FROM owneduse WHERE ";
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        sql = sql + keys[pos].code + " = ";
        sql = sql + "'" + keys[pos].value.toString() + "' AND ";
    }
    sql = sql + "crop_cod not like '(V%'";

    if (tquery.exec(sql))
    {
        if (tquery.first())
        {
            if (tquery.value(0).toInt() == 1)
                return "SC";
            if (tquery.value(0).toInt() > 1)
                return "INT";
        }
    }
    else
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        return "Error!";
    }

    //Check for vegetables
    sql = "SELECT COUNT(*) FROM ownusedv WHERE ";
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        sql = sql + keys[pos].code + " = ";
        sql = sql + "'" + keys[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    if (tquery.exec(sql))
    {
        if (tquery.first())
        {
            if (tquery.value(0).toInt() > 0)
                return "VO";
        }
    }
    else
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        return "Error!";
    }


    //Empty vege orchard
    sql = "SELECT COUNT(*) FROM owneduse WHERE ";
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        sql = sql + keys[pos].code + " = ";
        sql = sql + "'" + keys[pos].value.toString() + "' AND ";
    }
    sql = sql + "crop_cod like '(V%'";

    if (tquery.exec(sql))
    {
        if (tquery.first())
        {
            if (tquery.value(0).toInt() > 0)
                return "EVO";
        }
    }
    else
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        return "Error!";
    }
    return "ES";
}

void cropplots::on_pushButton_clicked()
{
    QModelIndex index;
    index = ui->AdvListView2->currentIndex();
    if (index.isValid())
    {
        if (m_seasonsModel->getRowStatus(index.row()) == "C")
        {
            QList<TmoduleFieldDef> keys;
            keys.append(m_seasonsModel->getRowKeyValues(index.row()));
            if (getSeasonType(keys) == "ES")
            {

            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText(tr("You can only assign a vegetable orchad to an empty season"));
                msgBox.exec();
            }
        }
    }
}

void cropplots::on_BitBtn2_clicked()
{
    m_seasonsModel->insertNewItem();
}

void cropplots::on_BitBtn3_clicked()
{
    QModelIndex index;
    index = ui->AdvListView2->currentIndex();
    if (index.isValid())
    {
        m_seasonsModel->deleteItem(index.row());
    }
}

void cropplots::on_BitBtn4_clicked()
{
    if (m_seasonsModel->applyChanges())
    {

    }
}

void cropplots::on_BitBtn6_clicked()
{
    m_seasonsModel->cancelChanges();
}

void cropplots::on_BitBtn10_clicked()
{
    if (m_calendarModel->applyData())
    {

    }
}

void cropplots::on_BitBtn11_clicked()
{
    m_calendarModel->cancelData();
}


void cropplots::loadCalculator (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if (index.column() == 1)
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_calendarSortModel->setData(index,calc.getCurrentDisplay());
    }
}
