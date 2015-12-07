#include "impgenmaint.h"
#include "maintclases.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QHeaderView>

impgenmaint::impgenmaint(QWidget *parent) :
    QWidget(parent)
{
    m_tableView = 0;
    maintUpdated = false;
    sortColID = 1;
}

void impgenmaint::loadMaintenance(QSqlDatabase db,QTableView *tableView)
{
    if (tableView)
    {
        database = db;
        m_mainmodel = new maintModel(this,database);
        m_tableView = tableView;

        m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);

        m_mainmodel->setTable(m_tableName);
        m_mainmodel->setKeyColumn(m_keyField);
        m_mainmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        m_mainmodel->setSort(sortColID,Qt::AscendingOrder);

        QString filter;
        for (int pos = 0; pos <= m_keyFields.count()-1;pos++)
        {
            filter = m_keyFields[pos].code + " = '" +  m_keyFields[pos].value.toString() + "' AND ";
        }
        if (!filter.isEmpty())
        {

            filter = filter.left(filter.length()-5);
            //qDebug() << filter;
            m_mainmodel->setFilter(filter);
        }

        if (m_mainmodel->select())
        {
            m_mainmodel->loadStatus();
            m_tableView->setModel(m_mainmodel);
            m_mainmodel->setView(m_tableView);
            m_mainmodel->setDisplayColumn(m_displayColumn,m_displayColumnDescription);
            m_tableView->horizontalHeader()->setSectionResizeMode(sortColID, QHeaderView::Stretch);
        }
        else
        {
            qWarning() << m_mainmodel->lastError().driverText();
        }
        connect(m_tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(loadItem(QModelIndex)));
        //Connect the value changed signal of IMPWidgets to the slot

        genericFunctions fnc;
        fields = fnc.getFields(m_tableName,this);

        if (m_mainmodel->rowCount() >0)
        {
            m_tableView->setCurrentIndex(m_mainmodel->index(0,sortColID));
            loadItem(m_mainmodel->index(0,sortColID));
        }
        else
        {
            fnc.disableFields(m_tableName,this);
        }
    }
    else
    {
        //qWarning() << "No table view. ok.";
        if (m_keyFields.count() <= 0)
        {
            qWarning() << "Error!!! TableView is 0 and no KeyFields!";
        }
        else
        {
            database = db;
            loadMaintTable();
        }
    }
}

void impgenmaint::resizeEvent ( QResizeEvent *)
{
    //if (m_tableView)
    //    m_tableView->setColumnWidth(1,m_tableView->width()-65);
}

void impgenmaint::loadItem(const QModelIndex & index)
{
    if (m_tableView)
    {
        QVariant value;
        genericFunctions fnc;
        for (int pos = 0; pos <= fields.count()-1;pos++)
        {
            value = m_mainmodel->record(index.row()).value(fields[pos]);
            fnc.applyDataToField(this,m_tableName,fields[pos],value);
        }

        emit itemSelected(m_mainmodel,index);
    }
}

void impgenmaint::unLoadMaintenance()
{
    database = QSqlDatabase();
    if (m_tableView)
    {
        disconnect(m_tableView);
        m_tableView->setModel(new QSqlTableModel());
        m_tableView = 0;
        //delete m_mainmodel;
        //m_mainmodel = 0;
    }
}

impgenmaint::~impgenmaint()
{
    //qDebug() << "Destructor of impgenmaint";
}


void impgenmaint::addNewItem()
{
    if (m_tableView)
    {
        QSqlRecord newRecord;
        genericFunctions fnc;
        fnc.setFieldsToNew(m_tableName,this);
        newRecord = m_mainmodel->record();
        m_mainmodel->insertRecord(-1,newRecord);
        m_mainmodel->insertNewRecord();
        m_tableView->setCurrentIndex(m_mainmodel->index(m_mainmodel->rowCount()-1,sortColID));
        loadItem(m_mainmodel->index(m_mainmodel->rowCount()-1,1));
        m_tableView->scrollTo(m_mainmodel->index(m_mainmodel->rowCount()-1,sortColID));
        fnc.enableFields(m_tableName,this);
    }
}

void impgenmaint::removeItem()
{
    if (m_tableView)
    {
        QModelIndex index;
        index = m_tableView->currentIndex();
        m_mainmodel->updateRecordStatus(index.row(),"D");
        m_tableView->update(index);
    }
}

void impgenmaint::applyChanges()
{
    if (m_tableView)
    {
        if (m_mainmodel->applyChanges())
        {
            maintUpdated = false;
            goToFirst();
        }
    }
    else
    {
        updateTable();
    }
}

void impgenmaint::cancelChanges()
{
    if (m_tableView)
    {
        maintUpdated = false;
        m_mainmodel->cancelChanges();
        goToFirst();
    }
    else
    {
        loadMaintTable();
    }
}

void impgenmaint::closeForm()
{
    emit closeCalled();
}

void impgenmaint::goToFirst()
{
    if (m_tableView)
    {
        m_mainmodel->setSort(1,Qt::AscendingOrder);
        QModelIndex index;
        index = m_mainmodel->index(0,sortColID);
        m_tableView->setCurrentIndex(index);
        loadItem(index);
        m_tableView->scrollTo(index);
        m_tableView->update(index);
    }
}

void impgenmaint::valueHasChanged(QString, QString field, QString value)
{
    if (m_tableView)
    {
        QModelIndex index;
        QSqlRecord currRecord;
        index = m_tableView->currentIndex();
        currRecord = m_mainmodel->record(index.row());
        currRecord.setValue(field,value);
        m_mainmodel->updateRecordStatus(index.row(),"M");
        m_mainmodel->setRecord(index.row(),currRecord);
        m_tableView->update(index);
        if (!maintUpdated)
        {
            maintUpdated = true;
            emit maintHasChanged();
        }
    }
}

void impgenmaint::valueHasChanged(QString, QString field, bool value)
{
    if (m_tableView)
    {
        QModelIndex index;
        QSqlRecord currRecord;
        index = m_tableView->currentIndex();
        currRecord = m_mainmodel->record(index.row());
        currRecord.setValue(field,value);
        m_mainmodel->updateRecordStatus(index.row(),"M");
        m_mainmodel->setRecord(index.row(),currRecord);
        m_tableView->update(index);
        if (!maintUpdated)
        {
            maintUpdated = true;
            emit maintHasChanged();
        }
    }
}


void impgenmaint::setFieldsEnable(bool status)
{
    genericFunctions fnc;
    if (status)
        fnc.enableFields(m_tableName,this);
    else
        fnc.disableFields(m_tableName,this);
}

//*****************Processes to control one maintenace table****************

int impgenmaint::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

void impgenmaint::insertKeyField(QString code, QVariant value, bool useImApplyToAll)
{
    TkeyField field;
    field.code = code;
    field.value = value;
    field.useImApplyToAll = useImApplyToAll;
    m_keyFields.append(field);
}

void impgenmaint::clearKeyFields()
{
    m_keyFields.clear();
}

void impgenmaint::setDataBase(QSqlDatabase db)
{
    database = db;
}

void impgenmaint::loadMaintTable()
{
    genericFunctions fnc;
    fnc.setDataBase(database,this);
    fields = fnc.getFields(m_tableName,this);

    QSqlQuery query(database);
    QString sql;
    int pos;

    sql = "SELECT ";
    for (pos = 0; pos <= fields.count()-1;pos++)
    {
        sql = sql + fields[pos] + ",";
    }
    sql = sql.left(sql.length()-1);
    sql = sql + " FROM " + m_tableName + " WHERE ";
    for (pos = 0; pos <= m_keyFields.count()-1;pos++)
    {
        sql = sql + m_keyFields[pos].code + " = '" ;
        sql = sql + m_keyFields[pos].value.toString() + "' AND " ;
    }
    sql = sql.left(sql.length()-5);
    if (query.exec(sql))
    {
        if (query.first())
        {
            QVariant value;
            for (pos = 0; pos <= fields.count()-1;pos++)
            {
                value = query.value(getFieldIndex(query,fields[pos]));
                fnc.applyDataToField(this,m_tableName,fields[pos],value);
            }
        }
        else
        {
            qWarning() << "Warning: Maintenance table returned 0 rows. Check values";
        }
    }
    else
    {
        qWarning() << "Error: " << query.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

bool impgenmaint::insertCurrentData(QSqlDatabase db,QList<TkeyField> otherData)
{
    genericFunctions fnc;
    QStringList insFields;
    insFields = fnc.getFields(m_tableName,this);
    QSqlQuery tquery(db);
    QString sql;
    sql = "INSERT INTO " + m_tableName + " (";
    int pos;
    for (pos = 0; pos <= insFields.count()-1;pos++)
    {
        sql = sql + insFields[pos] + ",";
    }
    for (pos = 0; pos <= otherData.count()-1;pos++)
    {
        sql = sql + otherData[pos].code + ",";
    }

    sql = sql.left(sql.length()-1);
    sql = sql + ") VALUES (";
    QVariant pvalue;
    for (pos = 0; pos <= insFields.count()-1;pos++)
    {
        pvalue = fnc.getDataFromField(this,m_tableName,insFields[pos]);
        sql = sql + "'" + pvalue.toString().replace("'","_") + "',";
    }
    for (pos = 0; pos <= otherData.count()-1;pos++)
    {
        sql = sql + "'" + otherData[pos].value.toString().replace("'","_") + "',";
    }

    sql = sql.left(sql.length()-1);
    sql = sql + ")";
    if (!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        return false;
    }
    return true;
}

void impgenmaint::updateTable()
{
    genericFunctions fnc;
    QSqlQuery tquery(database);
    QString sql;
    int pos;
    int pos2;
    QList <TIMPlinkedField> lkfields;
    QVariant pvalue;
    QVariant lkvalue;
    sql = "UPDATE " + m_tableName + " SET ";
    for (pos = 0; pos <= fields.count()-1;pos++)
    {
        sql = sql + fields[pos] + " = ";
        pvalue = fnc.getDataFromField(this,m_tableName,fields[pos]);
        if ((pvalue.toString() != "NULL") && (!pvalue.toString().isEmpty()))
            sql = sql + "'" + pvalue.toString().replace("'","_") + "',";
        else
            sql = sql + "NULL,";
        lkfields = getLinkedFields(fields[pos]);
        for (pos2 = 0; pos2 <= lkfields.count()-1;pos2++)
        {
            sql = sql + lkfields[pos2].childField + " = ";
            lkvalue = getLinkedValue(fields[pos],lkfields[pos2].childField,pvalue);
            if ((lkvalue.toString() != "NULL") && (!lkvalue.toString().isEmpty()))
                sql = sql + "'" + lkvalue.toString() + "',";
            else
                sql = sql + "NULL,";
        }
    }
    sql = sql.left(sql.length()-1);
    sql = sql + " WHERE ";
    for (pos = 0; pos <= m_keyFields.count()-1;pos++)
    {
        sql = sql + m_keyFields[pos].code + " = ";
        sql = sql + "'" + m_keyFields[pos].value.toString() + "' AND ";
    }
    sql = sql.left(sql.length()-5);
    //qDebug() << "Executing: " << sql;
    if(!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

void impgenmaint::applyToAll()
{
    applyToAll("NULL");
}

void impgenmaint::applyToAll(QString whereClause)
{
    genericFunctions fnc;
    QSqlQuery tquery(database);
    QString sql;
    int pos;
    int pos2;
    QList <TIMPlinkedField> lkfields;
    QVariant pvalue;
    QVariant lkvalue;
    sql = "UPDATE " + m_tableName + " SET ";
    for (pos = 0; pos <= fields.count()-1;pos++)
    {
        sql = sql + fields[pos] + " = ";
        pvalue = fnc.getDataFromField(this,m_tableName,fields[pos]);
        if ((pvalue.toString() != "NULL") && (!pvalue.toString().isEmpty()))
            sql = sql + "'" + pvalue.toString() + "',";
        else
            sql = sql + "NULL,";
        lkfields = getLinkedFields(fields[pos]);
        for (pos2 = 0; pos2 <= lkfields.count()-1;pos2++)
        {
            sql = sql + lkfields[pos2].childField + " = ";
            lkvalue = getLinkedValue(fields[pos],lkfields[pos2].childField,pvalue);
            if ((lkvalue.toString() != "NULL") && (!lkvalue.toString().isEmpty()))
                sql = sql + "'" + lkvalue.toString() + "',";
            else
                sql = sql + "NULL,";
        }
    }
    sql = sql.left(sql.length()-1);
    sql = sql + " WHERE ";
    for (pos = 0; pos <= m_keyFields.count()-1;pos++)
    {
        if (m_keyFields[pos].useImApplyToAll)
        {
            sql = sql + m_keyFields[pos].code + " = ";
            sql = sql + "'" + m_keyFields[pos].value.toString() + "' AND ";
        }
    }
    if (whereClause != "NULL")
        sql = sql + whereClause;
    else
        sql = sql.left(sql.length()-5);
    if(!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

void impgenmaint::setFieldsToTable(QString table)
{
    genericFunctions fnc;
    fnc.setFieldsToTable(table,this);
}

void impgenmaint::hideField(QString table, QString field)
{
    genericFunctions fnc;
    fnc.hideField(table,field,this);
}

void impgenmaint::disableField(QString table, QString field)
{
    genericFunctions fnc;
    fnc.disableField(table,field,this);
}

void impgenmaint::addLinkedValue(QString parent, QString child, QVariant parentValue, QVariant childValue)
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
        TIMPlinkedValue value;
        value.parentValue = parentValue;
        value.childValue = childValue;
        m_linkedFields[pos2].values.append(value);
    }
    else
    {
        qWarning() << "Cannot add linked value. Parent with child not found!";
    }
}

void impgenmaint::addLinkedField(QString parent, QString child, QVariant defaultValue)
{
    int pos;
    bool found;
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
        TIMPlinkedField field;
        field.parentField = parent;
        field.childField = child;
        field.defaultValue = defaultValue;
        m_linkedFields.append(field);
    }
    else
        qWarning() << "Cannot add linked field. Linked field already in list";
}

QList <TIMPlinkedField> impgenmaint::getLinkedFields(QString parent)
{
   QList <TIMPlinkedField> res;
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

QVariant impgenmaint::getLinkedValue(QString parent, QString child, QVariant parentValue)
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
