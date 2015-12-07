#include "prchinslvexpenses.h"
#include "ui_prchinslvexpenses.h"
#include "delegates/imagecheckdelegate.h"
#include "delegates/fixcombodelegate.h"
#include <qsqlquery.h>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>

prchinslvexpenses::prchinslvexpenses(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::prchinslvexpenses)
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

prchinslvexpenses::~prchinslvexpenses()
{
    //qDebug() << "Before destroy prchinslvexpenses UI";
    delete ui;
    //qDebug() << "After destroy prchinslvexpenses UI";
}

void prchinslvexpenses::loadAndroidEffects()
{
    charmLive.activateOn(ui->AdvListView0);
    charmProds.activateOn(ui->AdvListView1);
    charmDetail.activateOn(ui->AdvListView2);
    connect(ui->AdvListView2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadDetCalc(QModelIndex)));
}

void prchinslvexpenses::loadForm()
{

    m_parentModel = new fieldInColModel(this);
    m_parentModel->setReadOnly(true);
    m_parentModel->setDatabase(dataBase);
    //m_parentModel->setImpactModule(this);
    m_parentModel->setTableName("lvstosys");


    m_parentModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_parentModel->insertField("LIVE_ICOD","Livestock","000","000",true,false,true,true);
    m_parentModel->insertField("PURPOSE","Purpose","000","000",false,false,true);
    m_parentModel->insertField("BREDD","Breed","000","000",false,false,true);

    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_parentModel->loadData(keys);
    ui->AdvListView0->setModel(m_parentModel);
    //m_parentModel->distributeColums();
    connect(ui->AdvListView0,SIGNAL(clicked(QModelIndex)),this,SLOT(parentViewActivated(QModelIndex)));
    ui->AdvListView0->setSelectionBehavior(QAbstractItemView::SelectRows);


    m_colModel = new fieldInColModel(this);
    m_colModel->setDatabase(dataBase);
    //m_colModel->setImpactModule(this);
    m_colModel->setTableName("otherprod");


    m_colModel->insertExtraColumn("STS","  ","","","","");
    m_colModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_colModel->insertField("LIVE_ICOD","Plot ID","000","000",true,false,false);
    m_colModel->insertField("PROD_COD","Product ID","000","000",true,true,false);
    m_colModel->insertField("PROD_DESC","Description","","",false,false,true);
    m_colModel->addReadOnlyField("STS","ALL");

    m_colModel->addDistributionException(0);

    connect(m_colModel,SIGNAL(modeldDataChanged()),this,SLOT(dataChanged()));
    connect(m_colModel,SIGNAL(afterApply(QList<TmoduleInsertedKeys>)),this,SLOT(afterApply(QList<TmoduleInsertedKeys>)));

    ui->AdvListView1->setModel(m_colModel);

    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadChildData(QModelIndex)));

    m_periodModel = new periodTableModel(this);
    m_periodModel->setDatabase(dataBase);
    //m_periodModel->setImpactModule(this);

    m_periodModel->setTableName("otherprodperiods");
    m_periodModel->addField("tmp","Accrued in","N","");
    m_periodModel->addField("pri","Price (KSH)","0",""); //Add currency!!!!

    connect(m_periodModel,SIGNAL(modeldDataChanged()),this,SLOT(childDataChanged()));

    ui->AdvListView2->setModel(m_periodModel);

    if (m_parentModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_parentModel->index(0,0);
        parentViewActivated(start);
        ui->AdvListView0->setCurrentIndex(start);
    }
    else
    {
        setParentStatus(false);
        setChildStatus(false);
        //m_colModel->distributeColums();
    }

    for (int rsize = 0; rsize <= m_parentModel->rowCount()-1;rsize++)
        ui->AdvListView0->setRowHeight(rsize,listViewRowSize);

}

void prchinslvexpenses::constructCustomDelegator()
{

    fixComboDelegate *pdelegate = new fixComboDelegate(this);
    pdelegate->setReadOnly(true);
    QSqlQuery livestock(dataBase);
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
    ui->AdvListView0->setItemDelegateForColumn(0,pdelegate);

    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate->addIgnoredColumn(0);

    ui->AdvListView2->setItemDelegateForRow(0,ckhdelegate);

    for (int pos =0; pos <= m_periodModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    ui->AdvListView0->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->AdvListView0->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AdvListView0->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

}

void prchinslvexpenses::parentViewActivated(const QModelIndex &index)
{
    QList<TmoduleFieldDef> keys;
    keys = m_parentModel->getRowKeyValues(index.row());
    if (keys.count() > 0)
    {
         m_colModel->loadData(keys);
         //m_colModel->distributeColums();

         if (m_colModel->rowCount() > 0)
         {
             QModelIndex start;
             setChildStatus(true);
             start = m_colModel->index(0,0);
             loadChildData(start);
             start = m_colModel->index(0,1);
             ui->AdvListView1->setCurrentIndex(start);
         }
         else
         {
             m_periodModel->clearData();
             setChildStatus(false);
             //m_colModel->distributeColums();
         }

         for (int rsize = 0; rsize <= m_colModel->rowCount()-1;rsize++)
             ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

    }
}


void prchinslvexpenses::loadChildData(QModelIndex index)
{
   if (ui->AdvListView2->isEnabled())
    {
        if (!index.isValid())
            return;
        if ((m_colModel->getRowStatus(index.row()) != "N") ||
            (m_colModel->getRowStatus(index.row()) != "D"))
        {
            m_periodModel->loadData(m_colModel->getRowKeyValues(index.row()));
            //m_periodModel->distributeColums();
        }


    }
   for (int rsize = 0; rsize <= m_periodModel->rowCount()-1;rsize++)
       ui->AdvListView2->setRowHeight(rsize,listViewRowSize);
}

void prchinslvexpenses::setChildStatus(bool status)
{
    ui->AdvListView2->setEnabled(status);
    ui->BitBtn11->setEnabled(status);
    ui->BitBtn12->setEnabled(status);
}

void prchinslvexpenses::setParentStatus(bool status)
{
    ui->AdvListView1->setEnabled(status);
    ui->BitBtn14->setEnabled(status);
    ui->BitBtn15->setEnabled(status);
    ui->BitBtn9->setEnabled(status);
    ui->BitBtn10->setEnabled(status);
}

void prchinslvexpenses::childDataChanged()
{
    ui->AdvListView0->setEnabled(false);
    setParentStatus(false);
    ui->BitBtn11->setEnabled(true);
    ui->BitBtn12->setEnabled(true);
}

void prchinslvexpenses::dataChanged()
{
    ui->AdvListView0->setEnabled(false);
    setChildStatus(false);
    ui->BitBtn9->setEnabled(true);
    ui->BitBtn10->setEnabled(true);
}

void prchinslvexpenses::on_BitBtn14_clicked()
{
    m_colModel->insertNewItem();
}

void prchinslvexpenses::on_BitBtn15_clicked()
{
    QModelIndex idx;
    idx = ui->AdvListView1->currentIndex();
    if (idx.isValid())
    {
        m_colModel->deleteItem(idx.row());
    }
}

void prchinslvexpenses::on_BitBtn9_clicked()
{
    if (m_colModel->applyChanges())
    {
        setChildStatus(true);
        ui->BitBtn11->setEnabled(false);
        ui->BitBtn12->setEnabled(false);
        ui->AdvListView0->setEnabled(true);

        QModelIndex start;
        start = ui->AdvListView0->currentIndex();
        parentViewActivated(start);
    }
}

void prchinslvexpenses::on_BitBtn10_clicked()
{
    m_colModel->cancelChanges();
    setChildStatus(true);
    ui->BitBtn11->setEnabled(false);
    ui->BitBtn12->setEnabled(false);
    ui->AdvListView0->setEnabled(true);
}

void prchinslvexpenses::on_BitBtn11_clicked()
{
    m_periodModel->applyData();
    ui->AdvListView0->setEnabled(true);
    setParentStatus(true);
    ui->BitBtn9->setEnabled(false);
    ui->BitBtn10->setEnabled(false);
}

void prchinslvexpenses::on_BitBtn12_clicked()
{
    m_periodModel->cancelData();
    ui->AdvListView0->setEnabled(true);
    setParentStatus(true);
    ui->BitBtn9->setEnabled(false);
    ui->BitBtn10->setEnabled(false);
}

void prchinslvexpenses::afterApply(QList<TmoduleInsertedKeys> insertedKeys)
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
            sql = "INSERT INTO otherprodperiods (";
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

void prchinslvexpenses::loadDetCalc (const QModelIndex &index)
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
