#include "genmodels.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include "prjsettings.h"

genericFldModel::genericFldModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    nShowCols = -1;
    m_tableName = "";
    isReadOnly = false;
}

genericFldModel::~genericFldModel()
{
    //qDebug() << "Destroy: genericFldModel";
}

void genericFldModel::resetModelStructure()
{
    this->beginResetModel();
    nShowCols = -1;
    //Clear the whole structure
    m_fields.clear();
    m_insertedKeys.clear();
    m_genModelROFields.clear();
    m_genModelROFieldFlags.clear();
    m_showValues.clear();
    m_columns.clear();
    m_items.clear();
    m_keyValues.clear();
    m_distException.clear();
    errors.clear();
    this->endResetModel();
}

int genericFldModel::getFieldByIndex(int idx) const
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

int genericFldModel::getColumnByIndex(int idx) const
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

int genericFldModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int genericFldModel::columnCount(const QModelIndex &) const
{
    return nShowCols+1;
}

Qt::ItemFlags genericFldModel::flags(const QModelIndex &index) const
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

QVariant genericFldModel::data(const QModelIndex &index, int role) const
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

bool genericFldModel::setData(const QModelIndex &index,const QVariant &value,int role)
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

QVariant genericFldModel::headerData(int section, Qt::Orientation orientation, int role) const
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

int genericFldModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

void genericFldModel::setDatabase(QSqlDatabase database)
{
    m_database = database;
}

void genericFldModel::setTableName(QString tablename)
{
    m_tableName = tablename;
}

void genericFldModel::insertField(QString code,
                 QString description,
                 QVariant value,
                 QVariant defvalue,
                 bool key,
                 bool autogen,
                 bool show,
                 bool editable,
                 QColor Color)
{
    TgenModelFieldDef field;
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

void genericFldModel::insertExtraColumn(QString code,
                       QString description,
                       QVariant defaultValue,
                       QVariant valueWhenNew,
                       QVariant valueWhenModified,
                       QVariant valueWhenDeleted,
                       QColor color)
{
    TgenModelextDataCol column;
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

void genericFldModel::insertNewItem()
{

    TgenModelItemDef item;
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
    emit modeldDataChanged();
}

QVariant genericFldModel::getDataFromField(int row, QString field)
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

QVariant genericFldModel::getOldDataFromField(int row, QString field)
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

void genericFldModel::setDataToField(int row, QString field, QVariant value)
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

QList<TgenModelFieldDef> genericFldModel::getRowKeyValues(int row)
{
    QList<TgenModelFieldDef> res;
    for (int pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].key == true)
        {
            res.append(m_items[row].fields[pos]);
        }
    }
    return res;
}

QString genericFldModel::getRowStatus(int row)
{
    return m_items[row].currentStatus;
}

void genericFldModel::deleteItem(int row)
{
    m_items[row].currentStatus = "D";
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,nShowCols);
    emit dataChanged(begin,end);
    emit modeldDataChanged();
}

void genericFldModel::loadData(QList<TgenModelFieldDef> keyValues)
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
        if (!table.exec(sql))
        {
            qWarning() << "Error: " << table.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }


        while (table.next())
        {
            TgenModelItemDef item;
            item.currentStatus = "C";
            item.previousStatus = "C";
            item.error = false;
            for (pos = 0; pos <= m_fields.count()-1;pos++)
            {
                TgenModelFieldDef field;
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

void genericFldModel::removeDeleted()
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
    }
}

bool genericFldModel::applyChanges()
{
    errors.clear();
    m_insertedKeys.clear();
    m_updatedKeys.clear();
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

void genericFldModel::cancelChanges()
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

QString genericFldModel::genAutoGenKey(int fieldIndex)
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
    for (pos = 1; pos <= 3 - tl; pos++)
    {
        newCode = "0" + newCode;
    }
    return newCode;
}

void genericFldModel::insertData(int row)
{
    int pos;
    QSqlQuery tquery(m_database);
    QString sql;

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
        TgenModelErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        TgenModelInsertedKeys inserted;
        inserted.row = row;
        for (pos = 0; pos <= m_fields.count()-1;pos++)
        {
            if (m_fields[pos].key == true)
            {
                TgenModelFieldDef inskey;
                inskey.code = m_items[row].fields[pos].code;
                inskey.value = m_items[row].fields[pos].value;
                inserted.keys.append(inskey);
            }
        }
        m_insertedKeys.append(inserted);
        updateRow(row,"C");
    }
}

void genericFldModel::updateData(int row)
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
        TgenModelErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    else
    {
        TgenModelInsertedKeys inserted;
        inserted.row = row;
        for (pos = 0; pos <= m_fields.count()-1;pos++)
        {
            if (m_fields[pos].key == true)
            {
                TgenModelFieldDef inskey;
                inskey.code = m_items[row].fields[pos].code;
                inskey.value = m_items[row].fields[pos].value;
                inserted.keys.append(inskey);
            }
        }
        m_updatedKeys.append(inserted);
        updateRow(row,"C");
    }
}

void genericFldModel::deleteData(int row)
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
        TgenModelErrorDef error;
        error.row = row;
        error.error = tquery.lastError().databaseText();
        errors.append(error);
    }
    else
    {
        m_items[row].currentStatus = "CLR";
    }
}

void genericFldModel::updateRow(int row,QString status)
{
    m_items[row].currentStatus = status;
    m_items[row].previousStatus = status;
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,nShowCols);
    emit dataChanged(begin,end);
}

void genericFldModel::setReadOnly(bool readOnly)
{
    isReadOnly = readOnly;
}

void genericFldModel::setWhereClause(QString clause)
{
    m_whereClause = clause;
}

void genericFldModel::addReadOnlyField(QString field, QString whenStatus)
{
    TgenModelROField afield;
    afield.field = field;
    afield.whenStatus = whenStatus;
    m_genModelROFields.append(afield);
}

void genericFldModel::addReadOnlyField(QString field, QString whenField, QVariant whenFieldValue, QString whenStatus)
{
    TgenModelROFieldFlag flag;
    flag.field = field;
    flag.whenField = whenField;
    flag.whenFieldValue = whenFieldValue;
    flag.whenStatus = whenStatus;
    m_genModelROFieldFlags.append(flag);
}

bool genericFldModel::isFieldReadOnly(QString field, QString status) const
{
    bool res;
    res = false;
    for (int pos = 0; pos <= m_genModelROFields.count()-1;pos++)
    {
        if ((m_genModelROFields[pos].field == field) &&
            ((m_genModelROFields[pos].whenStatus == status) || (m_genModelROFields[pos].whenStatus == "ALL")))
        {
            res = true;
            break;
        }
    }
    return res;
}

bool genericFldModel::isFieldReadOnly(QString field, int itemIndex, QString status) const
{
    bool res;
    res = false;
    QString whenField;
    QVariant whenValue;
    QString whenStatus;
    int pos;
    int pos2;
    {
        for (pos = 0; pos <= m_genModelROFieldFlags.count()-1;pos++)
        {
            if (m_genModelROFieldFlags[pos].field == field)
            {
                whenField = m_genModelROFieldFlags[pos].whenField;
                whenValue = m_genModelROFieldFlags[pos].whenFieldValue;
                whenStatus = m_genModelROFieldFlags[pos].whenStatus;
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

void genericFldModel::addShowValue(QString field, QVariant value, QString whenField, QVariant whenValue)
{
    TgenModelFldShowValue showvalue;
    showvalue.field = field;
    showvalue.value = value;
    showvalue.whenField = whenField;
    showvalue.whenValue = whenValue;
    m_showValues.append(showvalue);
}

QVariant genericFldModel::getShowValue(int itemIndex, QString field,QVariant currentValue) const
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

void genericFldModel::addDistributionException(int column)
{
    m_distException.append(column);
}

bool genericFldModel::isColInExceptions(int column) const
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

/************************************systemTypesModel***********************************/

systemTypesModel::systemTypesModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    m_readOnly = true;
}

systemTypesModel::~systemTypesModel()
{
    //qDebug() << "Destroy: systemTypesModel";
}

void systemTypesModel::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
}

int systemTypesModel::rowCount(const QModelIndex &) const
{
    return items.count();
}

int systemTypesModel::columnCount(const QModelIndex &) const
{
    return 3;
}

Qt::ItemFlags systemTypesModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant systemTypesModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return items[index.row()].description;
        if (index.column() == 1)
            return items[index.row()].crop;
        if (index.column() == 2)
            return items[index.row()].livestock;
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
            QBrush Background(QColor(166,202,240));
            return Background;
        }
    }
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
    return QVariant();
}

bool systemTypesModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (m_readOnly)
        return false;
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
        begin = this->index(index.row(),0);
        QModelIndex end;
        end = this->index(index.row(),this->columnCount()-1);
        emit dataChanged(begin,end);
        emit modelDataChanged();
        return true;
    }
    return false;
}

void systemTypesModel::loadItems()
{
    this->beginResetModel();
    items.clear();
    QSqlQuery table(database);
    QString sql;
    sql = "SELECT systype.SYST_COD,systype.SYST_DES,crop.CROP_DES,livestoc.LIVE_DES";
    sql = sql + ",crop.CROP_COD,livestoc.LIVE_COD";
    sql = sql + " FROM systype,crop,livestoc WHERE";
    sql = sql + " systype.crop_cod = crop.crop_cod";
    sql = sql + " AND systype.live_cod = livestoc.live_cod";
    if (table.exec(sql))
    {
        while (table.next())
        {
            systemTypeItem item;
            item.code = table.value(0).toString();
            item.description = table.value(1).toString();
            item.crop = table.value(2).toString();
            item.livestock = table.value(3).toString();
            item.cropCode = table.value(4).toString();
            item.livestockCode = table.value(5).toString();
            item.currentStatus = "N";
            items.append(item);
        }
    }
    else
    {
        qWarning() << "Error: " << table.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    this->endResetModel();
}

void systemTypesModel::checkItems(QString code)
{
    int pos;
    for (pos = 0; pos <= items.count()-1;pos++)
    {
        items[pos].currentStatus = "N";
    }
    for (pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].code == code)
        {
            items[pos].currentStatus = "C";
        }
    }
    QModelIndex begin;
    begin = this->index(0,0);

    QModelIndex end;
    end = this->index(this->rowCount()-1,0);
    dataChanged(begin,end);
}

QVariant systemTypesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "System Type";
            if (section == 1)
                return "Crop";
            if (section == 2)
                return "Livestock";
        }
    }
    return QVariant();
}

QString systemTypesModel::getSortDescription(QModelIndex index)
{
    return items[index.row()].currentStatus + items[index.row()].description.toUpper();
}

bool systemTypesModel::hasCheckedItems()
{
    bool res;
    res = false;
    for (int pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].currentStatus == "C")
        {
            res = true;
            break;
        }
    }
    return res;
}

QString systemTypesModel::getSystemTypeCode()
{
    QString res;
    for (int pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].currentStatus == "C")
        {
            res = items[pos].code;
            break;
        }
    }
    return res;
}

QString systemTypesModel::getCropCode()
{
    QString res;
    for (int pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].currentStatus == "C")
        {
            res = items[pos].cropCode;
            break;
        }
    }
    return res;
}

QString systemTypesModel::getLivestockCode()
{
    QString res;
    for (int pos = 0; pos <= items.count()-1;pos++)
    {
        if (items[pos].currentStatus == "C")
        {
            res = items[pos].livestockCode;
            break;
        }
    }
    return res;
}

/************************************systemTypesSortModel***********************************/


systemTypesSortModel::systemTypesSortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool systemTypesSortModel::lessThan(const QModelIndex &left,
                                   const QModelIndex &right) const
{
    if (sourceModel())
    {

        QString leftData = qobject_cast<systemTypesModel *>(sourceModel())->getSortDescription(left);
        QString rightData = qobject_cast<systemTypesModel *>(sourceModel())->getSortDescription(right);
        return QString::localeAwareCompare(leftData, rightData) < 0;
    }
    else
    {
        return false;
    }
}
