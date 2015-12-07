#ifndef IMPCHECKBOX_H
#define IMPCHECKBOX_H

#include <QCheckBox>
#include "export.h"

class IMPWIDGETSLIB_EXPORT  IMPCheckBox : public QCheckBox
{
    Q_OBJECT

public:
    IMPCheckBox(QWidget *parent = 0);

    Q_PROPERTY(fieldTypes FieldType READ fieldType WRITE setFieldType)
    Q_ENUMS(fieldTypes)
    Q_PROPERTY(QString TableName READ tableName WRITE setTableName)
    Q_PROPERTY(QString FieldName READ fieldName WRITE setFieldName)
    Q_PROPERTY(QString CheckedValue READ checkedValue WRITE setcheckedValue)
    Q_PROPERTY(QString UnCheckedValue READ unCheckedValue WRITE setUnCheckedValue)


    enum fieldTypes {
             Character = 0,
             Numeric,
             Decimal,
             Boolean,
             Date,
             Time
         };


    QString tableName();
    void setTableName(QString name);
    QString fieldName();
    void setFieldName(QString name);

    QString checkedValue();
    void setcheckedValue(QString value);
    QString unCheckedValue();
    void setUnCheckedValue(QString value);

    fieldTypes fieldType() const {return m_fieldType;}
    void setFieldType(fieldTypes type) {m_fieldType = type;}
private:
    QString m_tableName;
    QString m_fieldName;
    QString m_checkedValue;
    QString m_unCheckedValue;

    fieldTypes m_fieldType;
private slots:
    void valueHasChanged(bool value);
signals:
    void valueChanged(QString table, QString field, bool value);
};

#endif
