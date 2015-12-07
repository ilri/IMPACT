#include "calvpattern.h"
#include "ui_calvpattern.h"
#include <QDebug>

calvpattern::calvpattern(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::calvpattern)
{
    ui->setupUi(this);
}

calvpattern::~calvpattern()
{
    //qDebug() << "Before destroy calvpattern UI";
    delete ui;
    //qDebug() << "After destroy calvpattern UI";
}

void calvpattern::loadMaintenance(QSqlDatabase db,QTableView *)
{
    //impgenmaint::loadMaintenance(db);

    m_periodModel = new periodTableModel(this);

    connect(ui->BitBtn1,SIGNAL(clicked()),m_periodModel,SLOT(applyData()));
    connect(ui->BitBtn5,SIGNAL(clicked()),m_periodModel,SLOT(cancelData()));
    //connect(ui->BitBtn13,SIGNAL(clicked()),this,SIGNAL(closeCalled()));


    m_periodModel->setDatabase(db);

    m_periodModel->setTableName("lvstosysperiods");
    if (m_lvType == "S")
        m_periodModel->addField("part12","1 to 2","N","");
    m_periodModel->addField("part23","2 to 3","N","");
    m_periodModel->addField("parto3","Over 3","N","");
    m_periodModel->loadData(m_keys);
    ui->AdvListView4->setModel(m_periodModel);


    int pos;
    for (pos =0; pos <= m_periodModel->columnCount()-1;pos++)
    {
        if (pos < m_periodModel->columnCount()-1)
            ui->AdvListView4->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView4->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

}

void calvpattern::setKeyValues(QList<TmoduleFieldDef> keys)
{
    m_keys.append(keys);
}

void calvpattern::applyChanges()
{
    impgenmaint::applyChanges();
    m_periodModel->applyData();
}

void calvpattern::cancelChanges()
{
    impgenmaint::cancelChanges();
    m_periodModel->cancelData();
}

void calvpattern::setLvType(QString lvtype)
{
    m_lvType = lvtype;
}

void calvpattern::on_BitBtn13_clicked()
{
    emit closeCalled();
}
