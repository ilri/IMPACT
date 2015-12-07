#include "housesize.h"
#include "ui_housesize.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/labeldelegate.h"
#include "delegates/imagecheckdelegate.h"
#include "modulesubscreen.h"
#include "submodules/hhsotherinfo.h"
#include <qsqlquery.h>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>

housesize::housesize(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::housesize)
{
    ui->setupUi(this);
    dataBase = db;
    currentSystem = systemID;

    QSqlQuery sysinfo(dataBase);
    QString sql;
    sql = "SELECT numseasons FROM system WHERE sys_cod = '" + currentSystem + "'";
    sysinfo.exec(sql);
    sysinfo.first();
    numseasons = sysinfo.value(0).toInt();

    loadForm();
    ui->AdvListView1->setMouseTracking(true);
    constructCustomDelegator();
    if (modules_use_Android == true)
    {
        qDebug() << "Android";
        loadAndroidEffects();
    }
}

housesize::~housesize()
{
    //qDebug() << "Before destroy housesize UI";
    delete ui;
    //qDebug() << "After destroy housesize UI";
}

void housesize::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView1);
    connect(ui->AdvListView1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalc(QModelIndex)));
}

void housesize::constructCustomDelegator()
{

    fixComboDelegate *ldelegate = new fixComboDelegate(this);
    ldelegate->insertItem("HH","Household member");
    ldelegate->insertItem("HL","Hired labour");
    ui->AdvListView1->setItemDelegateForColumn(1,ldelegate);



    fixComboDelegate *cdelegate = new fixComboDelegate(this);
    cdelegate->insertItem("M","Male");
    cdelegate->insertItem("F","Female");
    cdelegate->addNoDrawValue("NA");
    ui->AdvListView1->setItemDelegateForColumn(4,cdelegate);



    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ui->AdvListView1->setItemDelegateForColumn(5,ckhdelegate);



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
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);

}



void housesize::loadForm()
{
    m_colModel = new fieldInColModel(this);
    m_colModel->setDatabase(dataBase);
    //m_colModel->setImpactModule(this);
    m_colModel->setTableName("labforce");


    m_colModel->insertExtraColumn("STS","  ","","","","");
    m_colModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_colModel->insertField("PERS_COD","Plot ID","000","000",true,true,false);
    m_colModel->insertField("PERS_TYP","Type","HH","HH",false,false,true,true);
    m_colModel->insertField("SHORTDESC","Description","","",false,false,true);

    m_colModel->insertField("AGE","Age","0","0",false,false,true);
    m_colModel->insertField("SEX","Gender","M","M",false,false,true,true);
    m_colModel->insertField("WORK","Work","Y","Y",false,false,true,true);
    m_colModel->insertExtraColumn("OPT","Optional \ndata","CS","AS","AS","TD");
    m_colModel->addReadOnlyField("PERS_TYP","C");
    m_colModel->addReadOnlyField("PERS_TYP","M");
    m_colModel->addReadOnlyField("STS","ALL");

    m_colModel->addReadOnlyField("SHORTDESC","PERS_TYP","HL","ALL");
    m_colModel->addReadOnlyField("AGE","PERS_TYP","HL","ALL");
    m_colModel->addReadOnlyField("SEX","PERS_TYP","HL","ALL");
    m_colModel->addReadOnlyField("WORK","PERS_TYP","HL","ALL");
    m_colModel->addReadOnlyField("OPT","PERS_TYP","HL","ALL");
    m_colModel->addShowValue("SHORTDESC","NA","PERS_TYP","HL");
    m_colModel->addShowValue("AGE","NA","PERS_TYP","HL");
    m_colModel->addShowValue("SEX","NA","PERS_TYP","HL");
    m_colModel->addShowValue("OPT","NA","PERS_TYP","HL");

    m_colModel->addDistributionException(0);

    connect(m_colModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(dataChanged(QModelIndex,QModelIndex)));
    connect(ui->cmdapply,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->cmdcancel,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->cmdnew,SIGNAL(clicked()),this,SLOT(insertItem()));
    connect(ui->cmddelete,SIGNAL(clicked()),this,SLOT(deleteItem()));
    connect(m_colModel,SIGNAL(modeldDataChanged()),this,SLOT(valueChanged()));
    connect(m_colModel,SIGNAL(afterApply(QList<TmoduleInsertedKeys>)),this,SLOT(afterApply(QList<TmoduleInsertedKeys>)));


    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_colModel->loadData(keys);
    ui->AdvListView1->setModel(m_colModel);

    for (int rsize = 0; rsize <= m_colModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

    //m_colModel->distributeColums();
}

void housesize::dataChanged(const QModelIndex &, const QModelIndex &)
{
    //m_colModel->distributeColums();
}

void housesize::deleteItem()
{
    if (ui->AdvListView1->currentIndex().isValid())
    {
        m_colModel->deleteItem(ui->AdvListView1->currentIndex().row());
    }
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void housesize::valueInserted(int row)
{
    QString value;
    value = m_colModel->getDataFromField(row,"PLOT_COD").toString();
    m_colModel->setDataToField(row,"PLOT_ID",value);
}

void housesize::insertItem()
{
    m_colModel->insertNewItem();
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void housesize::applyChanges()
{
    if (m_colModel->applyChanges())
    {
        ui->cmdapply->setEnabled(false);
        ui->cmdcancel->setEnabled(false);
    }
}

void housesize::cancelChanges()
{
    m_colModel->cancelChanges();
    ui->cmdapply->setEnabled(false);
    ui->cmdcancel->setEnabled(false);
}
void housesize::valueChanged()
{
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}


void housesize::loadExtra(QModelIndex index)
{
    if ((m_colModel->getRowStatus(index.row()) != "N") ||
        (m_colModel->getRowStatus(index.row()) != "D"))
    {
        hhsotherinfo *extinfo = new hhsotherinfo(0);
        extinfo->setTableName("labforce");
        for (int pos=0; pos <= m_colModel->getRowKeyValues(index.row()).count()-1;pos++)
        {
            if (m_colModel->getRowKeyValues(index.row())[pos].code.toUpper() == "PLOT_COD")
                extinfo->insertKeyField(m_colModel->getRowKeyValues(index.row())[pos].code,
                                     m_colModel->getRowKeyValues(index.row())[pos].value,false);
            else
                extinfo->insertKeyField(m_colModel->getRowKeyValues(index.row())[pos].code,
                                     m_colModel->getRowKeyValues(index.row())[pos].value);

        }
        extinfo->setKeyValues(m_colModel->getRowKeyValues(index.row()));
        extinfo->loadMaintenance(dataBase);
        moduleSubScreen m_dialogWindow;
        m_dialogWindow.loadSubScreen(extinfo);
        m_dialogWindow.setWindowTitle("Extra information");
        m_dialogWindow.exec();        
    }
}

void housesize::afterApply(QList<TmoduleInsertedKeys> insertedKeys)
{
    int pos;
    int pos2;
    int pos3;
    QString sql;
    QSqlQuery sysinfo(dataBase);
    if (insertedKeys.count() >0)
    {
        for (pos = 1; pos <= numseasons; pos++)
        {
            for (pos2 = 0; pos2 <= insertedKeys.count()-1;pos2++)
            {
                sql = "INSERT INTO labforceperiods (";
                for (pos3 = 0; pos3 <= insertedKeys[pos2].keys.count()-1;pos3++)
                {
                    sql = sql + insertedKeys[pos2].keys[pos3].code + ",";
                }
                sql = sql + "period_id) VALUES (";
                for (pos3 = 0; pos3 <= insertedKeys[pos2].keys.count()-1;pos3++)
                {
                    sql = sql + "'" +insertedKeys[pos2].keys[pos3].value.toString() + "',";
                }
                sql = sql + "'" + QString::number(pos) + "')";
                sysinfo.exec(sql);
            }
        }
    }
}

void housesize::loadCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if (index.column() == 3)
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(dataBase);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_colModel->setData(index,calc.getCurrentDisplay());
    }
}
