#include "npkcconscrops.h"
#include "ui_npkcconscrops.h"
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

NPKCConsCrops::NPKCConsCrops(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::NPKCConsCrops)
{
    ui->setupUi(this);
    dataBase = db;
    currentSystem = systemID;
    loadForm();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

NPKCConsCrops::~NPKCConsCrops()
{
    delete ui;
}

void NPKCConsCrops::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView1);
    connect(ui->AdvListView1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalc(QModelIndex)));
}

void NPKCConsCrops::loadForm()
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

void NPKCConsCrops::valueChanged()
{
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void NPKCConsCrops::loadProducts()
{
    QList<TmoduleFieldDef> masterKeys;
    TmoduleFieldDef key;
    //Add crop products
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
    m_cpprodsModel->addMasterTable("crpprodman",masterKeys,"Crop product");
    m_cpprodsModel->addYieldTable("crpprodman","crpprodmantim","YIELD","PORHC");
    m_cpprodsModel->addFieldToMaster("crpprodman","NITROGEN"," Nitrogen \n (%)","0");
    m_cpprodsModel->addFieldToMaster("crpprodman","PHOSPHORUS","Phosphorus \n (%)","0");
    m_cpprodsModel->addFieldToMaster("crpprodman","POTASSIUM","Potassium \n (%)","0");
    m_cpprodsModel->addFieldToMaster("crpprodman","CARBON","Carbon \n (%)","0");
    m_cpprodsModel->addFieldToMaster("crpprodman","DRYMATTER","Dy matter \n (%)","0");


    //Add crop forages
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
    m_cpprodsModel->addMasterTable("crpforaman",masterKeys,"Crop Forage");
    m_cpprodsModel->addYieldTable("crpforaman","crpforamantim","YIELD","PORHC");
    m_cpprodsModel->addFieldToMaster("crpforaman","NITROGEN"," Nitrogen \n (%)","0");
    m_cpprodsModel->addFieldToMaster("crpforaman","PHOSPHORUS","Phosphorus \n (%)","0");
    m_cpprodsModel->addFieldToMaster("crpforaman","POTASSIUM","Potassium \n (%)","0");
    m_cpprodsModel->addFieldToMaster("crpforaman","CARBON","Carbon \n (%)","0");
    m_cpprodsModel->addFieldToMaster("crpforaman","DRYMATTER","Dy matter \n (%)","0");



    //Vegetables
    //Add vegetable products
    masterKeys.clear();
    key.code = "SYS_COD";
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
    m_cpprodsModel->addMasterTable("vegprodman",masterKeys,"Vegetable product");
    m_cpprodsModel->addYieldTable("vegprodman","vegprodmantim","YIELD","PORHC");
    m_cpprodsModel->addFieldToMaster("vegprodman","NITROGEN"," Nitrogen \n (%)","0");
    m_cpprodsModel->addFieldToMaster("vegprodman","PHOSPHORUS","Phosphorus \n (%)","0");
    m_cpprodsModel->addFieldToMaster("vegprodman","POTASSIUM","Potassium \n (%)","0");
    m_cpprodsModel->addFieldToMaster("vegprodman","CARBON","Carbon \n (%)","0");
    m_cpprodsModel->addFieldToMaster("vegprodman","DRYMATTER","Dy matter \n (%)","0");



    //Add vegetable forages
    masterKeys.clear();
    key.code = "SYS_COD";
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
    m_cpprodsModel->addMasterTable("vegforaman",masterKeys,"Vegetable forage");
    m_cpprodsModel->addYieldTable("vegforaman","vegforamantim","YIELD","PORHC");
    m_cpprodsModel->addFieldToMaster("vegforaman","NITROGEN"," Nitrogen \n (%)","0");
    m_cpprodsModel->addFieldToMaster("vegforaman","PHOSPHORUS","Phosphorus \n (%)","0");
    m_cpprodsModel->addFieldToMaster("vegforaman","POTASSIUM","Potassium \n (%)","0");
    m_cpprodsModel->addFieldToMaster("vegforaman","CARBON","Carbon \n (%)","0");
    m_cpprodsModel->addFieldToMaster("vegforaman","DRYMATTER","Dy matter \n (%)","0");

    masterKeys.clear();
    key.code = "SYS_COD";
    key.value = currentSystem;
    masterKeys.append(key);
    m_cpprodsModel->loadItems(masterKeys);


}


void NPKCConsCrops::on_cmdapply_clicked()
{
    if (m_cpprodsModel->applyData())
    {
        ui->cmdapply->setEnabled(false);
        ui->cmdcancel->setEnabled(false);
    }
}

void NPKCConsCrops::on_cmdcancel_clicked()
{
    m_cpprodsModel->cancelData();
    ui->cmdapply->setEnabled(false);
    ui->cmdcancel->setEnabled(false);
}


void NPKCConsCrops::loadCalc (const QModelIndex &index)
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

void NPKCConsCrops::on_cmdgetvalues_clicked()
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
