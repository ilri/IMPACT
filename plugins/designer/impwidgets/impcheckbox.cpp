#include "impcheckbox.h"

IMPCheckBox::IMPCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    connect(this,SIGNAL(clicked(bool)),this,SLOT(valueHasChanged(bool)));
}

QString IMPCheckBox::tableName()
{
    return m_tableName;
}

void IMPCheckBox::setTableName(QString name)
{
    m_tableName = name;
}

QString IMPCheckBox::fieldName()
{
    return m_fieldName;
}

void IMPCheckBox::setFieldName(QString name)
{
    m_fieldName = name;
}

void IMPCheckBox::valueHasChanged(bool value)
{
    emit valueChanged(m_tableName,m_fieldName,value);
}

QString IMPCheckBox::checkedValue()
{
    return m_checkedValue;
}

void IMPCheckBox::setcheckedValue(QString value)
{
    m_checkedValue = value;
}

QString IMPCheckBox::unCheckedValue()
{
    return m_unCheckedValue;
}

void IMPCheckBox::setUnCheckedValue(QString value)
{
    m_unCheckedValue = value;
}
