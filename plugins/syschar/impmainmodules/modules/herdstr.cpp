#include "herdstr.h"
#include "ui_herdstr.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/labeldelegate.h"
#include "submodules/calvpattern.h"
#include "modulesubscreen.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <calcdialog.h>
#include "moduleglobal.h"

herdstr::herdstr(QWidget *parent, QSqlDatabase db, QString systemID) :
    impgenmaint(parent),
    ui(new Ui::herdstr)
{
    ui->setupUi(this);

    database = db;
    currentSystem = systemID;

    m_liveModel = new fieldInColModel(this);
    m_femaleModel = new fieldInColModel(this);
    m_maleModel = new fieldInColModel(this);

    connect(ui->BitBtn3,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn4,SIGNAL(clicked()),this,SLOT(cancelChanges()));


    connect(m_femaleModel,SIGNAL(modeldDataChanged()),this,SLOT(femaleDataChanged()));
    connect(m_maleModel,SIGNAL(modeldDataChanged()),this,SLOT(maleDataChanged()));


    connect(ui->Edit1,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit2,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit3,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit4,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit5,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit6,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit7,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    applyMaleModel = false;
    applyFemaleModel = false;
    loadMaintenance(database);
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

herdstr::~herdstr()
{
    //qDebug() << "Before destroy herdstr UI";
    delete ui;
    //qDebug() << "After destroy herdstr UI";
}

void herdstr::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView1);
    connect(ui->AdvListView2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadFemaleCalc(QModelIndex)));
    connect(ui->AdvListView3,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadMaleCalc(QModelIndex)));
}

void herdstr::loadMaintenance(QSqlDatabase, QTableView *)
{
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
    QString wsql;
    wsql = "LIVE_COD NOT IN (SELECT LIVE_COD FROM livestoc WHERE LIVE_TPY = 'O')";
    m_liveModel->setWhereClause(wsql);
    m_liveModel->setReadOnly(true);
    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_liveModel->loadData(keys);
    ui->AdvListView1->setModel(m_liveModel);

    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);

    //m_liveModel->distributeColums();
    //Creates the female model
    m_femaleModel->setDatabase(database);
    //m_femaleModel->setImpactModule(this);
    m_femaleModel->setTableName("lvstosysherdstr");

    ui->AdvListView2->setModel(m_femaleModel);
    //Creates the male model
    m_maleModel->setDatabase(database);
    //m_maleModel->setImpactModule(this);
    m_maleModel->setTableName("lvstosysherdstr");

    m_maleModel->insertExtraColumn("STS","  ","","","","");
    m_maleModel->insertField("SYS_COD","Sys Cod","000","000",true,false,false);
    m_maleModel->insertField("LIVE_ICOD","Live ID","000","000",true,true,false);
    m_maleModel->insertField("herd_cod","Age \n (Years)","CT","CT",true,false,true,true);
    m_maleModel->insertField("NANIMAL","Number of \n animals","0","0",false,false,true,false);
    m_maleModel->insertField("bw","Body weight \n (kg)","0","0",false,false,true,false);
    m_maleModel->setWhereClause(" ((herd_cod = '1') OR (herd_cod = '3') OR (herd_cod = '5') OR (herd_cod = '7'))");
    m_maleModel->insertField("cvalue","Capital value \n (KsH/animal)","0","0",false,false,true,false); //Add price
    m_maleModel->insertField("mortality","Mortality \n (%)","0","0",false,false,true,false); //Add price
    m_maleModel->addReadOnlyField("STS","ALL");
    m_maleModel->addReadOnlyField("herd_cod","ALL");
    m_maleModel->addDistributionException(0);
    ui->AdvListView3->setModel(m_maleModel);

    //Positions the livestock model
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

void herdstr::constructCustomDelegator()
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

       ui->AdvListView1->setItemDelegateForColumn(0,ldelegate);

       ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
       ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
       ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
       ui->AdvListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

       fixComboDelegate *cdelegate = new fixComboDelegate(this);
       cdelegate->insertItem("2","0  1");
       cdelegate->insertItem("4","1  2");
       cdelegate->insertItem("6","2  3");
       cdelegate->insertItem("8","Over 3");
       cdelegate->insertItem("1","0  1");
       cdelegate->insertItem("3","1  2");
       cdelegate->insertItem("5","2  3");
       cdelegate->insertItem("7","Over 3");
       ui->AdvListView2->setItemDelegateForColumn(1,cdelegate);




       fixComboDelegate *pdelegate = new fixComboDelegate(this);
       pdelegate->insertItem("2","0  1");
       pdelegate->insertItem("4","1  2");
       pdelegate->insertItem("6","2  3");
       pdelegate->insertItem("8","Over 3");
       pdelegate->insertItem("1","0  1");
       pdelegate->insertItem("3","1  2");
       pdelegate->insertItem("5","2  3");
       pdelegate->insertItem("7","Over 3");

       ui->AdvListView3->setItemDelegateForColumn(1,pdelegate);

}



void herdstr::maleDataChanged()
{
    applyMaleModel = true;
    ui->BitBtn3->setEnabled(true);
    ui->BitBtn4->setEnabled(true);
}

void herdstr::femaleDataChanged()
{
    applyFemaleModel = true;
    ui->BitBtn3->setEnabled(true);
    ui->BitBtn4->setEnabled(true);
}

void herdstr::applyChanges()
{
    if (applyFemaleModel)
    {
        if (m_femaleModel->applyChanges())
        {
            ui->BitBtn3->setEnabled(false);
            ui->BitBtn4->setEnabled(false);
            ui->AdvListView1->setEnabled(true);
            applyFemaleModel = false;
        }
        else
            return;
    }
    if (applyMaleModel)
    {
        if (m_maleModel->applyChanges())
        {
            ui->BitBtn3->setEnabled(false);
            ui->BitBtn4->setEnabled(false);
            ui->AdvListView1->setEnabled(true);
            applyMaleModel = false;
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
        applyForm = false;
    }
}

void herdstr::cancelChanges()
{
    if (applyFemaleModel)
    {
        m_femaleModel->cancelChanges();
    }
    if (applyMaleModel)
    {
        m_maleModel->cancelChanges();
    }
    if (applyForm)
    {
        impgenmaint::cancelChanges();
    }
    ui->AdvListView1->setEnabled(true);
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn4->setEnabled(false);
    applyFemaleModel = false;
    applyMaleModel = false;
    applyForm = false;
}

void herdstr::valueHasChanged(QString, QString, QString)
{
    applyForm = true;
    ui->AdvListView1->setEnabled(false);
    ui->BitBtn3->setEnabled(true);
    ui->BitBtn4->setEnabled(true);
}

void herdstr::loadPattern(QModelIndex index)
{

    calvpattern *cvpattern = new calvpattern(0);
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
}

void herdstr::loadChildData(QModelIndex index)
{
    if (!index.isValid())
        return;




    QString lvCode;

    QSqlQuery tquery(database);
    QString sql;
    QString lType;
    //Creates and loads the female model
    lvCode = m_liveModel->getDataFromField(index.row(),"LIVE_COD").toString();
    sql = "SELECT live_tpy FROM livestoc WHERE live_cod = '" + lvCode + "'";
    tquery.exec(sql);
    tquery.first();
    lType = tquery.value(0).toString();
    m_femaleModel->resetModelStructure();
    m_femaleModel->insertExtraColumn("STS","  ","","","","");
    m_femaleModel->insertField("SYS_COD","Sys Cod","000","000",true,false,false);
    m_femaleModel->insertField("LIVE_ICOD","Live ID","000","000",true,true,false);
    m_femaleModel->insertField("herd_cod","Age \n (Years)","CT","CT",true,false,true,true);
    m_femaleModel->insertField("NANIMAL","Number of \n animals","0","0",false,false,true,false);
    m_femaleModel->insertField("bw","Body weight \n (kg)","0","0",false,false,true,false);
    m_femaleModel->setWhereClause(" ((herd_cod = '2') OR (herd_cod = '4') OR (herd_cod = '6') OR (herd_cod = '8'))");
    if (lType == "R")
    {
        m_femaleModel->insertField("numdry","Number of \n dry animals","0","0",false,false,true,false);
        m_femaleModel->insertField("numlactating","Number of \n lactating animals","0","0",false,false,true,false);
        m_femaleModel->addReadOnlyField("numdry","herd_cod","2","ALL");
        m_femaleModel->addShowValue("numdry","NA","herd_cod","2");
        m_femaleModel->addReadOnlyField("numdry","herd_cod","4","ALL");
        m_femaleModel->addShowValue("numdry","NA","herd_cod","4");

        m_femaleModel->addReadOnlyField("numlactating","herd_cod","2","ALL");
        m_femaleModel->addShowValue("numlactating","NA","herd_cod","2");
        m_femaleModel->addReadOnlyField("numlactating","herd_cod","4","ALL");
        m_femaleModel->addShowValue("numlactating","NA","herd_cod","4");
    }
    else
    {
        m_femaleModel->insertField("perfertile","Percentage of \n fertile animals","0","0",false,false,true,false);
        m_femaleModel->insertField("permilked","Percentage of \n milked animals","0","0",false,false,true,false);
        m_femaleModel->addReadOnlyField("perfertile","herd_cod","2","ALL");
        m_femaleModel->addShowValue("perfertile","NA","herd_cod","2");
        m_femaleModel->addReadOnlyField("permilked","herd_cod","2","ALL");
        m_femaleModel->addShowValue("permilked","NA","herd_cod","2");
    }
    m_femaleModel->insertField("cvalue","Capital value \n (KsH/animal)","0","0",false,false,true,false); //Add price
    m_femaleModel->insertField("mortality","Mortality \n (%)","0","0",false,false,true,false); //Add price
    m_femaleModel->addReadOnlyField("STS","ALL");
    m_femaleModel->addReadOnlyField("herd_cod","ALL");
    m_femaleModel->addDistributionException(0);
    m_femaleModel->loadData(m_liveModel->getRowKeyValues(index.row()));
    //m_femaleModel->distributeColums();

    //Load the male model
    m_maleModel->loadData(m_liveModel->getRowKeyValues(index.row()));
    //m_maleModel->distributeColums();

    this->setDataBase(database);
    this->setTableName("lvstosys");
    this->clearKeyFields();
    for (int pos=0; pos <= m_liveModel->getRowKeyValues(index.row()).count()-1;pos++)
    {
        this->insertKeyField(m_liveModel->getRowKeyValues(index.row())[pos].code,
                             m_liveModel->getRowKeyValues(index.row())[pos].value);

    }
    this->loadMaintTable();

    int rsize;
    for (rsize = 0; rsize <= m_femaleModel->rowCount()-1;rsize++)
        ui->AdvListView2->setRowHeight(rsize,listViewRowSize);

    for (rsize = 0; rsize <= m_maleModel->rowCount()-1;rsize++)
        ui->AdvListView3->setRowHeight(rsize,listViewRowSize);

    int pos;
    for (pos =0; pos <= m_femaleModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }


    for (pos =0; pos <= m_maleModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView3->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView3->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

}

void herdstr::setChildStatus(bool /*status*/)
{

}

void herdstr::on_Button5_clicked()
{
    QModelIndex start;
    start = ui->AdvListView1->currentIndex();
    loadPattern(start);
}


void herdstr::loadFemaleCalc (const QModelIndex &index)
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
        calc.setDatabase(database);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_femaleModel->setData(index,calc.getCurrentDisplay());
    }
}

void herdstr::loadMaleCalc (const QModelIndex &index)
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
        calc.setDatabase(database);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_maleModel->setData(index,calc.getCurrentDisplay());
    }
}
