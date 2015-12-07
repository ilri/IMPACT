#include "plotsgps.h"
#include "ui_plotsgps.h"
#include <calcdialog.h>
#include "moduleglobal.h"

plotsgps::plotsgps(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::plotsgps)
{
    ui->setupUi(this);
}

plotsgps::~plotsgps()
{
    //qDebug() << "Before destroy plotsgps UI";
    delete ui;
    //qDebug() << "After destroy plotsgps UI";
}

void plotsgps::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView2);
    connect(ui->AdvListView2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalc(QModelIndex)));
}

void plotsgps::loadModel(QSqlDatabase dataBase)
{
    m_colModel = new fieldInColModel(this);
    m_colModel->setDatabase(dataBase);
    m_colModel->setTableName("plotgpspos");


    m_colModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_colModel->insertField("PLOT_COD","Plot ID","000","000",true,true,false);
    m_colModel->insertField("POINT_COD","Point","000","000",true,true,true);
    m_colModel->insertField("LATITUDE","Latitude (°)","0","0",false,false,true);
    m_colModel->insertField("LONGITUDE","Longitude (°)","0","0",false,false,true);


    connect(ui->BitBtn16,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn17,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->BitBtn18,SIGNAL(clicked()),this,SLOT(insertItem()));
    connect(ui->BitBtn15,SIGNAL(clicked()),this,SLOT(deleteItem()));
    connect(ui->BitBtn21,SIGNAL(clicked()),this,SIGNAL(closeCalled()));
    connect(m_colModel,SIGNAL(modeldDataChanged()),this,SLOT(valueChanged()));

    m_colModel->loadData(m_keys);
    ui->AdvListView2->setModel(m_colModel);

    //m_colModel->distributeColums();
    if (modules_use_Android == true)
        loadAndroidEffects();

    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    for (int rsize = 0; rsize <= m_colModel->rowCount()-1;rsize++)
        ui->AdvListView2->setRowHeight(rsize,listViewRowSize);
}

void plotsgps::setKeyValues(QList<TmoduleFieldDef> keys)
{
    m_keys.append(keys);
}

void plotsgps::valueChanged()
{
    ui->BitBtn16->setEnabled(true);
    ui->BitBtn17->setEnabled(true);
}

void plotsgps::deleteItem()
{
    if (ui->AdvListView2->currentIndex().isValid())
    {
        m_colModel->deleteItem(ui->AdvListView2->currentIndex().row());
    }
    ui->BitBtn16->setEnabled(true);
    ui->BitBtn17->setEnabled(true);
}

void plotsgps::insertItem()
{
    m_colModel->insertNewItem();
    ui->BitBtn16->setEnabled(true);
    ui->BitBtn17->setEnabled(true);
}

void plotsgps::applyChanges()
{
    if (m_colModel->applyChanges())
    {
        ui->BitBtn16->setEnabled(false);
        ui->BitBtn17->setEnabled(false);
    }
}

void plotsgps::cancelChanges()
{
    m_colModel->cancelChanges();
    ui->BitBtn16->setEnabled(false);
    ui->BitBtn17->setEnabled(false);
}

void plotsgps::loadCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if (index.column() > 0)
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(database);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_colModel->setData(index,calc.getCurrentDisplay());
    }
}
