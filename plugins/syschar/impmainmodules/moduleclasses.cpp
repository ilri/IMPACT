#include "moduleclasses.h"
#include <qsqlquery.h>
#include <qsqlrecord.h>
#include <qsqlerror.h>
#include "tabledescriptor.h"
#include <QDebug>


//********************************inputsTableModel****************************
inputsTableModel::inputsTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
}

inputsTableModel::~inputsTableModel()
{
    //qDebug() << "Destroy: inputsTableModel";
}

int inputsTableModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int inputsTableModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant inputsTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return m_items[index.row()].description;
        if (index.column() == 1)
            return m_items[index.row()].itemType;
        if (index.column() == 2)
            return m_items[index.row()].currentStatus;
        if (index.column() == 3)
            return m_items[index.row()].totalYield;
    }
    return QVariant();
}

bool inputsTableModel::setData(const QModelIndex &/*index*/,const QVariant &/*value*/,int /*role*/)
{
    return false;
}

Qt::ItemFlags inputsTableModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant inputsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Product";
            if (section == 1)
                return "Type";
            if (section == 2)
                return "Activity";
            if (section == 3)
                return "Quantity (kg/plot)";
        }
    }
    return QVariant();
}

//void inputsTableModel::setImpactModule(impModuleForm *)
//{
//    //m_impModule = module;/
//}

void inputsTableModel::setDatabase (QSqlDatabase db)
{
    database = db;
}

int inputsTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

int inputsTableModel::getMasterIndex(QString masterTable) const
{
    int res;
    res = -1;
    for (int pos =0; pos <= m_manTables.count()-1;pos++)
    {
        if (m_manTables[pos].name == masterTable)
        {
            res = pos;
            break;
        }
    }
    return res;
}

void inputsTableModel::addMasterTable(QString tableName, QList<TmoduleFieldDef> keyFields, QString tableType)
{
    if (getMasterIndex(tableName) == -1)
    {
        mnrManTable table;
        table.name = tableName;
        table.keysFields.append(keyFields);
        table.lookUpTable = "";
        table.lookUpTableType = tableType;
        table.yieldTable = "";
        table.yieldField = "";
        table.percField = "";
        m_manTables.append(table);
    }
    else
    {
        qWarning() << "Error: Adding a table that already exists!";
    }
}

void inputsTableModel::addYieldTable(QString masterTable, QString yieldTable, QString yieldField)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].yieldTable = yieldTable;
        m_manTables[idx].yieldField = yieldField;
        m_manTables[idx].percField = "";
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void inputsTableModel::loadItems(QList<TmoduleFieldDef> keyFields)
{
    int pos;
    int pos2;
    m_items.clear();
    QString sql;
    QSqlQuery lktable(database);
    for (pos = 0; pos <= m_manTables.count()-1; pos++)
    {
        sql = "SELECT ";
        for (pos2 = 0; pos2 <= m_manTables[pos].keysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].name + ".";
            sql = sql + m_manTables[pos].keysFields[pos2].code + ",";
        }

        sql = sql + "SUM(";
        sql = sql + m_manTables[pos].yieldTable + ".";
        sql = sql + m_manTables[pos].yieldField + ") AS TYIELD FROM ";

        sql = sql + m_manTables[pos].name + ",";
        sql = sql + m_manTables[pos].yieldTable + " WHERE ";
        for (pos2 = 0; pos2 <= m_manTables[pos].keysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].name + "." + m_manTables[pos].keysFields[pos2].code + " = ";
            sql = sql + m_manTables[pos].yieldTable + "." + m_manTables[pos].keysFields[pos2].code + " AND ";
        }
        for (pos2 = 0; pos2 <= keyFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].name + "." + keyFields[pos2].code + " = ";
            sql = sql + "'" + keyFields[pos2].value.toString() + "' AND ";
        }
        sql = sql.left(sql.length()-5);
        sql = sql + " GROUP BY ";
        for (pos2 = 0; pos2 <= m_manTables[pos].keysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].name + "." + m_manTables[pos].keysFields[pos2].code + ",";
        }
        sql = sql.left(sql.length()-1);
        sql = sql + " HAVING TYIELD > 0";
        if (lktable.exec(sql))
        {
            while (lktable.next())
            {
                TfeedingManItem item;
                item.masterTable = m_manTables[pos].name;
                for (pos2 = 0; pos2 <= m_manTables[pos].keysFields.count()-1;pos2++)
                {
                    TmoduleFieldDef field;
                    field.code = m_manTables[pos].keysFields[pos2].code;
                    field.value = lktable.value(getFieldIndex(lktable,m_manTables[pos].keysFields[pos2].code));
                    item.keyValues.append(field);
                }

                tableDescriptor tbldesc(this,database);
                item.description = tbldesc.getCustomDescription(m_manTables[pos].name,item.keyValues);

                item.itemType = m_manTables[pos].lookUpTableType;
                item.currentStatus = lktable.value(getFieldIndex(lktable,"acty_cod")).toString();
                item.totalYield = lktable.value(getFieldIndex(lktable,"TYIELD"));
                item.error = false;
                m_items.append(item);
            }
        }
        else
        {
            qWarning() << "Error: " << lktable.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
}

QString inputsTableModel::getSortDescription(QModelIndex /*index*/)
{
    return QString();
}

QString inputsTableModel::getMasterTable(int row)
{
    if ((row >= 0) && (row <= rowCount()-1))
    {
        return m_items[row].masterTable;
    }
    else
        return QString();
}

QList<TmoduleFieldDef> inputsTableModel::getRowKeyValues(int row)
{
    QList<TmoduleFieldDef> res;
    res.append(m_items[row].keyValues);
    return res;
}

//******************************* YieldTableModel********************************
yieldTableModel::yieldTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
    m_calculateYield = true;
}

yieldTableModel::~yieldTableModel()
{
    //qDebug() << "Destroy: yieldTableModel";
}

int yieldTableModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int yieldTableModel::columnCount(const QModelIndex &) const
{
    if (m_calculateYield)
        return 4 + m_manTables[0].fields.count();
    else
    {
        return 2 + m_manTables[0].fields.count();
    }
}

QVariant yieldTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (m_calculateYield)
        {
            if (index.column() == 0)
                return m_items[index.row()].description;
            if (index.column() == 1)
                return m_items[index.row()].itemType;
            if (index.column() == 2)
                return m_items[index.row()].totalYield;
            if (index.column() == 3)
                return m_items[index.row()].totalAllocated;
            if (columnCount() > 4)
            {
                if (index.column() > 3)
                    return m_items[index.row()].currentValues[index.column()-4].value.toString();
            }
        }
        else
        {
            if (index.column() == 0)
                return m_items[index.row()].description;
            if (index.column() == 1)
                return m_items[index.row()].itemType;
            if (columnCount() > 2)
            {
                if (index.column() > 1)
                    return m_items[index.row()].currentValues[index.column()-2].value.toString();
            }
        }
    }

    if (role == Qt::BackgroundRole)
    {
        if (m_calculateYield)
        {
            if (index.column() == 3)
            {
                if (m_items[index.row()].totalAllocated != m_items[index.row()].totalYield)
                {
                    QBrush Background(Qt::yellow);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
            }
            if (index.column() > 3)
            {
                QBrush Background(QColor(191,255,191));
                Background.setStyle(Qt::SolidPattern);
                return Background;
            }
        }
        else
        {
            if (index.column() > 1)
            {
                QBrush Background(QColor(191,255,191));
                Background.setStyle(Qt::SolidPattern);
                return Background;
            }
        }
    }

    if (role == Qt::DecorationRole)
    {
        if (m_items[index.row()].currentStatus == "M")
        {
            if (index.column() == 0)
            {
                return QPixmap(":/images/modify2.png");
            }
        }
    }

    return QVariant();
}

bool yieldTableModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (m_calculateYield)
    {
        if (columnCount() > 4)
        {
            if (index.column() > 3)
            {
                if (role == Qt::EditRole)
                {
                    m_items[index.row()].currentValues[index.column()-4].value = value;
                    m_items[index.row()].currentStatus = "M";
                    emit dataChanged(this->index(index.row(),0),this->index(index.row(),this->columnCount()));
                    emit modeldDataChanged();
                    return true;
                }
            }
        }
    }
    else
    {
        if (columnCount() > 2)
        {
            if (index.column() > 1)
            {
                if (role == Qt::EditRole)
                {
                    m_items[index.row()].currentValues[index.column()-2].value = value;
                    m_items[index.row()].currentStatus = "M";
                    emit dataChanged(this->index(index.row(),0),this->index(index.row(),this->columnCount()));
                    emit modeldDataChanged();
                    return true;
                }
            }
        }
    }
    return false;
}

Qt::ItemFlags yieldTableModel::flags(const QModelIndex &index) const
{
    if (m_calculateYield)
    {
        if (columnCount() > 4)
        {
            if (index.column() > 3)
            {
                return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
            }
        }
    }
    else
    {
        if (columnCount() > 2)
        {
            if (index.column() > 1)
            {
                return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
            }
        }
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant yieldTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (m_calculateYield)
            {
                if (section == 0)
                    return "Product";
                if (section == 1)
                    return "Type";
                if (section == 2)
                    return "Total yield \n (kg)";
                if (section == 3)
                    return "Total allocated \n (kg)";
                if (columnCount() > 4)
                {
                    if (section > 3)
                    {
                        return m_manTables[0].fields[section-4].description;
                    }
                }
            }
            else
            {
                if (section == 0)
                    return "Product";
                if (section == 1)
                    return "Type";
                if (columnCount() > 2)
                {
                    if (section > 1)
                    {
                        return m_manTables[0].fields[section-2].description;
                    }
                }
            }
        }
    }
    return QVariant();
}

//void yieldTableModel::setImpactModule(impModuleForm *)
//{
//    //m_impModule = module;
//}

void yieldTableModel::setDatabase (QSqlDatabase db)
{
    database = db;
}

int yieldTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

int yieldTableModel::getMasterIndex(QString masterTable) const
{
    int res;
    res = -1;
    for (int pos =0; pos <= m_manTables.count()-1;pos++)
    {
        if (m_manTables[pos].name == masterTable)
        {
            res = pos;
            break;
        }
    }
    return res;
}

void yieldTableModel::addFieldToMaster(QString masterTable, QString field, QString description, QVariant value)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        TmoduleFieldDef fieldDef;
        fieldDef.code = field;
        fieldDef.description = description;
        fieldDef.defValue = value;
        m_manTables[idx].fields.append(fieldDef);
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void yieldTableModel::addMasterTable(QString tableName, QList<TmoduleFieldDef> keyFields, QString tableType)
{
    if (getMasterIndex(tableName) == -1)
    {
        TyieldManTable table;
        table.name = tableName;
        table.keysFields.append(keyFields);
        table.lookUpTable = "";
        table.lookUpTableType = tableType;
        table.yieldTable = "";
        table.yieldField = "";
        table.allocationTable = "";
        table.allocationField = "";
        table.percField = "";
        m_manTables.append(table);
    }
    else
    {
        qWarning() << "Error: Adding a table that already exists!";
    }
}

void yieldTableModel::addAllocationTable(QString masterTable, QString allocationTable, QString allocationField)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].allocationTable = allocationTable;
        m_manTables[idx].allocationField = allocationField;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void yieldTableModel::addYieldTable(QString masterTable, QString yieldTable, QString yieldField, QString percField)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].yieldTable = yieldTable;
        m_manTables[idx].yieldField = yieldField;
        m_manTables[idx].percField = percField;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

bool yieldTableModel::applyData()
{
    int pos;
    int pos2;
    QString sql;
    QSqlQuery lktable(database);
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus == "M")
        {
            sql = "UPDATE " + m_items[pos].masterTable + " SET ";
            for (pos2 = 0; pos2 <= m_items[pos].currentValues.count()-1;pos2++)
            {
                sql = sql + m_items[pos].currentValues[pos2].code + " = ";
                sql = sql + "'" + m_items[pos].currentValues[pos2].value.toString() + "',";
            }
            sql = sql.left(sql.length()-1);
            sql= sql + " WHERE ";
            for (pos2 = 0; pos2 <= m_items[pos].keyValues.count()-1;pos2++)
            {
                sql = sql + m_items[pos].keyValues[pos2].code + " = ";
                sql = sql + "'" + m_items[pos].keyValues[pos2].value.toString() + "' AND ";
            }
            sql = sql.left(sql.length()-5);
            if (lktable.exec(sql))
            {
                m_items[pos].currentStatus = "C";
                m_items[pos].previousStatus = "C";
                emit dataChanged(this->index(pos,0),this->index(pos,this->columnCount()));
            }
            else
            {
                qWarning() << "Error; " << lktable.lastError().databaseText();
                qWarning() << "Executing: " << sql;
            }
        }
    }
    return true;
}

void yieldTableModel::cancelData()
{
    int pos;
    int pos2;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus == "M")
        {
            for (pos2 = 0; pos2 <= m_items[pos].currentValues.count()-1;pos2++)
            {
                m_items[pos].currentValues[pos2].value = m_items[pos].previousValues[pos2].value;
            }
            m_items[pos].currentStatus = "C";
            m_items[pos].previousStatus = "C";
            emit dataChanged(this->index(pos,0),this->index(pos,this->columnCount()));
        }
    }
}

void yieldTableModel::setCalculateYield(bool calculateYield)
{
    m_calculateYield = calculateYield;
}

void yieldTableModel::updateAllocation()
{
    int pos;
    int idx;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        idx = this->getMasterIndex(m_items[pos].masterTable);
        if (idx >=0)
        {
            if (!m_manTables[idx].allocationTable.isEmpty())
            {
                m_items[pos].totalAllocated = getTotalAllocation(m_manTables[idx].allocationTable,m_manTables[idx].allocationField,m_items[pos].keyValues);
                emit dataChanged(this->index(pos,0),this->index(pos,this->columnCount()));
            }
        }
    }
}

QVariant yieldTableModel::getTotalAllocation(QString table, QString field, QList<TmoduleFieldDef> keyFields)
{
    QString sql;
    QSqlQuery lktable(database);
    sql = "SELECT SUM(";
    sql = sql + field + ") AS TALLOCATED FROM " + table;
    sql = sql + " WHERE ";
    int pos2;
    for (pos2 = 0; pos2 <= keyFields.count()-1;pos2++)
    {
        sql = sql + keyFields[pos2].code + " = '";
        sql = sql + keyFields[pos2].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    if (lktable.exec(sql))
    {
        if (lktable.first())
        {
            return lktable.value(0);
        }
        else
        {
            qWarning() << "No record for quantity assigned. Returning 0";
        }
    }
    else
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        qWarning() << "Returning 0";
    }
    return 0;
}

void yieldTableModel::loadItems(QList<TmoduleFieldDef> keyFields)
{
    int pos;
    int pos2;
    m_items.clear();
    QString sql;
    QSqlQuery lktable(database);
    for (pos = 0; pos <= m_manTables.count()-1; pos++)
    {
        sql = "SELECT ";
        for (pos2 = 0; pos2 <= m_manTables[pos].keysFields.count()-1;pos2++)
        {

            sql = sql + m_manTables[pos].name + ".";

            sql = sql + m_manTables[pos].keysFields[pos2].code + ",";
        }
        for (pos2 = 0; pos2 <= m_manTables[pos].fields.count()-1;pos2++)
        {

            sql = sql + m_manTables[pos].name + ".";

            sql = sql + m_manTables[pos].fields[pos2].code + ",";
        }
        if (m_manTables[pos].percField != "NULL")
        {
            //Impact now uses Kgs
            //sql = sql + "SUM( (cast(";
            //sql = sql + m_manTables[pos].yieldTable + "." + m_manTables[pos].percField + " as real)/100) * " +  m_manTables[pos].yieldTable + ".";
            //sql = sql + m_manTables[pos].yieldField + ") AS TYIELD FROM ";

            sql = sql + "SUM(";
            sql = sql + m_manTables[pos].yieldTable + "." + m_manTables[pos].percField;
            sql = sql + ") AS TYIELD FROM ";
        }
        else
        {
            sql = sql + "SUM(";
            sql = sql + m_manTables[pos].yieldTable + ".";
            sql = sql + m_manTables[pos].yieldField + ") AS TYIELD FROM ";
        }

        sql = sql + m_manTables[pos].name + ",";

        sql = sql + m_manTables[pos].yieldTable + " WHERE ";
        for (pos2 = 0; pos2 <= m_manTables[pos].keysFields.count()-1;pos2++)
        {

            sql = sql + m_manTables[pos].name + "." + m_manTables[pos].keysFields[pos2].code + " = ";

            sql = sql + m_manTables[pos].yieldTable + "." + m_manTables[pos].keysFields[pos2].code + " AND ";
        }
        for (pos2 = 0; pos2 <= keyFields.count()-1;pos2++)
        {

            sql = sql + m_manTables[pos].name + "." + keyFields[pos2].code + " = ";

            sql = sql + "'" + keyFields[pos2].value.toString() + "' AND ";
        }
        sql = sql.left(sql.length()-5);
        sql = sql + " GROUP BY ";
        for (pos2 = 0; pos2 <= m_manTables[pos].keysFields.count()-1;pos2++)
        {

            sql = sql + m_manTables[pos].name + "." + m_manTables[pos].keysFields[pos2].code + ",";

        }
        sql = sql.left(sql.length()-1);
        if (m_calculateYield)
            sql = sql + " HAVING TYIELD > 0";

        if (lktable.exec(sql))
        {
            while (lktable.next())
            {
                TyieldManItem item;
                item.masterTable = m_manTables[pos].name;                
                for (pos2 = 0; pos2 <= m_manTables[pos].keysFields.count()-1;pos2++)
                {
                    TmoduleFieldDef field;
                    field.code = m_manTables[pos].keysFields[pos2].code;
                    field.value = lktable.value(getFieldIndex(lktable,m_manTables[pos].keysFields[pos2].code));
                    item.keyValues.append(field);
                }

                tableDescriptor tbldesc(this,database);
                item.description = tbldesc.getCustomDescription(m_manTables[pos].name,item.keyValues);

                item.itemType = m_manTables[pos].lookUpTableType;
                item.currentStatus = "C";
                item.previousStatus = "C";
                item.currentValues.append(m_manTables[pos].fields);
                item.previousValues.append(m_manTables[pos].fields);
                for (pos2 = 0; pos2 <= m_manTables[pos].fields.count()-1;pos2++)
                {
                    item.currentValues[pos2].value = lktable.value(getFieldIndex(lktable,m_manTables[pos].fields[pos2].code));
                    item.previousValues[pos2].value = item.currentValues[pos2].value;
                }
                item.totalYield = lktable.value(getFieldIndex(lktable,"TYIELD"));
                if (m_manTables[pos].allocationTable.isEmpty())
                {
                    item.totalAllocated = item.totalYield;
                }
                else
                {
                    item.totalAllocated = getTotalAllocation(m_manTables[pos].allocationTable,m_manTables[pos].allocationField,item.keyValues);
                }
                item.error = false;
                m_items.append(item);
            }
        }
        else
        {
            qWarning() << "Error: " << lktable.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
}

QString yieldTableModel::getSortDescription(QModelIndex /*index*/)
{
    return QString();
}

QString yieldTableModel::getMasterTable(int row)
{
    if ((row >= 0) && (row <= rowCount()-1))
    {
        return m_items[row].masterTable;
    }
    else
        return QString();
}

QList<TmoduleFieldDef> yieldTableModel::getRowKeyValues(int row)
{
    QList<TmoduleFieldDef> res;
    res.append(m_items[row].keyValues);
    return res;
}

//************************************cropsTableModel************************************
cropsTableModel::cropsTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    m_isVegOrchad = false;
}

cropsTableModel::~cropsTableModel()
{
    //qDebug() << "Destroy: cropsTableModel";
}

int cropsTableModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

void cropsTableModel::setTableName(QString table)
{
    m_tableName = table;
}

int cropsTableModel::columnCount(const QModelIndex &) const
{
    return nperiods + 2;
}

QVariant cropsTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return m_items[index.row()].description;
        if (index.column() == 1)
        {
            if (!m_isVegOrchad)
                return m_items[index.row()].currentLandCover;
            else
                return "NA";
        }
        if (index.column() > 1)
        {
            return m_items[index.row()].currentValues[index.column()-2].toString();
        }

    }

    if (role == Qt::BackgroundRole)
    {
        if (m_items[index.row()].code != ")FO")
        {
            if (m_items[index.row()].currentStatus == "N")
            {
                QBrush Background(QColor(191,255,191)); //We can change this to a nice color
                return Background;
            }
            else
            {
                QBrush Background(Qt::white);
                return Background;
            }
        }
        else
        {
            QBrush Background(QColor(255,149,14));
            return Background;
        }
    }

    if (role == Qt::DecorationRole)
    {
        if (m_items[index.row()].currentStatus == "M")
        {
            if (index.column() == 0)
            {
                if (m_items[index.row()].previousStatus == "C")
                    return QPixmap(":/images/modify2.png");
                else
                    return QPixmap(":/images/new3.png");
            }
            else
                return QVariant();
        }
        else
        {
            if (m_items[index.row()].error == true)
            {
                if (index.column() == 0)
                    return QPixmap(":/images/rowerror.png");
            }

            if ((m_items[index.row()].currentStatus == "C") &&
                (m_items[index.row()].previousStatus == "N"))
            {
                if (index.column() == 0)
                    return QPixmap(":/images/new3.png");
            }

            if ((m_items[index.row()].currentStatus == "N") &&
                (m_items[index.row()].previousStatus == "C"))
            {
                if (index.column() == 0)
                    return QPixmap(":/images/delete3.png");
            }

            return QVariant();
        }
    }

    if (role == Qt::CheckStateRole)
    {

        if (index.column() == 0)
        {
            if (m_items[index.row()].currentStatus != "N")
            {                
                return Qt::Checked;
            }
            else
            {
                return Qt::Unchecked;
            }
        }
    }
    return QVariant();
}

bool cropsTableModel::hasCheckedItems()
{
    for (int pos = 0; pos <= m_items.count()-1;pos++)
    {
        if ((m_items[pos].currentStatus == "C") ||
            (m_items[pos].currentStatus == "M"))
            return true;
    }
    return false;
}

bool cropsTableModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (role == Qt::EditRole)
    {

        if (index.column() > 1)
        {
            if ((m_items[index.row()].currentStatus == "C") ||
                    (m_items[index.row()].currentStatus == "M"))
            {
                m_items[index.row()].currentStatus = "M";
                m_items[index.row()].currentValues[index.column()-2] = value;
                QModelIndex pix;
                pix = this->index(index.row(),0);
                emit dataChanged(pix,index);
                emit modeldDataChanged();
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (index.column() == 1)
            {
                if (!m_isVegOrchad)
                {
                    m_items[index.row()].currentStatus = "M";
                    m_items[index.row()].currentLandCover = value;
                    QModelIndex pix;
                    pix = this->index(index.row(),0);
                    emit dataChanged(pix,index);
                    emit modeldDataChanged();
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }

    }
    if (role == Qt::CheckStateRole)
    {
        if (m_items[index.row()].currentStatus != "M")
        {
            if (value.toInt() == Qt::Checked)
            {
                if (m_items[index.row()].code != ")FO")
                    m_items[index.row()].currentStatus = "C";
                else
                {
                    if (!hasCheckedItems())
                        m_items[index.row()].currentStatus = "C";
                    else
                        return false;
                }
            }
            else
            {
                m_items[index.row()].currentStatus = "N";
            }
            QModelIndex begin;
            begin = this->index(index.row(),0);
            QModelIndex end;
            end = this->index(index.row(),this->columnCount()-1);
            emit dataChanged(begin,end);
            emit modeldDataChanged();
            return true;
        }
        else
        {
            if (m_items[index.row()].previousStatus == "N")
            {
                m_items[index.row()].currentStatus = "N";
                QModelIndex begin;
                begin = this->index(index.row(),0);
                QModelIndex end;
                end = this->index(index.row(),this->columnCount()-1);
                emit dataChanged(begin,end);
                emit modeldDataChanged();
                return true;
            }
            else
                return false;
        }
    }
    return false;
}

bool cropsTableModel::isPeriodUsed(QString period) const
{
    QString sql;
    QSqlQuery lktable(database);
    int pos;

    sql = "SELECT COUNT(period_id) FROM " + m_tableName + "periods";
    sql = sql + " WHERE ";
    for (pos = 0; pos <= m_keyFields.count()-1;pos++)
    {
        if ( m_keyFields[pos].code.toUpper() != "NUMSEASON")
        {
            sql = sql + m_keyFields[pos].code + " = ";
            sql = sql + "'" + m_keyFields[pos].value.toString() + "' AND ";
        }
        else
        {
            sql = sql + m_keyFields[pos].code + " <> ";
            sql = sql + "'" + m_keyFields[pos].value.toString() + "' AND ";
        }
    }
    if (!m_isVegOrchad)
        sql = sql + "crp = 'Y' AND ";
    else
        sql = sql + "harv = 'Y' AND ";
    sql = sql + "period_id = '" + period + "'";
    if (lktable.exec(sql))
    {
        if (lktable.first())
        {
            if (lktable.value(0).toInt() > 0)
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

Qt::ItemFlags cropsTableModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    }
    else
    {

        if (index.column() == 1)
        {
            if (!m_isVegOrchad)
            {
                if ((m_items[index.row()].currentStatus == "C") ||
                        (m_items[index.row()].currentStatus == "M"))
                    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
                else
                    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
            }
            else
                return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        if (index.column() > 1)
        {
            if (isPeriodUsed(m_periods[index.column()-2].code))
                return Qt::ItemIsSelectable;
            else
                return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        }
    }
    return Qt::ItemIsEnabled;
}

QVariant cropsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Crop/Fallow";
            if (section == 1)
                return "Land cover \n (%)";
            if (section > 1)
                return m_periods[section-2].description;
        }
    }
    if (role == Qt::FontRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section > 1)
            {
                if (!isPeriodUsed(m_periods[section-2].code))
                {
                    QFont font;
                    font.setBold(true);
                    return font;
                }
            }
        }
    }
    return QVariant();
}

void cropsTableModel::setIsVegOrchad(bool isVegOrchad)
{
    m_isVegOrchad = isVegOrchad;
}

QColor cropsTableModel::getColorForRow(int row)
{
    if (m_items[row].code == ")FO")
        return QColor(255,149,14);
    else
    {
        if (m_items[row].currentStatus == "N")
            return QColor(191,255,191);
        else
            return QColor(Qt::white);
    }

}

void cropsTableModel::setDatabase (QSqlDatabase db)
{
    database = db;
    QSqlQuery ptable(database);
    QString sql;
    sql = "SELECT * FROM periods";
    ptable.exec(sql);
    nperiods = 0;
    while (ptable.next())
    {
        TperiodHeaderData period;
        period.code = ptable.value(getFieldIndex(ptable,"period_id")).toString();
        period.description = ptable.value(getFieldIndex(ptable,"period_des")).toString();
        m_periods.append(period);
        nperiods++;
    }
}


int cropsTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

void cropsTableModel::loadItems()
{
    //this->beginResetModel();
    m_items.clear();

    QSqlQuery itable(database);
    QString sql;

    sql = "SELECT crop_cod,crop_des FROM crop WHERE crop_cod not like '(V%'";
    sql = sql + " AND crop_cod not like ')F%'";
    if (m_isVegOrchad)
        sql = sql + " AND vegetable = 'Y'";
    int pos;
    if (itable.exec(sql))
    {
        while (itable.next())
        {
            TcropItem crop;
            crop.code = itable.value(0).toString();
            crop.description = itable.value(1).toString();
            crop.error = false;
            crop.currentLandCover = "0";
            crop.previousLandCover = "0";
            crop.currentStatus = "N";
            crop.previousStatus = "N";
            for (pos = 1; pos <= nperiods; pos++)
            {
                crop.currentValues.append("N");
                crop.previousValues.append("N");
            }
            m_items.append(crop);
        }
        if (!m_isVegOrchad)
        {
            TcropItem crop;
            crop.code = ")FO";
            crop.description = "Fallow";
            crop.error = false;
            crop.currentLandCover = "0";
            crop.previousLandCover = "0";
            crop.currentStatus = "N";
            crop.previousStatus = "N";
            for (pos = 1; pos <= nperiods; pos++)
            {
                crop.currentValues.append("N");
                crop.previousValues.append("N");
            }
            m_items.append(crop);
        }
    }
    else
    {
        qWarning() << "Error: " << itable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    //this->endResetModel();
}

void cropsTableModel::loadPeriodData(int index, QList<TmoduleFieldDef> keyFields)
{
    QSqlQuery mtable(database);
    QString sql;

    if (!m_isVegOrchad)
        sql = "SELECT period_id,crp FROM " + m_tableName + "periods WHERE ";
    else
    {
        sql = "SELECT period_id,harv FROM " + m_tableName + "periods WHERE ";
    }
    int pos;
    for (pos = 0; pos <= keyFields.count()-1;pos++)
    {
        sql = sql + keyFields[pos].code + " = ";
        sql = sql + "'" + keyFields[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    if (mtable.exec(sql))
    {
        while (mtable.next())
        {
            m_items[index].currentValues[mtable.value(0).toInt()-1] = mtable.value(1);
            m_items[index].previousValues[mtable.value(0).toInt()-1] = mtable.value(1);
        }
    }
    else
    {
        qWarning() << "Error: " << mtable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

void cropsTableModel::chkitems(QList<TmoduleFieldDef> keyFields)
{
    this->beginResetModel();

    m_keyFields.clear();
    m_keyFields.append(keyFields);

    QSqlQuery mtable(database);
    QString sql;

    if (!m_isVegOrchad)
        sql = "SELECT crop_cod,landcover FROM " + m_tableName;
    else
        sql = "SELECT veg_cod FROM " + m_tableName;
    sql = sql + " WHERE ";
    int pos;
    for (pos = 0; pos <= keyFields.count()-1;pos++)
    {
        sql = sql + keyFields[pos].code + " = ";
        sql = sql + "'" + keyFields[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    if (mtable.exec(sql))
    {
        QList<TmoduleFieldDef> pkeyFields;
        while (mtable.next())
        {
            for (pos = 0; pos <= m_items.count()-1;pos++)
            {
                if (m_items[pos].code == mtable.value(0).toString())
                {
                    m_items[pos].currentStatus = "C";
                    m_items[pos].previousStatus = "C";
                    if (!m_isVegOrchad)
                    {
                        m_items[pos].currentLandCover = mtable.value(1);
                        m_items[pos].previousLandCover = mtable.value(1);
                    }
                    pkeyFields.clear();
                    pkeyFields.append(keyFields);
                    TmoduleFieldDef cropKey;
                    if (!m_isVegOrchad)
                        cropKey.code = "crop_cod";
                    else
                        cropKey.code = "veg_cod";
                    cropKey.value = mtable.value(0).toString();
                    pkeyFields.append(cropKey);
                    loadPeriodData(pos,pkeyFields);
                    break;
                }
            }
        }
    }
    else
    {
        qWarning() << "Error: " << mtable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    this->endResetModel();
}

QString cropsTableModel::getSortDescription(QModelIndex index)
{
    return m_items[index.row()].currentStatus + m_items[index.row()].description;
}

bool cropsTableModel::getCheckedStatusForRow(int row)
{
    if ((m_items[row].currentStatus == "C") ||
        (m_items[row].currentStatus == "M"))
        return true;
    else
        return false;
}

QString cropsTableModel::getCurrentStatusForRow(int row)
{
    return m_items[row].currentStatus;
}

void cropsTableModel::deleteData(int row)
{
    QString sql;
    int pos;
    QSqlQuery lktable(database);
    sql = "DELETE FROM " + m_tableName;
    sql = sql + " WHERE ";
    for (pos = 0; pos <= m_keyFields.count()-1;pos++)
    {
        sql = sql + m_keyFields[pos].code + " = ";
        sql = sql + "'" +m_keyFields[pos].value.toString() + "' AND ";
    }
    if (!m_isVegOrchad)
    {
        sql = sql + "crop_cod = ";
        sql = sql + "'" + m_items[row].code + "'";
    }
    else
    {
        sql = sql + "veg_cod = ";
        sql = sql + "'" + m_items[row].code + "'";
    }
    if (!lktable.exec(sql))
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        TmoduleErrorDef error;
        error.row = row;
        error.error = lktable.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        updateRow(row,"N");
    }
}

void cropsTableModel::updateData(int row)
{
    QString sql;
    int pos;
    int pos2;
    QSqlQuery lktable(database);

    if (!m_isVegOrchad)
    {
        sql = "UPDATE " + m_tableName + " SET ";
        sql = sql + "landcover = " ;
        sql = sql + m_items[row].currentLandCover.toString();
        sql = sql + " WHERE ";
        for (pos = 0; pos <= m_keyFields.count()-1;pos++)
        {
            sql = sql + m_keyFields[pos].code + " = ";
            sql = sql + "'" + m_keyFields[pos].value.toString() + "' AND ";
        }
        sql = sql + "crop_cod = ";
        sql = sql + "'" + m_items[row].code + "'";

        if (!lktable.exec(sql))
        {
            qWarning() << "Error: " << lktable.lastError().databaseText();
            qWarning() << "Executing: " << sql;
            TmoduleErrorDef error;
            error.row = row;
            error.error = lktable.lastError().databaseText();
            errors.append(error);
        }
    }

    for (pos2 = 1; pos2 <= nperiods; pos2++)
    {
        sql = "UPDATE " + m_tableName + "periods SET ";
        if (!m_isVegOrchad)
            sql = sql + "crp = '" ;
        else
             sql = sql + "harv = '" ;
        sql = sql + m_items[row].currentValues[pos2-1].toString() + "'";
        sql = sql + " WHERE ";
        for (pos = 0; pos <= m_keyFields.count()-1;pos++)
        {
            sql = sql + m_keyFields[pos].code + " = ";
            sql = sql + "'" + m_keyFields[pos].value.toString() + "' AND ";
        }
        if (!m_isVegOrchad)
        {
            sql = sql + "crop_cod = ";
            sql = sql + "'" + m_items[row].code + "' AND ";
        }
        else
        {
            sql = sql + "veg_cod = ";
            sql = sql + "'" + m_items[row].code + "' AND ";
        }

        sql = sql + "period_id = '" + QString::number(pos2) + "'";
        if (!lktable.exec(sql))
        {
            qWarning() << "Error: " << lktable.lastError().databaseText();
            qWarning() << "Executing: " << sql;
            TmoduleErrorDef error;
            error.row = row;
            error.error = lktable.lastError().databaseText();
            errors.append(error);
        }
        else
        {
            updateRow(row,"C");
        }
    }
}

void cropsTableModel::insertData(int row)
{
    QString sql;
    int pos;
    QSqlQuery lktable(database);
    sql = "INSERT INTO " + m_tableName + "(";
    for (pos = 0; pos <= m_keyFields.count()-1;pos++)
    {
        sql = sql + m_keyFields[pos].code + ",";
    }
    if (!m_isVegOrchad)
        sql = sql + "crop_cod";
    else
        sql = sql + "veg_cod";

    if (!m_isVegOrchad)
        sql = sql + ",landcover) VALUES (";
    else
        sql = sql + ") VALUES (";
    for (pos = 0; pos <= m_keyFields.count()-1;pos++)
    {
        sql = sql + "'" + m_keyFields[pos].value.toString() + "',";
    }
    if (!m_isVegOrchad)
    {
        sql = sql + "'" + m_items[row].code + "',";
        sql = sql + m_items[row].currentLandCover.toString() + ")";
    }
    else
        sql = sql + "'" + m_items[row].code + "')";
    if (!lktable.exec(sql))
    {
        TmoduleErrorDef error;
        error.row = row;
        error.error = lktable.lastError().databaseText();
        errors.append(error);
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    else
    {
        int pos2;
        bool errorInChild;
        errorInChild = false;
        for (pos2 = 1; pos2 <= nperiods; pos2++)
        {
            sql = "INSERT INTO " + m_tableName + "periods (";
            for (pos = 0; pos <= m_keyFields.count()-1;pos++)
            {
                sql = sql + m_keyFields[pos].code + ",";
            }
            if (!m_isVegOrchad)
                sql = sql + "crop_cod,";
            else
                sql = sql + "veg_cod,";

            if (!m_isVegOrchad)
                sql = sql + "period_id,crp) VALUES (";
            else
                sql = sql + "period_id,harv) VALUES (";
            for (pos = 0; pos <= m_keyFields.count()-1;pos++)
            {
                sql = sql + "'" + m_keyFields[pos].value.toString() + "',";
            }
            sql = sql + "'" + m_items[row].code + "',";
            sql = sql + "'" + QString::number(pos2) + "',";
            sql = sql + "'" + m_items[row].currentValues[pos2-1].toString() + "'";
            sql = sql + ")";
            if (!lktable.exec(sql))
            {
                TmoduleErrorDef error;
                error.row = row;
                error.error = lktable.lastError().databaseText();
                errors.append(error);
                qWarning() << "Error: " << lktable.lastError().databaseText();
                qWarning() << "Executing: " << sql;
                errorInChild = true;
            }
        }
        if (!errorInChild)
        updateRow(row,"C");
    }
}

bool cropsTableModel::applyData()
{
    int pos;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus != m_items[pos].previousStatus)
        {
            if (m_items[pos].previousStatus == "N")
            {
                insertData(pos);
            }
            if ((m_items[pos].previousStatus == "C") && (m_items[pos].currentStatus == "M"))
            {
                updateData(pos);
            }
            if ((m_items[pos].previousStatus == "C") && (m_items[pos].currentStatus == "N"))
            {
                deleteData(pos);
            }
        }
    }
    if (errors.count() == 0)
    {
        return true;
    }
    else
    {
        //errorDialog error;
        QStringList terror;
        QModelIndex begin;
        QModelIndex end;
        for (pos = 0; pos <= errors.count()-1; pos++)
        {
            m_items[errors[pos].row].error = true;
            begin = this->index(errors[pos].row,0);
            end = this->index(errors[pos].row,this->columnCount()-1);
            dataChanged(begin,end);
            terror << errors[pos].error;
        }
        //error.setErrorMessage(terror);
        //error.exec();
        errors.clear();
        return false;
    }
}

void cropsTableModel::cancelData()
{
    int pos;
    int pos2;
    this->beginResetModel();
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus != m_items[pos].previousStatus)
        {
             m_items[pos].currentStatus = m_items[pos].previousStatus;
             m_items[pos].currentLandCover = m_items[pos].previousLandCover;
             for (pos2 = 1; pos2 <= nperiods; pos2++)
             {
                 m_items[pos].currentValues[pos2-1] =  m_items[pos].previousValues[pos2-1];
             }
        }
    }
    errors.clear();
    this->endResetModel();
}

void cropsTableModel::updateRow(int row,QString status)
{
    m_items[row].currentStatus = status;
    m_items[row].previousStatus = status;
    m_items[row].previousLandCover = m_items[row].currentLandCover;
    if (status == "U")
    {
        for (int pos = 0; pos <= m_items[row].currentValues.count()-1;pos++)
        {
            m_items[row].currentValues[pos] = "F";
            m_items[row].previousValues[pos] = "F";
        }
    }
    else
    {
        for (int pos = 0; pos <= m_items[row].currentValues.count()-1;pos++)
        {
            m_items[row].previousValues[pos] = m_items[row].currentValues[pos];
        }
    }
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,this->columnCount());
    emit dataChanged(begin,end);
}

//***********************************cropsTableSortModel*******************************

cropsTableSortModel::cropsTableSortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool cropsTableSortModel::lessThan(const QModelIndex &left,
                                   const QModelIndex &right) const
{
    if (sourceModel())
    {
        QString leftData = qobject_cast<cropsTableModel *>(sourceModel())->getSortDescription(left);
        QString rightData = qobject_cast<cropsTableModel *>(sourceModel())->getSortDescription(right);
        return QString::localeAwareCompare(leftData, rightData) < 0;
    }
    else
    {
        return false;
    }
}


/************************************feedingTableModel***********************************/

feedingTableModel::feedingTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
}

feedingTableModel::~feedingTableModel()
{
    //qDebug() << "Destroy: feedingTableModel";
}

int feedingTableModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int feedingTableModel::columnCount(const QModelIndex &) const
{
    return nperiods + 3;
}

QVariant feedingTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return m_items[index.row()].description;
        if (index.column() == 1)
            return m_items[index.row()].itemType;
        if (index.column() == 2)
            return m_items[index.row()].totalYield;
        if (index.column() > 2)
        {
            return m_items[index.row()].currentValues[index.column()-3].toString();
        }

    }

    if (role == Qt::DecorationRole)
    {
        if (m_items[index.row()].currentStatus == "M")
        {
            if (index.column() == 0)
            {
                if (m_items[index.row()].previousStatus == "C")
                    return QPixmap(":/images/modify2.png");
                else
                    return QPixmap(":/images/new3.png");
            }
            else
                return QVariant();
        }
        else
        {
            if (m_items[index.row()].error == true)
            {
                if (index.column() == 0)
                    return QPixmap(":/images/rowerror.png");
            }

            if ((m_items[index.row()].currentStatus == "C") &&
                (m_items[index.row()].previousStatus == "U"))
            {
                if (index.column() == 0)
                    return QPixmap(":/images/new3.png");
            }

            if ((m_items[index.row()].currentStatus == "U") &&
                (m_items[index.row()].previousStatus == "C"))
            {
                if (index.column() == 0)
                    return QPixmap(":/images/delete3.png");
            }

            return QVariant();
        }
    }

    if (role == Qt::CheckStateRole)
    {

        if (index.column() == 0)
        {
            if (m_items[index.row()].currentStatus != "U")
            {
                return Qt::Checked;
            }
            else
            {
                return Qt::Unchecked;
            }
        }
    }
    return QVariant();
}

bool feedingTableModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (role == Qt::EditRole)
    {
        if (index.column() > 2)
        {
            if ((m_items[index.row()].currentStatus == "C") ||
                (m_items[index.row()].currentStatus == "M"))
            {
                m_items[index.row()].currentStatus = "M";
                m_items[index.row()].currentValues[index.column()-3] = value;
                QModelIndex pix;
                pix = this->index(index.row(),0);
                emit dataChanged(pix,index);
                emit modeldDataChanged();
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    if (role == Qt::CheckStateRole)
    {
        if (m_items[index.row()].currentStatus != "M")
        {
            if (value.toInt() == Qt::Checked)
            {
                m_items[index.row()].currentStatus = "C";
            }
            else
            {
                m_items[index.row()].currentStatus = "U";
            }
            QModelIndex begin;
            begin = this->index(index.row(),0);
            QModelIndex end;
            end = this->index(index.row(),this->columnCount()-1);
            emit dataChanged(begin,end);
            emit modeldDataChanged();
            return true;
        }
        else
        {
            if (m_items[index.row()].previousStatus == "U")
            {
                m_items[index.row()].currentStatus = "U";
                QModelIndex begin;
                begin = this->index(index.row(),0);
                QModelIndex end;
                end = this->index(index.row(),this->columnCount()-1);
                emit dataChanged(begin,end);
                emit modeldDataChanged();
                return true;
            }
            else
                return false;
        }
    }
    return false;
}

Qt::ItemFlags feedingTableModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    }
    else
    {
        if ((index.column() >= 1) && (index.column() <= 2))
            return Qt::ItemIsEnabled;
        if ((m_items[index.row()].currentStatus == "C") ||
            (m_items[index.row()].currentStatus == "M"))
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        else
            return Qt::ItemIsSelectable;
    }
    return Qt::ItemIsEnabled;
}

QVariant feedingTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Product";
            if (section == 1)
                return "Type";
            if (section == 2)
                return "Total yield \n (kg)";
            if (section > 2)
                return m_periods[section-3].description;
        }
    }
    return QVariant();
}

//void feedingTableModel::setImpactModule(impModuleForm *)
//{
//    //m_impModule = module;
//}

void feedingTableModel::setDatabase (QSqlDatabase db)
{
    database = db;
    QSqlQuery ptable(database);
    QString sql;
    sql = "SELECT * FROM periods";
    ptable.exec(sql);
    nperiods = 0;
    while (ptable.next())
    {
        TperiodHeaderData period;
        period.description = ptable.value(getFieldIndex(ptable,"period_des")).toString();
        m_periods.append(period);
        nperiods++;
    }
}

int feedingTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

int feedingTableModel::getMasterIndex(QString masterTable) const
{
    int res;
    res = -1;
    for (int pos =0; pos <= m_manTables.count()-1;pos++)
    {
        if (m_manTables[pos].name == masterTable)
        {
            res = pos;
            break;
        }
    }
    return res;
}

void feedingTableModel::addMasterTable(QString tableName, QList<TmoduleFieldDef> keyFields)
{
    if (getMasterIndex(tableName) == -1)
    {
        mnrManTable table;
        table.name = tableName;
        table.keysFields.append(keyFields);
        table.lookUpTable = "";
        table.lookUpTableType = "";
        table.yieldTable = "";
        table.yieldField = "";
        table.percField = "";
        m_manTables.append(table);
    }
    else
    {
        qWarning() << "Error: Adding a table that already exists!";
    }
}

void feedingTableModel::addLookUpTable(QString masterTable, QString lkTableName, QString lkTableType,QList<TmoduleFieldDef> keyFields)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].lookUpTable = lkTableName;
        m_manTables[idx].lookUpTableType = lkTableType;
        m_manTables[idx].lkKeysFields.append(keyFields);
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void feedingTableModel::addYieldTable(QString masterTable, QString yieldTable, QString yieldField, QString percField)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].yieldTable = yieldTable;
        m_manTables[idx].yieldField = yieldField;
        m_manTables[idx].percField = percField;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void feedingTableModel::setlkUpColDescription(QString description)
{
    lookUpColDescription = description;
}

void feedingTableModel::loadItems(QList<TmoduleFieldDef> keyFields)
{
    int pos;
    int pos2;

    //this->beginResetModel();

    m_items.clear();
    QString sql;
    QSqlQuery lktable(database);
    for (pos = 0; pos <= m_manTables.count()-1; pos++)
    {
        sql = "SELECT ";
        for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].lookUpTable + ".";
            sql = sql + m_manTables[pos].lkKeysFields[pos2].code + ",";
        }
        if (m_manTables[pos].percField != "NULL")
        {
            // Impact now use kgs
            //sql = sql + "SUM( (cast(";
            //sql = sql + m_manTables[pos].yieldTable + "." + m_manTables[pos].percField + " as real)/100) * " +  m_manTables[pos].yieldTable + ".";
            //sql = sql + m_manTables[pos].yieldField + ") AS TYIELD FROM ";

            sql = sql + "SUM(";
            sql = sql + m_manTables[pos].yieldTable + "." + m_manTables[pos].percField;
            sql = sql + ") AS TYIELD FROM ";
        }
        else
        {
            sql = sql + "SUM(";
            sql = sql + m_manTables[pos].yieldTable + ".";
            sql = sql + m_manTables[pos].yieldField + ") AS TYIELD FROM ";
        }
        sql = sql + m_manTables[pos].lookUpTable + ",";
        sql = sql + m_manTables[pos].yieldTable + " WHERE ";
        for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].lookUpTable + "." + m_manTables[pos].lkKeysFields[pos2].code + " = ";
            sql = sql + m_manTables[pos].yieldTable + "." + m_manTables[pos].lkKeysFields[pos2].code + " AND ";
        }
        for (pos2 = 0; pos2 <= keyFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].lookUpTable + "." + keyFields[pos2].code + " = ";
            sql = sql + "'" + keyFields[pos2].value.toString() + "' AND ";
        }
        sql = sql.left(sql.length()-5);
        sql = sql + " GROUP BY ";
        for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].lookUpTable + "." + m_manTables[pos].lkKeysFields[pos2].code + ",";
        }
        sql = sql.left(sql.length()-1);
        sql = sql + " HAVING TYIELD > 0";
        if (lktable.exec(sql))
        {
            while (lktable.next())
            {
                TfeedingManItem item;
                item.masterTable = m_manTables[pos].name;
                for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
                {
                    TmoduleFieldDef field;
                    field.code = m_manTables[pos].lkKeysFields[pos2].code;
                    field.value = lktable.value(getFieldIndex(lktable,m_manTables[pos].lkKeysFields[pos2].code));
                    item.keyValues.append(field);
                }

                tableDescriptor tbldesc(this,database);
                item.description = tbldesc.getCustomDescription(m_manTables[pos].lookUpTable,item.keyValues);

                item.itemType = m_manTables[pos].lookUpTableType;
                item.currentStatus = "U";
                item.previousStatus = "U";

                for (pos2 = 1; pos2 <= nperiods; pos2++)
                {
                    item.currentValues.append("F");
                    item.previousValues.append("F");
                }

                item.totalYield = lktable.value(getFieldIndex(lktable,"TYIELD"));

                item.error = false;
                m_items.append(item);
            }
        }
        else
        {
            qWarning() << "Error: " << lktable.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
    //this->endResetModel();
}

void feedingTableModel::setPeriodValues(int row, QString sql)
{
    QSqlQuery querylite(database);

    if (querylite.exec(sql))
    {
        while (querylite.next())
        {
            if ((querylite.value(0).toInt() >= 1) &&
                (querylite.value(0).toInt() <= nperiods))
            {
                m_items[row].currentValues[querylite.value(0).toInt()-1] = querylite.value(1).toString();
                m_items[row].previousValues[querylite.value(0).toInt()-1] = querylite.value(1).toString();
            }
        }
    }
    else
    {
        qWarning() << "Error: " << querylite.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

void feedingTableModel::chkitems(QList<TmoduleFieldDef> keyFields)
{
    int pos;
    int pos2;
    int pos3;
    QString sql;
    QSqlQuery lktable(database);

    QString kIstring;
    QString kQstring;
    checkedKeyValues.clear();
    checkedKeyValues.append(keyFields);

    this->beginResetModel();

    for (pos2 = 0; pos2 <= m_items.count()-1;pos2++)
    {
        m_items[pos2].currentStatus = "U";
        m_items[pos2].previousStatus = "U";
        m_items[pos2].currentValues.clear();
        m_items[pos2].previousValues.clear();
        for (pos = 1; pos <= nperiods; pos++)
        {
            m_items[pos2].currentValues.append("F");
            m_items[pos2].previousValues.append("F");
        }
    }

    for (pos = 0; pos <= m_manTables.count()-1; pos++)
    {
        sql = "SELECT ";
        for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].lkKeysFields[pos2].code + ",";
        }
        sql = sql.left(sql.length()-1);
        sql = sql + " FROM " + m_manTables[pos].name + " WHERE ";
        for (pos2 = 0; pos2 <= keyFields.count()-1;pos2++)
        {
            sql = sql + keyFields[pos2].code + " = ";
            sql = sql + "'" + keyFields[pos2].value.toString() + "' AND ";
        }
        sql = sql.left(sql.length()-5);
        if (lktable.exec(sql))
        {
            while (lktable.next())
            {
                kQstring = "";
                for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
                {
                    kQstring = kQstring + lktable.value(getFieldIndex(lktable,m_manTables[pos].lkKeysFields[pos2].code)).toString();
                }

                for (pos2 = 0; pos2 <= m_items.count()-1;pos2++)
                {
                    if (m_items[pos2].masterTable == m_manTables[pos].name)
                    {
                        kIstring = "";
                        for (pos3 = 0; pos3 <= m_items[pos2].keyValues.count()-1;pos3++)
                        {
                            kIstring = kIstring + m_items[pos2].keyValues[pos3].value.toString();
                        }
                        if (kIstring == kQstring)
                        {
                            m_items[pos2].currentStatus = "C";
                            m_items[pos2].previousStatus = "C";

                            sql = "SELECT period_id,tim FROM " + m_manTables[pos].name + "periods";
                            sql = sql + " WHERE ";
                            for (pos3 = 0; pos3 <= m_items[pos2].keyValues.count()-1;pos3++)
                            {
                                sql = sql + m_items[pos2].keyValues[pos3].code + " = ";
                                sql = sql + "'" + m_items[pos2].keyValues[pos3].value.toString() + "' AND ";
                            }
                            for (pos3 = 0; pos3 <= keyFields.count()-1;pos3++)
                            {
                                sql = sql + keyFields[pos3].code + " = ";
                                sql = sql + "'" + keyFields[pos3].value.toString() + "' AND ";
                            }
                            sql = sql.left(sql.length()-5);
                            setPeriodValues(pos2,sql);
                        }
                    }
                }
            }
        }
        else
        {
            qWarning() << "Error: " << lktable.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
    this->endResetModel();
}

QString feedingTableModel::getSortDescription(QModelIndex /*index*/)
{
    return QString();
}

bool feedingTableModel::getCheckedStatusForRow(int row)
{
    if ((m_items[row].currentStatus == "C") ||
        (m_items[row].currentStatus == "M"))
        return true;
    else
        return false;
}

QVariant feedingTableModel::getPeriodValueForRow(int row, int period)
{
    return m_items[row].currentValues[period-1];
}

QString feedingTableModel::getCurrentStatusForRow(int row)
{
    return m_items[row].currentStatus;
}

void feedingTableModel::deleteData(int row)
{
    QString sql;
    int pos;
    QSqlQuery lktable(database);
    sql = "DELETE FROM " + m_items[row].masterTable;
    sql = sql + " WHERE ";
    for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
    {
        sql = sql + checkedKeyValues[pos].code + " = ";
        sql = sql + "'" +checkedKeyValues[pos].value.toString() + "' AND ";
    }
    for (pos = 0; pos <= m_items[row].keyValues.count()-1;pos++)
    {
        sql = sql + m_items[row].keyValues[pos].code + " = ";
        sql = sql + "'" + m_items[row].keyValues[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    //qWarning() << sql;
    if (!lktable.exec(sql))
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        TmoduleErrorDef error;
        error.row = row;
        error.error = lktable.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        updateRow(row,"U");
    }
}

void feedingTableModel::updateData(int row)
{
    QString sql;
    int pos;
    int pos2;
    QSqlQuery lktable(database);

    for (pos2 = 1; pos2 <= nperiods; pos2++)
    {
        sql = "UPDATE " + m_items[row].masterTable + "periods SET ";
        sql = sql + "tim = '" ;
        sql = sql + m_items[row].currentValues[pos2-1].toString() + "'";
        if (m_items[row].currentValues[pos2-1].toString() == "F")
        {
            sql = sql + ", qty = 0";
        }
        sql = sql + " WHERE ";
        for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
        {
            sql = sql + checkedKeyValues[pos].code + " = ";
            sql = sql + "'" +checkedKeyValues[pos].value.toString() + "' AND ";
        }
        for (pos = 0; pos <= m_items[row].keyValues.count()-1;pos++)
        {
            sql = sql + m_items[row].keyValues[pos].code + " = ";
            sql = sql + "'" + m_items[row].keyValues[pos].value.toString() + "' AND ";
        }
        sql = sql + "period_id = '" + QString::number(pos2) + "'";
        if (!lktable.exec(sql))
        {
            qWarning() << "Error: " << lktable.lastError().databaseText();
            qWarning() << "Executing: " << sql;
            TmoduleErrorDef error;
            error.row = row;
            error.error = lktable.lastError().databaseText();
            errors.append(error);
        }
        else
        {
            updateRow(row,"C");
        }
    }
}

void feedingTableModel::insertData(int row)
{
    QString sql;
    int pos;
    QSqlQuery lktable(database);
    sql = "INSERT INTO " + m_items[row].masterTable + "(";
    for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
    {
        sql = sql + checkedKeyValues[pos].code + ",";
    }
    QList<TmoduleFieldDef> keyValues;
    for (pos = 0; pos <= m_items[row].keyValues.count()-1;pos++)
    {
        if (!sql.contains(m_items[row].keyValues[pos].code))
        {
            keyValues.append(m_items[row].keyValues[pos]);
            sql = sql + m_items[row].keyValues[pos].code + ",";
        }
    }
    sql = sql.left(sql.length()-1);
    sql = sql + ") VALUES (";
    for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
    {
        sql = sql + "'" +checkedKeyValues[pos].value.toString() + "',";
    }
    for (pos = 0; pos <= keyValues.count()-1;pos++)
    {
        sql = sql + "'" + keyValues[pos].value.toString() + "',";
    }
    sql = sql.left(sql.length()-1);
    sql = sql + ")";
    if (!lktable.exec(sql))
    {
        TmoduleErrorDef error;
        error.row = row;
        error.error = lktable.lastError().databaseText();
        errors.append(error);
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    else
    {
        int pos2;
        bool errorInChild;
        errorInChild = false;
        for (pos2 = 1; pos2 <= nperiods; pos2++)
        {
            sql = "INSERT INTO " + m_items[row].masterTable + "periods (";
            for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
            {
                sql = sql + checkedKeyValues[pos].code + ",";
            }
            keyValues.clear();
            for (pos = 0; pos <= m_items[row].keyValues.count()-1;pos++)
            {
                if (!sql.contains(m_items[row].keyValues[pos].code))
                {
                    keyValues.append(m_items[row].keyValues[pos]);
                    sql = sql + m_items[row].keyValues[pos].code + ",";
                }
            }
            sql = sql + "period_id) VALUES (";
            for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
            {
                sql = sql + "'" +checkedKeyValues[pos].value.toString() + "',";
            }
            for (pos = 0; pos <= keyValues.count()-1;pos++)
            {
                sql = sql + "'" + keyValues[pos].value.toString() + "',";
            }
            sql = sql + "'" + QString::number(pos2) + "'";
            sql = sql + ")";
            if (!lktable.exec(sql))
            {
                TmoduleErrorDef error;
                error.row = row;
                error.error = lktable.lastError().databaseText();
                errors.append(error);
                qWarning() << "Error: " << lktable.lastError().databaseText();
                qWarning() << "Executing: " << sql;
                errorInChild = true;
            }
        }
        if (!errorInChild)
        updateRow(row,"C");
    }
}

bool feedingTableModel::applyData()
{
    int pos;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus != m_items[pos].previousStatus)
        {
            qWarning() << "PE:" << m_items[pos].previousStatus;
            qWarning() << "CE:" << m_items[pos].currentStatus;
            if (m_items[pos].previousStatus == "U")
            {
                insertData(pos);
            }
            if ((m_items[pos].previousStatus == "C") && (m_items[pos].currentStatus == "M"))
            {
                updateData(pos);
            }
            if ((m_items[pos].previousStatus == "C") && (m_items[pos].currentStatus == "U"))
            {
                deleteData(pos);
            }
        }
    }
    if (errors.count() == 0)
    {
        return true;
    }
    else
    {
        //errorDialog error;
        QStringList terror;
        QModelIndex begin;
        QModelIndex end;
        for (pos = 0; pos <= errors.count()-1; pos++)
        {
            m_items[errors[pos].row].error = true;
            begin = this->index(errors[pos].row,0);
            end = this->index(errors[pos].row,this->columnCount()-1);
            dataChanged(begin,end);
            terror << errors[pos].error;
        }
        //error.setErrorMessage(terror);
        //error.exec();
        errors.clear();
        return false;
    }
}

void feedingTableModel::cancelData()
{
    QList<TmoduleFieldDef> keys;
    keys.append(checkedKeyValues);
    chkitems(keys);
    errors.clear();
}


void feedingTableModel::updateRow(int row,QString status)
{
    m_items[row].currentStatus = status;
    m_items[row].previousStatus = status;

    if (status == "U")
    {
        for (int pos = 0; pos <= m_items[row].currentValues.count()-1;pos++)
        {
            m_items[row].currentValues[pos] = "F";
        }
    }

    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,this->columnCount());
    emit dataChanged(begin,end);
}

QString feedingTableModel::getMasterTable(int row)
{
    if ((row >= 0) && (row <= rowCount()-1))
    {
        return m_items[row].masterTable;
    }
    else
        return QString();
}



QList<TmoduleFieldDef> feedingTableModel::getMasterKeyFields(int row)
{
    QList<TmoduleFieldDef> res;
    if ((row >= 0) && (row <= rowCount()-1))
    {
        int idx;
        idx = getMasterIndex(m_items[row].masterTable);
        if (idx >= 0)
        {
            return m_manTables[idx].keysFields;
        }
        else
        {
            return res;
        }
    }
    else
        return res;
}

QList<TmoduleFieldDef> feedingTableModel::getLookUpFields(QModelIndex index)
{
    QList<TmoduleFieldDef> res;
    if ((index.row() >= 0) && (index.row() <= rowCount()-1))
    {
        int idx;
        idx = getMasterIndex(m_items[index.row()].masterTable);
        if (idx >= 0)
        {
            res =  m_items[index.row()].keyValues;
            return res;
        }
        else
        {
            return res;
        }
    }
    else
        return res;
}



QList<TmoduleFieldDef> feedingTableModel::getRowKeyValues(int row)
{
    QList<TmoduleFieldDef> res;
    res.append(m_items[row].keyValues);
    return res;
}


QString feedingTableModel::getLookUpTable(int row)
{
    int idx;
    idx = getMasterIndex(m_items[row].masterTable);
    if (idx > 0)
    {
        return m_manTables[idx].lookUpTable;
    }
    else
    {
        return QString();
    }
}

/************************************mnrManTableModel***********************************/

mnrManTableModel::mnrManTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
}

mnrManTableModel::~mnrManTableModel()
{
    //qDebug() << "Destroy: mnrManTableModel";
}

int mnrManTableModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int mnrManTableModel::columnCount(const QModelIndex &) const
{
    return 7;
}

QVariant mnrManTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return m_items[index.row()].description;
        if (index.column() == 1)
            return m_items[index.row()].itemType;
        if (index.column() == 2)
        {
            if ((m_items[index.row()].currentStatus == "C") ||
                m_items[index.row()].currentStatus == "M")
            {
                if (m_items[index.row()].previousStatus == "U")
                    return "AE";
                else
                    return "CE";
            }
            else
            {
              if (m_items[index.row()].previousStatus == "C")
                return "TD";
              else
                return "NA";
            }
        }
        if (index.column() == 3)
            return m_items[index.row()].totalYield;
        if (index.column() == 4)
            return m_items[index.row()].currentAllocation;
        if (index.column() == 5)
        {
            return m_items[index.row()].totalAvailable;
        }
        if (index.column() == 6)
        {
            if ((m_items[index.row()].currentStatus == "C") ||
                m_items[index.row()].currentStatus == "M")
            {
                if (m_items[index.row()].previousStatus == "U")
                    return "AV";
                else
                    return "CV";
            }
            else
            {
              if (m_items[index.row()].previousStatus == "C")
                return "TD";
              else
                return "NA";
            }
        }
    }


    if (role == Qt::DecorationRole)
    {
        if (m_items[index.row()].currentStatus == "M")
        {
            if (index.column() == 0)
            {
                if (m_items[index.row()].previousStatus == "C")
                    return QPixmap(":/images/modify2.png");
                else
                    return QPixmap(":/images/new3.png");
            }
            else
                return QVariant();
        }
        else
        {
            if (m_items[index.row()].error == true)
            {
                if (index.column() == 0)
                    return QPixmap(":/images/rowerror.png");
            }

            if ((m_items[index.row()].currentStatus == "C") &&
                (m_items[index.row()].previousStatus == "U"))
            {
                if (index.column() == 0)
                    return QPixmap(":/images/new3.png");
            }

            if ((m_items[index.row()].currentStatus == "U") &&
                (m_items[index.row()].previousStatus == "C"))
            {
                if (index.column() == 0)
                    return QPixmap(":/images/delete3.png");
            }

            return QVariant();
        }
    }

    if (role == Qt::BackgroundRole)
    {
        if (index.column() == 5)
        {
            if (index.data().toFloat() >= 0)
            {
                if (m_items[index.row()].totalAvailable != m_items[index.row()].totalYield)
                {
                    QBrush Background(Qt::white);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
                else
                {
                    QBrush Background(Qt::yellow);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
            }
            else
            {
                QBrush Background(Qt::red);
                Background.setStyle(Qt::SolidPattern);
                return Background;
            }
        }
    }

    if (role == Qt::CheckStateRole)
    {

        if (index.column() == 0)
        {
            if (m_items[index.row()].currentStatus != "U")
            {
                return Qt::Checked;
            }
            else
            {
                return Qt::Unchecked;
            }
        }
    }
    return QVariant();
}

bool mnrManTableModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (role == Qt::EditRole)
    {
        if ((index.column() == 2) || (index.column() == 4) || (index.column() == 6))
        {
            if ((m_items[index.row()].currentStatus == "C") ||
                (m_items[index.row()].currentStatus == "M"))
            {
                m_items[index.row()].currentStatus = "M";
                if (index.column() == 4)
                {
                    if (m_items[index.row()].previousStatus == "U")
                    {
                        if (value.toFloat() <= m_items[index.row()].totalAvailable.toFloat())
                        {
                            m_items[index.row()].currentAllocation = value;
                        }
                        else
                        {
                            m_items[index.row()].currentAllocation = m_items[index.row()].previousAllocation;
                        }
                    }
                    else
                    {
                        float available;
                        available = m_items[index.row()].currentAllocation.toFloat();
                        available = available + m_items[index.row()].totalAvailable.toFloat();
                        if (value.toFloat() <= available)
                        {
                            m_items[index.row()].currentAllocation = value;
                        }
                        else
                        {
                            m_items[index.row()].currentAllocation = m_items[index.row()].previousAllocation;
                        }
                    }
                }
                QModelIndex pix;
                pix = this->index(index.row(),0);
                emit dataChanged(pix,index);
                emit modeldDataChanged();
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    if (role == Qt::CheckStateRole)
    {
        if (m_items[index.row()].currentStatus != "M")
        {
            if (value.toInt() == Qt::Checked)
            {
                if (m_items[index.row()].totalAvailable.toFloat() > 0)
                    m_items[index.row()].currentStatus = "C";
                else
                {
                    if (m_items[index.row()].previousStatus == "U")
                        return false;
                    else
                    {
                        m_items[index.row()].currentAllocation = m_items[index.row()].previousAllocation;
                        m_items[index.row()].currentStatus = "C";
                    }
                }
            }
            else
            {
                m_items[index.row()].currentStatus = "U";
                m_items[index.row()].currentAllocation = 0;
            }
            QModelIndex begin;
            begin = this->index(index.row(),0);
            QModelIndex end;
            end = this->index(index.row(),this->columnCount()-1);
            emit dataChanged(begin,end);
            emit modeldDataChanged();
            return true;
        }
        else
        {
            if (m_items[index.row()].previousStatus == "U")
            {
                m_items[index.row()].currentStatus = "U";
                m_items[index.row()].currentAllocation = 0;

                QModelIndex begin;
                begin = this->index(index.row(),0);
                QModelIndex end;
                end = this->index(index.row(),this->columnCount()-1);
                emit dataChanged(begin,end);
                emit modeldDataChanged();
                return true;
            }
            else
                return false;
        }
    }
    return false;
}

Qt::ItemFlags mnrManTableModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    }
    else
    {
        if ((index.column() == 1) || (index.column() == 3) || (index.column() == 5))
            return Qt::ItemIsEnabled;
        if ((m_items[index.row()].currentStatus == "C") ||
            (m_items[index.row()].currentStatus == "M"))
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        else
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    return Qt::ItemIsEnabled;
}

QVariant mnrManTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Product";
            if (section == 1)
                return "Type";
            if (section == 2)
                return "Mineral \n composition";
            if (section == 3)
                return "Total yield \n (kg)";
            if (section == 4)
                return "Yield \n allocated (kg)";
            if (section == 5)
                return "Available \n yield (kg)";
            if (section == 6)
                return "Availability \n by period";
        }
    }

    return QVariant();
}

//void mnrManTableModel::setImpactModule(impModuleForm *)
//{
//    //m_impModule = module;
//}

void mnrManTableModel::setDatabase (QSqlDatabase db)
{
    database = db;
}

int mnrManTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

int mnrManTableModel::getMasterIndex(QString masterTable) const
{
    int res;
    res = -1;
    for (int pos =0; pos <= m_manTables.count()-1;pos++)
    {
        if (m_manTables[pos].name == masterTable)
        {
            res = pos;
            break;
        }
    }
    return res;
}

void mnrManTableModel::addMasterTable(QString tableName, QList<TmoduleFieldDef> keyFields)
{
    if (getMasterIndex(tableName) == -1)
    {
        mnrManTable table;
        table.name = tableName;
        table.keysFields.append(keyFields);
        table.lookUpTable = "";
        table.lookUpTableType = "";
        table.yieldTable = "";
        table.yieldField = "";
        table.percField = "";
        m_manTables.append(table);
    }
    else
    {
        qWarning() << "Error: Adding a table that already exists!";
    }
}

void mnrManTableModel::addLookUpTable(QString masterTable, QString lkTableName, QString lkTableType,QList<TmoduleFieldDef> keyFields)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].lookUpTable = lkTableName;
        m_manTables[idx].lookUpTableType = lkTableType;
        m_manTables[idx].lkKeysFields.append(keyFields);
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void mnrManTableModel::addYieldTable(QString masterTable, QString yieldTable, QString yieldField, QString percField)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].yieldTable = yieldTable;
        m_manTables[idx].yieldField = yieldField;
        m_manTables[idx].percField = percField;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void mnrManTableModel::setlkUpColDescription(QString description)
{
    lookUpColDescription = description;
}

float mnrManTableModel::getUsage(QString materTable,QList<TmoduleFieldDef> keyFields)
{
    int pos;
    QString sql;
    QSqlQuery lktable(database);
    sql = "SELECT SUM(QTYASS) FROM " + materTable + " WHERE ";
    for (pos = 0; pos <= keyFields.count()-1;pos++)
    {
        sql = sql + keyFields[pos].code + " = ";
        sql = sql + "'" + keyFields[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    if (lktable.exec(sql))
    {
        lktable.first();
        return lktable.value(0).toFloat();
    }
    else
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    return 9999999;
}

void mnrManTableModel::loadItems(QList<TmoduleFieldDef> keyFields)
{
    int pos;
    int pos2;
    m_items.clear();
    QString sql;
    QSqlQuery lktable(database);
    for (pos = 0; pos <= m_manTables.count()-1; pos++)
    {
        sql = "SELECT ";
        for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].lookUpTable + ".";
            sql = sql + m_manTables[pos].lkKeysFields[pos2].code + ",";
        }
        if (m_manTables[pos].percField != "NULL")
        {
            //sql = sql + "SUM( (cast(";
            //sql = sql + m_manTables[pos].yieldTable + "." + m_manTables[pos].percField + " as real)/100) * " +  m_manTables[pos].yieldTable + ".";
            //sql = sql + m_manTables[pos].yieldField + ") AS TYIELD FROM ";

            sql = sql + "SUM(";
            sql = sql + m_manTables[pos].yieldTable + "." + m_manTables[pos].percField;
            sql = sql + ") AS TYIELD FROM ";
        }
        else
        {
            sql = sql + "SUM(";
            sql = sql + m_manTables[pos].yieldTable + ".";
            sql = sql + m_manTables[pos].yieldField + ") AS TYIELD FROM ";
        }
        sql = sql + m_manTables[pos].lookUpTable + ",";
        sql = sql + m_manTables[pos].yieldTable + " WHERE ";
        for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].lookUpTable + "." + m_manTables[pos].lkKeysFields[pos2].code + " = ";
            sql = sql + m_manTables[pos].yieldTable + "." + m_manTables[pos].lkKeysFields[pos2].code + " AND ";
        }
        for (pos2 = 0; pos2 <= keyFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].lookUpTable + "." + keyFields[pos2].code + " = ";
            sql = sql + "'" + keyFields[pos2].value.toString() + "' AND ";
        }
        sql = sql.left(sql.length()-5);
        sql = sql + " GROUP BY ";
        for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].lookUpTable + "." + m_manTables[pos].lkKeysFields[pos2].code + ",";
        }
        sql = sql.left(sql.length()-1);
        sql = sql + " HAVING TYIELD > 0";
        if (lktable.exec(sql))
        {
            while (lktable.next())
            {
                TmnrMamItem item;
                item.masterTable = m_manTables[pos].name;
                for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
                {
                    TmoduleFieldDef field;
                    field.code = m_manTables[pos].lkKeysFields[pos2].code;
                    field.value = lktable.value(getFieldIndex(lktable,m_manTables[pos].lkKeysFields[pos2].code));
                    item.keyValues.append(field);
                }

                tableDescriptor tbldesc(this,database);
                item.description = tbldesc.getCustomDescription(m_manTables[pos].name,item.keyValues);

                item.itemType = m_manTables[pos].lookUpTableType;
                item.currentStatus = "U";
                item.previousStatus = "U";
                item.currentAllocation = "0";
                item.previousAllocation = "0";
                item.totalYield = lktable.value(getFieldIndex(lktable,"TYIELD"));
                item.totalAvailable = item.totalYield.toFloat() - getUsage(m_manTables[pos].name,item.keyValues);
                item.error = false;
                m_items.append(item);
            }
        }
        else
        {
            qWarning() << "Error: " << lktable.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
}

void mnrManTableModel::chkitems(QList<TmoduleFieldDef> keyFields)
{
    int pos;
    int pos2;
    int pos3;
    QString sql;
    QSqlQuery lktable(database);
    QString kIstring;
    QString kQstring;
    checkedKeyValues.clear();
    checkedKeyValues.append(keyFields);
    this->beginResetModel();
    for (pos2 = 0; pos2 <= m_items.count()-1;pos2++)
    {
        m_items[pos2].currentStatus = "U";
        m_items[pos2].previousStatus = "U";
        m_items[pos2].currentAllocation = "0";
        m_items[pos2].previousAllocation = "0";
    }

    for (pos = 0; pos <= m_manTables.count()-1; pos++)
    {
        sql = "SELECT ";
        for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].lkKeysFields[pos2].code + ",";
        }
        sql = sql + "QTYASS FROM " + m_manTables[pos].name + " WHERE ";
        for (pos2 = 0; pos2 <= keyFields.count()-1;pos2++)
        {
            sql = sql + keyFields[pos2].code + " = ";
            sql = sql + "'" + keyFields[pos2].value.toString() + "' AND ";
        }
        sql = sql.left(sql.length()-5);
        if (lktable.exec(sql))
        {
            while (lktable.next())
            {
                kQstring = "";
                for (pos2 = 0; pos2 <= m_manTables[pos].lkKeysFields.count()-1;pos2++)
                {
                    kQstring = kQstring + lktable.value(getFieldIndex(lktable,m_manTables[pos].lkKeysFields[pos2].code)).toString();
                }

                for (pos2 = 0; pos2 <= m_items.count()-1;pos2++)
                {
                    if (m_items[pos2].masterTable == m_manTables[pos].name)
                    {
                        kIstring = "";
                        for (pos3 = 0; pos3 <= m_items[pos2].keyValues.count()-1;pos3++)
                        {
                            kIstring = kIstring + m_items[pos2].keyValues[pos3].value.toString();
                        }
                        if (kIstring == kQstring)
                        {
                            m_items[pos2].currentStatus = "C";
                            m_items[pos2].previousStatus = "C";
                            m_items[pos2].currentAllocation = lktable.value(getFieldIndex(lktable,"QTYASS"));
                            m_items[pos2].previousAllocation = m_items[pos2].currentAllocation;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            qWarning() << "Error: " << lktable.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
    this->endResetModel();
}

QString mnrManTableModel::getSortDescription(QModelIndex /*index*/)
{
    return QString();
}

bool mnrManTableModel::getCheckedStatusForRow(int row)
{
    if ((m_items[row].currentStatus == "C") ||
        (m_items[row].currentStatus == "M"))
        return true;
    else
        return false;
}

QString mnrManTableModel::getCurrentStatusForRow(int row)
{
    return m_items[row].currentStatus;
}

void mnrManTableModel::deleteData(int row)
{
    QString sql;
    int pos;
    QSqlQuery lktable(database);
    sql = "DELETE FROM " + m_items[row].masterTable;
    sql = sql + " WHERE ";
    for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
    {
        sql = sql + checkedKeyValues[pos].code + " = ";
        sql = sql + "'" +checkedKeyValues[pos].value.toString() + "' AND ";
    }
    for (pos = 0; pos <= m_items[row].keyValues.count()-1;pos++)
    {
        sql = sql + m_items[row].keyValues[pos].code + " = ";
        sql = sql + "'" + m_items[row].keyValues[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    if (!lktable.exec(sql))
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        TmoduleErrorDef error;
        error.row = row;
        error.error = lktable.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        updateRow(row,"C");
    }
}

void mnrManTableModel::updateData(int row)
{
    QString sql;
    int pos;
    QSqlQuery lktable(database);
    sql = "UPDATE " + m_items[row].masterTable + " SET ";
    sql = sql + "QTYASS = " ;
    sql = sql + m_items[row].currentAllocation.toString() + " WHERE ";
    for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
    {
        sql = sql + checkedKeyValues[pos].code + " = ";
        sql = sql + "'" +checkedKeyValues[pos].value.toString() + "' AND ";
    }
    for (pos = 0; pos <= m_items[row].keyValues.count()-1;pos++)
    {
        sql = sql + m_items[row].keyValues[pos].code + " = ";
        sql = sql + "'" + m_items[row].keyValues[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    if (!lktable.exec(sql))
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        TmoduleErrorDef error;
        error.row = row;
        error.error = lktable.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        updateRow(row,"C");
    }
}

void mnrManTableModel::insertData(int row)
{
    QString sql;
    int pos;
    QSqlQuery lktable(database);
    sql = "INSERT INTO " + m_items[row].masterTable + "(";
    for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
    {
        sql = sql + checkedKeyValues[pos].code + ",";
    }
    for (pos = 0; pos <= m_items[row].keyValues.count()-1;pos++)
    {
        sql = sql + m_items[row].keyValues[pos].code + ",";
    }
    sql = sql + "QTYASS)" + " VALUES (";
    for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
    {
        sql = sql + "'" +checkedKeyValues[pos].value.toString() + "',";
    }
    for (pos = 0; pos <= m_items[row].keyValues.count()-1;pos++)
    {
        sql = sql + "'" + m_items[row].keyValues[pos].value.toString() + "',";
    }
    sql = sql + m_items[row].currentAllocation.toString() + ")";
    if (!lktable.exec(sql))
    {
        TmoduleErrorDef error;
        error.row = row;
        error.error = lktable.lastError().databaseText();
        errors.append(error);
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    else
    {
        updateRow(row,"C");
    }
}

float mnrManTableModel::getTotalUsage()
{
    float total;
    int pos;
    total = 0;
    for (pos = 0; pos <= m_manTables.count()-1;pos++)
    {
        total = total + getUsage(m_manTables[pos].name,checkedKeyValues);
    }
    return total;
}

void mnrManTableModel::updateMaster()
{
    QString sql;
    QSqlQuery lktable(database);
    int pos;
    float total;
    total = 0;
    for (pos = 0; pos <= m_manTables.count()-1;pos++)
    {
        total = total + getUsage(m_manTables[pos].name,checkedKeyValues);
    }
    sql = "UPDATE mnrheap SET ";
    sql = sql + "QTYASS = " ;
    sql = sql + QString::number(total) + ",";
    sql = sql + "YIELD = " + QString::number(total) +  " - ((cast(PERCLOSS as real) /100) * " + QString::number(total) + ")";
    sql = sql  + " WHERE ";
    for (pos = 0; pos <= checkedKeyValues.count()-1;pos++)
    {
        sql = sql + checkedKeyValues[pos].code + " = ";
        sql = sql + "'" +checkedKeyValues[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    if (!lktable.exec(sql))
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}
void mnrManTableModel::updateAvailable()
{
    for (int pos = 0; pos <= m_items.count()-1;pos++)
    {
        m_items[pos].totalAvailable = m_items[pos].totalYield.toFloat() - getUsage(m_items[pos].masterTable,m_items[pos].keyValues);
    }
}

bool mnrManTableModel::applyData()
{
    int pos;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus != m_items[pos].previousStatus)
        {
            if (m_items[pos].previousStatus == "U")
            {
                insertData(pos);
            }
            if ((m_items[pos].previousStatus == "C") && (m_items[pos].currentStatus == "M"))
            {
                updateData(pos);
            }
            if ((m_items[pos].previousStatus == "C") && (m_items[pos].currentStatus == "U"))
            {
                deleteData(pos);
            }
        }
    }
    if (errors.count() == 0)
    {
        updateMaster();
        updateAvailable();
        QList<TmoduleFieldDef> keys;
        keys.append(checkedKeyValues);
        chkitems(keys);
        return true;
    }
    else
    {
        //errorDialog error;
        QStringList terror;
        QModelIndex begin;
        QModelIndex end;
        for (pos = 0; pos <= errors.count()-1; pos++)
        {
            m_items[errors[pos].row].error = true;
            begin = this->index(errors[pos].row,0);
            end = this->index(errors[pos].row,this->columnCount()-1);
            dataChanged(begin,end);
            terror << errors[pos].error;            
        }
        //error.setErrorMessage(terror);
        //error.exec();
        errors.clear();
        return false;
    }
}

void mnrManTableModel::cancelData()
{
    QList<TmoduleFieldDef> keys;
    keys.append(checkedKeyValues);
    chkitems(keys);
    updateMaster();
    errors.clear();
}


void mnrManTableModel::updateRow(int row,QString status)
{
    m_items[row].currentStatus = status;
    m_items[row].previousStatus = status;
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,this->columnCount());
    emit dataChanged(begin,end);
}

QString mnrManTableModel::getMasterTable(int row)
{
    if ((row >= 0) && (row <= rowCount()-1))
    {
        return m_items[row].masterTable;
    }
    else
        return QString();
}



QList<TmoduleFieldDef> mnrManTableModel::getMasterKeyFields(int row)
{
    QList<TmoduleFieldDef> res;
    if ((row >= 0) && (row <= rowCount()-1))
    {
        int idx;
        idx = getMasterIndex(m_items[row].masterTable);
        if (idx >= 0)
        {
            return m_manTables[idx].keysFields;
        }
        else
        {
            return res;
        }
    }
    else
        return res;
}

QList<TmoduleFieldDef> mnrManTableModel::getLookUpFields(QModelIndex index)
{
    QList<TmoduleFieldDef> res;
    if ((index.row() >= 0) && (index.row() <= rowCount()-1))
    {
        int idx;
        idx = getMasterIndex(m_items[index.row()].masterTable);
        if (idx >= 0)
        {
            res =  m_items[index.row()].keyValues;
            return res;
        }
        else
        {
            return res;
        }
    }
    else
        return res;
}



QList<TmoduleFieldDef> mnrManTableModel::getRowKeyValues(int row)
{
    QList<TmoduleFieldDef> res;
    res.append(m_items[row].keyValues);
    return res;
}


QString mnrManTableModel::getLookUpTable(int row)
{
    int idx;
    idx = getMasterIndex(m_items[row].masterTable);
    if (idx > 0)
    {
        return m_manTables[idx].lookUpTable;
    }
    else
    {
        return QString();
    }
}


//********************************Grazing table model************************

grazingTableModel::grazingTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
}

grazingTableModel::~grazingTableModel()
{
    //qDebug() << "Destroy: grazingTableModel";
}

int grazingTableModel::rowCount(const QModelIndex &) const
{
    return items.count();
}

int grazingTableModel::columnCount(const QModelIndex &) const
{
   return 4;
}

Qt::ItemFlags grazingTableModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

}

QVariant grazingTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return items[index.row()].LiveDesc;
        if (index.column() == 1)
            return items[index.row()].LivePurpose;
        if (index.column() == 2)
            return items[index.row()].LiveBreed;
        if (index.column() == 3)
            return items[index.row()].FeedDesc;
    }
    if (role == Qt::DecorationRole)
    {
        if (items[index.row()].error == true)
        {
            if (index.column() == 0)
                return QPixmap(":/images/rowerror.png");
        }
        if (items[index.row()].currentStatus == "M")
        {
            if (index.column() == 0)
                return QPixmap(":/images/modify2.png");
        }
        if ((items[index.row()].currentStatus == "N") &&
            (items[index.row()].previousStatus == "C"))
        {
            if (index.column() == 0)
                return QPixmap(":/images/delete3.png");
        }
        if ((items[index.row()].currentStatus == "C") &&
            (items[index.row()].previousStatus == "N"))
        {
            if (index.column() == 0)
                return QPixmap(":/images/new3.png");
        }
        return QVariant();
    }
    if (role == Qt::BackgroundRole)
    {
        if (items[index.row()].currentStatus == "N")
        {
            QBrush Background(QColor(191,255,191)); //We can change this to a nice color
            return Background;
        }
        else
        {
            QBrush Background(Qt::white);
            return Background;
        }
    }
    if (role == Qt::CheckStateRole)
    {
        if (index.column() == 0)
        {
            if (items[index.row()].currentStatus != "N")
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
    }
    return QVariant();
}

bool grazingTableModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (role == Qt::EditRole)
    {
        return false;
    }
    if (role == Qt::CheckStateRole)
    {
        if (items[index.row()].currentStatus != "M")
        {
            if (value.toInt() == Qt::Checked)
            {
                items[index.row()].currentStatus = "C";
            }
            else
            {
                items[index.row()].currentStatus = "N";
            }
            QModelIndex begin;
            begin = this->index(index.row(),0);
            QModelIndex end;
            end = this->index(index.row(),this->columnCount()-1);
            emit dataChanged(begin,end);
            emit modelDataChanged();
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

int grazingTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

void grazingTableModel::loadItems(QString system)
{
    QSqlQuery tquery(database);
    QString sql;

    sql = "SELECT lvstosys.live_icod,livestoc.live_des,lvstosys.purpose,";
    sql = sql + "lvstosys.bredd,feedgroups.group_cod,feedgroups.group_des";
    sql = sql + " FROM lvstosys,livestoc,feedgroups ";
    sql = sql + " WHERE lvstosys.live_cod = livestoc.live_cod";
    sql = sql + " AND lvstosys.sys_cod = feedgroups.sys_cod";
    sql = sql + " AND lvstosys.live_icod = feedgroups.live_icod";
    sql = sql + " AND lvstosys.sys_cod = '" + system + "'";
    sql = sql + " AND livestoc.live_tpy <> 'O'";

    if (tquery.exec(sql))
    {
        while (tquery.next())
        {
            TgrazingItem item;
            item.liveCode = tquery.value(0).toString();
            item.LiveDesc = tquery.value(1).toString();
            item.LivePurpose = tquery.value(2).toString();
            item.LiveBreed = tquery.value(3).toString();
            item.FeedCode = tquery.value(4).toString();
            item.FeedDesc = tquery.value(5).toString();
            item.currentStatus = "N";
            item.previousStatus = "N";
            item.error = false;
            items.append(item);
        }
    }
    else
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

}

void grazingTableModel::checkItems(QList<TmoduleFieldDef> keys)
{
    checkedKeys.clear();
    checkedKeys.append(keys);
    QSqlQuery tquery(database);
    QString sql;
    QString liveCode;
    QString groupCode;
    int pos;
    this->beginResetModel();

    for (pos = 0; pos <= items.count()-1;pos++)
    {
        items[pos].currentStatus = "N";
        items[pos].previousStatus = "N";
    }

    sql = "SELECT * FROM grazingman WHERE ";
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        sql = sql + keys[pos].code + " = ";
        sql = sql + "'" + keys[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    if (tquery.exec(sql))
    {
        while (tquery.next())
        {
            liveCode = tquery.value(getFieldIndex(tquery,"live_icod")).toString();
            groupCode = tquery.value(getFieldIndex(tquery,"group_cod")).toString();
            for (pos = 0; pos <= items.count()-1;pos++)
            {
                if ((items[pos].liveCode == liveCode) &&
                    (items[pos].FeedCode == groupCode))
                {
                    items[pos].currentStatus = "C";
                    items[pos].previousStatus = "C";
                }
            }
        }
    }
    else
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    this->endResetModel();
}

QString grazingTableModel::getItemStatus(int row)
{
   if (items[row].currentStatus == items[row].previousStatus)
    return items[row].currentStatus;
   else
   {
       if ((items[row].currentStatus == "C") && (items[row].previousStatus == "N"))
           return "N";
       if ((items[row].currentStatus == "N") && (items[row].previousStatus == "C"))
           return "D";
       if ((items[row].currentStatus == "M") && (items[row].previousStatus == "C"))
           return "M";
       if ((items[row].currentStatus == "M") && (items[row].previousStatus == "N"))
           return "N";
   }
   return "D";
}

QVariant grazingTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Livestock";
            if (section == 1)
                return "Purpose";
            if (section == 2)
                return "Breed";
            if (section == 3)
                return "Feeding group";
        }
    }
    return QVariant();
}

QList<TmoduleFieldDef> grazingTableModel::getRowKeyValues(int row)
{
    QList<TmoduleFieldDef> res;

    res.append(checkedKeys);

    TmoduleFieldDef key;
    key.code = "live_icod";
    key.value = items[row].liveCode;
    res.append(key);

    key.code = "group_cod";
    key.value = items[row].FeedCode;
    res.append(key);

    return res;
}

bool grazingTableModel::applyChanges()
{
    int pos;

    m_insertedKeys.clear();

    for (pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].currentStatus != items[pos].previousStatus)
        {
            if ((items[pos].previousStatus == "N") && (items[pos].currentStatus == "C"))
            {
                insertData(pos);
            }
            if ((items[pos].previousStatus == "N") && (items[pos].currentStatus == "M"))
            {
                insertData(pos);
            }
            if ((items[pos].previousStatus == "C") && (items[pos].currentStatus == "N"))
            {
                deleteData(pos);
            }
            if ((items[pos].previousStatus == "C") && (items[pos].currentStatus == "M"))
            {
                updateData(pos);
            }
        }
    }
    if (errors.count() == 0)
    {
        if (m_insertedKeys.count() > 0)
            emit afterInsert(m_insertedKeys);
        return true;
    }
    else
    {
        //errorDialog error;
        QStringList terror;
        QModelIndex begin;
        QModelIndex end;
        for (pos = 0; pos <= errors.count()-1; pos++)
        {
            items[errors[pos].row].error = true;

            begin = this->index(errors[pos].row,0);
            end = this->index(errors[pos].row,3);
            dataChanged(begin,end);

            terror << errors[pos].error;
        }
        //error.setErrorMessage(terror);
        //error.exec();
        errors.clear();
        return false;
    }
}

void grazingTableModel::cancelChanges()
{
    //This will cancel the changes
}

void grazingTableModel::updateRow(int row,QString status)
{
    items[row].currentStatus = status;
    items[row].previousStatus = status;
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,3);
    dataChanged(begin,end);
}

void grazingTableModel::insertData(int row)
{
    QSqlQuery tquery(database);
    QString sql;
    int pos;
    sql = "INSERT INTO grazingman (";
    for (pos = 0; pos <= checkedKeys.count()-1;pos++)
    {
        sql = sql + checkedKeys[pos].code + ",";
    }
    sql = sql + "live_icod,group_cod)";
    sql = sql + " VALUES (";
    for (pos = 0; pos <= checkedKeys.count()-1;pos++)
    {
        sql = sql + "'" + checkedKeys[pos].value.toString() + "',";
    }
    sql = sql + "'" + items[row].liveCode + "',";
    sql = sql + "'" + items[row].FeedCode + "')";

    if (!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        TmoduleErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        TmoduleInsertedKeys inserted;
        inserted.row = row;
        for (pos = 0; pos <= checkedKeys.count()-1;pos++)
        {
            TmoduleFieldDef inskey;
            inskey.code = checkedKeys[pos].code;
            inskey.value = checkedKeys[pos].value;
            inserted.keys.append(inskey);
        }
        TmoduleFieldDef linkedkey;
        linkedkey.code = "live_icod";
        linkedkey.value = items[row].liveCode;
        inserted.keys.append(linkedkey);

        linkedkey.code = "group_cod";
        linkedkey.value = items[row].FeedCode;
        inserted.keys.append(linkedkey);

        m_insertedKeys.append(inserted);

        updateRow(row,"C");
    }
}

void grazingTableModel::updateData(int row)
{
    //Not needed at this moment.
    updateRow(row,"C");
}

void grazingTableModel::deleteData(int row)
{
    QSqlQuery tquery(database);
    QString sql;
    sql = "DELETE FROM  grazingman";
    sql = sql + " WHERE ";
    int pos;
    for (pos = 0; pos <= checkedKeys.count()-1;pos++)
    {
        sql = sql + checkedKeys[pos].code + " = '";
        sql = sql + checkedKeys[pos].value.toString() + "' AND ";
    }
    sql = sql + "live_icod = '" + items[row].liveCode + "' AND ";
    sql = sql + "group_cod = '" + items[row].FeedCode + "'";
    if (!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        TmoduleErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        updateRow(row,"N");
    }
}

QString grazingTableModel::getSortDescription(QModelIndex index)
{
    QString res;
    res = items[index.row()].currentStatus;
    res = res + items[index.row()].LiveDesc;
    res = res + items[index.row()].LivePurpose;
    res = res + items[index.row()].LiveBreed;
    res = res + items[index.row()].FeedDesc;
    return res;
}


/************************************grazingTableSortModel***********************************/


grazingTableSortModel::grazingTableSortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool grazingTableSortModel::lessThan(const QModelIndex &left,
                                   const QModelIndex &right) const
{
    if (sourceModel())
    {

        QString leftData = qobject_cast<grazingTableModel *>(sourceModel())->getSortDescription(left);
        QString rightData = qobject_cast<grazingTableModel *>(sourceModel())->getSortDescription(right);
        return QString::localeAwareCompare(leftData, rightData) < 0;
    }
    else
    {
        return false;
    }
}


/************************************linkedTableModel***********************************/

linkedTableModel::linkedTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
}

linkedTableModel::~linkedTableModel()
{
    //qDebug() << "Destroy: linkedTableModel";
}

QList <TlinkedField> linkedTableModel::getLinkedFields(QString parent)
{
   QList <TlinkedField> res;
   int pos;
   for (pos = 0; pos <= m_linkedFields.count()-1;pos++)
   {
       if (m_linkedFields[pos].parentField == parent)
       {
            res.append(m_linkedFields[pos]);
       }
   }
   return res;
}

QVariant linkedTableModel::getLinkedValue(QString parent, QString child, QVariant parentValue)
{
    int pos;
    int pos2;
    for (pos = 0; pos <= m_linkedFields.count()-1;pos++)
    {
        if ((m_linkedFields[pos].parentField == parent) &&
            (m_linkedFields[pos].childField == child))
        {
            for (pos2 = 0; pos2 <= m_linkedFields[pos].values.count()-1;pos2++)
            {
                if (m_linkedFields[pos].values[pos2].parentValue.toString() == parentValue.toString())
                {
                    return m_linkedFields[pos].values[pos2].childValue;
                }
            }
            return m_linkedFields[pos].defaultValue;
        }
    }
    return QVariant();
}

void linkedTableModel::addLinkedValue(QString parent, QString child, QVariant parentValue, QVariant childValue)
{
    int pos;
    int pos2;
    pos2 = -1;
    for (pos = 0; pos <= m_linkedFields.count()-1;pos++)
    {
        if ((m_linkedFields[pos].parentField == parent) &&
            (m_linkedFields[pos].childField == child))
        {
            pos2 = pos;
            break;
        }
    }
    if (pos2 >= 0)
    {
        TlinkedValue value;
        value.parentValue = parentValue;
        value.childValue = childValue;
        m_linkedFields[pos2].values.append(value);
    }
    else
    {
        qWarning() << "Cannot add linked value. Parent with child not found!";
    }
}

void linkedTableModel::addLinkedField(QString parent, QString child, QVariant defaultValue)
{
    bool found;
    int pos;
    found = false;
    for (pos = 0; pos <= fields.count()-1;pos++)
    {
        if (fields[pos].code == parent)
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        found = false;
        for (pos = 0; pos <= m_linkedFields.count()-1;pos++)
        {
            if ((m_linkedFields[pos].parentField == parent) &&
                (m_linkedFields[pos].childField == child))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            TlinkedField field;
            field.parentField = parent;
            field.childField = child;
            field.defaultValue = defaultValue;
            m_linkedFields.append(field);
        }
        else
            qWarning() << "Cannot add linked field. Linked field already in list";
    }
    else
        qWarning() << "Cannot add linked field. Parent not found!";
}

void linkedTableModel::addField(QString name, QString description, QVariant defaultValue ,QColor color)
{
    TmoduleFieldDef field;
    field.code = name;
    field.description = description;
    field.Color = color;
    field.value = "0";
    field.previousValue = "0";
    field.defValue = defaultValue;
    fields.append(field);
}


int linkedTableModel::rowCount(const QModelIndex &) const
{
    return items.count();
}

int linkedTableModel::columnCount(const QModelIndex &) const
{
    if (fields.count() > 0)
        return 1 + fields.count();
    else
        return 0;
}

Qt::ItemFlags linkedTableModel::flags(const QModelIndex &index) const
{
    if (index.column() != 0)
    {
        if ((items[index.row()].currentStatus == "C") || items[index.row()].currentStatus == "M")
            return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled;
        else
            return Qt::NoItemFlags;
    }
    else
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    }
}

QVariant linkedTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
        {
            if (!m_useCustomDescription)
                return items[index.row()].description;
            else
            {
                tableDescriptor tbldesc(0,database);
                return tbldesc.getCustomDescription(lkTableName,items[index.row()].code,m_currSystem);
            }
        }
        else
        {
            if (items[index.row()].currentStatus != "N")
            {
                return items[index.row()].fields[index.column()-1].value;
            }
            else
            {
                return QString();
            }
        }
    }
    if (role == Qt::DecorationRole)
    {
        if (items[index.row()].error == true)
        {
            if (index.column() == 0)
                return QPixmap(":/images/rowerror.png");
        }
        if (items[index.row()].currentStatus == "M")
        {
            if (index.column() == 0)
                return QPixmap(":/images/modify2.png");
        }
        if ((items[index.row()].currentStatus == "N") &&
            (items[index.row()].previousStatus == "C"))
        {
            if (index.column() == 0)
                return QPixmap(":/images/delete3.png");
        }
        if ((items[index.row()].currentStatus == "C") &&
            (items[index.row()].previousStatus == "N"))
        {
            if (index.column() == 0)
                return QPixmap(":/images/new3.png");
        }
        return QVariant();
    }
    if (role == Qt::BackgroundRole)
    {
        if (items[index.row()].currentStatus == "N")
        {
            QBrush Background(QColor(191,255,191)); //We can change this to a nice color
            return Background;
        }
        else
        {
            if (index.column() == 0)
            {
                QBrush Background(Qt::white);
                return Background;
            }
            else
            {
                QBrush Background(items[index.row()].fields[index.column()-1].Color);
                return Background;return QVariant();
            }
        }
    }
    if (role == Qt::CheckStateRole)
    {
        if (index.column() == 0)
        {
            if (items[index.row()].currentStatus != "N")
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
    }
    return QVariant();
}

bool linkedTableModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (role == Qt::EditRole)
    {
        items[index.row()].fields[index.column()-1].value = value;
        items[index.row()].currentStatus = "M";
        emit dataChanged(index,index);
        emit modelDataChanged();
        return true;
    }
    if (role == Qt::CheckStateRole)
    {
        if (items[index.row()].currentStatus != "M")
        {
            if (value.toInt() == Qt::Checked)
            {
                items[index.row()].currentStatus = "C";
                if (items[index.row()].previousStatus == "N")
                {
                    for (int pos = 0; pos <= items[index.row()].fields.count()-1;pos++)
                    {
                        items[index.row()].fields[pos].value = items[index.row()].fields[pos].defValue;
                    }
                }
            }
            else
            {
                items[index.row()].currentStatus = "N";
            }
            QModelIndex begin;
            begin = this->index(index.row(),0);
            QModelIndex end;
            end = this->index(index.row(),this->columnCount()-1);
            emit dataChanged(begin,end);
            emit modelDataChanged();
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

int linkedTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

void linkedTableModel::loadItems()
{
    QList<TmoduleFieldDef> keyFields;
    loadItems(keyFields);
}

void linkedTableModel::loadItems(QList<TmoduleFieldDef> keyFields)
{
    if (fields.count() > 0)
    {
        items.clear();
        m_currSystem = "";
        //Load the lookup table
        //We get the last record value of the key to get a new value
        QSqlQuery lktable(database);
        QString sql;
        sql = "SELECT * FROM " + lkTableName;
        if (keyFields.count() > 0)
            sql = sql + " WHERE ";
        for (int pos = 0; pos <= keyFields.count()-1;pos++)
        {
            sql = sql + keyFields[pos].code + " = '";
            sql = sql + keyFields[pos].value.toString() + "' AND ";
            if (keyFields[pos].code.toLower() == "sys_cod")
                m_currSystem = keyFields[pos].value.toString();
        }
        if (keyFields.count() > 0)
            sql = sql.left(sql.length()-5);
        if (lktable.exec(sql))
        {
            while (lktable.next())
            {
                TlinkeditemDef item;
                item.code = lktable.value(getFieldIndex(lktable,lkKeyField)).toString();
                item.description = lktable.value(getFieldIndex(lktable,lkDisplayColumn)).toString();
                item.currentStatus = "N";
                item.previousStatus = "N";
                item.error = false;
                item.fields.append(fields);
                items.append(item);
            }
        }
        else
        {
            qWarning() << "Error: " << lktable.lastError().databaseText();
            qWarning() << "Executing : " << sql;
        }
        this->beginResetModel();
        this->endResetModel();
    }
    else
    {
        qWarning() << "Error: Set the fields first!!!";
    }
}

QString linkedTableModel::getItemStatus(int row)
{
   if (items[row].currentStatus == items[row].previousStatus)
    return items[row].currentStatus;
   else
   {
       if ((items[row].currentStatus == "C") && (items[row].previousStatus == "N"))
           return "N";
       if ((items[row].currentStatus == "N") && (items[row].previousStatus == "C"))
           return "D";
       if ((items[row].currentStatus == "M") && (items[row].previousStatus == "C"))
           return "M";
       if ((items[row].currentStatus == "M") && (items[row].previousStatus == "N"))
           return "N";
   }
   return "D";
}

void linkedTableModel::insertItem(QString code, QString description)
{
    TlinkeditemDef item;
    item.code = code;
    item.description = description;
    item.currentStatus = "N";
    item.previousStatus = "N";
    item.error = false;
    item.fields.append(fields);
    items.append(item);
}

void linkedTableModel::clearItems()
{
    items.clear();
}

void linkedTableModel::resetModel()
{
    this->beginResetModel();
    this->endResetModel();
}

void linkedTableModel::checkItems()
{
    if (fields.count() > 0)
    {
        int pos;
        int pos2;
        for (pos = 0; pos <= items.count()-1;pos++)
        {
            items[pos].currentStatus = "N";
            items[pos].previousStatus = "N";
        }
        QSqlQuery childTable(database);
        QString sql;
        sql = "SELECT * FROM " + tableName + " WHERE " ;
        for (pos = 0; pos <= keyFields.count()-1;pos++)
        {
            sql = sql + keyFields[pos].code + " = '";
            sql = sql + keyFields[pos].value.toString() + "' AND ";
        }
        sql = sql.left(sql.length()-5);
        if (childTable.exec(sql))
        {
            while (childTable.next())
            {
                for (pos = 0; pos <= items.count()-1;pos++)
                {
                    if (items[pos].code == childTable.value(getFieldIndex(childTable,linkedKeyField)).toString())
                    {
                        items[pos].currentStatus = "C";
                        items[pos].previousStatus = "C";
                        for (pos2 = 0; pos2 <= items[pos].fields.count()-1;pos2++)
                        {
                            if (!childTable.value(getFieldIndex(childTable,items[pos].fields[pos2].code)).toString().isEmpty())
                                items[pos].fields[pos2].value = childTable.value(getFieldIndex(childTable,items[pos].fields[pos2].code));
                            else
                                items[pos].fields[pos2].value = "NULL";
                            items[pos].fields[pos2].previousValue = items[pos].fields[pos2].value;
                        }
                    }
                }
            }
        }
        else
        {
            qWarning() << "Error: " << childTable.lastError().databaseText();
            qWarning() << "Executing : " << sql;
        }
        QModelIndex begin;
        begin = this->index(0,0);

        QModelIndex end;
        end = this->index(this->rowCount()-1,this->fields.count());
        dataChanged(begin,end);


    }
    else
    {
        qWarning() << "Error: Set the fields first!!!";
    }
}

QVariant linkedTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
              return lkDisplayColumnDescription;
            else
                return fields[section-1].description;
        }
    }
    return QVariant();
}

QList<TmoduleFieldDef> linkedTableModel::getRowKeyValues(int row)
{
    QList<TmoduleFieldDef> res;
    res.append(keyFields);

    TmoduleFieldDef linkedkey;
    linkedkey.code = linkedKeyField;
    linkedkey.value = items[row].code;
    res.append(linkedkey);
    return res;
}

bool linkedTableModel::applyChanges()
{
    int pos;

    m_insertedKeys.clear();
    m_deletedKeys.clear();
    for (pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].currentStatus != items[pos].previousStatus)
        {
            if ((items[pos].previousStatus == "N") && (items[pos].currentStatus == "C"))
            {
                insertData(pos);
            }
            if ((items[pos].previousStatus == "N") && (items[pos].currentStatus == "M"))
            {
                insertData(pos);
            }
            if ((items[pos].previousStatus == "C") && (items[pos].currentStatus == "N"))
            {
                deleteData(pos);
            }
            if ((items[pos].previousStatus == "C") && (items[pos].currentStatus == "M"))
            {
                updateData(pos);
            }
        }
    }
    if (errors.count() == 0)
    {
        if (m_insertedKeys.count() > 0)
            emit afterInsert(m_insertedKeys);
        if (m_deletedKeys.count() > 0)
            emit afterDelete(m_deletedKeys);
        return true;
    }
    else
    {
        //errorDialog error;
        QStringList terror;
        QModelIndex begin;
        QModelIndex end;
        for (pos = 0; pos <= errors.count()-1; pos++)
        {
            items[errors[pos].row].error = true;

            begin = this->index(errors[pos].row,0);
            end = this->index(errors[pos].row,this->fields.count());
            dataChanged(begin,end);

            terror << errors[pos].error;
        }
        //error.setErrorMessage(terror);
        //error.exec();
        errors.clear();
        return false;
    }
}

void linkedTableModel::cancelChanges()
{
    int pos;
    int pos2;
    for (pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].currentStatus != items[pos].previousStatus)
        {
            for (pos2 = 0; pos2 <= fields.count()-1;pos2++)
            {
                items[pos].fields[pos2].value = items[pos].fields[pos2].previousValue;
                items[pos].currentStatus = items[pos].previousStatus;
                items[pos].error = false;
                updateRow(pos,items[pos].currentStatus);
            }
        }
    }
}

void linkedTableModel::insertData(int row)
{
    QSqlQuery tquery(database);
    QString sql;
    int pos;
    int pos2;
    QList <TlinkedField> lkfields;
    QVariant lkValue;
    QVariant pvalue;
    sql = "INSERT INTO " + tableName + "(";
    for (pos = 0; pos <= keyFields.count()-1;pos++)
    {
        sql = sql + keyFields[pos].code + ",";
    }
    sql = sql + linkedKeyField + ",";
    for (pos = 0; pos <= fields.count()-1;pos++)
    {
        sql = sql + fields[pos].code + ",";
        lkfields = getLinkedFields(fields[pos].code);
        for (pos2 = 0; pos2 <= lkfields.count()-1;pos2++)
        {
            sql = sql + lkfields[pos2].childField + ",";
        }
    }
    sql = sql.left(sql.length()-1) + ")";
    sql = sql + " VALUES (";
    for (pos = 0; pos <= keyFields.count()-1;pos++)
    {
        if (keyFields[pos].value.toString() != "NULL")
            sql = sql + "'" + keyFields[pos].value.toString() + "',";
        else
            sql = sql + "NULL,";
    }
    sql = sql + "'" + items[row].code + "',";
    for (pos = 0; pos <= fields.count()-1;pos++)
    {
        pvalue = items[row].fields[pos].value;
        if ((pvalue.toString() != "NULL") && (!pvalue.toString().isEmpty()))
            sql = sql + "'" + pvalue.toString() + "',";
        else
            sql = sql + "NULL,";
        lkfields = getLinkedFields(fields[pos].code);
        for (pos2 = 0; pos2 <= lkfields.count()-1;pos2++)
        {
            lkValue = getLinkedValue(fields[pos].code,lkfields[pos2].childField,pvalue);
            if ((lkValue.toString() != "NULL") && (!lkValue.toString().isEmpty()))
                sql = sql + "'" + lkValue.toString() + "',";
            else
                sql = sql + "NULL,";
        }
    }
    sql = sql.left(sql.length()-1) + ")";
    if (!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        TmoduleErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        TmoduleInsertedKeys inserted;
        inserted.row = row;
        for (pos = 0; pos <= keyFields.count()-1;pos++)
        {
            TmoduleFieldDef inskey;
            inskey.code = keyFields[pos].code;
            inskey.value = keyFields[pos].value;
            inserted.keys.append(inskey);

        }
        TmoduleFieldDef linkedkey;
        linkedkey.code = linkedKeyField;
        linkedkey.value = items[row].code;
        inserted.keys.append(linkedkey);

        m_insertedKeys.append(inserted);

        updateRow(row,"C");
    }
}

void linkedTableModel::updateData(int row)
{
    QSqlQuery tquery(database);
    QString sql;
    int pos;
    int pos2;
    QList <TlinkedField> lkfields;
    QVariant lkValue;
    QVariant pvalue;
    sql = "UPDATE " + tableName + " SET ";
    for (pos = 0; pos <= fields.count()-1;pos++)
    {
        sql = sql + fields[pos].code + " = ";
        pvalue = items[row].fields[pos].value;
        if ((pvalue.toString() != "NULL") && (!pvalue.toString().isEmpty()))
            sql = sql + "'" + pvalue.toString() + "',";
        else
            sql = sql + "NULL,";
        lkfields = getLinkedFields(fields[pos].code);
        for (pos2 = 0; pos2 <= lkfields.count()-1;pos2++)
        {
            sql = sql + lkfields[pos2].childField + " = ";
            lkValue = getLinkedValue(fields[pos].code,lkfields[pos2].childField,pvalue);
            if ((lkValue.toString() != "NULL") && (!lkValue.toString().isEmpty()))
                sql = sql + "'" + lkValue.toString() + "',";
            else
                sql = sql + "NULL,";
        }
    }
    sql = sql.left(sql.length()-1);
    sql = sql + " WHERE ";
    for (pos = 0; pos <= keyFields.count()-1;pos++)
    {
        sql = sql + keyFields[pos].code + " = '";
        sql = sql + keyFields[pos].value.toString() + "' AND ";
    }
    sql = sql + linkedKeyField + " = '" + items[row].code + "'";
    if (!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        TmoduleErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        updateRow(row,"C");
    }
}

void linkedTableModel::deleteData(int row)
{
    QSqlQuery tquery(database);
    QString sql;
    sql = "DELETE FROM  " + tableName;
    sql = sql + " WHERE ";
    int pos;
    for (pos = 0; pos <= keyFields.count()-1;pos++)
    {
        sql = sql + keyFields[pos].code + " = '";
        sql = sql + keyFields[pos].value.toString() + "' AND ";
    }
    sql = sql + linkedKeyField + " = '" + items[row].code + "'";
    if (!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        TmoduleErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        TmoduleInsertedKeys deleted;
        deleted.row = row;
        for (pos = 0; pos <= keyFields.count()-1;pos++)
        {
            TmoduleFieldDef delkey;
            delkey.code = keyFields[pos].code;
            delkey.value = keyFields[pos].value;
            deleted.keys.append(delkey);

        }
        TmoduleFieldDef linkedkey;
        linkedkey.code = linkedKeyField;
        linkedkey.value = items[row].code;
        deleted.keys.append(linkedkey);

        m_deletedKeys.append(deleted);
        updateRow(row,"N");
    }
}

void linkedTableModel::updateRow(int row,QString status)
{
    items[row].currentStatus = status;
    items[row].previousStatus = status;
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,this->fields.count());
    dataChanged(begin,end);
}

QString linkedTableModel::getSortDescription(QModelIndex index)
{
    return items[index.row()].currentStatus + items[index.row()].description.toUpper();
}

void linkedTableModel::setKeyFields(QList<TmoduleFieldDef> keys)
{
    keyFields.clear();
    keyFields.append(keys);
}

void linkedTableModel::setLinkedKeyField(QString field)
{
    linkedKeyField = field;
}

void linkedTableModel::setLkUseCustomDescription(bool use)
{
    m_useCustomDescription = use;
}

//void linkedTableModel::setImpactModule(impModuleForm *)
//{
//    //m_impModule = module;
//}

bool linkedTableModel::hasCheckedItems()
{
    bool res;
    res = false;
    for (int pos = 0; pos <= items.count()-1;pos++)
    {
        if ((items[pos].currentStatus == "C")|| (items[pos].currentStatus == "M") ||
            (items[pos].previousStatus == "C"))
        {
            res = true;
            break;
        }
    }
    return res;
}

QColor linkedTableModel::getColorForRow(int row)
{
    if ((items[row].currentStatus == "C") || ((items[row].currentStatus == "M") && (items[row].previousStatus == "C")))
        return QColor(Qt::white);
    else
        return QColor(191,255,191);
}

/************************************linkedTableSortModel***********************************/


linkedTableSortModel::linkedTableSortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool linkedTableSortModel::lessThan(const QModelIndex &left,
                                   const QModelIndex &right) const
{
    if (sourceModel())
    {

        QString leftData = qobject_cast<linkedTableModel *>(sourceModel())->getSortDescription(left);
        QString rightData = qobject_cast<linkedTableModel *>(sourceModel())->getSortDescription(right);
        return QString::localeAwareCompare(leftData, rightData) < 0;
    }
    else
    {
        return false;
    }
}


/************************************managementSortModel***********************************/


managementSortModel::managementSortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool managementSortModel::lessThan(const QModelIndex &left,
                                   const QModelIndex &right) const
{
    if (sourceModel())
    {

        QString leftData = qobject_cast<managementModel *>(sourceModel())->getSortDescription(left);
        QString rightData = qobject_cast<managementModel *>(sourceModel())->getSortDescription(right);
        return QString::localeAwareCompare(leftData, rightData) < 0;
    }
    else
    {
        return false;
    }
}


//**************************managementModel****************************

managementModel::managementModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
}

managementModel::~managementModel()
{
    //qDebug() << "Destroy: managementModel";
}

int managementModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int managementModel::columnCount(const QModelIndex &) const
{
    return nperiods+1;
}

QVariant managementModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
        {
            return m_items[index.row()].description;
        }
        else
        {
            if (m_items[index.row()].isEditable)
            {
                if (m_items[index.row()].currentStatus != "U")
                {
                    return m_items[index.row()].currentValues[index.column()-1];
                }
                else
                {
                    return QVariant();
                }
            }
            else
                return QVariant();
        }
    }

    if (role == Qt::EditRole)
        return QVariant();

    if (role == Qt::DecorationRole)
    {
        if (m_items[index.row()].currentStatus == "M")
        {
            if (index.column() == 0)
            {
                if (m_items[index.row()].previousStatus == "C")
                    return QPixmap(":/images/modify2.png");
                else
                    return QPixmap(":/images/new3.png");
            }
            else
                return QVariant();
        }
        else
        {
            if (m_items[index.row()].error == true)
            {
                if (index.column() == 0)
                    return QPixmap(":/images/rowerror.png");
            }

            if ((m_items[index.row()].currentStatus == "C") &&
                (m_items[index.row()].previousStatus == "U"))
            {
                if (index.column() == 0)
                    return QPixmap(":/images/new3.png");
            }

            if ((m_items[index.row()].currentStatus == "U") &&
                (m_items[index.row()].previousStatus == "C"))
            {
                if (index.column() == 0)
                    return QPixmap(":/images/delete3.png");
            }

            return QVariant();
        }
    }
    if (role == Qt::BackgroundRole)
    {
        if (m_items[index.row()].isEditable)
        {
            QColor color;
            int idx;
            if (m_items[index.row()].currentStatus == "U")
            {
                idx = getMasterIndex(m_items[index.row()].masterTable);
                color = m_manTables[idx].lookUpUnChkColor;
                QBrush Background(color); //We can change this to a nice color
                return Background;
            }
            else
            {
                idx = getMasterIndex(m_items[index.row()].masterTable);
                color = m_manTables[idx].lookUpChkColor;
                QBrush Background(color);
                return Background;
            }
        }
        else
        {
            QBrush Background(Qt::lightGray);
            return Background;
        }
    }
    if (role == Qt::CheckStateRole)
    {
        if (m_items[index.row()].isEditable)
        {
            if (index.column() == 0)
            {
                if (m_items[index.row()].currentStatus != "U")
                {
                    return Qt::Checked;
                }
                else
                {
                    return Qt::Unchecked;
                }
            }
        }
        else
        {
            return QVariant();
        }
    }
    return QVariant();
}

bool managementModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (role == Qt::EditRole)
    {
        m_items[index.row()].currentStatus = "M";
        m_items[index.row()].currentValues[index.column()-1] = value;
        QModelIndex pix;
        pix = this->index(index.row(),0);
        emit dataChanged(pix,index);
        emit modeldDataChanged();
        return true;
    }
    if (role == Qt::CheckStateRole)
    {
        if (m_items[index.row()].currentStatus != "M")
        {
            if (value.toInt() == Qt::Checked)
            {
                m_items[index.row()].currentStatus = "C";
                if (m_items[index.row()].previousStatus == "U")
                {
                    int idx;
                    QVariant defvalue;
                    idx = getMasterIndex(m_items[index.row()].masterTable);
                    defvalue = m_manTables[idx].periodDefaultValue;
                    for (int pos = 1; pos <= nperiods; pos++)
                    {
                        m_items[index.row()].currentValues[pos-1] = defvalue;
                    }
                }
            }
            else
            {
                m_items[index.row()].currentStatus = "U";
            }
            QModelIndex begin;
            begin = this->index(index.row(),0);
            QModelIndex end;
            end = this->index(index.row(),this->columnCount()-1);
            emit dataChanged(begin,end);
            emit modeldDataChanged();
            return true;
        }
        else
        {
            if (m_items[index.row()].previousStatus == "U")
            {
                m_items[index.row()].currentStatus = "U";
                int idx;
                QVariant defvalue;
                idx = getMasterIndex(m_items[index.row()].masterTable);
                defvalue = m_manTables[idx].periodDefaultValue;
                for (int pos = 1; pos <= nperiods; pos++)
                {
                    m_items[index.row()].currentValues[pos-1] = defvalue;
                }
                QModelIndex begin;
                begin = this->index(index.row(),0);
                QModelIndex end;
                end = this->index(index.row(),this->columnCount()-1);
                emit dataChanged(begin,end);
                emit modeldDataChanged();
                return true;
            }
            else
                return false;
        }
    }
    return false;
}

Qt::ItemFlags managementModel::flags(const QModelIndex &index) const
{
    if (m_items[index.row()].isEditable)
    {
        if (index.column() == 0)
        {
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
        }
        else
        {
            if ((m_items[index.row()].currentStatus == "C") ||
                (m_items[index.row()].currentStatus == "M"))
            {
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
            }
            else
            {
                 return Qt::NoItemFlags;
            }
        }
    }
    else
    {
        return Qt::ItemIsEnabled;
    }
}

QVariant managementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return lookUpColDescription;
            else
                return m_periods[section-1].description;
        }
    }
    if (role == Qt::FontRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return QVariant();
            else
                return m_periods[section-1].font;
        }
    }
    if (role == Qt::ForegroundRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return QVariant();
            else
            {
                QBrush foreGround;
                foreGround.setStyle(Qt::SolidPattern);
                foreGround.setColor(m_periods[section-1].color);
                return foreGround;
            }
        }
    }
    return QVariant();
}

//void managementModel::setImpactModule(impModuleForm *)
//{
//    //m_impModule = module;
//}

void managementModel::setDatabase (QSqlDatabase db)
{
    database = db;
    QSqlQuery ptable(database);
    QString sql;
    sql = "SELECT * FROM periods";
    ptable.exec(sql);
    nperiods = 0;
    while (ptable.next())
    {
        TperiodHeaderData period;
        period.description = ptable.value(getFieldIndex(ptable,"period_des")).toString();
        m_periods.append(period);
        nperiods++;
    }
}

void managementModel::setPeriodDescription(int period,QString description)
{
    if ((period-1 >= 0) && (period-1 <= nperiods-1))
    {
        m_periods[period-1].description = description;
    }
    else
    {
        qWarning() << "Error: Period is out for bounds!";
    }
}

void managementModel::setPeriodFont(int period,QFont font)
{
    if ((period-1 >= 0) && (period-1 <= nperiods-1))
    {
        m_periods[period-1].font = font;
    }
    else
    {
        qWarning() << "Error: Period is out for bounds!";
    }
}

void managementModel::setPeriodColor(int period,QColor color)
{
    if ((period-1 >= 0) && (period-1 <= nperiods-1))
    {
        m_periods[period-1].color = color;
    }
    else
    {
        qWarning() << "Error: Period is out for bounds!";
    }
}

int managementModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

int managementModel::getMasterIndex(QString masterTable) const
{
    int res;
    res = -1;
    for (int pos =0; pos <= m_manTables.count()-1;pos++)
    {
        if (m_manTables[pos].name == masterTable)
        {
            res = pos;
            break;
        }
    }
    return res;
}

void managementModel::addMasterTable(QString tableName)
{    
    if (getMasterIndex(tableName) == -1)
    {
        TmanagementTable table;
        table.name = tableName;
        table.lookUpLinkedField = "";
        table.periodTable = "";
        table.periodField = "";
        table.periodStatusField = "";
        table.lookUpTable = "";
        table.lookUpCodeField = "";
        table.lookUpDescField = "";
        table.lookUpWhereClause = "";
        table.lookUpChkColor = QColor(Qt::white);
        table.lookUpUnChkColor = QColor(191,255,191);
        m_manTables.append(table);
    }
    else
    {
        qWarning() << "Error: Adding a table that already exists!";
    }
}

void managementModel::setKeysFields(QString masterTable, QList<TmoduleFieldDef> keyFields)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].keysFields.append(keyFields);
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void  managementModel::setLookUpLinkedField(QString masterTable, QString field)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].lookUpLinkedField = field;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setPeriodTable(QString masterTable,QString tableName)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].periodTable = tableName;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setPeriodKeyFields(QString masterTable, QList<TmoduleFieldDef> keyFields)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].periodKeysFields.append(keyFields);
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setPeriodField(QString masterTable, QString field)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].periodField = field;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setPeriodStatusField(QString masterTable, QString field)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].periodStatusField = field;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setPeriodDefaultValue(QString masterTable, QVariant value)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].periodDefaultValue = value;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setlkUpTable(QString masterTable, QString tableName)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].lookUpTable = tableName;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setlkUpCodeFields(QString masterTable, QString field)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].lookUpCodeField = field;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setlkUpDescFields(QString masterTable, QString field)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].lookUpDescField = field;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setlkUpWhereClause(QString masterTable, QString clause)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].lookUpWhereClause = clause;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setlkUpColDescription(QString description)
{
    lookUpColDescription = description;
}

void managementModel::setlkUpColOrder(QString masterTable, QString order)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].lookUpColOrder = order;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setlkUpChkColor(QString masterTable, QColor color)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].lookUpChkColor = color;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

void managementModel::setlkUpUnChkColor(QString masterTable, QColor color)
{
    int idx;
    idx = getMasterIndex(masterTable);
    if (idx != -1)
    {
        m_manTables[idx].lookUpUnChkColor = color;
    }
    else
    {
        qWarning() << "Error: Adding referenced keys to an unknown table";
    }
}

int managementModel::getItemIndex(QString masterTable, QString itemCode)
{
    int res;
    res = -1;
    for (int pos = 0; pos <= m_items.count()-1;pos++)
    {
        if ((m_items[pos].masterTable == masterTable) &&
            (m_items[pos].code == itemCode))
        {
            res = pos;
            break;
        }
    }
    return res;
}

void managementModel::loadItems()
{
    int pos;
    int pos2;
    int nrow;
    QSqlQuery ptable(database);
    QString sql;
    nrow = -1;
    for (pos = 0; pos <= m_manTables.count()-1;pos++)
    {
        if (!m_manTables[pos].lookUpTable.isEmpty())
        {
            sql = "SELECT " + m_manTables[pos].lookUpCodeField;
            sql = sql + "," + m_manTables[pos].lookUpDescField;
            sql = sql + " FROM " + m_manTables[pos].lookUpTable;
            if (!m_manTables[pos].lookUpWhereClause.isEmpty())
            {
                sql = sql + " WHERE " + m_manTables[pos].lookUpWhereClause;
            }
            if (ptable.exec(sql) == true)
            {
                while (ptable.next())
                {
                    TmanagementItem item;
                    item.code = ptable.value(0).toString();
                    item.description = ptable.value(1).toString();
                    item.masterTable = m_manTables[pos].name;
                    item.order = m_manTables[pos].lookUpColOrder;                    
                    item.currentStatus = "U";
                    item.previousStatus = "U";
                    item.isEditable = true;
                    item.error = false;
                    for (pos2 = 1; pos2 <= nperiods;pos2++)
                    {
                        item.currentValues.append(QVariant());
                        item.previousValues.append(QVariant());
                    }
                    nrow++;
                    m_items.append(item);
                }
            }
            else
            {
                qWarning() << "Error: " << ptable.lastError().databaseText();
                qWarning() << "Executing: " << sql;
            }
        }
    }
}

void managementModel::clearItems()
{
    m_items.clear();
}

void managementModel::resetData()
{
   this->beginResetModel();
   this->endResetModel();
}

void managementModel::insertItem(QString masterTable, QString code, QString description)
{
    int idx;
    int nrow;
    int pos2;
    idx = getMasterIndex(masterTable);
    nrow = m_items.count()-1;
    if (idx >= 0)
    {
        TmanagementItem item;
        item.code = code;
        item.description = description;
        item.masterTable = masterTable;
        item.order = m_manTables[idx].lookUpColOrder;

        item.currentStatus = "U";
        item.previousStatus = "U";
        item.isEditable = true;
        item.error = false;
        for (pos2 = 1; pos2 <= nperiods;pos2++)
        {
            item.currentValues.append(QVariant());
            item.previousValues.append(QVariant());
        }
        nrow++;

        m_items.append(item);
    }
    else
    {
        qWarning() << "Error: Cannot insert item. Master table not found.";
    }
}

void managementModel::chkitems(QList<TmoduleFieldDef> keyFields)
{
    int pos;
    int pos2;
    int idx;
    int periodid;
    int npers;
    QVariant periodValue;
    QVariant keyValue;
    QSqlQuery masterTable(database);
    QSqlQuery periodTable(database);
    QString sql;
    bool actHasRecords;
    //Uncheck all the items and set its periods values to empty
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        m_items[pos].currentStatus = "U";
        m_items[pos].previousStatus = "U";
        for (pos2 = 1; pos2 <= nperiods;pos2++)
        {
            m_items[pos].currentValues[pos2-1] = "N";
            m_items[pos].previousValues[pos2-1] = "N";
        }
        m_items[pos].error = false;
        m_items[pos].periodKeysFields.clear();
    }
    //qWarning() << "Clear completed";
    //Get the information from the master tables to check the appropiate items
    for (pos = 0; pos <= m_manTables.count()-1;pos++)
    {
        //Select the records from the master table using the passed key fields
        sql = "SELECT ";
        for (pos2 = 0; pos2 <= m_manTables[pos].keysFields.count()-1;pos2++)
        {
            sql = sql + m_manTables[pos].keysFields[pos2].code + ",";
        }
        sql = sql + m_manTables[pos].lookUpLinkedField;

        sql = sql + " FROM " + m_manTables[pos].name;
        sql = sql + " WHERE ";
        for (pos2 = 0; pos2 <= keyFields.count()-1;pos2++)
        {
            m_manTables[pos].keysFields[pos2].value = keyFields[pos2].value;
            sql = sql + m_manTables[pos].keysFields[pos2].code + " = '";
            sql = sql + keyFields[pos2].value.toString() + "' AND ";
        }
        sql = sql.left(sql.length()-4);

        //qWarning() << "Executing in master: " << sql;

        if (!masterTable.exec(sql))
        {
            qWarning() << "Error: " << masterTable.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
        QString lkValue;
        while (masterTable.next())
        {
            lkValue = masterTable.value(getFieldIndex(masterTable,m_manTables[pos].lookUpLinkedField)).toString();
            idx = getItemIndex(m_manTables[pos].name,
                               masterTable.value(getFieldIndex(masterTable,m_manTables[pos].lookUpLinkedField)).toString());
            if (idx >= 0)
            {
                m_items[idx].currentStatus = "C";
                m_items[idx].previousStatus = "C";
                for (pos2 = 0; pos2 <= m_manTables[pos].keysFields.count()-1;pos2++)
                {
                    keyValue = masterTable.value(getFieldIndex(masterTable,m_manTables[pos].keysFields[pos2].code));
                    m_manTables[pos].keysFields[pos2].value = keyValue;
                }

                //Select the records from the period table using the master table key values
                sql = "SELECT ";
                for (pos2 = 0; pos2 <= m_manTables[pos].periodKeysFields.count()-1;pos2++)
                {
                    sql = sql + m_manTables[pos].periodKeysFields[pos2].code + ",";
                }
                sql = sql + m_manTables[pos].periodField + ",";
                sql = sql + m_manTables[pos].periodStatusField;
                sql = sql + " FROM " +  m_manTables[pos].periodTable;
                sql = sql + " WHERE ";
                for (pos2 = 0; pos2 <=  m_manTables[pos].keysFields.count()-1;pos2++)
                {
                    sql = sql + m_manTables[pos].periodKeysFields[pos2].code;
                    sql = sql + " = '" + masterTable.value(getFieldIndex(masterTable,m_manTables[pos].keysFields[pos2].code)).toString();
                    sql = sql + "' AND ";
                }
                sql = sql + m_manTables[pos].lookUpLinkedField + " = ";
                sql = sql + "'" + masterTable.value(getFieldIndex(masterTable,m_manTables[pos].lookUpLinkedField)).toString() + "'";

                //Append periodKeysFields based on master keyfields
                m_items[idx].periodKeysFields.append(m_manTables[pos].keysFields);
                //Append the activity to the list of keyfields
                TmoduleFieldDef field;
                field.code = m_manTables[pos].lookUpLinkedField;
                field.value = masterTable.value(getFieldIndex(masterTable,m_manTables[pos].lookUpLinkedField)).toString();
                m_items[idx].periodKeysFields.append(field);

                //qWarning() << "Executing in child: " << sql;
                if (periodTable.exec(sql))
                {
                    actHasRecords = false;
                    while (periodTable.next())
                    {
                        //for (pos2 = 0; pos2 <= m_items[idx].periodKeysFields.count()-1;pos2++)
                        //{
                          //  keyValue = periodTable.value(getFieldIndex(periodTable,m_manTables[pos].periodKeysFields[pos2].code));
                            //m_items[idx].periodKeysFields[pos2].value = keyValue;
                        //}
                        periodid = periodTable.value(getFieldIndex(periodTable,m_manTables[pos].periodField)).toInt();
                        periodValue = periodTable.value(getFieldIndex(periodTable,m_manTables[pos].periodStatusField));
                        m_items[idx].currentValues[periodid-1] = periodValue;
                        m_items[idx].previousValues[periodid-1] = periodValue;
                        actHasRecords = true;
                    }
                    if (actHasRecords == false)
                    {
                        qWarning() << "Activity: " << masterTable.value(getFieldIndex(masterTable,m_manTables[pos].lookUpLinkedField)).toString() << " does not have periods values. Creating them";
                        for (npers = 1; npers <= nperiods;npers++)
                        {
                            sql = "INSERT INTO " + m_manTables[pos].periodTable + "(";
                            for (pos2 = 0; pos2 <= m_items[idx].periodKeysFields.count()-1;pos2++)
                            {
                                sql = sql + m_items[idx].periodKeysFields[pos2].code + ",";
                            }
                            sql = sql + m_manTables[pos].periodField + ")";
                            sql = sql + " VALUES (";
                            for (pos2 = 0; pos2 <= m_items[idx].periodKeysFields.count()-1;pos2++)
                            {
                                sql = sql + "'" + m_items[idx].periodKeysFields[pos2].value.toString() + "',";
                            }
                            sql = sql + "'" + QString::number(npers) + "')";
                            if (!periodTable.exec(sql))
                            {
                                qWarning() << "Error: " << periodTable.lastError().databaseText();
                                qWarning() << "Executing: " << sql;
                            }
                        }
                        qWarning() << "Done with creating";
                    }
                }
                else
                {
                    qWarning() << "Error: " << periodTable.lastError().databaseText();
                    qWarning() << "Executing: " << sql;
                }
            }
            else
            {
                qWarning() << "Error: Cannot find linked product " << m_manTables[pos].lookUpLinkedField << " for table: " << m_manTables[pos].name << " with value: " << lkValue;
            }
        }

    }

    QModelIndex begin;
    begin = this->index(0,0);

    QModelIndex end;
    end = this->index(rowCount()-1,columnCount()-1);

    //qWarning() << "Updating view...";
    emit dataChanged(begin,end);
}

QString managementModel::getSortDescription(QModelIndex index)
{
    QString status;
    if (m_items[index.row()].isEditable)
        status = "E";
    else
        status = "N";
    return m_items[index.row()].currentStatus + status + m_items[index.row()].order + m_items[index.row()].description.toUpper();
}

void managementModel::setItemEdtStatus(QString masterTable, QString itemCode, bool status)
{
    for (int pos = 0; pos <= m_items.count()-1; pos++)
    {
        if ((m_items[pos].masterTable ==  masterTable) &&
            (m_items[pos].code ==  itemCode))
        {
            m_items[pos].isEditable = status;
            break;
        }
    }
}

QColor managementModel::getColorForRow(int row)
{
    if (m_items[row].isEditable)
    {
        QColor color;
        int idx;
        if ((m_items[row].currentStatus == "C") ||
            (m_items[row].currentStatus == "M"))
        {
            idx = getMasterIndex(m_items[row].masterTable);
            color = m_manTables[idx].lookUpChkColor;
            return color;
        }
        else
        {
            idx = getMasterIndex(m_items[row].masterTable);
            color = m_manTables[idx].lookUpUnChkColor;
            return color;
        }
    }
    else
    {
        return QColor(Qt::lightGray);
    }
}

bool managementModel::getCheckedStatusForRow(int row)
{
    if ((m_items[row].currentStatus == "C") ||
        (m_items[row].currentStatus == "M"))
        return true;
    else
        return false;
}

QString managementModel::getCurrentStatusForRow(int row)
{
    return m_items[row].currentStatus;
}

bool managementModel::getEditableStatusForRow(int row)
{
    return m_items[row].isEditable;
}

bool managementModel::applyData()
{
    QList<TmoduleFieldDef> fixedKeyValues;
    return applyData(fixedKeyValues);
}

bool managementModel::applyData(QList<TmoduleFieldDef> fixedKeyValues)
{
    errors.clear();
    int pos;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus != m_items[pos].previousStatus)
        {
            //qWarning() << "Current: " << m_items[pos].currentStatus;
            //qWarning() << "Previous: " << m_items[pos].previousStatus;
            if ((m_items[pos].currentStatus == "M") &&
                (m_items[pos].previousStatus == "C"))
            {
                updateDataInPeriod(pos,fixedKeyValues);
            }
            if ((m_items[pos].currentStatus == "U") &&
                (m_items[pos].previousStatus == "C"))
            {
                DeleteDataFromMaster(pos,fixedKeyValues);
            }
            if ((m_items[pos].currentStatus == "M") &&
                (m_items[pos].previousStatus == "U"))
            {
                insertDataToMaster(pos,fixedKeyValues);
            }
            if ((m_items[pos].currentStatus == "C") &&
                (m_items[pos].previousStatus == "U"))
            {
                insertDataToMaster(pos,fixedKeyValues);
            }
        }
    }
    if (errors.count() == 0)
    {
        return true;
    }
    else
    {
        QStringList terror;
        QModelIndex begin;
        QModelIndex end;
        for (pos = 0; pos <= errors.count()-1; pos++)
        {
            m_items[errors[pos].row].error = true;
            begin = this->index(errors[pos].row,0);
            end = this->index(errors[pos].row,this->columnCount());
            emit dataChanged(begin,end);
            terror << errors[pos].error;
        }
        /*errorDialog error;
        error.setErrorMessage(terror);
        error.exec();
        errors.clear();*/
        return false;
    }
}

void managementModel::cancelData()
{
    int pos;
    int pos2;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        m_items[pos].currentStatus = m_items[pos].previousStatus;
        m_items[pos].error = false;
        for (pos2 = 1; pos2 <= nperiods; pos2++)
        {
           m_items[pos].currentValues[pos2-1] = m_items[pos].previousValues[pos2-1];
        }
    }
    errors.clear();
}

void managementModel::beforeInsert(QString, QList<TmoduleFieldDef>)
{
    //Reimplemented by subclasses of managementModel
}

void managementModel::insertDataToMaster(int row,QList<TmoduleFieldDef> fixedKeyValues)
{
    //Set the fixkey value
    int pos;
    int idx;
    idx = getMasterIndex(m_items[row].masterTable);
    if (fixedKeyValues.count() > 0)
    {
        int pos2;
        for (pos = 0; pos <= fixedKeyValues.count()-1;pos++)
        {
            for (pos2 = 0; pos2 <= m_manTables[idx].keysFields.count()-1;pos2++)
            {
                if (m_manTables[idx].keysFields[pos2].code == fixedKeyValues[pos].code)
                {
                    m_manTables[idx].keysFields[pos2].value = fixedKeyValues[pos].value;
                }
            }
        }
    }
    QString sql;
    QSqlQuery mtable(database);
    sql = "INSERT INTO " + m_manTables[idx].name + " (";
    for (pos = 0; pos <= m_manTables[idx].keysFields.count()-1;pos++)
    {
        sql = sql + m_manTables[idx].keysFields[pos].code + ",";
    }
    sql = sql + m_manTables[idx].lookUpLinkedField + ") VALUES (";
    for (pos = 0; pos <= m_manTables[idx].keysFields.count()-1;pos++)
    {
        sql = sql + "'" + m_manTables[idx].keysFields[pos].value.toString() + "',";
    }
    sql = sql + "'" + m_items[row].code + "')";

    this->beforeInsert(m_manTables[idx].name,m_manTables[idx].keysFields);

    if (!mtable.exec(sql))
    {
        qWarning() << "Error: " << mtable.lastError().databaseText();
        qWarning() << "Executing : " << sql;
        TmoduleErrorDef error;
        error.row = row;
        error.error = mtable.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        if (insertDataToPeriod(row))
            updateRow(row,"C");
    }
}

bool managementModel::insertDataToPeriod(int row)
{
    int pos;
    int idx;
    int nper;
    QString sql;
    QSqlQuery mtable(database);
    bool res;
    res = true;
    idx = getMasterIndex(m_items[row].masterTable);
    for (nper = 1; nper <= nperiods; nper++)
    {
        sql = "INSERT INTO " + m_manTables[idx].periodTable + " (";
        for (pos = 0; pos <= m_manTables[idx].keysFields.count()-1;pos++)
        {
            sql = sql + m_manTables[idx].periodKeysFields[pos].code + ","; //sql = sql + m_manTables[idx].periodKeysFields[pos].code + ",";
        }
        sql = sql + m_manTables[idx].lookUpLinkedField + ",";
        sql = sql + m_manTables[idx].periodField + ",";
        sql = sql + m_manTables[idx].periodStatusField + ") VALUES (";
        for (pos = 0; pos <= m_manTables[idx].keysFields.count()-1;pos++)
        {
            sql = sql + "'" + m_manTables[idx].keysFields[pos].value.toString() + "',";
        }
        sql = sql + "'" + m_items[row].code + "',";
        sql = sql + "'" + QString::number(nper) + "',";
        sql = sql + "'" + m_items[row].currentValues[nper-1].toString() + "')";
        if (!mtable.exec(sql))
        {
            qWarning() << "Error: " << mtable.lastError().databaseText();
            qWarning() << "Executing : " << sql;
            TmoduleErrorDef error;
            error.row = row;
            error.error = mtable.lastError().databaseText();
            errors.append(error);
            res = false;
        }
    }
    if (res == true)
    {
        m_items[row].periodKeysFields.clear();
        m_items[row].periodKeysFields.append(m_manTables[idx].keysFields);
        TmoduleFieldDef field;
        field.code = m_manTables[idx].lookUpLinkedField;
        field.value = m_items[row].code;
        m_items[row].periodKeysFields.append(field);
    }
    return res;
}

void managementModel::DeleteDataFromMaster(int row,QList<TmoduleFieldDef> fixedKeyValues)
{
    //Set the fixkey value
    int pos;
    int idx;
    idx = getMasterIndex(m_items[row].masterTable);
    if (fixedKeyValues.count() > 0)
    {
        int pos2;
        for (pos = 0; pos <= fixedKeyValues.count()-1;pos++)
        {
            for (pos2 = 0; pos2 <= m_manTables[idx].keysFields.count()-1;pos2++)
            {
                if (m_manTables[idx].keysFields[pos2].code == fixedKeyValues[pos].code)
                {
                    m_manTables[idx].keysFields[pos2].value = fixedKeyValues[pos].value;
                }
            }
        }
    }
    QString sql;
    QSqlQuery mtable(database);
    sql = "DELETE FROM " + m_manTables[idx].name + " WHERE ";
    for (pos = 0; pos <= m_manTables[idx].keysFields.count()-1;pos++)
    {
        sql = sql + m_manTables[idx].keysFields[pos].code + " = ";
        sql = sql + "'" + m_manTables[idx].keysFields[pos].value.toString() + "' AND ";
    }
    sql = sql + m_manTables[idx].lookUpLinkedField + " = ";
    sql = sql + "'" + m_items[row].code + "'";
    if (!mtable.exec(sql))
    {
        TmoduleErrorDef error;
        error.row = row;
        error.error = mtable.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        updateRow(row,"U");
    }
}

void managementModel::updateDataInPeriod(int row,QList<TmoduleFieldDef> fixedKeyValues)
{
    //Set the fixkey value
    int pos;
    int idx;
    int nper;
    QString sql;
    QSqlQuery mtable(database);
    idx = getMasterIndex(m_items[row].masterTable);
    if (fixedKeyValues.count() > 0)
    {
        int pos2;
        for (pos = 0; pos <= fixedKeyValues.count()-1;pos++)
        {
            for (pos2 = 0; pos2 <= m_manTables[idx].keysFields.count()-1;pos2++)
            {
                if (m_manTables[idx].keysFields[pos2].code == fixedKeyValues[pos].code)
                {
                    m_manTables[idx].keysFields[pos2].value = fixedKeyValues[pos].value;
                }
            }
        }
    }
    for (nper = 1; nper <= nperiods; nper++)
    {
        if (m_items[row].currentValues[nper-1].toString() !=
            m_items[row].previousValues[nper-1].toString())
        {
            if ((m_items[row].currentValues[nper-1].toString() == "Y") ||
                (m_items[row].currentValues[nper-1].toString() == "T") ||
                (m_items[row].currentValues[nper-1].toString() == "1"))
            {
                sql = "UPDATE " + m_manTables[idx].periodTable + " SET ";
                sql = sql + m_manTables[idx].periodStatusField + " = ";
                sql = sql + "'" + m_items[row].currentValues[nper-1].toString() + "'";
                sql = sql + " WHERE ";
                for (pos = 0; pos <= m_manTables[idx].keysFields.count()-1;pos++)
                {
                    sql = sql + m_manTables[idx].periodKeysFields[pos].code + " = "; //
                    sql = sql + "'" + m_manTables[idx].keysFields[pos].value.toString() + "' AND ";
                }
                sql = sql + m_manTables[idx].lookUpLinkedField + " = ";
                sql = sql + "'" + m_items[row].code + "' AND ";
                sql = sql + m_manTables[idx].periodField + " = ";
                sql = sql + "'" + QString::number(nper) + "'";
                if (!mtable.exec(sql))
                {
                    TmoduleErrorDef error;
                    error.row = row;
                    error.error = mtable.lastError().databaseText();
                    errors.append(error);
                }
                else
                {
                    updateRow(row,"C");
                }
            }
            else
            {
                //Delete the period so child data is also deleted
                sql = "DELETE FROM " + m_manTables[idx].periodTable;
                sql = sql + " WHERE ";
                for (pos = 0; pos <= m_manTables[idx].keysFields.count()-1;pos++)
                {
                    sql = sql + m_manTables[idx].periodKeysFields[pos].code + " = ";
                    sql = sql + "'" + m_manTables[idx].keysFields[pos].value.toString() + "' AND ";
                }
                sql = sql + m_manTables[idx].lookUpLinkedField + " = ";
                sql = sql + "'" + m_items[row].code + "' AND ";
                sql = sql + m_manTables[idx].periodField + " = ";
                sql = sql + "'" + QString::number(nper) + "'";
                if (!mtable.exec(sql))
                {
                    TmoduleErrorDef error;
                    error.row = row;
                    error.error = mtable.lastError().databaseText();
                    errors.append(error);
                }
                else
                {
                    //qWarning() << "Delete: " << sql;
                    //Inserts it again but as not checked
                    sql = "INSERT INTO " + m_manTables[idx].periodTable + " (";
                    for (pos = 0; pos <= m_manTables[idx].keysFields.count()-1;pos++)
                    {
                        sql = sql + m_manTables[idx].periodKeysFields[pos].code + ",";
                    }
                    sql = sql + m_manTables[idx].lookUpLinkedField + ",";
                    sql = sql + m_manTables[idx].periodField + ",";
                    sql = sql + m_manTables[idx].periodStatusField + ") VALUES (";
                    for (pos = 0; pos <= m_manTables[idx].keysFields.count()-1;pos++)
                    {
                        sql = sql + "'" + m_manTables[idx].keysFields[pos].value.toString() + "',";
                    }
                    sql = sql + "'" + m_items[row].code + "',";
                    sql = sql + "'" + QString::number(nper) + "',";
                    sql = sql + "'" + m_manTables[idx].periodDefaultValue.toString() + "')";
                    if (!mtable.exec(sql))
                    {
                        qWarning() << "DATA CONSISTENCY ERROR!!! Send an email to support@qlands.com if you see this message!";
                        TmoduleErrorDef error;
                        error.row = row;
                        error.error = mtable.lastError().databaseText();
                        errors.append(error);
                    }
                    else
                    {
                        updateRow(row,"C");
                    }
                }
            }
        }
        else
        {
            updateRow(row,"C");
        }
    }
}

void managementModel::updateRow(int row,QString status)
{
    m_items[row].currentStatus = status;
    m_items[row].previousStatus = status;
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,this->columnCount());
    emit dataChanged(begin,end);
}

QString managementModel::getMasterTable(int row)
{
    if ((row >= 0) && (row <= rowCount()-1))
    {
        return m_items[row].masterTable;
    }
    else
        return QString();
}

QString managementModel::getPeriodTable(int row)
{
    if ((row >= 0) && (row <= rowCount()-1))
    {
        int idx;
        idx = getMasterIndex(m_items[row].masterTable);
        if (idx >= 0)
        {
            return m_manTables[idx].periodTable;
        }
        else
        {
            return QString();
        }
    }
    else
        return QString();
}

QList<TmoduleFieldDef> managementModel::getMasterKeyFields(int row)
{
    QList<TmoduleFieldDef> res;
    if ((row >= 0) && (row <= rowCount()-1))
    {
        int idx;
        idx = getMasterIndex(m_items[row].masterTable);
        if (idx >= 0)
        {
            return m_manTables[idx].keysFields;
        }
        else
        {
            return res;
        }
    }
    else
        return res;
}

QVariant managementModel::getPeriodValue(QModelIndex index)
{
    return m_items[index.row()].currentValues[index.column()-1];
}

QString managementModel::getPeriodDescription(QModelIndex index)
{
    QString res;
    res = m_items[index.row()].description;
    res = res + " - Period: " + this->headerData(index.column(),Qt::Horizontal).toString();
    return res;
}

QList<TmoduleFieldDef> managementModel::getItemKeyFields(int row)
{
    QList<TmoduleFieldDef> res;
    if ((row >= 0) && (row <= rowCount()-1))
    {
        int idx;
        idx = getMasterIndex(m_items[row].masterTable);
        if (idx >= 0)
        {
            res.append(m_items[row].periodKeysFields);
            return res;
        }
        else
        {
            return res;
        }
    }
    else
        return res;
}

QList<TmoduleFieldDef> managementModel::getPeriodKeyFields(QModelIndex index)
{
    QList<TmoduleFieldDef> res;
    if ((index.row() >= 0) && (index.row() <= rowCount()-1))
    {
        int idx;
        idx = getMasterIndex(m_items[index.row()].masterTable);
        if (idx >= 0)
        {
            res.append(m_items[index.row()].periodKeysFields);
            moduleFieldDef field;
            field.code = m_manTables[idx].periodField;
            field.value = index.column();
            res.append(field);
            return res;
        }
        else
        {
            return res;
        }
    }
    else
        return res;
}

//**************************periodTableModel****************************

periodTableModel::periodTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
    nShowRows = -1;
    m_readOnly = false;
}

void periodTableModel::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
}

periodTableModel::~periodTableModel()
{
   // qDebug() << "Destroy: periodTableModel";
}

bool periodTableModel::isRowGroup(int row) const
{
    bool res;
    res = false;
    for (int pos = 0; pos <= m_groups.count()-1;pos++)
    {
        if (m_groups[pos].rowIndex == row)
        {
            res = true;
            break;
        }
    }
    return res;
}

int periodTableModel::getFieldByIndex(int idx) const
{
    int pos;
    int res;
    res = -1;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].rowIndex == idx)
        {
            res = pos;
            break;
        }
    }
    if (res == -1)
        qWarning() << "Invalid index from getFieldByIndex: " << idx;
    return res;
}

int periodTableModel::getGroupByIndex(int idx) const
{
    int pos;
    int res;
    res = -1;
    for (pos = 0; pos <= m_groups.count()-1;pos++)
    {
        if (m_groups[pos].rowIndex == idx)
        {
            res = pos;
            break;
        }
    }
    if (res == -1)
        qWarning() << "Invalid index from getGroupByIndex:" << idx;
    return res;
}


int periodTableModel::rowCount(const QModelIndex &) const
{
    return nShowRows+1;
}

int periodTableModel::columnCount(const QModelIndex &) const
{
    int res;
    res = m_periods.count();
    res++; //Field Desc Column
    if (m_groups.count() > 0)
        res++; //Grop column
    return res; //Test value
}
QVariant periodTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (isRowGroup(index.row()))
        {
            if (index.column() == 0)
            {
                return m_groups[getGroupByIndex(index.row())].description;
            }
        }
        else
        {
            int datacol;
            datacol = 1;
            if (m_groups.count() > 0)
                datacol ++;
            if (index.column() >= datacol)
            {
                return m_fields[getFieldByIndex(index.row())].values[index.column() - datacol];
            }
            else
            {
                int descCol;
                descCol = 0;
                if (m_groups.count() > 0)
                    descCol ++;
                if (index.column() == descCol)
                {
                    return m_fields[getFieldByIndex(index.row())].description;
                }
            }
        }
    }

    if (role == Qt::DecorationRole)
    {
        if (!isRowGroup(index.row()))
        {
            if (m_fields[getFieldByIndex(index.row())].status == "M")
            {
                int descCol;
                descCol = 0;
                if (m_groups.count() > 0)
                    descCol ++;
                if (index.column() == descCol)
                {
                    return QPixmap(":/images/modify2.png");
                }
            }
        }
    }

    if (role == Qt::FontRole)
    {
        if (m_groups.count() > 0)
        {
            if (index.column() == 0)
            {
                QFont boldFont;
                boldFont.setBold(true);
                return boldFont;
            }
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        if (m_groups.count() > 0)
        {
            if (index.column() <= 1)
                return Qt::AlignCenter;
        }
        else
        {
            if (index.column() == 0)
                return Qt::AlignCenter;
        }
    }

    if (role == Qt::BackgroundRole)
    {
        QColor color;
        QBrush Background;
        Background.setStyle(Qt::SolidPattern);

        if (isRowGroup(index.row()))
        {
            color = m_groups[getGroupByIndex(index.row())].color;
        }
        else
        {
            int datacol;
            datacol = 1;
            if (m_groups.count() > 0)
                datacol ++;
            if (index.column() >= datacol)
            {
                color = m_fields[getFieldByIndex(index.row())].color;
            }
            else
            {
                color = QColor(Qt::lightGray);
            }
        }

        Background.setColor(color);
        return Background;

    }
    return QVariant();
}

bool periodTableModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (role == Qt::EditRole)
    {
        int datacol;
        datacol = 1;
        if (m_groups.count() > 0)
            datacol ++;
        if (index.column() >= datacol)
        {
            if (!isRowGroup(index.row()))
            {
                m_fields[getFieldByIndex(index.row())].values[index.column() - datacol] = value;
                m_fields[getFieldByIndex(index.row())].status = "M";
                emit dataChanged(index,index);
                emit modeldDataChanged();
                return true;
            }
        }
    }
    return false;
}

Qt::ItemFlags periodTableModel::flags(const QModelIndex &index) const
{
    if (m_readOnly)
    {
        return Qt::ItemIsEnabled;
    }
    if (!isRowGroup(index.row()))
    {
        int datacol;
        datacol = 1;
        if (m_groups.count() > 0)
            datacol ++;
        if (index.column() >= datacol)
        {
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        }
        else
            return Qt::ItemIsEnabled;
    }
    else
    {
        if (index.column() == 0)
            return Qt::ItemIsEnabled;
        else
            return Qt::NoItemFlags;
    }
}

QVariant periodTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            int datacol;
            datacol = 1;
            if (m_groups.count() > 0)
                datacol ++;
            if (section >= datacol)
                return m_periods[section-datacol];
            else
                return QVariant();
        }
    }
    return QVariant();
}

//void periodTableModel::setImpactModule(impModuleForm *)
//{
//    //m_impModule = module;
//}

void periodTableModel::setDatabase (QSqlDatabase db)
{
    database = db;
    QSqlQuery ptable(database);
    QString sql;
    sql = "SELECT * FROM periods";
    ptable.exec(sql);
    while (ptable.next())
    {
        m_periods.append(ptable.value(getFieldIndex(ptable,"period_des")).toString());
    }
}

int periodTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

void periodTableModel::setTableName(QString tableName)
{
    m_tableName = tableName;
}

QString periodTableModel::getTableName()
{
   return m_tableName;
}

void periodTableModel::clearData()
{
    int pos;
    int period;
    this->beginResetModel();

    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        for (period = 0; period <= m_periods.count()-1;period++ )
        {
            m_fields[pos].values[period] = m_fields[pos].defaultValue;
            m_fields[pos].previousValues[period] = m_fields[pos].defaultValue;;
        }
    }
    m_keyFields.clear();
    this->endResetModel();
}

void periodTableModel::loadData (QList<TmoduleFieldDef> keyFields)
{
    int pos;
    int period;
    this->beginResetModel();

    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        for (period = 0; period <= m_periods.count()-1;period++ )
        {
            m_fields[pos].values[period] = m_fields[pos].defaultValue;
            m_fields[pos].previousValues[period] = m_fields[pos].defaultValue;;
        }
    }
    m_keyFields.clear();
    m_keyFields.append(keyFields);
    QSqlQuery table(database);
    QString sql;

    sql = "SELECT period_id,";
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        sql = sql + m_fields[pos].code + ",";
        m_fields[pos].status = "C";
    }
    sql = sql.left(sql.length()-1);
    sql = sql + " FROM " + m_tableName + " WHERE ";
    for (pos = 0; pos <= m_keyFields.count()-1;pos++)
    {
        sql = sql + m_keyFields[pos].code + " = '";
        sql = sql + m_keyFields[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);

    if (table.exec(sql))
    {
        QVariant value;
        while (table.next())
        {
            period = table.value(getFieldIndex(table,"period_id")).toInt();
            for (pos = 0; pos <= m_fields.count()-1;pos++)
            {
                value = table.value(getFieldIndex(table,m_fields[pos].code));
                m_fields[pos].values[period-1] = value;
                m_fields[pos].previousValues[period-1] = value;
            }
        }
    }
    else
    {
        qWarning() << "Error: " << table.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    this->endResetModel();
}

void periodTableModel::addField(QString code, QString description, QVariant defaultValue ,QString group, QColor color)
{
    if (m_periods.count() > 0)
    {
        nShowRows++;
        TperiodField field;
        field.code = code;
        field.description = description;
        field.groupCode = group;

        field.rowIndex = nShowRows;

        field.color = color;
        field.defaultValue = defaultValue;
        for (int pos = 0; pos <= m_periods.count()-1;pos++)
        {
            field.values.append(defaultValue);
            field.previousValues.append(defaultValue);
        }
        m_fields.append(field);
    }
    else
    {
        qWarning() << "Error: There are no periods!";
    }
}

void periodTableModel::addGroup(QString code, QString description, QColor color)
{
    nShowRows++;
    TperiodGroup group;
    group.code = code;
    group.description = description;
    group.color = color;
    group.rowIndex = nShowRows;
    m_groups.append(group);
}

void periodTableModel::applyData()
{
    this->beginResetModel();
    int pos2;
    for (int pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].status == "M")
        {
            modifyItem(pos);
            m_fields[pos].status = "C";
            for (pos2 = 0; pos2 <= m_periods.count()-1;pos2++)
            {
                m_fields[pos].previousValues[pos2] = m_fields[pos].values[pos2];
            }
        }
    }

    this->endResetModel();
    //this->distributeColums();
}

void periodTableModel::cancelData()
{
    int pos2;
    for (int pos = 0; pos <= m_fields.count()-1;pos++)
    {
        m_fields[pos].status = "C";
        for (pos2 = 0; pos2 <= m_periods.count()-1;pos2++)
        {
            m_fields[pos].values[pos2] = m_fields[pos].previousValues[pos2];
        }
    }
    this->beginResetModel();
    this->endResetModel();
    //this->distributeColums();
}

void periodTableModel::modifyItem(int index)
{
    QString sql;
    QSqlQuery table(database);
    int pos2;
    int pos;
    for (pos = 0; pos <= m_periods.count()-1;pos++)
    {
        sql = "UPDATE " + m_tableName + " SET ";
        sql = sql + m_fields[index].code + " = '";
        sql = sql + m_fields[index].values[pos].toString() + "'";
        sql = sql + " WHERE ";
        for (pos2 = 0; pos2 <= m_keyFields.count()-1;pos2++)
        {
            sql = sql + m_keyFields[pos2].code + " = '";
            sql = sql + m_keyFields[pos2].value.toString() + "' AND ";
        }
        sql = sql + "period_id = '" + QString::number(pos+1) + "'";
        if (!table.exec(sql))
        {
            qWarning() << "Error: " << table.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
}

//******************************fieldInColModel*****************************


fieldInColModel::fieldInColModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    nShowCols = -1;
    //m_impModule = 0;
    m_tableName = "";
    isReadOnly = false;
    maxKeySize = 3;
    beforeInsertTable = "";
}

fieldInColModel::~fieldInColModel()
{
    //qDebug() << "Destroy: fieldInColModel";
}

void fieldInColModel::setMaxKeySize(int size)
{
    maxKeySize = size;
}

void fieldInColModel::setBeforeInsertTable(QString table)
{
    beforeInsertTable = table;
}

void fieldInColModel::addBeforeInsertIgnoreField(QString field)
{
    TmoduleFieldDef fdef;
    fdef.code = field;
    beforeInsertIngKeys.append(fdef);
}

void fieldInColModel::resetModelStructure()
{
    this->beginResetModel();
    nShowCols = -1;
    //Clear the whole structure
    m_fields.clear();
    m_insertedKeys.clear();
    m_readOnlyfields.clear();
    m_readOnlyfieldFlags.clear();
    m_showValues.clear();
    m_columns.clear();
    m_items.clear();
    m_keyValues.clear();
    m_distException.clear();
    errors.clear();
    this->endResetModel();
}

int fieldInColModel::getFieldByIndex(int idx) const
{
    int pos;
    int res;
    res = -1;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].colIndex == idx)
        {
            res = pos;
            break;
        }
    }
    return res;
}

int fieldInColModel::getColumnByIndex(int idx) const
{
    int pos;
    int res;
    res = -1;
    for (pos = 0; pos <= m_columns.count()-1;pos++)
    {
        if (m_columns[pos].colIndex == idx)
        {
            res = pos;
            break;
        }
    }
    return res;
}

int fieldInColModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int fieldInColModel::columnCount(const QModelIndex &) const
{
    return nShowCols+1;
}

Qt::ItemFlags fieldInColModel::flags(const QModelIndex &index) const
{
    if (isReadOnly)
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    int idx;
    idx = getFieldByIndex(index.column());
    if (idx >= 0)
    {
        if (m_items[index.row()].fields[idx].editable)
        {
            if (m_items[index.row()].currentStatus != "D")
            {
                if ((!isFieldReadOnly(m_items[index.row()].fields[idx].code,m_items[index.row()].currentStatus)) &&
                    (!isFieldReadOnly(m_items[index.row()].fields[idx].code,index.row(),m_items[index.row()].currentStatus)))
                    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled;
                else
                    return Qt::ItemIsSelectable;
            }
            else
                return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        else
        {
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
    }
    else
    {
        idx = getColumnByIndex(index.column());
        if (idx >= 0)
        {
            if (m_items[index.row()].currentStatus != "D")
            {
                if ((!isFieldReadOnly(m_items[index.row()].columns[idx].code,m_items[index.row()].currentStatus)) &&
                    (!isFieldReadOnly(m_items[index.row()].columns[idx].code,index.row(),m_items[index.row()].currentStatus)))
                    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled;
                else
                    return Qt::ItemIsSelectable;
            }
            else
                return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        else
        {
            return Qt::NoItemFlags;
        }
    }
}

QVariant fieldInColModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        int idx;
        idx = getFieldByIndex(index.column());
        if (idx >= 0)
        {
            if (m_showValues.count() == 0)
                return m_items[index.row()].fields[idx].value;
            else
            {
                return getShowValue(index.row(),m_items[index.row()].fields[idx].code,
                                    m_items[index.row()].fields[idx].value);
            }
        }
        else
        {
            idx = getColumnByIndex(index.column());
            if (idx >= 0)
            {
                if (m_showValues.count() == 0)
                {
                    if (m_items[index.row()].currentStatus == "C")
                        return m_items[index.row()].columns[idx].defaultValue;
                    if (m_items[index.row()].currentStatus == "N")
                        return m_items[index.row()].columns[idx].valueWhenNew;
                    if (m_items[index.row()].currentStatus == "M")
                        return m_items[index.row()].columns[idx].valueWhenModified;
                    if (m_items[index.row()].currentStatus == "D")
                        return m_items[index.row()].columns[idx].valueWhenDeleted;
                }
                else
                {
                    QVariant rvalue;
                    if (m_items[index.row()].currentStatus == "C")
                        rvalue =  m_items[index.row()].columns[idx].defaultValue;
                    if (m_items[index.row()].currentStatus == "N")
                        rvalue = m_items[index.row()].columns[idx].valueWhenNew;
                    if (m_items[index.row()].currentStatus == "M")
                        rvalue = m_items[index.row()].columns[idx].valueWhenModified;
                    if (m_items[index.row()].currentStatus == "D")
                        rvalue = m_items[index.row()].columns[idx].valueWhenDeleted;
                    return getShowValue(index.row(),m_items[index.row()].columns[idx].code,
                                        rvalue);
                }
            }
            else
            {
                return QVariant("Error!");
            }
        }
    }
    if (role == Qt::DecorationRole)
    {
        if (m_items[index.row()].error == false)
        {
            if (index.column() == 0)
            {
                if (m_items[index.row()].currentStatus == "N")
                    return QPixmap(":/images/new3.png");
                if (m_items[index.row()].currentStatus == "D")
                    return QPixmap(":/images/delete3.png");
                if (m_items[index.row()].currentStatus == "M")
                    return QPixmap(":/images/modify2.png");
                return QVariant();
            }
            else
            {
                return QVariant();
            }
        }
        else
        {
            if (index.column() == 0)
                return QPixmap(":/images/rowerror.png");
            else
                return QVariant();
        }
    }
    if (role == Qt::BackgroundRole)
    {
        QColor color;
        QBrush Background;
        int idx;
        idx = getFieldByIndex(index.column());
        if (idx >= 0)
            color = m_items[index.row()].fields[idx].Color;
        else
        {
            idx = getColumnByIndex(index.column());
            if (idx >= 0)
                color = m_items[index.row()].columns[idx].Color;
            else
                color = QColor("RED");
        }
        Background.setColor(color);
        return Background;
    }
    return QVariant();
}

bool fieldInColModel::setData(const QModelIndex &index,const QVariant &value,int role)
{

    if ((role == Qt::EditRole) && (!isReadOnly))
    {
        if (m_items[index.row()].error == false)
        {
            int idx;
            idx = getFieldByIndex(index.column());
            if (idx >= 0)
            {
                m_items[index.row()].fields[idx].value = value;
                if ((m_items[index.row()].currentStatus != "N") && (m_items[index.row()].currentStatus != "D"))
                    m_items[index.row()].currentStatus = "M";
                QModelIndex begin;
                QModelIndex end;
                begin = this->index(index.row(),0);
                end = this->index(index.row(),columnCount()-1);
                emit dataChanged(begin,end);
                emit modeldDataChanged();
                emit modeldDataChanged(index.row(), index.column());
                return true;
            }
        }
    }
    return false;
}

QVariant fieldInColModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            int idx;
            idx = getFieldByIndex(section);
            if (idx >= 0)
                return m_fields[idx].description;
            else
            {
                idx = getColumnByIndex(section);
                if (idx >= 0)
                    return m_columns[idx].description;
                else
                    return QVariant("Error!");
            }
        }
    }
    return QVariant();
}

int fieldInColModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

//void fieldInColModel::setImpactModule(impModuleForm *)
//{
//    //m_impModule = module;
//}

void fieldInColModel::setDatabase(QSqlDatabase database)
{
    m_database = database;
}

void fieldInColModel::setTableName(QString tablename)
{
    m_tableName = tablename;
}

void fieldInColModel::insertField(QString code,
                 QString description,
                 QVariant value,
                 QVariant defvalue,
                 bool key,
                 bool autogen,
                 bool show,
                 bool editable,
                 QColor Color)
{
    TmoduleFieldDef field;
    field.code = code;
    field.newRecords = 0;
    field.description = description;
    field.value = value;
    field.defValue = defvalue;
    field.key = key;
    field.autogen = autogen;
    field.show = show;
    if (show)
    {
        nShowCols++;
        field.colIndex = nShowCols;
    }
    else
    {
        field.colIndex = -2;
    }
    field.editable = editable;
    field.Color = Color;
    m_fields.append(field);
}

void fieldInColModel::insertExtraColumn(QString code,
                       QString description,
                       QVariant defaultValue,
                       QVariant valueWhenNew,
                       QVariant valueWhenModified,
                       QVariant valueWhenDeleted,
                       QColor color)
{
    TextDataCol column;
    nShowCols++;
    column.colIndex = nShowCols;
    column.code = code;
    column.description = description;
    column.defaultValue = defaultValue;
    column.valueWhenNew = valueWhenNew;
    column.valueWhenModified = valueWhenModified;
    column.valueWhenDeleted = valueWhenDeleted;
    column.Color = color;

    m_columns.append(column);
}

void fieldInColModel::insertNewItem()
{

    TmoduleItemDef item;
    item.previousStatus = "";
    item.currentStatus = "N";
    item.error = false;
    item.fields.append(m_fields);
    item.columns.append(m_columns);
    int pos;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        item.fields[pos].value = item.fields[pos].defValue;
    }
    int pos2;
    //Generate an auto gen value for the keys that require it
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if ((m_fields[pos].key == true) && (m_fields[pos].autogen == true))
        {
            item.fields[pos].value = QVariant(genAutoGenKey(pos));
        }
    }
    //Set the value for keys fields to those passed to the model in the load function
    for (pos = 0; pos <= m_keyValues.count()-1;pos++)
    {
        for (pos2 = 0; pos2 <= m_fields.count()-1;pos2++)
        {
            if (item.fields[pos2].code.toLower() == m_keyValues[pos].code.toLower())
            {
                item.fields[pos2].value =  m_keyValues[pos].value;
                break;
            }
        }
    }
    m_items.append(item);

    QModelIndex end;
    end = this->index(m_items.count(),nShowCols);
    this->beginInsertRows(end,m_items.count(),m_items.count());

    emit afterInsert(m_items.count()-1);
    this->endInsertRows();
    /*if (m_tableView)
    {
        m_tableView->resizeColumnsToContents();
        m_tableView->update(this->index(m_items.count()-1,0));
        m_tableView->setCurrentIndex(this->index(m_items.count()-1,0));
        m_tableView->scrollTo(this->index(m_items.count()-1,0));
    }*/
    emit modeldDataChanged();
    //this->distributeColums();
}

QVariant fieldInColModel::getDataFromField(int row, QString field)
{
    for (int pos = 0; pos <= m_fields.count()-1; pos++)
    {
        if (m_items[row].fields[pos].code.toLower() == field.toLower())
        {
            return m_items[row].fields[pos].value;
        }
    }
    return QVariant();
}

QVariant fieldInColModel::getOldDataFromField(int row, QString field)
{
    for (int pos = 0; pos <= m_fields.count()-1; pos++)
    {
        if (m_items[row].fields[pos].code.toLower() == field.toLower())
        {
            return m_items[row].fields[pos].previousValue;
        }
    }
    return QVariant();
}

void fieldInColModel::setDataToField(int row, QString field, QVariant value)
{
    for (int pos = 0; pos <= m_fields.count()-1; pos++)
    {
        if (m_items[row].fields[pos].code.toLower() == field.toLower())
        {
            m_items[row].fields[pos].value = value;
            break;
        }
    }
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,nShowCols);
    emit dataChanged(begin,end);
    emit modeldDataChanged();
}

QList<TmoduleFieldDef> fieldInColModel::getRowKeyValues(int row)
{
    QList<TmoduleFieldDef> res;
    for (int pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].key == true)
        {
            res.append(m_items[row].fields[pos]);
        }
    }
    return res;
}

QString fieldInColModel::getRowStatus(int row)
{
    return m_items[row].currentStatus;
}

void fieldInColModel::deleteItem(int row)
{
    m_items[row].currentStatus = "D";
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,nShowCols);
    emit dataChanged(begin,end);
    emit modeldDataChanged();
}

void fieldInColModel::loadData(QList<TmoduleFieldDef> keyValues)
{
    this->beginResetModel();
    m_insertedKeys.clear();
    m_items.clear();
    m_keyValues.clear();
    errors.clear();
    if (m_fields.count() > 0)
    {
        int pos;
        QSqlQuery table(m_database);
        QString sql;

        sql = "SELECT ";
        for (pos = 0; pos <= m_fields.count()-1;pos++)
        {
            sql = sql + m_fields[pos].code + ",";
        }
        sql = sql.left(sql.length()-1);
        sql = sql + " FROM " + m_tableName;
        if (keyValues.count() > 0)
        {
            m_keyValues.append(keyValues);
            sql = sql + " WHERE ";
            for (pos = 0; pos <= keyValues.count()-1;pos++)
            {
                sql = sql + keyValues[pos].code + " = '";
                sql = sql + keyValues[pos].value.toString() + "' AND ";
            }
            if (m_whereClause.isEmpty())
                sql = sql.left(sql.length()-5);
            else
                sql = sql + m_whereClause;
        }
        //qDebug() << sql;
        if (!table.exec(sql))
        {
            qWarning() << "Error: " << table.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }


        while (table.next())
        {
            TmoduleItemDef item;
            item.currentStatus = "C";
            item.previousStatus = "C";
            item.error = false;
            for (pos = 0; pos <= m_fields.count()-1;pos++)
            {
                TmoduleFieldDef field;
                field = m_fields[pos];
                field.value = table.value(getFieldIndex(table,field.code));
                field.previousValue = field.value;
                item.fields.append(field);
            }
            item.columns.append(m_columns);
            m_items.append(item);
        }
    }
    else
        qWarning() << "Error: Set the fields first!!!";
    this->endResetModel();
}

void fieldInColModel::removeDeleted()
{
    int pos;
    pos = 0;
    bool reload;
    reload = false;
    while (pos <= m_items.count()-1)
    {
        if (m_items[pos].currentStatus == "CLR")
        {
            m_items.removeAt(pos);
            pos = 0;
            reload = true;
        }
        else
        {
            pos++;
        }
    }
    if (reload)
    {
        for (pos = 0; pos <= m_fields.count()-1;pos++)
        {
            m_fields[pos].newRecords = 0;
        }
        this->beginResetModel();
        this->endResetModel();
        //this->distributeColums();
    }
}

bool fieldInColModel::applyChanges()
{
    errors.clear();
    m_insertedKeys.clear();
    m_updatedKeys.clear();
    m_deletedKeys.clear();
    int pos;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus != m_items[pos].previousStatus)
        {
            if (m_items[pos].currentStatus == "N")
            {
                insertData(pos);
            }
            if (m_items[pos].currentStatus == "D")
            {
                deleteData(pos);
            }
            if (m_items[pos].currentStatus == "M")
            {
                updateData(pos);
            }
        }
    }
    if (m_insertedKeys.count() > 0)
        emit afterApply(m_insertedKeys);

    if (m_updatedKeys.count() > 0)
        emit afterUpdate(m_updatedKeys);

    if (m_deletedKeys.count() > 0)
        emit afterDelete(m_deletedKeys);

    if (errors.count() == 0)
    {
        removeDeleted();
        return true;
    }
    else
    {
        QStringList terror;
        QModelIndex begin;
        QModelIndex end;
        for (pos = 0; pos <= errors.count()-1; pos++)
        {
            m_items[errors[pos].row].error = true;
            begin = this->index(errors[pos].row,0);
            end = this->index(errors[pos].row,nShowCols);
            emit dataChanged(begin,end);
            terror << errors[pos].error;
        }
        removeDeleted();
        /*errorDialog error;
        error.setErrorMessage(terror);
        error.exec();
        errors.clear();*/
        return false;
    }
}

void fieldInColModel::cancelChanges()
{
    int pos;
    int pos2;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus != m_items[pos].previousStatus)
        {
            if (m_items[pos].currentStatus != "N")
            {
                for (pos2 = 0; pos2 <= m_fields.count()-1;pos2++)
                {
                    m_items[pos].fields[pos2].value = m_items[pos].fields[pos2].previousValue;
                    m_items[pos].currentStatus = m_items[pos].previousStatus;
                    m_items[pos].error = false;
                    updateRow(pos,m_items[pos].currentStatus);
                }
            }
            else
            {
                m_items[pos].currentStatus = "CLR";
            }
        }
    }
    removeDeleted();
    errors.clear();
}

QString fieldInColModel::genAutoGenKey(int fieldIndex)
{
    int pos;
    if (m_fields[fieldIndex].newRecords == 0)
    {
        QSqlQuery temp(m_database);
        QString sql;
        sql = "SELECT max(cast(" + m_fields[fieldIndex].code + " as integer)) FROM " + m_tableName;
        if (m_keyValues.count() > 0)
        {
            sql = sql + " WHERE ";
            for (pos = 0; pos <= m_keyValues.count() - 1; pos++)
            {
                sql = sql + m_keyValues[pos].code;
                sql = sql + " = '" + m_keyValues[pos].value.toString() + "' AND ";
            }
            sql = sql.left(sql.length()-5);
        }
        temp.exec(sql);
        temp.first();
        m_fields[fieldIndex].newRecords = temp.value(0).toInt() + 1;
    }
    else
    {
        m_fields[fieldIndex].newRecords++;
    }
    QString newCode;
    newCode = QString::number(m_fields[fieldIndex].newRecords);
    int tl;
    tl = newCode.length();
    for (pos = 1; pos <= maxKeySize - tl; pos++)
    {
        newCode = "0" + newCode;
    }
    return newCode;
}

void fieldInColModel::insertInParentFirst(int row)
{
    QSqlQuery tquery(m_database);
    QString sql;
    int pos;
    int pos2;

    QList<TmoduleFieldDef> pFields;
    bool found;
    for (pos = 0; pos <= m_items[row].fields.count()-1;pos++)
    {
         found = false;
         for (pos2 = 0; pos2 <= beforeInsertIngKeys.count()-1;pos2++)
         {
             if (m_items[row].fields[pos].code == beforeInsertIngKeys[pos2].code)
             {
                 found = true;
                 break;
             }
         }
         if (!found)
         {
             pFields.append(m_items[row].fields[pos]);
         }
    }
    sql = "INSERT INTO " + beforeInsertTable + "(" ;
    for (pos = 0; pos <= pFields.count()-1;pos++)
    {
        sql = sql + pFields[pos].code + ",";
    }
    sql = sql.left(sql.length()-1) + ")";
    sql = sql + " VALUES (";
    for (pos = 0; pos <= pFields.count()-1;pos++)
    {
        sql = sql + "'" + pFields[pos].value.toString() + "',";
    }
    sql = sql.left(sql.length()-1) + ")";
    if (!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

void fieldInColModel::insertData(int row)
{
    int pos;
    QSqlQuery tquery(m_database);
    QString sql;

    if (!beforeInsertTable.isEmpty())
        insertInParentFirst(row);

    sql = "INSERT INTO " + m_tableName + "(" ;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        sql = sql + m_fields[pos].code + ",";
    }
    sql = sql.left(sql.length()-1) + ")";
    sql = sql + " VALUES (";
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        sql = sql + "'" + m_items[row].fields[pos].value.toString() + "',";
    }
    sql = sql.left(sql.length()-1) + ")";
    if (!tquery.exec(sql))
    {
        TmoduleErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        TmoduleInsertedKeys inserted;
        inserted.row = row;
        for (pos = 0; pos <= m_fields.count()-1;pos++)
        {
            if (m_fields[pos].key == true)
            {
                TmoduleFieldDef inskey;
                inskey.code = m_items[row].fields[pos].code;
                inskey.value = m_items[row].fields[pos].value;
                inserted.keys.append(inskey);
            }
        }
        m_insertedKeys.append(inserted);
        updateRow(row,"C");
    }
}

void fieldInColModel::updateData(int row)
{
    QSqlQuery tquery(m_database);
    QString sql;
    int pos;
    sql = "UPDATE " + m_tableName + " SET ";
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].key == false)
        {
            sql = sql + m_fields[pos].code + " = ";
            sql = sql + "'" + m_items[row].fields[pos].value.toString() + "',";
        }
    }
    sql = sql.left(sql.length()-1);
    sql = sql + " WHERE ";
    bool keys;
    keys = false;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].key == true)
        {
            keys = true;
            sql = sql + m_fields[pos].code + " = ";
            sql = sql + "'" + m_items[row].fields[pos].value.toString() + "' AND ";
        }
    }
    if (keys)
        sql = sql.left(sql.length()-5);
    else
        sql = sql.left(sql.length()-7);

    if (!tquery.exec(sql))
    {
        TmoduleErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    else
    {
        TmoduleInsertedKeys inserted;
        inserted.row = row;
        for (pos = 0; pos <= m_fields.count()-1;pos++)
        {
            if (m_fields[pos].key == true)
            {
                TmoduleFieldDef inskey;
                inskey.code = m_items[row].fields[pos].code;
                inskey.value = m_items[row].fields[pos].value;
                inserted.keys.append(inskey);
            }
        }
        m_updatedKeys.append(inserted);
        updateRow(row,"C");
    }
}

void fieldInColModel::deleteData(int row)
{
    QSqlQuery tquery(m_database);
    QString sql;
    int pos;
    sql = "DELETE FROM  " + m_tableName;
    sql = sql + " WHERE ";
    bool keys;
    keys = false;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].key == true)
        {
            keys = true;
            sql = sql + m_fields[pos].code + " = ";
            sql = sql + "'" + m_items[row].fields[pos].value.toString() + "' AND ";
        }
    }
    if (keys)
        sql = sql.left(sql.length()-5);
    else
        sql = sql.left(sql.length()-7);


    if (!tquery.exec(sql))
    {
        TmoduleErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        TmoduleInsertedKeys deleted;
        deleted.row = row;
        for (pos = 0; pos <= m_fields.count()-1;pos++)
        {
            if (m_fields[pos].key == true)
            {
                TmoduleFieldDef inskey;
                inskey.code = m_items[row].fields[pos].code;
                inskey.value = m_items[row].fields[pos].value;
                deleted.keys.append(inskey);
            }
        }
        m_deletedKeys.append(deleted);

        m_items[row].currentStatus = "CLR";
    }
}

void fieldInColModel::updateRow(int row,QString status)
{
    m_items[row].currentStatus = status;
    m_items[row].previousStatus = status;
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,nShowCols);
    emit dataChanged(begin,end);
}

void fieldInColModel::setReadOnly(bool readOnly)
{
    isReadOnly = readOnly;
}

void fieldInColModel::setWhereClause(QString clause)
{
    m_whereClause = clause;
}

void fieldInColModel::addReadOnlyField(QString field, QString whenStatus)
{
    TreadOnlyField afield;
    afield.field = field;
    afield.whenStatus = whenStatus;
    m_readOnlyfields.append(afield);
}

void fieldInColModel::addReadOnlyField(QString field, QString whenField, QVariant whenFieldValue, QString whenStatus)
{
    TreadOnlyFieldFlag flag;
    flag.field = field;
    flag.whenField = whenField;
    flag.whenFieldValue = whenFieldValue;
    flag.whenStatus = whenStatus;
    m_readOnlyfieldFlags.append(flag);
}

bool fieldInColModel::isFieldReadOnly(QString field, QString status) const
{
    bool res;
    res = false;
    for (int pos = 0; pos <= m_readOnlyfields.count()-1;pos++)
    {
        if ((m_readOnlyfields[pos].field == field) &&
            ((m_readOnlyfields[pos].whenStatus == status) || (m_readOnlyfields[pos].whenStatus == "ALL")))
        {
            res = true;
            break;
        }
    }
    return res;
}

bool fieldInColModel::isFieldReadOnly(QString field, int itemIndex, QString status) const
{
    bool res;
    res = false;
    QString whenField;
    QVariant whenValue;
    QString whenStatus;
    int pos;
    int pos2;
    {
        for (pos = 0; pos <= m_readOnlyfieldFlags.count()-1;pos++)
        {
            if (m_readOnlyfieldFlags[pos].field == field)
            {
                whenField = m_readOnlyfieldFlags[pos].whenField;
                whenValue = m_readOnlyfieldFlags[pos].whenFieldValue;
                whenStatus = m_readOnlyfieldFlags[pos].whenStatus;
                for (pos2 = 0; pos2 <= m_items[itemIndex].fields.count()-1;pos2++)
                {
                    if ((m_items[itemIndex].fields[pos2].code == whenField) &&
                        (m_items[itemIndex].fields[pos2].value == whenValue) &&
                        ((whenStatus == status) || (whenStatus == "ALL")))
                    {
                        res = true;
                        break;
                    }
                }
            }
            if (res == true)
                break;
        }
    }
    return res;
}

void fieldInColModel::addShowValue(QString field, QVariant value, QString whenField, QVariant whenValue)
{
    TmoduleFldShowValue showvalue;
    showvalue.field = field;
    showvalue.value = value;
    showvalue.whenField = whenField;
    showvalue.whenValue = whenValue;
    m_showValues.append(showvalue);
}

QVariant fieldInColModel::getShowValue(int itemIndex, QString field,QVariant currentValue) const
{
    QVariant res;
    res = currentValue;
    int pos;
    int pos2;
    for (pos = 0; pos <= m_showValues.count()-1;pos++)
    {
        if (m_showValues[pos].field == field)
        {
            for (pos2 = 0; pos2 <= m_items[itemIndex].fields.count()-1;pos2++)
            {
                if ((m_showValues[pos].whenField == m_items[itemIndex].fields[pos2].code) &&
                    (m_showValues[pos].whenValue == m_items[itemIndex].fields[pos2].value))
                {
                    res = m_showValues[pos].value;
                    break;
                }
            }
        }
        if (res != currentValue)
            break;
    }
    return res;
}

void fieldInColModel::addDistributionException(int column)
{
    m_distException.append(column);
}

bool fieldInColModel::isColInExceptions(int column) const
{
    bool res;
    res = false;
    for (int pos = 0; pos <= m_distException.count()-1;pos++)
    {
        if (m_distException[pos] == column)
        {
            res = true;
            break;
        }
    }
    return res;
}
