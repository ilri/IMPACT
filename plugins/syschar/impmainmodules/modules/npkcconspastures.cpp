#include "npkcconspastures.h"
#include "ui_npkcconspastures.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>

NPKCConsPastures::NPKCConsPastures(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::NPKCConsPastures)
{
    ui->setupUi(this);
    dataBase = db;
    currentSystem = systemID;

    QSqlQuery sysinfo(dataBase);
    QString sql;
    sql = "SELECT NUMSEASONS FROM system WHERE sys_cod = '" + currentSystem + "'";
    sysinfo.exec(sql);
    sysinfo.first();
    numseasons = sysinfo.value(0).toInt();

    loadForm();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

NPKCConsPastures::~NPKCConsPastures()
{
    delete ui;
}

void NPKCConsPastures::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView1);
    connect(ui->AdvListView1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalc(QModelIndex)));
}

void NPKCConsPastures::loadForm()
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

    for (int pos = 0; pos <= m_cpprodsModel->columnCount()-1;pos++)
    {
        if (pos <= m_cpprodsModel->columnCount()-1)
            ui->AdvListView1->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView1->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }
}

void NPKCConsPastures::valueChanged()
{
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void NPKCConsPastures::loadProducts()
{
    QList<TmoduleFieldDef> masterKeys;
    TmoduleFieldDef key;
    //Add crop products
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
    m_cpprodsModel->addMasterTable("grassprodman",masterKeys,"Crop product");
    m_cpprodsModel->addYieldTable("grassprodman","grassprodmantim","YIELD","PORHC");

    if (numseasons == 2)
    {
        m_cpprodsModel->addFieldToMaster("grassprodman","NSEA1"," Nitrogen \n first season \n  (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","NSEA2"," Nitrogen \n second season \n  (%)","0");

        m_cpprodsModel->addFieldToMaster("grassprodman","PSEA1","Phosphorus \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","PSEA2","Phosphorus \n second season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassprodman","KSEA1","Potassium \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","KSEA2","Potassium \n second season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassprodman","CSEA1","Carbon \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","CSEA2","Carbon \n second season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassprodman","DMSEA1","Dy matter \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","DMSEA2","Dy matter \n second season \n (%)","0");
    }
    else
    {
        m_cpprodsModel->addFieldToMaster("grassprodman","NSEA1"," Nitrogen \n first season \n  (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","NSEA2"," Nitrogen \n second season \n  (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","NSEA3"," Nitrogen \n third season \n  (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","NSEA4"," Nitrogen \n fourth season \n  (%)","0");

        m_cpprodsModel->addFieldToMaster("grassprodman","PSEA1","Phosphorus \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","PSEA2","Phosphorus \n second season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","PSEA3","Phosphorus \n third season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","PSEA4","Phosphorus \n fourth season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassprodman","KSEA1","Potassium \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","KSEA2","Potassium \n second season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","KSEA3","Potassium \n third season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","KSEA4","Potassium \n fourth season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassprodman","CSEA1","Carbon \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","CSEA2","Carbon \n second season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","CSEA3","Carbon \n third season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","CSEA4","Carbon \n fourth season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassprodman","DMSEA1","Dy matter \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","DMSEA2","Dy matter \n second season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","DMSEA3","Dy matter \n third season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassprodman","DMSEA4","Dy matter \n fourth season \n (%)","0");
    }


    //Add crop forages
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
    m_cpprodsModel->addMasterTable("grassforaman",masterKeys,"Crop Forage");
    m_cpprodsModel->addYieldTable("grassforaman","grassforamantim","YIELD","PORHC");

    if (numseasons == 2)
    {
        m_cpprodsModel->addFieldToMaster("grassforaman","NSEA1"," Nitrogen \n first season \n  (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","NSEA2"," Nitrogen \n second season \n  (%)","0");

        m_cpprodsModel->addFieldToMaster("grassforaman","PSEA1","Phosphorus \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","PSEA2","Phosphorus \n second season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassforaman","KSEA1","Potassium \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","KSEA2","Potassium \n second season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassforaman","CSEA1","Carbon \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","CSEA2","Carbon \n second season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassforaman","DMSEA1","Dy matter \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","DMSEA2","Dy matter \n second season \n (%)","0");
    }
    else
    {
        m_cpprodsModel->addFieldToMaster("grassforaman","NSEA1"," Nitrogen \n first season \n  (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","NSEA2"," Nitrogen \n second season \n  (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","NSEA3"," Nitrogen \n third season \n  (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","NSEA4"," Nitrogen \n fourth season \n  (%)","0");

        m_cpprodsModel->addFieldToMaster("grassforaman","PSEA1","Phosphorus \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","PSEA2","Phosphorus \n second season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","PSEA3","Phosphorus \n third season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","PSEA4","Phosphorus \n fourth season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassforaman","KSEA1","Potassium \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","KSEA2","Potassium \n second season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","KSEA3","Potassium \n third season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","KSEA4","Potassium \n fourth season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassforaman","CSEA1","Carbon \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","CSEA2","Carbon \n second season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","CSEA3","Carbon \n third season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","CSEA4","Carbon \n fourth season \n (%)","0");

        m_cpprodsModel->addFieldToMaster("grassforaman","DMSEA1","Dy matter \n first season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","DMSEA2","Dy matter \n second season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","DMSEA3","Dy matter \n third season \n (%)","0");
        m_cpprodsModel->addFieldToMaster("grassforaman","DMSEA4","Dy matter \n fourth season \n (%)","0");
    }





    masterKeys.clear();
    key.code = "SYS_COD";
    key.value = currentSystem;
    masterKeys.append(key);
    m_cpprodsModel->loadItems(masterKeys);
}

void NPKCConsPastures::on_cmdapply_clicked()
{
    if (m_cpprodsModel->applyData())
    {
        ui->cmdapply->setEnabled(false);
        ui->cmdcancel->setEnabled(false);
    }
}

void NPKCConsPastures::on_cmdcancel_clicked()
{
    m_cpprodsModel->cancelData();
    ui->cmdapply->setEnabled(false);
    ui->cmdcancel->setEnabled(false);
}

void NPKCConsPastures::loadCalc (const QModelIndex &index)
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

void NPKCConsPastures::on_cmdgetvalues_clicked()
{
    if (ui->AdvListView1->currentIndex().isValid())
    {
        QModelIndex idx;
        idx = ui->AdvListView1->currentIndex();
        QString table;
        QString prodOrFora;
        QString vegeOrCrop;
        QString prodTable;
        table = m_cpprodsModel->getMasterTable(idx.row()).toUpper();

        QList<TmoduleFieldDef> keys;
        keys = m_cpprodsModel->getRowKeyValues(idx.row());

        if (table.contains("FORA"))
        {
            prodOrFora = "fora_cod";
            if (table.contains("GRASS"))
                prodTable = "pastfora";
            else
                prodTable = "cropfora";
        }
        else
        {
            prodOrFora = "prod_cod";
            if (table.contains("GRASS"))
                prodTable = "pastprods";
            else
                prodTable = "cropprods";
        }
        if (table.contains("VEG"))
            vegeOrCrop = "veg_cod";
        else
            vegeOrCrop = "crop_cod";
        QString crop;
        QString prod;
        for (int pos =0; pos <= keys.count()-1;pos++)
        {
            if (keys[pos].code.toLower() == vegeOrCrop)
                crop = keys[pos].value.toString();
            if (keys[pos].code.toLower() == prodOrFora)
                prod = keys[pos].value.toString();
        }
        QSqlQuery query(dataBase);
        QString sql;

        sql = "SELECT nitrogen,phosphorus,potassium,carbon,drymatter FROM " + prodTable;
        sql = sql + " WHERE crop_cod = '" + crop + "'";
        sql = sql + " AND " + prodOrFora + " = '" + prod + "'";
        if (query.exec(sql))
        {
            if (query.first())
            {
                QModelIndex idx2;
                if (numseasons == 2)
                {
                    //N
                    idx2 = m_cpprodsModel->index(idx.row(),2);
                    m_cpprodsModel->setData(idx2,query.value(0).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),3);
                    m_cpprodsModel->setData(idx2,query.value(0).toString());
                    //P
                    idx2 = m_cpprodsModel->index(idx.row(),4);
                    m_cpprodsModel->setData(idx2,query.value(1).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),5);
                    m_cpprodsModel->setData(idx2,query.value(1).toString());
                    //K
                    idx2 = m_cpprodsModel->index(idx.row(),6);
                    m_cpprodsModel->setData(idx2,query.value(2).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),7);
                    m_cpprodsModel->setData(idx2,query.value(2).toString());
                    //C
                    idx2 = m_cpprodsModel->index(idx.row(),8);
                    m_cpprodsModel->setData(idx2,query.value(3).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),9);
                    m_cpprodsModel->setData(idx2,query.value(3).toString());
                    //DM
                    idx2 = m_cpprodsModel->index(idx.row(),10);
                    m_cpprodsModel->setData(idx2,query.value(4).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),11);
                    m_cpprodsModel->setData(idx2,query.value(4).toString());
                }
                else
                {
                    //N
                    idx2 = m_cpprodsModel->index(idx.row(),2);
                    m_cpprodsModel->setData(idx2,query.value(0).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),3);
                    m_cpprodsModel->setData(idx2,query.value(0).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),4);
                    m_cpprodsModel->setData(idx2,query.value(0).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),5);
                    m_cpprodsModel->setData(idx2,query.value(0).toString());
                    //P
                    idx2 = m_cpprodsModel->index(idx.row(),6);
                    m_cpprodsModel->setData(idx2,query.value(1).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),7);
                    m_cpprodsModel->setData(idx2,query.value(1).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),8);
                    m_cpprodsModel->setData(idx2,query.value(1).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),9);
                    m_cpprodsModel->setData(idx2,query.value(1).toString());
                    //K
                    idx2 = m_cpprodsModel->index(idx.row(),10);
                    m_cpprodsModel->setData(idx2,query.value(2).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),11);
                    m_cpprodsModel->setData(idx2,query.value(2).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),12);
                    m_cpprodsModel->setData(idx2,query.value(2).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),14);
                    m_cpprodsModel->setData(idx2,query.value(2).toString());
                    //C
                    idx2 = m_cpprodsModel->index(idx.row(),14);
                    m_cpprodsModel->setData(idx2,query.value(3).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),15);
                    m_cpprodsModel->setData(idx2,query.value(3).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),16);
                    m_cpprodsModel->setData(idx2,query.value(3).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),17);
                    m_cpprodsModel->setData(idx2,query.value(3).toString());
                    //DM
                    idx2 = m_cpprodsModel->index(idx.row(),18);
                    m_cpprodsModel->setData(idx2,query.value(4).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),19);
                    m_cpprodsModel->setData(idx2,query.value(4).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),20);
                    m_cpprodsModel->setData(idx2,query.value(4).toString());
                    idx2 = m_cpprodsModel->index(idx.row(),21);
                    m_cpprodsModel->setData(idx2,query.value(4).toString());
                }
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
