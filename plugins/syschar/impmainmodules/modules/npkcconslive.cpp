#include "npkcconslive.h"
#include "ui_npkcconslive.h"
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>


NPKCConsLive::NPKCConsLive(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::NPKCConsLive)
{
    ui->setupUi(this);
    dataBase = db;
    currentSystem = systemID;
    loadForm();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

NPKCConsLive::~NPKCConsLive()
{
    delete ui;
}

void NPKCConsLive::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView1);
    connect(ui->AdvListView1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalc(QModelIndex)));
}

void NPKCConsLive::loadForm()
{
    m_cpprodsModel = new yieldTableModel(this);
    //m_cpprodsModel->setImpactModule(this);

    m_cpprodsModel->setDatabase(dataBase);
    m_cpprodsModel->setCalculateYield(false);
    loadProducts();
    ui->AdvListView1->setModel(m_cpprodsModel);
    //m_cpprodsModel->distributeColums();
    connect(m_cpprodsModel,SIGNAL(modeldDataChanged()),this,SLOT(valueChanged()));

    for (int rsize = 0; rsize <= m_cpprodsModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);

}

void NPKCConsLive::valueChanged()
{
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void NPKCConsLive::loadProducts()
{
    QList<TmoduleFieldDef> masterKeys;
    TmoduleFieldDef key;
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
    m_cpprodsModel->addMasterTable("livehvst",masterKeys,"Crop product");
    m_cpprodsModel->addYieldTable("livehvst","livehvsttim","YIELD","PORHC");
    m_cpprodsModel->addFieldToMaster("livehvst","NITROGEN"," Nitrogen \n (%)","0");
    m_cpprodsModel->addFieldToMaster("livehvst","PHOSPHORUS","Phosphorus \n (%)","0");
    m_cpprodsModel->addFieldToMaster("livehvst","POTASSIUM","Potassium \n (%)","0");
    m_cpprodsModel->addFieldToMaster("livehvst","CARBON","Carbon \n (%)","0");
    m_cpprodsModel->addFieldToMaster("livehvst","DRYMATTER","Dy matter \n (%)","0");

    masterKeys.clear();
    key.code = "SYS_COD";
    key.value = currentSystem;
    masterKeys.append(key);
    m_cpprodsModel->loadItems(masterKeys);
}

void NPKCConsLive::on_cmdapply_clicked()
{
    if (m_cpprodsModel->applyData())
    {
        ui->cmdapply->setEnabled(false);
        ui->cmdcancel->setEnabled(false);
    }
}

void NPKCConsLive::on_cmdcancel_clicked()
{
    m_cpprodsModel->cancelData();
    ui->cmdapply->setEnabled(false);
    ui->cmdcancel->setEnabled(false);
}

void NPKCConsLive::loadCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if (index.column() > 1)
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

void NPKCConsLive::on_cmdgetvalues_clicked()
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
        sql = "SELECT nitrogen,phosphorus,potassium,carbon,drymatter FROM lvproducts";
        sql = sql + " WHERE live_cod = '" + livestock + "'";
        sql = sql + " AND prod_cod = '" + prod + "'";
        if (query.exec(sql))
        {
            if (query.first())
            {
                QModelIndex idx2;
                idx2 = m_cpprodsModel->index(idx.row(),2);
                m_cpprodsModel->setData(idx2,query.value(0).toString());
                idx2 = m_cpprodsModel->index(idx.row(),3);
                m_cpprodsModel->setData(idx2,query.value(1).toString());
                idx2 = m_cpprodsModel->index(idx.row(),4);
                m_cpprodsModel->setData(idx2,query.value(2).toString());
                idx2 = m_cpprodsModel->index(idx.row(),5);
                m_cpprodsModel->setData(idx2,query.value(3).toString());
                idx2 = m_cpprodsModel->index(idx.row(),6);
                m_cpprodsModel->setData(idx2,query.value(4).toString());
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
