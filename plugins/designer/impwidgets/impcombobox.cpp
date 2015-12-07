#include "impcombobox.h"
#include <qsqlquery.h>
#include <QSqlRecord>



IMPComboBox::IMPComboBox(QWidget *parent) :
    QComboBox(parent)
{
    user_changed = true;
    connect(this,SIGNAL(currentIndexChanged(QString)),this,SLOT(valueHasChanged(QString)));
}

int IMPComboBox::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

QString IMPComboBox::tableName()
{
    return m_tableName;
}

void IMPComboBox::setTableName(QString name)
{
    m_tableName = name;
}

QString IMPComboBox::fieldName()
{
    return m_fieldName;
}

void IMPComboBox::setFieldName(QString name)
{
    m_fieldName = name;
}

void IMPComboBox::addFixItem(QString code,QString description)
{
    TcomboItem item;
    item.code = code;
    item.description = description;
    m_items.append(item);
    user_changed = false;
    addItem(description);
    user_changed = true;
}

void IMPComboBox::selectValue(QString value)
{
    for (int pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].code.toUpper() == value.toUpper())
        {
            user_changed = false;
            setCurrentIndex(pos);
            break;
        }
    }
    user_changed = true;
}

void IMPComboBox::valueHasChanged(QString text)
{
    if (user_changed)
    {
        emit valueChanged(m_tableName,m_fieldName,text);
    }
    user_changed = true;
}

bool IMPComboBox::useLkTable()
{
    return m_useLkTable;
}

void IMPComboBox::setUseLkTable(bool use)
{
    m_useLkTable = use;
}

QString IMPComboBox::lkTableName()
{
    return m_lkTableName;
}

void IMPComboBox::setLkTableName(QString name)
{
    m_lkTableName = name;
}

void IMPComboBox::loadLkValues(QSqlDatabase db,QString fieldForCode,QString fieldForDescription, QString whereClause)
{
    if ((m_useLkTable) && (!m_lkTableName.isEmpty()))
    {
        QSqlQuery query(db);
        QString sql;
        sql = "SELECT " + fieldForCode + "," + fieldForDescription;
        sql = sql + " FROM " + m_lkTableName + " " + whereClause;
        query.exec(sql);
        user_changed = false;
        while (query.next())
        {
            TcomboItem item;
            item.code = query.value(getFieldIndex(query,fieldForCode)).toString();
            item.description = query.value(getFieldIndex(query,fieldForDescription)).toString();;
            m_items.append(item);
            addItem(item.description);
        }
        user_changed = true;
    }
}

QString IMPComboBox::getCurrentValue()
{
    if (currentIndex() >= 0)
    {
        return m_items[currentIndex()].code;
    }
    else
        return QString();
}
