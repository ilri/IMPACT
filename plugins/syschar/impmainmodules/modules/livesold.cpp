#include "livesold.h"
#include "ui_livesold.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>

livesold::livesold(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::livesold)
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

livesold::~livesold()
{
    //qDebug() << "Before destroy livesold UI";
    delete ui;
    //qDebug() << "After destroy livesold UI";
}

void livesold::loadAndroidEffects()
{
    charmprod.activateOn(ui->AdvListView1);
    charmmarks.activateOn(ui->AdvListView2);
    charmdetail.activateOn(ui->AdvListView3);
    connect(ui->AdvListView2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadMarkCalc(QModelIndex)));
    connect(ui->AdvListView3,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadDetCalc(QModelIndex)));
}

void livesold::loadForm()
{

    m_cpprodsModel = new yieldTableModel(this);
    //m_cpprodsModel->setImpactModule(this);

    m_cpprodsModel->setDatabase(dataBase);
    loadProducts();
    ui->AdvListView1->setModel(m_cpprodsModel);
    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadMarketData(QModelIndex)));
    //m_cpprodsModel->distributeColums();

    m_marksModel = new linkedTableModel(this);
    m_marksSortModel = new linkedTableSortModel(this);
    //m_marksModel->setImpactModule(this);

    m_marksModel->setDatabase(dataBase);

    m_marksModel->setLinkedKeyField("MARK_COD");
    m_marksModel->addField("INDRELAT","Use referenced \n unit","F",Qt::white);
    m_marksModel->addField("DESRELAT","Referenced \n unit","");
    m_marksModel->addField("QTYRELAT","Equivalence \n to kgs","0");
    m_marksModel->setLkTableName("markoutlet");
    m_marksModel->setLkKeyField("MARK_COD");
    m_marksModel->setLkDisplayColumn("MARK_DES");
    m_marksModel->setLkDisplayColumnDescription("Marketing outlets");

    connect(m_marksModel,SIGNAL(modelDataChanged()),this,SLOT(marketDataChanged()));
    connect(m_marksModel,SIGNAL(afterInsert(QList<TmoduleInsertedKeys>)),this,SLOT(afterApply(QList<TmoduleInsertedKeys>)));

    m_marksModel->loadItems();
    m_marksSortModel->setSourceModel(m_marksModel);
    ui->AdvListView2->setModel(m_marksSortModel);
    //m_marksModel->distributeColums();
    connect(ui->AdvListView2,SIGNAL(clicked(QModelIndex)),this,SLOT(loadCalendarData(QModelIndex)));


    m_periodModel = new periodTableModel(this);
    m_periodModel->setDatabase(dataBase);
    //m_periodModel->setImpactModule(this);


    m_periodModel->addField("tim","Sold in","N","");
    m_periodModel->addField("qty","Quantity (kg)","0","");
    m_periodModel->addField("pri","Price (KSH)","0",""); //Add currency!!!!
    connect(m_periodModel,SIGNAL(modeldDataChanged()),this,SLOT(calendarDataChanged()));
    ui->AdvListView3->setModel(m_periodModel);

   if (m_cpprodsModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_cpprodsModel->index(0,0);
        loadMarketData(start);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
   {
        setMarketStatus(false);
        setCalendarStatus(false);
    }

   for (int rsize = 0; rsize <= m_cpprodsModel->rowCount()-1;rsize++)
       ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

}

void livesold::loadProducts()
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
    m_cpprodsModel->addMasterTable("livehvst",masterKeys,"Livestock product");
    m_cpprodsModel->addYieldTable("livehvst","livehvsttim","YIELD","PORFS");
    key.code = "HERD_COD";
    masterKeys.append(key);
    m_cpprodsModel->addMasterTable("livehvstcat",masterKeys,"Livestock product by herd structure");
    m_cpprodsModel->addYieldTable("livehvstcat","livehvsttimcat","YIELD","PORFS");


    masterKeys.clear();
    key.code = "SYS_COD";
    key.value = currentSystem;
    masterKeys.append(key);
    m_cpprodsModel->loadItems(masterKeys);


}

void livesold::constructCustomDelegator()
{

    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setManagementModel(m_marksModel);
    ckhdelegate->setSortModel(m_marksSortModel);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ui->AdvListView2->setItemDelegateForColumn(1,ckhdelegate);

    imageCheckDelegate *ckhdelegate2 = new imageCheckDelegate(this);
    ckhdelegate2->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate2->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate2->addIgnoredColumn(0);

    ui->AdvListView3->setItemDelegateForRow(0,ckhdelegate2);

    for (int pos =0; pos <= m_periodModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView3->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView3->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->AdvListView2->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

}

void livesold::loadMarketData(QModelIndex index)
{
    if (!index.isValid())
        return;
    if (m_cpprodsModel->getMasterTable(index.row()) != "livehvstcat")
        m_marksModel->setTableName(m_cpprodsModel->getMasterTable(index.row()) + "sold");
    else
        m_marksModel->setTableName("livehvstsoldcat");
    m_marksModel->setKeyFields(m_cpprodsModel->getRowKeyValues(index.row()));
    m_marksModel->checkItems();
    m_marksSortModel->sort(1);
    //m_marksModel->distributeColums();
    if (m_cpprodsModel->getMasterTable(index.row()) != "livehvstcat")
        m_periodModel->setTableName(m_cpprodsModel->getMasterTable(index.row()) + "soldperiods");
    else
        m_periodModel->setTableName("livehvstsoldcatperiods");

    if (m_marksSortModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_marksSortModel->index(0,0);
        loadCalendarData(start);
        ui->AdvListView2->setCurrentIndex(start);
    }
    else
    {
        setCalendarStatus(false);
    }

    for (int rsize = 0; rsize <= m_marksModel->rowCount()-1;rsize++)
        ui->AdvListView2->setRowHeight(rsize,listViewRowSize);
}

void livesold::loadCalendarData(QModelIndex index)
{
    QModelIndex idx;
    idx = m_marksSortModel->mapToSource(index);
    if (!idx.isValid())
        return;
    if ((m_marksModel->getItemStatus(idx.row()) != "N") &&
        (m_marksModel->getItemStatus(idx.row()) != "D"))
    {
        m_periodModel->loadData(m_marksModel->getRowKeyValues(idx.row()));
        //m_periodModel->distributeColums();
        ui->AdvListView3->setEnabled(true);
    }
    else
    {
        m_periodModel->loadData(m_marksModel->getRowKeyValues(idx.row()));
        //m_periodModel->distributeColums();
        ui->AdvListView3->setEnabled(false);
    }

    for (int rsize = 0; rsize <= m_periodModel->rowCount()-1;rsize++)
            ui->AdvListView3->setRowHeight(rsize,listViewRowSize);
}

void livesold::setProductsStatus(bool status)
{
    ui->AdvListView1->setEnabled(status);
}

void livesold::setCalendarStatus(bool status)
{
    ui->AdvListView3->setEnabled(status);
    ui->BitBtn1->setEnabled(status);
    ui->BitBtn2->setEnabled(status);
}

void livesold::setMarketStatus(bool status)
{
    ui->AdvListView2->setEnabled(status);
    ui->BitBtn3->setEnabled(status);
    ui->BitBtn4->setEnabled(status);
}

void livesold::calendarDataChanged()
{
    setProductsStatus(false);
    setMarketStatus(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn2->setEnabled(true);
}

void livesold::marketDataChanged()
{
   setProductsStatus(false);
   ui->BitBtn3->setEnabled(true);
   ui->BitBtn4->setEnabled(true);
}

void livesold::afterApply(QList<TmoduleInsertedKeys> insertedKeys)
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
            sql = "INSERT INTO " + m_periodModel->getTableName() + " (";
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

void livesold::on_BitBtn3_clicked()
{
    if (m_marksModel->applyChanges())
    {
        setProductsStatus(true);
        ui->BitBtn3->setEnabled(false);
        ui->BitBtn4->setEnabled(false);
        ui->BitBtn1->setEnabled(false);
        ui->BitBtn2->setEnabled(false);
        QModelIndex idx;
        idx = ui->AdvListView2->currentIndex();
        loadCalendarData(idx);
    }
}

void livesold::on_BitBtn4_clicked()
{
    m_marksModel->cancelChanges();
    setProductsStatus(true);
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn4->setEnabled(false);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
    QModelIndex idx;
    idx = ui->AdvListView2->currentIndex();
    loadCalendarData(idx);
}

void livesold::on_BitBtn1_clicked()
{
    m_periodModel->applyData();
    setProductsStatus(true);
    setMarketStatus(true);
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn4->setEnabled(false);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
}

void livesold::on_BitBtn2_clicked()
{
    m_periodModel->cancelData();
    setProductsStatus(true);
    setMarketStatus(true);
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn4->setEnabled(false);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
}

void livesold::loadMarkCalc (const QModelIndex &index)
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
        m_cpprodsModel->setData(index,calc.getCurrentDisplay());
    }
}

void livesold::loadDetCalc (const QModelIndex &index)
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