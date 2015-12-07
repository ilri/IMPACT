#include "hhconsprodslkp.h"
#include "ui_hhconsprodslkp.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>



hhProdsModel::hhProdsModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
}

int hhProdsModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}
int hhProdsModel::columnCount(const QModelIndex &) const
{
    return 4;
}
QVariant hhProdsModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return m_items[index.row()].type;
        if (index.column() == 1)
            return m_items[index.row()].desc;
        if (index.column() == 2)
            return m_items[index.row()].energy;
        if (index.column() == 3)
            return m_items[index.row()].protein;
    }
    return QVariant();
}
bool hhProdsModel::setData(const QModelIndex &,const QVariant &,int)
{
    return false;
}

Qt::ItemFlags hhProdsModel::flags (const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
QVariant hhProdsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Type";
            if (section == 1)
                return "Product";
            if (section == 2)
                return "Energy (MK/kg)";
            if (section == 3)
                return "Protein (g/kg)";
        }
    }
    return QVariant();
}

void hhProdsModel::setDatabase(QSqlDatabase database)
{
    m_database = database;
}

int hhProdsModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

void hhProdsModel::loadItems()
{
    QSqlQuery query(m_database);
    QString sql;
    sql = "SELECT * from hcompprods ORDER BY prod_typ,prod_des";
    if (query.exec(sql))
    {
        while (query.next())
        {
            ThhProd item;
            item.code = query.value(getFieldIndex(query,"prod_cod")).toString();
            item.type = query.value(getFieldIndex(query,"prod_typ")).toString();
            item.desc = query.value(getFieldIndex(query,"prod_des")).toString();
            item.energy = query.value(getFieldIndex(query,"energy")).toFloat();
            item.protein = query.value(getFieldIndex(query,"protein")).toFloat();
            m_items.append(item);
        }
    }
    else
    {
        qWarning() << "Error: " << query.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

float hhProdsModel::getEnergy(int row)
{
    return m_items[row].energy;
}

float hhProdsModel::getProtein(int row)
{
    return m_items[row].protein;
}

//*****************************************************

hhconsprodslkp::hhconsprodslkp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hhconsprodslkp)
{
    ui->setupUi(this);
    selected = false;
    energy = 0;
    protein = 0;
}

hhconsprodslkp::~hhconsprodslkp()
{
    delete ui;
}

void hhconsprodslkp::setDatabase(QSqlDatabase database)
{
    m_database = database;
}

void hhconsprodslkp::loadData()
{
    m_prodsmodel = new hhProdsModel(this);
    m_prodsmodel->setDatabase(m_database);
    m_prodsmodel->loadItems();

    m_sortModel = new QSortFilterProxyModel(this);
    m_sortModel->setSourceModel(m_prodsmodel);

    ui->tableView->setModel(m_sortModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QSqlQuery query(m_database);
    QString sql;
    sql = "SELECT distinct prod_typ from hcompprods";

    ui->comboBox->addItem("Show all");

    if (query.exec(sql))
    {
        while (query.next())
        {
            ui->comboBox->addItem(query.value(0).toString());
        }
    }
    else
    {
        qWarning() << "Error: " << query.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    ui->comboBox->setCurrentIndex(0);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
}

void hhconsprodslkp::on_pushButton_3_clicked()
{
    this->close();
}

void hhconsprodslkp::on_comboBox_activated(const QString &arg1)
{
    if (arg1 != "Show all")
    {
        m_sortModel->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive,
                                             QRegExp::FixedString));
        m_sortModel->setFilterKeyColumn(0);
    }
    else
    {
        m_sortModel->setFilterRegExp(QRegExp());
        m_sortModel->setFilterKeyColumn(0);
    }
}

void hhconsprodslkp::on_pushButton_2_clicked()
{
    QModelIndex idx;
    idx = ui->tableView->currentIndex();
    idx = m_sortModel->mapToSource(idx);
    energy = m_prodsmodel->getEnergy(idx.row());
    protein = m_prodsmodel->getProtein(idx.row());
    selected = true;
    this->close();
}

