#include "maintclases.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QStringList>
#include <QPixmap>
#include <QDebug>
#include "impedit.h"
#include "impcheckbox.h"
#include "impgroupbox.h"
#include "impradiobutton.h"
#include "impcombobox.h"
#include "errordialog.h"
#include <QSqlError>
#include <QSqlQuery>


maintModel::maintModel(QObject *parent, QSqlDatabase db)
    :QSqlTableModel(parent,db)
{
}

maintModel::~maintModel()
{
    parentView = 0;
    status.clear();
}

QVariant maintModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole)
    {
        if (status[index.row()].status == "M")
        {
            return QPixmap(":/modify2.png");
        }
        if (status[index.row()].status == "D")
        {
            return QPixmap(":/delete3.png");
        }
        if (status[index.row()].status == "N")
        {
            return QPixmap(":/new3.png");
        }
        return QVariant();
    }
    else
    {
        return QSqlTableModel::data(index, role);
    }
}

void maintModel::loadStatus()
{
    TfieldStatus statusvalue;
    for (int pos=0; pos <= this->rowCount()-1; pos++)
    {     
        statusvalue.code = record(pos).value(keyColumn).toString();
        statusvalue.status = "C";
        status.append(statusvalue);
    }
    //We get the last record value of the key to get a new value
    QSqlQuery temp(this->database());
    QString sql;
    sql = "SELECT max(cast(" + keyColumn + " as integer)) FROM " + this->tableName();
    temp.exec(sql);
    temp.first();
    newRecords = temp.value(0).toInt();
}

void maintModel::insertNewRecord()
{

    QSqlRecord lastRecord;
    QString newCode;
    TfieldStatus statusvalue;

    newRecords++;

    //Add zeros to the begining of the code
    newCode = QString::number(newRecords);
    int tl;
    tl = newCode.length();
    for (int pos = 1; pos <= 3 - tl; pos++)
    {
        newCode = "0" + newCode;
    }
    statusvalue.code = newCode;
    statusvalue.status = "N";

    lastRecord = record(this->rowCount()-1);
    lastRecord.setValue(keyColumn,statusvalue.code);
    this->setRecord(this->rowCount()-1,lastRecord);
    status.append(statusvalue);
}

void maintModel::setDisplayColumn(QString column,QString description)
{
    displayColumnCode = column;
    displayColumnDesc = description;

    int pos;
    for (pos = 0; pos <= columnCount()-1;pos++)
    {
        parentView->hideColumn(pos);
    }
    pos = fieldIndex(column);
    parentView->showColumn(pos);
    this->setHeaderData(pos, Qt::Horizontal, displayColumnDesc);
}

void maintModel::setView(QTableView *view)
{
    parentView = view;
}

void maintModel::setKeyColumn(QString column)
{
    keyColumn = column;
}

int maintModel::getLastNewRecord()
{
    return newRecords;
}

void maintModel::updateRecordStatus(int index,QString value)
{
    if (value == "M")
    {
      if ((status[index].status != "N") && (status[index].status != "D"))
          status[index].status = value;
    }
    if (value == "D")
    {
        status[index].status = value;
    }
}

void maintModel::deleteMarkedRecords()
{
    QString key;
    int pos2;
    for (int pos = 0; pos <= status.count()-1 ;pos++)
    {
        if (status[pos].status == "D")
        {
            key = status[pos].code;
            for (pos2 = 0; pos2 <= this->rowCount()-1; pos2++)
            {
                if (this->record(pos2).value(keyColumn).toString() == key)
                {
                    this->removeRow(pos2);
                    break;
                }
            }
        }
    }
}

bool maintModel::applyChanges()
{
    deleteMarkedRecords();
    if (submitAll())
    {
        status.clear();
        loadStatus();
        return true;
    }
    else
    {


        errorDialog error;
        QSqlError errormsg;
        errormsg = lastError();

        qWarning() << "Error: " << errormsg.databaseText();
        //qWarning() << "Executing: " << errormsg.
        error.setErrorMessage(errormsg.databaseText());
        error.exec();
        return false;
    }
}

void maintModel::cancelChanges()
{
    revertAll();
    status.clear();
    loadStatus();
}

//*****************************Generic functions

genericFunctions::genericFunctions()
{
}

QStringList genericFunctions::getFields(QString tableName, QWidget *parent)
{
    fields.clear();
    BrowseChildren(tableName,parent,1);
    return fields;
}

void genericFunctions::enableFields(QString tableName, QWidget *parent)
{
    BrowseChildren(tableName,parent,2);
}

void genericFunctions::disableFields(QString tableName, QWidget *parent)
{
    BrowseChildren(tableName,parent,3);
}

void genericFunctions::setFieldsToNew(QString tableName, QWidget *parent)
{
    BrowseChildren(tableName,parent,4);
}

void genericFunctions::setFieldsToTable(QString tableName, QWidget *parent)
{
    QObjectList children = parent->children();
    QObjectList::const_iterator it = children.begin();
    QObjectList::const_iterator eIt = children.end();
    QWidget * pChild;
    while ( it != eIt )
    {
        pChild = (QWidget *)(*it++);
        if (qobject_cast<IMPEdit *>(pChild))
        {
            qobject_cast<IMPEdit *>(pChild)->setTableName(tableName);
        }
        if (qobject_cast<IMPCheckBox *>(pChild))
        {
            qobject_cast<IMPCheckBox *>(pChild)->setTableName(tableName);
        }

        if (qobject_cast<IMPRadioButton *>(pChild))
        {
            qobject_cast<IMPRadioButton *>(pChild)->setTableName(tableName);
        }
        if (qobject_cast<IMPComboBox *>(pChild))
        {
            qobject_cast<IMPComboBox *>(pChild)->setTableName(tableName);
        }
        setFieldsToTable(tableName, pChild);
    }
    pChild = 0;
}

void genericFunctions::setDataBase(QSqlDatabase db,QWidget *parent)
{
    QObjectList children = parent->children();
    QObjectList::const_iterator it = children.begin();
    QObjectList::const_iterator eIt = children.end();
    QWidget * pChild;
    while ( it != eIt )
    {
        pChild = (QWidget *)(*it++);
        if (qobject_cast<IMPEdit *>(pChild))
        {
            qobject_cast<IMPEdit *>(pChild)->setDataBase(db);
        }
        setDataBase(db,pChild);
    }
    pChild = 0;
}

void genericFunctions::disableField(QString tableName, QString fieldName, QWidget *parent)
{
    QObjectList children = parent->children();
    QObjectList::const_iterator it = children.begin();
    QObjectList::const_iterator eIt = children.end();
    QWidget * pChild;
    while ( it != eIt )
    {
        pChild = (QWidget *)(*it++);
        if (qobject_cast<IMPEdit *>(pChild))
        {
            if ((qobject_cast<IMPEdit *>(pChild)->tableName() == tableName) &&
                (qobject_cast<IMPEdit *>(pChild)->fieldName() == fieldName))
            {
                qobject_cast<IMPEdit *>(pChild)->setTableName("NULL");
                qobject_cast<IMPEdit *>(pChild)->setFieldName("NULL");
                pChild->setEnabled(false);
            }
        }
        if (qobject_cast<IMPCheckBox *>(pChild))
        {
            if ((qobject_cast<IMPCheckBox *>(pChild)->tableName() == tableName) &&
                (qobject_cast<IMPCheckBox *>(pChild)->fieldName() == fieldName))
            {
                qobject_cast<IMPCheckBox *>(pChild)->setTableName("NULL");
                qobject_cast<IMPCheckBox *>(pChild)->setFieldName("NULL");
                pChild->setEnabled(false);
            }
        }

        if (qobject_cast<IMPRadioButton *>(pChild))
        {
            if ((qobject_cast<IMPRadioButton *>(pChild)->tableName() == tableName) &&
                (qobject_cast<IMPRadioButton *>(pChild)->fieldName() == fieldName))
            {
                qobject_cast<IMPRadioButton *>(pChild)->setTableName("NULL");
                qobject_cast<IMPRadioButton *>(pChild)->setFieldName("NULL");
                pChild->setEnabled(false);
            }
        }
        if (qobject_cast<IMPComboBox *>(pChild))
        {
            if ((qobject_cast<IMPComboBox *>(pChild)->tableName() == tableName) &&
                (qobject_cast<IMPComboBox *>(pChild)->fieldName() == fieldName))
            {
                qobject_cast<IMPComboBox *>(pChild)->setTableName("NULL");
                qobject_cast<IMPComboBox *>(pChild)->setFieldName("NULL");
                pChild->setEnabled(false);
            }
        }
        disableField(tableName,fieldName,pChild);
    }
    pChild = 0;
}

void genericFunctions::hideField(QString tableName, QString fieldName, QWidget *parent)
{
    QObjectList children = parent->children();
    QObjectList::const_iterator it = children.begin();
    QObjectList::const_iterator eIt = children.end();
    QWidget * pChild;
    while ( it != eIt )
    {
        pChild = (QWidget *)(*it++);
        if (qobject_cast<IMPEdit *>(pChild))
        {
            if ((qobject_cast<IMPEdit *>(pChild)->tableName() == tableName) &&
                (qobject_cast<IMPEdit *>(pChild)->fieldName() == fieldName))
            {
                qobject_cast<IMPEdit *>(pChild)->setTableName("NULL");
                qobject_cast<IMPEdit *>(pChild)->setFieldName("NULL");
                pChild->setVisible(false);
            }
        }
        if (qobject_cast<IMPCheckBox *>(pChild))
        {
            if ((qobject_cast<IMPCheckBox *>(pChild)->tableName() == tableName) &&
                (qobject_cast<IMPCheckBox *>(pChild)->fieldName() == fieldName))
            {
                qobject_cast<IMPCheckBox *>(pChild)->setTableName("NULL");
                qobject_cast<IMPCheckBox *>(pChild)->setFieldName("NULL");
                pChild->setVisible(false);
            }
        }

        if (qobject_cast<IMPRadioButton *>(pChild))
        {
            if ((qobject_cast<IMPRadioButton *>(pChild)->tableName() == tableName) &&
                (qobject_cast<IMPRadioButton *>(pChild)->fieldName() == fieldName))
            {
                qobject_cast<IMPRadioButton *>(pChild)->setTableName("NULL");
                qobject_cast<IMPRadioButton *>(pChild)->setFieldName("NULL");
                pChild->setVisible(false);
            }
        }
        if (qobject_cast<IMPComboBox *>(pChild))
        {
            if ((qobject_cast<IMPComboBox *>(pChild)->tableName() == tableName) &&
                (qobject_cast<IMPComboBox *>(pChild)->fieldName() == fieldName))
            {
                qobject_cast<IMPComboBox *>(pChild)->setTableName("NULL");
                qobject_cast<IMPComboBox *>(pChild)->setFieldName("NULL");
                pChild->setVisible(false);
            }
        }
        hideField(tableName,fieldName,pChild);
    }
    pChild = 0;
}

void genericFunctions::BrowseChildren(QString tableName, QWidget * parent, int role )
{
    QObjectList children = parent->children();
    QObjectList::const_iterator it = children.begin();
    QObjectList::const_iterator eIt = children.end();
    QWidget * pChild;
    while ( it != eIt )
    {
        pChild = (QWidget *)(*it++);
        if (qobject_cast<IMPEdit *>(pChild))
        {
            if (qobject_cast<IMPEdit *>(pChild)->tableName().toUpper() == tableName.toUpper())
            {
                switch (role)
                {
                case 1:
                    fields << qobject_cast<IMPEdit *>(pChild)->fieldName();
                    break;
                case 2:
                    qobject_cast<IMPEdit *>(pChild)->setEnabled(true);
                    break;
                case 3:
                    qobject_cast<IMPEdit *>(pChild)->setEnabled(false);
                    break;
                case 4:
                    qobject_cast<IMPEdit *>(pChild)->setText("");
                    break;

                default:
                    fields << qobject_cast<IMPEdit *>(pChild)->fieldName();
                }
            }
        }
        if (qobject_cast<IMPCheckBox *>(pChild))
        {
            if (qobject_cast<IMPCheckBox *>(pChild)->tableName().toUpper() == tableName.toUpper())
            {
                switch (role)
                {
                case 1:
                    fields << qobject_cast<IMPCheckBox *>(pChild)->fieldName();
                    break;
                case 2:
                    qobject_cast<IMPCheckBox *>(pChild)->setEnabled(true);
                    break;
                case 3:
                    qobject_cast<IMPCheckBox *>(pChild)->setEnabled(false);
                    break;
                case 4:
                    qobject_cast<IMPCheckBox *>(pChild)->setChecked(false);
                    break;

                default:
                    fields << qobject_cast<IMPCheckBox *>(pChild)->fieldName();
                }
            }
        }

        if (qobject_cast<IMPRadioButton *>(pChild))
        {
            if (qobject_cast<IMPRadioButton *>(pChild)->tableName().toUpper() == tableName.toUpper())
            {
                switch (role)
                {
                case 1:
                    fields << qobject_cast<IMPRadioButton *>(pChild)->fieldName();
                    break;
                case 2:
                    qobject_cast<IMPRadioButton *>(pChild)->setEnabled(true);
                    break;
                case 3:
                    qobject_cast<IMPRadioButton *>(pChild)->setEnabled(false);
                    break;
                case 4:
                    qobject_cast<IMPRadioButton *>(pChild)->setChecked(false);
                    break;

                default:
                    fields << qobject_cast<IMPRadioButton *>(pChild)->fieldName();
                }
            }
        }
        if (qobject_cast<IMPComboBox *>(pChild))
        {
            if (qobject_cast<IMPComboBox *>(pChild)->tableName().toUpper() == tableName.toUpper())
            {
                switch (role)
                {
                case 1:
                    fields << qobject_cast<IMPComboBox *>(pChild)->fieldName();
                    break;
                case 2:
                    qobject_cast<IMPComboBox *>(pChild)->setEnabled(true);
                    break;
                case 3:
                    qobject_cast<IMPComboBox *>(pChild)->setEnabled(false);
                    break;
                case 4:
                    qobject_cast<IMPComboBox *>(pChild)->setCurrentIndex(0);
                    break;

                default:
                    fields << qobject_cast<IMPComboBox *>(pChild)->fieldName();
                }
            }
        }

        /*if (qobject_cast<IMPGroupBox *>(pChild))
        {
            if (qobject_cast<IMPGroupBox *>(pChild)->tableName().toUpper() == tableName.toUpper())
            {
                fields << qobject_cast<IMPGroupBox *>(pChild)->fieldName();
            }
        }*/

        BrowseChildren(tableName, pChild, role );
    }
    pChild = 0;
}

void genericFunctions::applyDataToField(QWidget *parent, QString tableName, QString fieldName, QVariant value)
{
    applyDataToChildren(parent,tableName,fieldName,value);
}

void genericFunctions::applyDataToChildren(QWidget *parent, QString tableName, QString fieldName, QVariant value)
{
    QObjectList children = parent->children();
    QObjectList::const_iterator it = children.begin();
    QObjectList::const_iterator eIt = children.end();
    QWidget * pChild;
    while ( it != eIt )
    {
        pChild = (QWidget *)(*it++);
        if (qobject_cast<IMPEdit *>(pChild))
        {
            if ((qobject_cast<IMPEdit *>(pChild)->tableName().toUpper() == tableName.toUpper()) &&
                qobject_cast<IMPEdit *>(pChild)->fieldName().toUpper() == fieldName.toUpper())
            {
                qobject_cast<IMPEdit *>(pChild)->setText(value.toString());
            }
        }
        if (qobject_cast<IMPCheckBox *>(pChild))
        {
            if ((qobject_cast<IMPCheckBox *>(pChild)->tableName().toUpper() == tableName.toUpper()) &&
                qobject_cast<IMPCheckBox *>(pChild)->fieldName().toUpper() == fieldName.toUpper())
            {
                if ((value.toString().toUpper() == "Y") ||
                    (value.toString().toUpper() == "T") ||
                    (value.toString().toUpper() == "1"))
                    qobject_cast<IMPCheckBox *>(pChild)->setChecked(true);
                else
                    qobject_cast<IMPCheckBox *>(pChild)->setChecked(false);
            }
        }
        if (qobject_cast<IMPRadioButton *>(pChild))
        {
            if ((qobject_cast<IMPRadioButton *>(pChild)->tableName().toUpper() == tableName.toUpper()) &&
                (qobject_cast<IMPRadioButton *>(pChild)->fieldName().toUpper() == fieldName.toUpper()) &&
                (qobject_cast<IMPRadioButton *>(pChild)->fieldValue().toUpper() == value.toString().toUpper()))
            {
                qobject_cast<IMPRadioButton *>(pChild)->setChecked(true);
            }
        }
        if (qobject_cast<IMPComboBox *>(pChild))
        {
            if ((qobject_cast<IMPComboBox *>(pChild)->tableName().toUpper() == tableName.toUpper()) &&
                (qobject_cast<IMPComboBox *>(pChild)->fieldName().toUpper() == fieldName.toUpper()))
            {
                qobject_cast<IMPComboBox *>(pChild)->selectValue(value.toString());
            }
        }

        applyDataToChildren(pChild,tableName,fieldName,value);
    }
    pChild = 0;
}

QVariant genericFunctions::getDataFromField(QWidget *parent, QString tableName, QString fieldName)
{
    QVariant res;
    res = getDataFromChildren(parent,tableName,fieldName);
    if (res.toString() == "~NoValue*!~")
        return QVariant();
    else
        return res;
}

QVariant genericFunctions::getDataFromChildren(QWidget *parent, QString tableName, QString fieldName)
{
    QObjectList children = parent->children();
    QObjectList::const_iterator it = children.begin();
    QObjectList::const_iterator eIt = children.end();
    QWidget * pChild;

    QVariant res;
    res = QString("~NoValue*!~");

    while ( it != eIt )
    {
        pChild = (QWidget *)(*it++);
        if (qobject_cast<IMPEdit *>(pChild))
        {
            if ((qobject_cast<IMPEdit *>(pChild)->tableName().toUpper() == tableName.toUpper()) &&
                qobject_cast<IMPEdit *>(pChild)->fieldName().toUpper() == fieldName.toUpper())
            {
                res = qobject_cast<IMPEdit *>(pChild)->text();
            }
        }
        if (qobject_cast<IMPCheckBox *>(pChild))
        {
            if ((qobject_cast<IMPCheckBox *>(pChild)->tableName().toUpper() == tableName.toUpper()) &&
                qobject_cast<IMPCheckBox *>(pChild)->fieldName().toUpper() == fieldName.toUpper())
            {
                if (qobject_cast<IMPCheckBox *>(pChild)->isChecked())
                    res = qobject_cast<IMPCheckBox *>(pChild)->checkedValue();
                else
                    res = qobject_cast<IMPCheckBox *>(pChild)->unCheckedValue();
            }
        }
        if (qobject_cast<IMPRadioButton *>(pChild))
        {
            if ((qobject_cast<IMPRadioButton *>(pChild)->tableName().toUpper() == tableName.toUpper()) &&
                qobject_cast<IMPRadioButton *>(pChild)->fieldName().toUpper() == fieldName.toUpper())
            {
                res = qobject_cast<IMPRadioButton *>(pChild)->isChecked();
            }
        }
        if (qobject_cast<IMPComboBox *>(pChild))
        {
            if ((qobject_cast<IMPComboBox *>(pChild)->tableName().toUpper() == tableName.toUpper()) &&
                qobject_cast<IMPComboBox *>(pChild)->fieldName().toUpper() == fieldName.toUpper())
            {
                res = qobject_cast<IMPComboBox *>(pChild)->getCurrentValue();
            }
        }
        if (res.toString() == "~NoValue*!~")
            res = getDataFromChildren(pChild,tableName,fieldName);
        else
            break;
    }
    pChild = 0;
    return res;
}
