#include "feedgroups.h"
#include "ui_feedgroups.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/labeldelegate.h"
#include "submodules/calvpattern.h"
#include "modulesubscreen.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include "moduleglobal.h"

feedgroups::feedgroups(QWidget *parent, QSqlDatabase db, QString systemID) :
    impgenmaint(parent),
    ui(new Ui::feedgroups)
{
    ui->setupUi(this);

    database = db;
    currentSystem = systemID;

    m_liveModel = new fieldInColModel(this);

    m_fgroupsModel = new fieldInColModel(this);

    connect(ui->BitBtn3,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn4,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->cmdnew,SIGNAL(clicked()),this,SLOT(insertNew()));
    connect(ui->cmddelete,SIGNAL(clicked()),this,SLOT(deleteCurrent()));

    connect(m_fgroupsModel,SIGNAL(modeldDataChanged()),this,SLOT(dataChanged()));

    connect(ui->Edit1,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit2,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit3,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit4,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit5,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit6,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit7,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit8,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit9,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit10,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));


    applyModel = false;

    loadMaintenance(database);
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();

    //For the now the livestock allocation is not implemented
    ui->cmdalloc->setVisible(false);

}

feedgroups::~feedgroups()
{
    //qDebug() << "Before destroy feedgroups UI";
    delete ui;
    //qDebug() << "After destroy feedgroups UI";
}

void feedgroups::loadAndroidEffects()
{
    livecharm.activateOn(ui->AdvListView1);
    groupcharm.activateOn(ui->AdvListView2);
}

void feedgroups::loadMaintenance(QSqlDatabase, QTableView *)
{

    this->setDataBase(database);
    this->setTableName("feedgroups");
    //Creates the livextock model
    m_liveModel->setDatabase(database);
    //m_liveModel->setImpactModule(this);
    m_liveModel->setTableName("lvstosys");

    m_liveModel->insertField("SYS_COD","Sys Cod","000","000",true,false,false);
    m_liveModel->insertField("LIVE_ICOD","Live ID","000","000",true,true,false);
    m_liveModel->insertField("LIVE_COD","Livestock","CT","CT",false,false,true,true);
    m_liveModel->insertField("PURPOSE","Purpose","DAIRY","DAIRY",false,false,true,true);
    m_liveModel->insertField("BREDD","Breed","","",false,false,true,false);
    m_liveModel->insertField("BOD_WEIG","Body \n weight (kg)","0","0",false,false,true,false);

    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_liveModel->setReadOnly(true);
    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_liveModel->loadData(keys);
    ui->AdvListView1->setModel(m_liveModel);
    //m_liveModel->distributeColums();

    //Creates the male model
    m_fgroupsModel->setDatabase(database);
    //m_fgroupsModel->setImpactModule(this);
    m_fgroupsModel->setTableName("feedgroups");

    m_fgroupsModel->insertExtraColumn("STS","  ","","","","");
    m_fgroupsModel->insertField("SYS_COD","Sys Cod","000","000",true,false,false);
    m_fgroupsModel->insertField("LIVE_ICOD","Live ID","000","000",true,false,false);
    m_fgroupsModel->insertField("GROUP_COD","Live ID","000","000",true,true,false);

    m_fgroupsModel->insertField("GROUP_DES","Feeding group","Write a description","Write a description",false,false,true);
    m_fgroupsModel->insertField("GROUP_TYP","Type","ONE","ONE",false,false,true,true);
    m_fgroupsModel->insertField("GROUP_CRIE","Criterion","NON","NON",false,false,true,true);
    m_fgroupsModel->addReadOnlyField("STS","ALL");

    m_fgroupsModel->addDistributionException(0);
    ui->AdvListView2->setModel(m_fgroupsModel);

    //Positions the livestock model
    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadFeedGroups(QModelIndex)));
    connect(ui->AdvListView2,SIGNAL(clicked(QModelIndex)),this,SLOT(loadFeedGDetail(QModelIndex)));
    if (m_liveModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_liveModel->index(0,0);
        loadFeedGroups(start);
        start = m_liveModel->index(0,0);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
        setChildStatus(false);

    for (int rsize = 0; rsize <= m_liveModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);
}

void feedgroups::constructCustomDelegator()
{

    fixComboDelegate *ldelegate = new fixComboDelegate(this);
    QSqlQuery tquery(database);
    QString sql;
    sql = "SELECT LIVE_COD,LIVE_DES FROM livestoc";
    tquery.exec(sql);
    while (tquery.next())
    {
        ldelegate->insertItem(tquery.value(0).toString(),tquery.value(1).toString());
    }
    ldelegate->setReadOnly(true);
    ui->AdvListView1->setItemDelegateForColumn(0,ldelegate);

    fixComboDelegate *pdelegate = new fixComboDelegate(this);
    pdelegate->insertItem("PRO","Productivity");
    pdelegate->insertItem("LAC","Milking phase");
    pdelegate->insertItem("PAR","Calving number");
    pdelegate->insertItem("AGE","Age");
    pdelegate->insertItem("WEI","Weight");
    pdelegate->insertItem("MIX","Mixed");
    pdelegate->insertItem("DRY","Dry");
    pdelegate->insertItem("NON","None");
    ui->AdvListView2->setItemDelegateForColumn(2,pdelegate);

    fixComboDelegate *cdelegate = new fixComboDelegate(this);
    cdelegate->insertItem("PRO","Production");
    cdelegate->insertItem("DRY","Dry");
    cdelegate->insertItem("PRE","Pre-calving");
    cdelegate->insertItem("GRO","Growing");
    cdelegate->insertItem("PRD","Pre-weaning");
    cdelegate->insertItem("DEV","Development");
    cdelegate->insertItem("FAT","Fattering");
    cdelegate->insertItem("EMP","Mating");
    cdelegate->insertItem("ONE","One group");
    ui->AdvListView2->setItemDelegateForColumn(3,cdelegate);

    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
}

void feedgroups::dataChanged()
{
    applyModel = true;
    ui->BitBtn3->setEnabled(true);
    ui->BitBtn4->setEnabled(true);

    QModelIndex index;
    index = ui->AdvListView2->currentIndex();

    if (index.isValid())
    {
        if ((m_fgroupsModel->getRowStatus(index.row()) != "D") &&
            (m_fgroupsModel->getRowStatus(index.row()) != "N"))
        {
            QString ccriteria;
            QString pcriteria;
            ccriteria = m_fgroupsModel->getDataFromField(index.row(),"GROUP_CRIE").toString();
            pcriteria = m_fgroupsModel->getOldDataFromField(index.row(),"GROUP_CRIE").toString();

            if (ccriteria != pcriteria)
            {
                disableFields();
                enableFields(ccriteria);
            }
        }
        else
        {
            disableFields();
        }
    }
    else
    {
        disableFields();
    }
}

void feedgroups::insertNew()
{
    m_fgroupsModel->insertNewItem();
}

void feedgroups::deleteCurrent()
{
    QModelIndex index;
    index = ui->AdvListView2->currentIndex();
    if (index.isValid())
    {
        m_fgroupsModel->deleteItem(index.row());
    }
}

void feedgroups::applyChanges()
{
    if (applyModel)
    {
        if (m_fgroupsModel->applyChanges())
        {
            ui->BitBtn3->setEnabled(false);
            ui->BitBtn4->setEnabled(false);
            ui->AdvListView1->setEnabled(true);
            applyModel = false;
        }
        else
            return;
    }
    if (applyForm)
    {
        impgenmaint::applyChanges();
        ui->BitBtn3->setEnabled(false);
        ui->BitBtn4->setEnabled(false);
        ui->AdvListView1->setEnabled(true);
        ui->AdvListView2->setEnabled(true);
        applyForm = false;
    }
}

void feedgroups::cancelChanges()
{
    if (applyModel)
    {
        m_fgroupsModel->cancelChanges();
    }
    if (applyForm)
    {
        impgenmaint::cancelChanges();
    }
    ui->AdvListView1->setEnabled(true);
    ui->AdvListView2->setEnabled(true);
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn4->setEnabled(false);
    applyModel = false;
    applyForm = false;
}

void feedgroups::valueHasChanged(QString, QString, QString)
{
    applyForm = true;
    ui->AdvListView1->setEnabled(false);
    ui->BitBtn3->setEnabled(true);
    ui->BitBtn4->setEnabled(true);
    ui->AdvListView2->setEnabled(false);
}

void feedgroups::loadPattern(QModelIndex /*index*/)
{

    /*calvpattern *cvpattern = new calvpattern();
    cvpattern->setTableName("lvstosys");
    cvpattern->setKeyValues(m_liveModel->getRowKeyValues(index.row()));

    QString lvCode;
    QSqlQuery tquery(database);
    QString sql;
    QString lvType;

    lvCode = m_liveModel->getDataFromField(index.row(),"LIVE_COD").toString();
    sql = "SELECT live_tpy FROM livestoc WHERE live_cod = '" + lvCode + "'";
    tquery.exec(sql);
    tquery.first();
    lvType = tquery.value(0).toString();
    cvpattern->setLvType(lvType);
    cvpattern->loadMaintenance(database);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(cvpattern);
    m_dialogWindow.setWindowTitle("Calving pattern");
    m_dialogWindow.exec();
    delete cvpattern;
    cvpattern = 0;*/

}

void feedgroups::loadFeedGDetail(QModelIndex index)
{
    disableFields();
    if ((m_fgroupsModel->getRowStatus(index.row()) != "D") &&
        (m_fgroupsModel->getRowStatus(index.row()) != "N"))
    {
        this->clearKeyFields();
        for (int pos=0; pos <= m_fgroupsModel->getRowKeyValues(index.row()).count()-1;pos++)
        {
            this->insertKeyField(m_fgroupsModel->getRowKeyValues(index.row())[pos].code,
                                 m_fgroupsModel->getRowKeyValues(index.row())[pos].value);

        }
        QString criteria;
        criteria = m_fgroupsModel->getDataFromField(index.row(),"GROUP_CRIE").toString();
        this->loadMaintTable();
        enableFields(criteria);
    }
}

void feedgroups::loadFeedGroups(QModelIndex index)
{    
    //Load the male model
    m_fgroupsModel->loadData(m_liveModel->getRowKeyValues(index.row()));
    //m_fgroupsModel->distributeColums();

    if (m_fgroupsModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_fgroupsModel->index(0,0);
        loadFeedGDetail(start);
        start = m_fgroupsModel->index(0,1);
        ui->AdvListView2->setCurrentIndex(start);

        for (int rsize = 0; rsize <= m_fgroupsModel->rowCount()-1;rsize++)
            ui->AdvListView2->setRowHeight(rsize,listViewRowSize);

    }
    else
        disableFields();

}

void feedgroups::setChildStatus(bool /*status*/)
{
    //Diable or enable the feedg data
}

void feedgroups::disableFields()
{
    ui->Edit1->setEnabled(false);
    ui->Edit2->setEnabled(false);
    ui->Edit3->setEnabled(false);
    ui->Edit4->setEnabled(false);
    ui->Edit5->setEnabled(false);
    ui->Edit6->setEnabled(false);
    ui->Edit7->setEnabled(false);
    ui->Edit8->setEnabled(false);
    ui->Edit9->setEnabled(false);
    ui->Edit10->setEnabled(false);

    ui->Edit1->setText("0");
    ui->Edit2->setText("0");
    ui->Edit3->setText("0");
    ui->Edit4->setText("0");
    ui->Edit5->setText("0");
    ui->Edit6->setText("0");
    ui->Edit7->setText("0");
    ui->Edit8->setText("0");
    ui->Edit9->setText("0");
    ui->Edit10->setText("0");
}

void feedgroups::enableFields(QString criteria)
{
    if (criteria == "PRO")
    {
        ui->Edit1->setEnabled(true);
        ui->Edit2->setEnabled(true);
    }
    if (criteria == "LAC")
    {
        ui->Edit3->setEnabled(true);
        ui->Edit4->setEnabled(true);
    }
    if (criteria == "PAR")
    {
        ui->Edit5->setEnabled(true);
        ui->Edit6->setEnabled(true);
    }
    if (criteria == "AGE")
    {
        ui->Edit7->setEnabled(true);
        ui->Edit8->setEnabled(true);
    }
    if (criteria == "WEI")
    {
        ui->Edit9->setEnabled(true);
        ui->Edit10->setEnabled(true);
    }
    if (criteria == "MIX")
    {
        ui->Edit1->setEnabled(true);
        ui->Edit2->setEnabled(true);
        ui->Edit3->setEnabled(true);
        ui->Edit4->setEnabled(true);
        ui->Edit5->setEnabled(true);
        ui->Edit6->setEnabled(true);
        ui->Edit7->setEnabled(true);
        ui->Edit8->setEnabled(true);
        ui->Edit9->setEnabled(true);
        ui->Edit10->setEnabled(true);
    }
}
