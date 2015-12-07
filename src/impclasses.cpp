#include "impclasses.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include "errordialog.h"
#include <QSqlError>
#include <QDebug>


//************************************Systems Table model************************

systemsTableModel::systemsTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{

}

systemsTableModel::~systemsTableModel()
{

}


int systemsTableModel::rowCount(const QModelIndex &) const
{
    return items.count();
}

int systemsTableModel::columnCount(const QModelIndex &) const
{
    return 2;
}

Qt::ItemFlags systemsTableModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant systemsTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return items[index.row()].systemCode;
        if (index.column() == 1)
            return items[index.row()].systemDescription;
    }
    return QVariant();
}

bool systemsTableModel::setData(const QModelIndex &,const QVariant &,int)
{
    return false;
}

int systemsTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

int systemsTableModel::getSystemIndex(QString code)
{
    int pos;
    for (pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].systemCode == code)
            return pos;
    }
    return -1;
}

void systemsTableModel::loadItems()
{
    //Load the lookup table
    //We get the last record value of the key to get a new value
    this->beginResetModel();
    items.clear();
    QSqlQuery lktable(database);
    QString sql;
    sql = "SELECT * FROM system";
    lktable.exec(sql);
    TsystemDef item;
    while (lktable.next())
    {
        item.systemCode = lktable.value(getFieldIndex(lktable,"sys_cod")).toString();
        item.systemDescription = lktable.value(getFieldIndex(lktable,"reg_desc")).toString();
        items.append(item);
    }
    this->endResetModel();
}



QVariant systemsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
              return "System ID";
            if (section == 1)
              return "Description";
        }
    }
    return QVariant();
}

QString systemsTableModel::getSystemDescription(int row)
{
    return items[row].systemDescription;
}

QString systemsTableModel::getSystemCode(int row)
{
    return items[row].systemCode;
}

//***********************************ChildTableModel******************************

childTableModel::childTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{

}

childTableModel::~childTableModel()
{
    database = QSqlDatabase();
}

void childTableModel::addField(QString name, QString description, QColor color)
{
    TfieldDef field;
    field.fieldName = name;
    field.fieldDescription = description;
    field.fieldColor = color;
    field.fieldValue = "0";
    field.previousValue = "0";
    fields.append(field);
}

QString childTableModel::getItemStatus(int row)
{
    return items[row].currentStatus;
}

int childTableModel::rowCount(const QModelIndex &) const
{
    return items.count();
}

int childTableModel::columnCount(const QModelIndex &) const
{
    if (fields.count() > 0)
        return 1 + fields.count();
    else
        return 0;
}

Qt::ItemFlags childTableModel::flags(const QModelIndex &index) const
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

QVariant childTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
        {
            return items[index.row()].description;
        }
        else
        {
            if (items[index.row()].currentStatus != "N")
            {
                return items[index.row()].fields[index.column()-1].fieldValue;
            }
            else
            {
                return QString();
            }
        }
    }
    if (role == Qt::DecorationRole)
    {
        if (items[index.row()].currentStatus == "M")
        {
            if (index.column() == 0)
                return QPixmap(":/images/modify2.png");
            else
                return QVariant();
        }
        else
        {
            if (items[index.row()].error == true)
            {
                if (index.column() == 0)
                    return QPixmap(":/images/rowerror.png");
            }
            else
                return QVariant();
        }
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
                QBrush Background(items[index.row()].fields[index.column()-1].fieldColor);
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

bool childTableModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (role == Qt::EditRole)
    {
        items[index.row()].fields[index.column()-1].fieldValue = value;
        items[index.row()].currentStatus = "M";
        emit dataChanged(index,index);
        emit childDataChanged();
        return true;
    }
    if (role == Qt::CheckStateRole)
    {
        if (items[index.row()].currentStatus != "M")
        {
            if (value.toInt() == Qt::Checked)
                items[index.row()].currentStatus = "C";
            else
            {
                items[index.row()].currentStatus = "N";
            }
            QModelIndex begin;
            begin = this->index(index.row(),0);
            QModelIndex end;
            end = this->index(index.row(),this->columnCount()-1);
            emit dataChanged(begin,end);
            emit childDataChanged();
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

int childTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

void childTableModel::loadItems()
{

    if (fields.count() > 0)
    {
        this->beginResetModel();
        items.clear();
        //Load the lookup table
        //We get the last record value of the key to get a new value
        QSqlQuery lktable(database);
        QString sql;
        sql = "SELECT * FROM " + lkTableName;
        lktable.exec(sql);
        TitemDef item;
        while (lktable.next())
        {
            item.code = lktable.value(getFieldIndex(lktable,lkKeyField)).toString();
            item.description = lktable.value(getFieldIndex(lktable,lkDisplayColumn)).toString();
            item.currentStatus = "N";
            item.previousStatus = "N";
            item.error = false;
            item.fields.append(fields);
            items.append(item);
        }
        this->endResetModel();
    }
    else
    {
        qWarning() << "Error: Set the fields first!!!";
    }
}

void childTableModel::checkItems(QString keyValue)
{
    if (fields.count() > 0)
    {
        keyFieldValue = keyValue;
        int pos;
        int pos2;
        for (pos = 0; pos <= items.count()-1;pos++)
        {
            items[pos].currentStatus = "N";
            items[pos].previousStatus = "N";
        }
        QSqlQuery childTable(database);
        QString sql;
        sql = "SELECT * FROM " + tableName + " WHERE " + keyField + " = '" + keyValue + "'";
        childTable.exec(sql);
        while (childTable.next())
        {
            for (pos = 0; pos <= items.count()-1;pos++)
            {
                if (items[pos].code == childTable.value(getFieldIndex(childTable,lkKeyField)).toString())
                {
                    items[pos].currentStatus = "C";
                    items[pos].previousStatus = "C";
                    for (pos2 = 0; pos2 <= items[pos].fields.count()-1;pos2++)
                    {
                        items[pos].fields[pos2].fieldValue = childTable.value(getFieldIndex(childTable,items[pos].fields[pos2].fieldName)).toString();
                        items[pos].fields[pos2].previousValue = items[pos].fields[pos2].fieldValue;
                    }
                }
            }
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

QVariant childTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
              return lkDisplayColumnDescription;
            else
                return fields[section-1].fieldDescription;
        }
    }
    return QVariant();
}

bool childTableModel::applyChanges()
{
    int pos;
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
        return true;
    else
    {
        errorDialog error;
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
        error.setErrorMessage(terror);
        error.exec();
        errors.clear();
        return false;
    }
}

void childTableModel::cancelChanges()
{
    int pos;
    int pos2;
    for (pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].currentStatus != items[pos].previousStatus)
        {
            for (pos2 = 0; pos2 <= fields.count()-1;pos2++)
            {
                items[pos].fields[pos2].fieldValue = items[pos].fields[pos2].previousValue;
                items[pos].currentStatus = items[pos].previousStatus;
                items[pos].error = false;
                updateRow(pos,items[pos].currentStatus);
            }
        }
    }
}

void childTableModel::insertData(int row)
{
    QSqlQuery tquery(database);
    QString sql;
    int pos;
    sql = "INSERT INTO " + tableName + "(" + keyField + "," + linkedKeyField + ",";
    for (pos = 0; pos <= fields.count()-1;pos++)
    {
        sql = sql + fields[pos].fieldName + ",";
    }
    sql = sql.left(sql.length()-1) + ")";
    sql = sql + " VALUES (" + "'" + keyFieldValue + "',";
    sql = sql + "'" + items[row].code + "',";
    for (pos = 0; pos <= fields.count()-1;pos++)
    {
        sql = sql + "'" + items[row].fields[pos].fieldValue.toString() + "',";
    }
    sql = sql.left(sql.length()-1) + ")";
    if (!tquery.exec(sql))
    {
        TerrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        updateRow(row,"C");
    }
}

void childTableModel::updateData(int row)
{
    QSqlQuery tquery(database);
    QString sql;
    int pos;
    sql = "UPDATE " + tableName + " SET ";
    for (pos = 0; pos <= fields.count()-1;pos++)
    {
        sql = sql + fields[pos].fieldName + " = ";
        sql = sql + "'" + items[row].fields[pos].fieldValue.toString() + "',";
    }
    sql = sql.left(sql.length()-1);
    sql = sql + " WHERE " + keyField + " = '" + keyFieldValue + "'";
    sql = sql + " AND " + linkedKeyField + " = '" + items[row].code + "'";
    if (!tquery.exec(sql))
    {
        TerrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        updateRow(row,"C");
    }
}

void childTableModel::deleteData(int row)
{
    QSqlQuery tquery(database);
    QString sql;
    sql = "DELETE FROM  " + tableName;
    sql = sql + " WHERE " + keyField + " = '" + keyFieldValue + "'";
    sql = sql + " AND " + linkedKeyField + " = '" + items[row].code + "'";
    if (!tquery.exec(sql))
    {
        TerrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        updateRow(row,"N");
    }
}

void childTableModel::updateRow(int row,QString status)
{
    items[row].currentStatus = status;
    items[row].previousStatus = status;
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,this->fields.count());
    dataChanged(begin,end);
}

QString childTableModel::getSortDescription(QModelIndex index)
{
    return items[index.row()].currentStatus + items[index.row()].description.toUpper();
}

/************************************childTableSortModel***********************************/


childTableSortModel::childTableSortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool childTableSortModel::lessThan(const QModelIndex &left,
                                   const QModelIndex &right) const
{
    if (sourceModel())
    {

        QString leftData = qobject_cast<childTableModel *>(sourceModel())->getSortDescription(left);
        QString rightData = qobject_cast<childTableModel *>(sourceModel())->getSortDescription(right);
        return QString::localeAwareCompare(leftData, rightData) < 0;
    }
    else
    {
        return false;
    }
}

//*************************************  childSelectTableModel ***********************


childSelectTableModel::childSelectTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    useCheckBoxes = true;
}

childSelectTableModel::~childSelectTableModel()
{
    database = QSqlDatabase();
}


int childSelectTableModel::rowCount(const QModelIndex &) const
{
    return items.count();
}

int childSelectTableModel::columnCount(const QModelIndex &) const
{
    return 1;
}

Qt::ItemFlags childSelectTableModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        if (useCheckBoxes)
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
        else
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant childSelectTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
        {
            return items[index.row()].description;
        }
        else
        {
            return QString();
        }
    }
    if (useCheckBoxes)
    {
        if (role == Qt::BackgroundRole)
        {
            if (items[index.row()].currentStatus == "N")
            {
                QBrush Background(QColor(191,255,191)); //We can change this to a nice color
                return Background;
            }
            else
            {
                QBrush Background(QColor(166,202,240));
                return Background;
            }
        }
    }

    if (useCheckBoxes)
    {
        if (role == Qt::CheckStateRole)
        {
            if (index.column() == 0)
            {
                if (items[index.row()].currentStatus == "C")
                    return Qt::Checked;
                else
                    return Qt::Unchecked;
            }
        }
    }
    return QVariant();
}

bool childSelectTableModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (useCheckBoxes)
    {
        if (role == Qt::CheckStateRole)
        {
            int pos;
            for (pos = 0; pos <= items.count()-1;pos++)
            {
                items[pos].currentStatus = "N";
            }

            if (value.toInt() == Qt::Checked)
                items[index.row()].currentStatus = "C";
            else
                items[index.row()].currentStatus = "N";

            QModelIndex begin;
            begin = this->index(0,0);
            QModelIndex end;
            end = this->index(items.count()-1,this->columnCount()-1);
            emit dataChanged(begin,end);
            emit childDataChanged();
            return true;
        }
    }
    return false;
}

int childSelectTableModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

void childSelectTableModel::loadItems(TfieldDef keyfield)
{
    //Load the lookup table
    //We get the last record value of the key to get a new value
    this->beginResetModel();
    items.clear();
    QSqlQuery lktable(database);
    QString sql;
    sql = "SELECT * FROM " + lkTableName;
    if (keyfield.fieldName != "NONE")
    {
        sql = sql + " WHERE " + keyfield.fieldName + " = ";
        sql = sql + "'" + keyfield.fieldValue.toString() + "'";
    }
    if (lktable.exec(sql))
    {
        while (lktable.next())
        {
            TitemDef item;
            item.code = lktable.value(getFieldIndex(lktable,lkKeyField)).toString();
            item.description = lktable.value(getFieldIndex(lktable,lkDisplayColumn)).toString();
            item.currentStatus = "N";
            item.previousStatus = "N";
            item.error = false;
            items.append(item);
        }
    }
    else
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    this->endResetModel();
}

void childSelectTableModel::loadItems()
{
    TfieldDef keyfield;
    keyfield.fieldName = "NONE";
    loadItems(keyfield);
}

void childSelectTableModel::checkItems(QString code)
{

    int pos;
    for (pos = 0; pos <= items.count()-1;pos++)
    {
        items[pos].currentStatus = "N";
        items[pos].previousStatus = "N";
    }

    for (pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].code == code)
        {
            items[pos].currentStatus = "C";
            items[pos].previousStatus = "C";
        }
    }

    QModelIndex begin;
    begin = this->index(0,0);

    QModelIndex end;
    end = this->index(this->rowCount()-1,0);
    dataChanged(begin,end);

}

QVariant childSelectTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
              return lkDisplayColumnDescription;
        }
    }
    return QVariant();
}

QString childSelectTableModel::getSortDescription(QModelIndex index)
{
    return items[index.row()].currentStatus + items[index.row()].description.toUpper();
}


QString childSelectTableModel::getSelectedCode()
{
    int pos;

    for (pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].currentStatus == "C")
        {
            return items[pos].code;
        }
    }
    return "";
}

//***********************

childSelectTableSortModel::childSelectTableSortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool childSelectTableSortModel::lessThan(const QModelIndex &left,
                                   const QModelIndex &right) const
{
    if (sourceModel())
    {

        QString leftData = qobject_cast<childSelectTableModel *>(sourceModel())->getSortDescription(left);
        QString rightData = qobject_cast<childSelectTableModel *>(sourceModel())->getSortDescription(right);
        return QString::localeAwareCompare(leftData, rightData) < 0;
    }
    else
    {
        return false;
    }
}
