#include "impgroupbox.h"

IMPGroupBox::IMPGroupBox(QWidget *parent) :
    QGroupBox(parent)
{
}

QString IMPGroupBox::tableName()
{
    return m_tableName;
}

void IMPGroupBox::setTableName(QString name)
{
    m_tableName = name;
}

QString IMPGroupBox::fieldName()
{
    return m_fieldName;
}

void IMPGroupBox::setFieldName(QString name)
{
    m_fieldName = name;
}
