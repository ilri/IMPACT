#include "feedingothprods.h"
#include "ui_feedingothprods.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>


feedingothprods::feedingothprods(QWidget *parent, QSqlDatabase db, QString systemID) :
    impgenmaint(parent),
    ui(new Ui::feedingothprods)
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
    changes = false;
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

feedingothprods::~feedingothprods()
{
    //qDebug() << "Before destroy feedingothprods UI";
    delete ui;
    //qDebug() << "After destroy feedingothprods UI";
}

void feedingothprods::loadAndroidEffects()
{
    charmProds.activateOn(ui->AdvListView1);
    charmDetail.activateOn(ui->AdvListView2);
    connect(ui->AdvListView2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadDetCalc(QModelIndex)));
}

void feedingothprods::loadForm()
{

    m_prodsModel = new linkedTableModel(this);
    m_prodsSortModel = new linkedTableSortModel(this);

    //m_prodsModel->setImpactModule(this);

    m_prodsModel->setDatabase(dataBase);
    m_prodsModel->setTableName("liveavailres");

    m_prodsModel->setLinkedKeyField("FEED_COD");
    m_prodsModel->addField("COLLECTED","Collected / \n gathered","N",Qt::white);
    m_prodsModel->addField("DRYMATTER","Dry matter \n (%)","0");
    m_prodsModel->addField("CPROTEIN","Crude protein \n (%)","0");
    m_prodsModel->addField("DETERFIBRE","Neutral \n detergent \n fibre (%)","0");
    m_prodsModel->addField("DIGESTI","Digestibility \n (%)","0");
    m_prodsModel->addField("ENERGY","Energy \n (MJ/kg)","0");
    m_prodsModel->setLkTableName("restypes");
    m_prodsModel->setLkKeyField("RES_COD");
    m_prodsModel->setLkDisplayColumn("RES_DES");
    m_prodsModel->setLkDisplayColumnDescription("Purchased feeds");

    connect(m_prodsModel,SIGNAL(modelDataChanged()),this,SLOT(dataChanged()));
    connect(m_prodsModel,SIGNAL(afterInsert(QList<TmoduleInsertedKeys>)),this,SLOT(afterApply(QList<TmoduleInsertedKeys>)));

    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_prodsModel->setKeyFields(keys);

    m_prodsModel->loadItems();
    m_prodsModel->checkItems();
    m_prodsSortModel->setSourceModel(m_prodsModel);
    m_prodsSortModel->sort(1);
    ui->AdvListView1->setModel(m_prodsSortModel);

    //m_prodsModel->distributeColums();

    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadChildData(QModelIndex)));

    m_periodModel = new periodTableModel(this);
    m_periodModel->setDatabase(dataBase);
    //m_periodModel->setImpactModule(this);

    m_periodModel->setTableName("liveavailresperiods");
    m_periodModel->addField("ava","Availability","N","");
    m_periodModel->addField("qty","Quantity (kg)","0","");
    m_periodModel->addField("pri","Price (KSH)","0",""); //Add currency!!!!
    connect(m_periodModel,SIGNAL(modeldDataChanged()),this,SLOT(childDataChanged()));
    ui->AdvListView2->setModel(m_periodModel);

    if (m_prodsModel->hasCheckedItems())
    {
        QModelIndex start;
        start = m_prodsSortModel->index(0,0);
        qWarning() << "Loading child data";
        loadChildData(start);
        qWarning() << "Set current index";
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
        setChildStatus(false);

    for (int rsize = 0; rsize <= m_prodsSortModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

}

void feedingothprods::constructCustomDelegator()
{

    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setManagementModel(m_prodsModel);
    ckhdelegate->setSortModel(m_prodsSortModel);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ui->AdvListView1->setItemDelegateForColumn(1,ckhdelegate);

    imageCheckDelegate *ckhdelegate2 = new imageCheckDelegate(this);
    ckhdelegate2->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate2->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate2->addIgnoredColumn(0);

    ui->AdvListView2->setItemDelegateForRow(0,ckhdelegate2);

    for (int pos =0; pos <= m_periodModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);

}

void feedingothprods::loadChildData(QModelIndex index)
{
    if (ui->AdvListView2->isEnabled())
    {
        QModelIndex idx;
        idx = m_prodsSortModel->mapToSource(index);
        if (!idx.isValid())
            return;
        if ((m_prodsModel->getItemStatus(idx.row()) != "N") ||
            (m_prodsModel->getItemStatus(idx.row()) != "D"))
        {            
            m_periodModel->loadData(m_prodsModel->getRowKeyValues(idx.row()));
            //m_periodModel->distributeColums();
        }

        for (int rsize = 0; rsize <= m_periodModel->rowCount()-1;rsize++)
            ui->AdvListView2->setRowHeight(rsize,listViewRowSize);
    }
}

void feedingothprods::setChildStatus(bool status)
{
    ui->AdvListView2->setEnabled(status);
    ui->BitBtn11->setEnabled(status);
    ui->BitBtn12->setEnabled(status);
}

void feedingothprods::setParentStatus(bool status)
{
    ui->AdvListView1->setEnabled(status);
    ui->BitBtn9->setEnabled(status);
    ui->BitBtn10->setEnabled(status);
}

void feedingothprods::childDataChanged()
{
    setParentStatus(false);
    ui->BitBtn11->setEnabled(true);
    ui->BitBtn12->setEnabled(true);
}

void feedingothprods::dataChanged()
{
    setChildStatus(false);
    ui->BitBtn9->setEnabled(true);
    ui->BitBtn10->setEnabled(true);
}

void feedingothprods::on_BitBtn9_clicked()
{
    if (m_prodsModel->applyChanges())
    {
        setChildStatus(true);
        ui->BitBtn11->setEnabled(false);
        ui->BitBtn12->setEnabled(false);
        changes = true;
    }
}

void feedingothprods::on_BitBtn10_clicked()
{
    m_prodsModel->cancelChanges();
    setChildStatus(true);
    ui->BitBtn11->setEnabled(false);
    ui->BitBtn12->setEnabled(false);
}

void feedingothprods::on_BitBtn8_clicked()
{
    emit closeCalled();
}

void feedingothprods::on_BitBtn11_clicked()
{
    m_periodModel->applyData();
    setParentStatus(true);
    ui->BitBtn9->setEnabled(false);
    ui->BitBtn10->setEnabled(false);
    changes = true;
}

void feedingothprods::on_BitBtn12_clicked()
{
    m_periodModel->cancelData();
    setParentStatus(true);
    ui->BitBtn9->setEnabled(false);
    ui->BitBtn10->setEnabled(false);
}


void feedingothprods::afterApply(QList<TmoduleInsertedKeys> insertedKeys)
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
            sql = "INSERT INTO liveavailresperiods (";
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

void feedingothprods::loadDetCalc (const QModelIndex &index)
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
