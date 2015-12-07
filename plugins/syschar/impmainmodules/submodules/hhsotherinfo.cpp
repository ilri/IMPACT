#include "hhsotherinfo.h"
#include "ui_hhsotherinfo.h"
#include "delegates/imagecheckdelegate.h"

hhsotherinfo::hhsotherinfo(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::hhsotherinfo)
{
    ui->setupUi(this);
    connect(ui->BitBtn1,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn17,SIGNAL(clicked(bool)),this,SLOT(cancelChanges()));
    connect(ui->BitBtn21,SIGNAL(clicked()),this,SIGNAL(closeCalled()));
}

hhsotherinfo::~hhsotherinfo()
{
    //qDebug() << "Before destroy hhsotherinfo UI";
    delete ui;
    //qDebug() << "After destroy hhsotherinfo UI";
}

void hhsotherinfo::loadMaintenance(QSqlDatabase db,QTableView *)
{
    impgenmaint::loadMaintenance(db);

    m_periodModel = new periodTableModel(this);
    m_periodModel->setDatabase(db);
    //m_periodModel->setImpactModule(this);

    m_periodModel->setTableName("labforceperiods");
    m_periodModel->addField("wrk","Work in the farm","N","");
    m_periodModel->loadData(m_keys);
    ui->tableView->setModel(m_periodModel);
    //m_periodModel->distributeColums();

    constructCustomDelegator();

}

void hhsotherinfo::constructCustomDelegator()
{

    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate->addIgnoredColumn(0);

    ui->tableView->setItemDelegateForRow(0,ckhdelegate);

    for (int pos =0; pos <= m_periodModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->tableView->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->tableView->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

}

void hhsotherinfo::setKeyValues(QList<TmoduleFieldDef> keys)
{
    m_keys.append(keys);
}

void hhsotherinfo::applyChanges()
{
    impgenmaint::applyChanges();
    m_periodModel->applyData();
}

void hhsotherinfo::cancelChanges()
{
    impgenmaint::cancelChanges();
    m_periodModel->cancelData();
}
