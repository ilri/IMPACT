#include "impradiobutton.h"

IMPRadioButton::IMPRadioButton(QWidget *parent) :
    QRadioButton(parent)
{
    connect(this,SIGNAL(clicked()),this,SLOT(valueHasChanged()));
}


QString IMPRadioButton::tableName()
{
    return m_tableName;
}

void IMPRadioButton::setTableName(QString name)
{
    m_tableName = name;
}

QString IMPRadioButton::fieldName()
{
    return m_fieldName;
}

void IMPRadioButton::setFieldName(QString name)
{
    m_fieldName = name;
}

QString IMPRadioButton::fieldValue()
{
    return m_fieldValue;
}

void IMPRadioButton::setFieldValue(QString value)
{
    m_fieldValue = value;
}

void IMPRadioButton::valueHasChanged()
{
    emit valueChanged(m_tableName,m_fieldName,m_fieldValue);
}
