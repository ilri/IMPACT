#include "farmassets.h"
#include "ui_farmassets.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"

farmassets::farmassets(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::farmassets)
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
    if (modules_use_Android == true)
        loadAndroidEffects();
}

farmassets::~farmassets()
{
    //qDebug() << "Before destroy farmassets UI";
    delete ui;
    //qDebug() << "After destroy farmassets UI";
}

void farmassets::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView1);
    connect(ui->AdvListView1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalculator(QModelIndex)));
}

void farmassets::loadForm()
{
    m_colModel = new fieldInColModel(this);
    m_colModel->setDatabase(dataBase);
    //m_colModel->setImpactModule(this);
    m_colModel->setTableName("farmassets");


    m_colModel->insertExtraColumn("STS","  ","","","","");
    m_colModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_colModel->insertField("PROD_COD","Product ID","000","000",true,true,false);
    m_colModel->insertField("PROD_DES","Asset","Set a description","Set a description",false,false,true);
    m_colModel->insertField("NOWNFEMALE","Number own \n by women","0","0",false,false,true);
    m_colModel->insertField("NOWNMALE","Number own \n by men","0","0",false,false,true);
    m_colModel->insertField("NOWNBOTH","Number own \n by both","0","0",false,false,true);
    m_colModel->insertField("NOWNOTHERS","Number own \n by others","0","0",false,false,true);
    m_colModel->insertField("TOTVALUE","Initial cost \n (KSH)","0","0",false,false,true);
    m_colModel->insertField("PRCHYEAR","Year of \n Purchase","0","0",false,false,true);
    m_colModel->insertField("DISPVALUE","Disposal \n value","0","0",false,false,true);
    m_colModel->insertField("ANUMANT","Anual \n maintenance \n cost (KSH)","0","0",false,false,true);

    m_colModel->addReadOnlyField("STS","ALL");

    m_colModel->addDistributionException(0);

    connect(m_colModel,SIGNAL(modeldDataChanged()),this,SLOT(dataChanged()));


    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_colModel->loadData(keys);
    ui->AdvListView1->setModel(m_colModel);

    //m_colModel->distributeColums();

    if (m_colModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_colModel->index(0,0);
        start = m_colModel->index(0,1);
        ui->AdvListView1->setCurrentIndex(start);
    }

    for (int pos = 0; pos <= m_colModel->columnCount()-1;pos++)
    {
        if ((pos == 0) || (pos == 1))
            ui->AdvListView1->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView1->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    for (int rsize = 0; rsize <= m_colModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);
}

void farmassets::dataChanged()
{
    ui->BitBtn9->setEnabled(true);
    ui->BitBtn10->setEnabled(true);
}

void farmassets::on_BitBtn14_clicked()
{
    m_colModel->insertNewItem();
}

void farmassets::on_BitBtn15_clicked()
{
    QModelIndex idx;
    idx = ui->AdvListView1->currentIndex();
    if (idx.isValid())
    {
        m_colModel->deleteItem(idx.row());
    }
}

void farmassets::on_BitBtn9_clicked()
{
    if (m_colModel->applyChanges())
    {

        ui->BitBtn9->setEnabled(false);
        ui->BitBtn10->setEnabled(false);
    }
}

void farmassets::on_BitBtn10_clicked()
{
    m_colModel->cancelChanges();

    ui->BitBtn9->setEnabled(false);
    ui->BitBtn10->setEnabled(false);
}


void farmassets::loadCalculator (const QModelIndex &index)
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
        m_colModel->setData(index,calc.getCurrentDisplay());
    }
}

