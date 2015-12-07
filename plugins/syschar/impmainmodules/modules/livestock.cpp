#include "livestock.h"
#include "ui_livestock.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/labeldelegate.h"
#include "submodules/lvotherinfo.h"
#include "modulesubscreen.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>

livestock::livestock(QWidget *parent, QSqlDatabase db, QString systemID) :
    impgenmaint(parent),
    ui(new Ui::livestock)
{
    ui->setupUi(this);

    database = db;
    currentSystem = systemID;

    m_liveModel = new fieldInColModel(this);

    connect(ui->hhBitBtn8,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->hhBitBtn10,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->cmdnew,SIGNAL(clicked()),m_liveModel,SLOT(insertNewItem()));
    connect(ui->cmdelete,SIGNAL(clicked()),this,SLOT(deleteItem()));


    connect(m_liveModel,SIGNAL(modeldDataChanged()),this,SLOT(dataChanged()));
    connect(m_liveModel,SIGNAL(afterApply(QList<TmoduleInsertedKeys>)),this,SLOT(afterApply(QList<TmoduleInsertedKeys>)));

    connect(ui->Edit1,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit2,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit3,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit4,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit5,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit6,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit7,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    applyModel = false;
    applyForm = false;
    loadMaintenance(database);
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

livestock::~livestock()
{
    //qDebug() << "Before destroy livestock UI";
    delete ui;
    //qDebug() << "After destroy livestock UI";
}

void livestock::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView1);
    connect(ui->AdvListView1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalc(QModelIndex)));
}

void livestock::loadMaintenance(QSqlDatabase, QTableView *)
{


    m_liveModel->setDatabase(database);
    //m_liveModel->setImpactModule(this);
    m_liveModel->setTableName("lvstosys");


    m_liveModel->insertExtraColumn("STS","  ","","","","");
    m_liveModel->insertField("SYS_COD","Sys Cod","000","000",true,false,false);
    m_liveModel->insertField("LIVE_ICOD","Live ID","000","000",true,true,false);
    m_liveModel->insertField("LIVE_COD","Livestock","CT","CT",false,false,true,true);
    m_liveModel->insertField("PURPOSE","Purpose","DAIRY","DAIRY",false,false,true,true);
    m_liveModel->insertField("BREDD","Breed","","",false,false,true,false);
    m_liveModel->insertField("BOD_WEIG","Body \n weight (kg)","0","0",false,false,true,false);
    m_liveModel->insertField("NANIMAL","Number of \n animals","0","0",false,false,true,false);
    m_liveModel->insertExtraColumn("OPT","Optional \ndata","CS","AS","AS","TD");
    m_liveModel->addReadOnlyField("LIVE_COD","C");
    m_liveModel->addReadOnlyField("LIVE_COD","M");
    m_liveModel->addReadOnlyField("STS","ALL");

    QSqlQuery tquery(database);
    QString sql;
    sql = "SELECT LIVE_COD FROM livestoc WHERE LIVE_TPY = 'R' OR LIVE_TPY = 'S'";
    tquery.exec(sql);

    while (tquery.next())
    {
        m_liveModel->addReadOnlyField("NANIMAL","LIVE_COD",tquery.value(0).toString(),"ALL");
        m_liveModel->addShowValue("NANIMAL","NA","LIVE_COD",tquery.value(0).toString());
    }


    m_liveModel->addDistributionException(0);

    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_liveModel->loadData(keys);
    ui->AdvListView1->setModel(m_liveModel);

    //m_liveModel->distributeColums();

    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadChildData(QModelIndex)));

    if (m_liveModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_liveModel->index(0,0);
        loadChildData(start);
        start = m_liveModel->index(0,2);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
        setChildStatus(false);

    for (int rsize = 0; rsize <= m_liveModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

}

void livestock::deleteItem()
{
    QModelIndex idx;
    idx = ui->AdvListView1->currentIndex();
    if (idx.isValid())
    {
        m_liveModel->deleteItem(idx.row());
    }
}

void livestock::constructCustomDelegator()
{

    fixComboDelegate *cdelegate = new fixComboDelegate(this);
    QSqlQuery tquery(database);
    QString sql;
    sql = "SELECT LIVE_COD,LIVE_DES FROM livestoc";
    tquery.exec(sql);
    while (tquery.next())
    {
        cdelegate->insertItem(tquery.value(0).toString(),tquery.value(1).toString());
    }
    ui->AdvListView1->setItemDelegateForColumn(1,cdelegate);


    fixComboDelegate *ldelegate = new fixComboDelegate(this);
    ldelegate->setParentModel(m_liveModel);
    ldelegate->insertItem("DAIRY","Dairy");
    ldelegate->insertItem("MEAT","Meat");
    ldelegate->insertItem("EGGS","Eggs");
    ldelegate->insertItem("BREEDING","Breeding");
    ldelegate->insertItem("DUAL","Dual");
    ldelegate->insertItem("TRACTION","Traction");

    //Add the constraints to items based on they type

    sql = "SELECT LIVE_COD FROM livestoc WHERE LIVE_TPY = 'R' OR LIVE_TPY = 'S'";
    tquery.exec(sql);
    while (tquery.next())
    {
        ldelegate->addShowContraint("DAIRY","LIVE_COD",tquery.value(0).toString());
        ldelegate->addShowContraint("MEAT","LIVE_COD",tquery.value(0).toString());
        ldelegate->addShowContraint("BREEDING","LIVE_COD",tquery.value(0).toString());
        ldelegate->addShowContraint("DUAL","LIVE_COD",tquery.value(0).toString());
        ldelegate->addShowContraint("TRACTION","LIVE_COD",tquery.value(0).toString());
    }
    sql = "SELECT LIVE_COD FROM livestoc WHERE LIVE_TPY = 'O'";
    tquery.exec(sql);
    while (tquery.next())
    {
        ldelegate->addShowContraint("EGGS","LIVE_COD",tquery.value(0).toString());
        ldelegate->addShowContraint("MEAT","LIVE_COD",tquery.value(0).toString());
        ldelegate->addShowContraint("BREEDING","LIVE_COD",tquery.value(0).toString());
        ldelegate->addShowContraint("DUAL","LIVE_COD",tquery.value(0).toString());
    }

    ui->AdvListView1->setItemDelegateForColumn(2,ldelegate);


    labelDelegate *pdelegate = new labelDelegate(this);
    pdelegate->insertItem("CS","Click to edit");
    pdelegate->insertItem("AS","Apply to edit");
    pdelegate->insertItem("TD","To be deleted");
    pdelegate->addNotDialogValue("AS");
    pdelegate->addNotDialogValue("TD");
    pdelegate->addNoDrawValue("NA");
    connect(pdelegate,SIGNAL(loadSubModule(QModelIndex)),this,SLOT(loadExtra(QModelIndex)));

    ui->AdvListView1->setItemDelegateForColumn(6,pdelegate);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);

}

void livestock::dataChanged()
{
    applyModel = true;
    ui->hhBitBtn8->setEnabled(true);
    ui->hhBitBtn10->setEnabled(true);
    QModelIndex idx;
    idx = ui->AdvListView1->currentIndex();
    if (idx.isValid())
    {
        loadChildData(idx);
    }
}

void livestock::applyChanges()
{
    if (applyModel)
    {
        if (m_liveModel->applyChanges())
        {
            ui->hhBitBtn8->setEnabled(false);
            ui->hhBitBtn10->setEnabled(false);
            impgenmaint::applyChanges();
            applyModel = false;
        }
        else
            return;
    }
    if (applyForm)
    {
        impgenmaint::applyChanges();
        ui->hhBitBtn8->setEnabled(false);
        ui->hhBitBtn10->setEnabled(false);
        ui->AdvListView1->setEnabled(true);
        ui->cmdnew->setEnabled(true);
        ui->cmdelete->setEnabled(true);
        applyForm = false;
    }
}

void livestock::cancelChanges()
{
    if (applyModel)
    {
        m_liveModel->cancelChanges();
    }
    if (applyForm)
    {
        impgenmaint::cancelChanges();
    }
    ui->AdvListView1->setEnabled(true);
    ui->cmdnew->setEnabled(true);
    ui->cmdelete->setEnabled(true);
    ui->hhBitBtn8->setEnabled(false);
    ui->hhBitBtn10->setEnabled(false);
    applyModel = false;
    applyForm = false;
}

void livestock::valueHasChanged(QString, QString, QString)
{
    applyForm = true;
    ui->AdvListView1->setEnabled(false);
    ui->hhBitBtn8->setEnabled(true);
    ui->hhBitBtn10->setEnabled(true);
    ui->cmdnew->setEnabled(false);
    ui->cmdelete->setEnabled(false);
}

void livestock::loadExtra(QModelIndex index)
{
    if ((m_liveModel->getRowStatus(index.row()) != "N") ||
        (m_liveModel->getRowStatus(index.row()) != "D"))
    {
        lvotherinfo *lvinfo = new lvotherinfo(0);
        lvinfo->setTableName("lvstosys");
        for (int pos=0; pos <= m_liveModel->getRowKeyValues(index.row()).count()-1;pos++)
        {
            if (m_liveModel->getRowKeyValues(index.row())[pos].code.toUpper() == "PLOT_COD")
                lvinfo->insertKeyField(m_liveModel->getRowKeyValues(index.row())[pos].code,
                                     m_liveModel->getRowKeyValues(index.row())[pos].value,false);
            else
                lvinfo->insertKeyField(m_liveModel->getRowKeyValues(index.row())[pos].code,
                                     m_liveModel->getRowKeyValues(index.row())[pos].value);

        }
        //lvinfo->setKeyValues(m_liveModel->getRowKeyValues(index.row()));
        lvinfo->loadMaintenance(database);
        moduleSubScreen m_dialogWindow;
        m_dialogWindow.loadSubScreen(lvinfo);
        m_dialogWindow.setWindowTitle("Extra information");
        m_dialogWindow.exec();
    }
}

void livestock::loadChildData(QModelIndex index)
{
    if (!index.isValid())
        return;
    if ((m_liveModel->getRowStatus(index.row()) != "N") ||
        (m_liveModel->getRowStatus(index.row()) != "D"))
    {
        QString lvCode;
        QString purpose;
        QSqlQuery tquery(database);
        QString sql;

        lvCode = m_liveModel->getDataFromField(index.row(),"LIVE_COD").toString();
        purpose = m_liveModel->getDataFromField(index.row(),"PURPOSE").toString();
        sql = "SELECT live_tpy FROM livestoc WHERE live_cod = '" + lvCode + "'";
        if (tquery.exec(sql))
        {
            tquery.first();
            if ((tquery.value(0).toString() == "R") ||
                (tquery.value(0).toString() == "S"))
            {
                setChildStatus(true);
                this->setDataBase(database);
                this->setTableName("lvstosys");
                this->clearKeyFields();
                for (int pos=0; pos <= m_liveModel->getRowKeyValues(index.row()).count()-1;pos++)
                {
                    this->insertKeyField(m_liveModel->getRowKeyValues(index.row())[pos].code,
                                         m_liveModel->getRowKeyValues(index.row())[pos].value);

                }
                this->loadMaintTable();
                if (tquery.value(0).toString() == "R")
                    setRuminantStatus(true);
                else
                    setRuminantStatus(false);
                if ((purpose.toUpper() == "DAIRY") || (purpose.toUpper() == "DUAL"))
                    setMilkStatus(true);
                else
                    setMilkStatus(false);
            }
            else
            {
                setChildStatus(false);
            }
        }
        else
        {
            setChildStatus(false);
        }
    }
    else
    {
        setChildStatus(false);
    }
}

void livestock::setChildStatus(bool status)
{
    ui->Edit1->setEnabled(status);
    ui->Edit2->setEnabled(status);
    ui->Edit3->setEnabled(status);
    ui->Edit4->setEnabled(status);
    ui->Edit5->setEnabled(status);
    ui->Edit6->setEnabled(status);
    ui->Edit7->setEnabled(status);
    if (status == false)
    {
        ui->Edit1->setText("0");
        ui->Edit2->setText("0");
        ui->Edit3->setText("0");
        ui->Edit4->setText("0");
        ui->Edit5->setText("0");
        ui->Edit6->setText("0");
        ui->Edit7->setText("0");
    }
}

void livestock::afterApply(QList<TmoduleInsertedKeys> keys)
{
    int pos;
    int pos2;
    int pos3;
    QSqlQuery tquery(database);
    QString sql;
    QString lvCode;
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        lvCode = m_liveModel->getDataFromField(keys[pos].row,"LIVE_COD").toString();
        sql = "SELECT live_tpy FROM livestoc WHERE live_cod = '" + lvCode + "'";
        if (tquery.exec(sql))
        {
            tquery.first();
            if ((tquery.value(0).toString() == "R") ||
                (tquery.value(0).toString() == "S"))
            {
                for (pos2 = 1; pos2 <= 8;pos2++)
                {
                    sql = "INSERT INTO lvstosysherdstr (";
                    for (pos3 = 0; pos3 <= keys[pos].keys.count()-1;pos3++)
                    {
                        sql = sql + keys[pos].keys[pos3].code + ",";
                    }
                    sql = sql + "herd_cod) VALUES (";
                    for (pos3 = 0; pos3 <= keys[pos].keys.count()-1;pos3++)
                    {
                        sql = sql + "'" + keys[pos].keys[pos3].value.toString() + "',";
                    }
                    sql = sql + "'" + QString::number(pos2) + "')";
                    if (!tquery.exec(sql))
                    {
                        qWarning() << "Error: " << tquery.lastError().databaseText();
                        qWarning() << "Executing " << sql;
                    }
                }
            }
        }
        else
        {
            qWarning() << "Error: " << tquery.lastError().databaseText();
            qWarning() << "Executing " << sql;
        }
    }
}

void livestock::setMilkStatus(bool status)
{
    ui->Edit1->setEnabled(status);
    ui->Edit3->setEnabled(status);
    if (!status)
    {
        ui->Edit1->setText("0");
        ui->Edit3->setText("0");
    }
}

void livestock::setRuminantStatus(bool status)
{
    ui->Edit4->setEnabled(!status);
    ui->Edit5->setEnabled(!status);
    if (status)
    {
        ui->Edit4->setText("0");
        ui->Edit5->setText("0");
    }
}

void livestock::loadCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if ((index.column() == 4) || (index.column() == 5))
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(database);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_liveModel->setData(index,calc.getCurrentDisplay());
    }
}
