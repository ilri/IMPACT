#include "hhconslive.h"
#include "ui_hhconslive.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>

hhconslive::hhconslive(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::hhconslive)
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

hhconslive::~hhconslive()
{
    //qDebug() << "Before destroy hhconslive UI";
    delete ui;
    //qDebug() << "After destroy hhconslive UI";
}

void hhconslive::loadAndroidEffects()
{
    charmprod.activateOn(ui->AdvListView1);
    charmdetail.activateOn(ui->AdvListView3);
    connect(ui->AdvListView1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadProdCalc(QModelIndex)));
    connect(ui->AdvListView3,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadDetCalc(QModelIndex)));
}

void hhconslive::loadForm()
{

    m_cpprodsModel = new yieldTableModel(this);
    //m_cpprodsModel->setImpactModule(this);

    m_cpprodsModel->setDatabase(dataBase);
    loadProducts();
    ui->AdvListView1->setModel(m_cpprodsModel);
    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadCalendarData(QModelIndex)));
    //m_cpprodsModel->distributeColums();
    connect(m_cpprodsModel,SIGNAL(modeldDataChanged()),this,SLOT(parentDataChanged()));

    m_periodModel = new periodTableModel(this);
    m_periodModel->setDatabase(dataBase);
    //m_periodModel->setImpactModule(this);


    m_periodModel->addField("tim","Consumed in","N","");
    m_periodModel->addField("qty","Quantity (kg)","0","");
    m_periodModel->addField("pri","Price (KSH)","0",""); //Add currency!!!!
    connect(m_periodModel,SIGNAL(modeldDataChanged()),this,SLOT(calendarDataChanged()));
    ui->AdvListView3->setModel(m_periodModel);

    if (m_cpprodsModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_cpprodsModel->index(0,0);
        loadCalendarData(start);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
    {
        setCalendarStatus(false);
    }

    for (int rsize = 0; rsize <= m_cpprodsModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);
}

void hhconslive::loadProducts()
{
    QList<TmoduleFieldDef> masterKeys;
    TmoduleFieldDef key;
    //Add crop products
    //Add crop products
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
    m_cpprodsModel->addMasterTable("livehvst",masterKeys,"Livestock product");
    m_cpprodsModel->addYieldTable("livehvst","livehvsttim","YIELD","PORHC");
    m_cpprodsModel->addFieldToMaster("livehvst","ENERGY","Energy \n (MJ/kg)","0");
    m_cpprodsModel->addFieldToMaster("livehvst","PROTEIN","Protein \n (g/kg)","0");
    m_cpprodsModel->addAllocationTable("livehvst","livehvstperiods","qty");
    key.code = "HERD_COD";
    masterKeys.append(key);
    m_cpprodsModel->addMasterTable("livehvstcat",masterKeys,"Livestock product by herd structure");
    m_cpprodsModel->addYieldTable("livehvstcat","livehvsttimcat","YIELD","PORHC");
    m_cpprodsModel->addFieldToMaster("livehvstcat","ENERGY","Energy \n (MJ/kg)","0");
    m_cpprodsModel->addFieldToMaster("livehvstcat","PROTEIN","Protein \n (g/kg)","0");
    m_cpprodsModel->addAllocationTable("livehvstcat","livehvstcatperiods","qty");

    masterKeys.clear();
    key.code = "SYS_COD";
    key.value = currentSystem;
    masterKeys.append(key);
    m_cpprodsModel->loadItems(masterKeys);


}

void hhconslive::constructCustomDelegator()
{

    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate->addIgnoredColumn(0);

    ui->AdvListView3->setItemDelegateForRow(0,ckhdelegate);


    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    for (int pos =0; pos <= m_periodModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView3->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView3->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

}

void hhconslive::loadCalendarData(QModelIndex index)
{
    if (!index.isValid())
        return;
    m_periodModel->setTableName(m_cpprodsModel->getMasterTable(index.row())+"periods");
    m_periodModel->loadData(m_cpprodsModel->getRowKeyValues(index.row()));
    //m_periodModel->distributeColums();
    ui->AdvListView3->setEnabled(true);

    for (int rsize = 0; rsize <= m_periodModel->rowCount()-1;rsize++)
        ui->AdvListView3->setRowHeight(rsize,listViewRowSize);
}

void hhconslive::setProductsStatus(bool status)
{
    ui->AdvListView1->setEnabled(status);
}

void hhconslive::setCalendarStatus(bool status)
{
    ui->AdvListView3->setEnabled(status);
    ui->BitBtn1->setEnabled(status);
    ui->BitBtn2->setEnabled(status);
}

void hhconslive::parentDataChanged()
{
    ui->BitBtn3->setEnabled(true);
    ui->BitBtn4->setEnabled(true);
}

void hhconslive::calendarDataChanged()
{
    setProductsStatus(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn2->setEnabled(true);
}

void hhconslive::on_BitBtn3_clicked()
{
    m_cpprodsModel->applyData();
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn4->setEnabled(false);
}

void hhconslive::on_BitBtn4_clicked()
{
    m_cpprodsModel->cancelData();
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn4->setEnabled(false);
}

void hhconslive::on_BitBtn1_clicked()
{
    m_periodModel->applyData();
    setProductsStatus(true);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
    m_cpprodsModel->updateAllocation();
}

void hhconslive::on_BitBtn2_clicked()
{
    m_periodModel->cancelData();
    setProductsStatus(true);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
}

void hhconslive::loadProdCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if (index.column() > 3)
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(dataBase);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_cpprodsModel->setData(index,calc.getCurrentDisplay());
    }
}

void hhconslive::loadDetCalc (const QModelIndex &index)
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
        m_periodModel->setData(index,calc.getCurrentDisplay());
    }
}

void hhconslive::on_pushButton_clicked()
{
    if (ui->AdvListView1->currentIndex().isValid())
    {
        QModelIndex idx;
        idx = ui->AdvListView1->currentIndex();

        QList<TmoduleFieldDef> keys;
        keys = m_cpprodsModel->getRowKeyValues(idx.row());

        QString livestock;
        QString prod;
        for (int pos =0; pos <= keys.count()-1;pos++)
        {
            if (keys[pos].code.toLower() == "live_cod")
                livestock = keys[pos].value.toString();
            if (keys[pos].code.toLower() == "prod_cod")
                prod = keys[pos].value.toString();
        }
        QSqlQuery query(dataBase);
        QString sql;
        sql = "SELECT energy,protein FROM lvproducts";
        sql = sql + " WHERE live_cod = '" + livestock + "'";
        sql = sql + " AND prod_cod = '" + prod + "'";
        if (query.exec(sql))
        {
            if (query.first())
            {
                QModelIndex idx2;
                idx2 = m_cpprodsModel->index(idx.row(),4);
                m_cpprodsModel->setData(idx2,query.value(0).toString());
                idx2 = m_cpprodsModel->index(idx.row(),5);
                m_cpprodsModel->setData(idx2,query.value(1).toString());
            }
            else
            {
                qWarning() << "Warning... Query returned no data ";
            }
        }
        else
        {
            qWarning() << "Error: " << query.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
}
