#include "otherincome.h"
#include "ui_otherincome.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>

otherincome::otherincome(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::otherincome)
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

otherincome::~otherincome()
{
    //qDebug() << "Before destroy otherincome UI";
    delete ui;
    //qDebug() << "After destroy otherincome UI";
}

void otherincome::loadAndroidEffects()
{
    charmProds.activateOn(ui->AdvListView1);
    charmDetail.activateOn(ui->AdvListView2);
    connect(ui->AdvListView2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadDetCalc(QModelIndex)));
}

void otherincome::loadForm()
{
    m_colModel = new fieldInColModel(this);
    m_colModel->setDatabase(dataBase);
    //m_colModel->setImpactModule(this);
    m_colModel->setTableName("otherincome");


    m_colModel->insertExtraColumn("STS","  ","","","","");
    m_colModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_colModel->insertField("PROD_COD","Product ID","000","000",true,true,false);
    m_colModel->insertField("PROD_DES","Description","","",false,false,true);
    m_colModel->addReadOnlyField("STS","ALL");

    m_colModel->addDistributionException(0);

    connect(m_colModel,SIGNAL(modeldDataChanged()),this,SLOT(dataChanged()));
    connect(m_colModel,SIGNAL(afterApply(QList<TmoduleInsertedKeys>)),this,SLOT(afterApply(QList<TmoduleInsertedKeys>)));

    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_colModel->loadData(keys);
    ui->AdvListView1->setModel(m_colModel);

    //m_colModel->distributeColums();

    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadChildData(QModelIndex)));

    m_periodModel = new periodTableModel(this);
    m_periodModel->setDatabase(dataBase);
    //m_periodModel->setImpactModule(this);

    m_periodModel->setTableName("otherincomeperiods");
    m_periodModel->addField("tmp","Accrued in","N","");
    m_periodModel->addField("pri","Price (KSH)","0",""); //Add currency!!!!

    connect(m_periodModel,SIGNAL(modeldDataChanged()),this,SLOT(childDataChanged()));

    ui->AdvListView2->setModel(m_periodModel);

    if (m_colModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_colModel->index(0,0);
        loadChildData(start);
        start = m_colModel->index(0,1);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
        setChildStatus(false);

    for (int rsize = 0; rsize <= m_colModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

}

void otherincome::constructCustomDelegator()
{

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

}

void otherincome::loadChildData(QModelIndex index)
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

void otherincome::setChildStatus(bool status)
{
    ui->AdvListView2->setEnabled(status);
    ui->BitBtn11->setEnabled(status);
    ui->BitBtn12->setEnabled(status);
}

void otherincome::setParentStatus(bool status)
{
    ui->AdvListView1->setEnabled(status);
    ui->BitBtn14->setEnabled(status);
    ui->BitBtn15->setEnabled(status);
    ui->BitBtn9->setEnabled(status);
    ui->BitBtn10->setEnabled(status);
}

void otherincome::childDataChanged()
{
    setParentStatus(false);
    ui->BitBtn11->setEnabled(true);
    ui->BitBtn12->setEnabled(true);
}

void otherincome::dataChanged()
{
    setChildStatus(false);
    ui->BitBtn9->setEnabled(true);
    ui->BitBtn10->setEnabled(true);
}

void otherincome::on_BitBtn14_clicked()
{
    m_colModel->insertNewItem();
}

void otherincome::on_BitBtn15_clicked()
{
    QModelIndex idx;
    idx = ui->AdvListView1->currentIndex();
    if (idx.isValid())
    {
        m_colModel->deleteItem(idx.row());
    }
}

void otherincome::on_BitBtn9_clicked()
{
    if (m_colModel->applyChanges())
    {
        setChildStatus(true);
        ui->BitBtn11->setEnabled(false);
        ui->BitBtn12->setEnabled(false);
    }
}

void otherincome::on_BitBtn10_clicked()
{
    m_colModel->cancelChanges();
    setChildStatus(true);
    ui->BitBtn11->setEnabled(false);
    ui->BitBtn12->setEnabled(false);
}

void otherincome::on_BitBtn11_clicked()
{
    m_periodModel->applyData();
    setParentStatus(true);
    ui->BitBtn9->setEnabled(false);
    ui->BitBtn10->setEnabled(false);
}

void otherincome::on_BitBtn12_clicked()
{
    m_periodModel->cancelData();
    setParentStatus(true);
    ui->BitBtn9->setEnabled(false);
    ui->BitBtn10->setEnabled(false);
}


void otherincome::afterApply(QList<TmoduleInsertedKeys> insertedKeys)
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
            sql = "INSERT INTO otherincomeperiods (";
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

void otherincome::loadDetCalc (const QModelIndex &index)
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
