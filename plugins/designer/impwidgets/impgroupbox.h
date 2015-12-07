#ifndef IMPGROUPBOX_H
#define IMPGROUPBOX_H

#include <QGroupBox>
#include "export.h"

class IMPWIDGETSLIB_EXPORT IMPGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    IMPGroupBox(QWidget *parent = 0);

    Q_PROPERTY(fieldTypes FieldType READ fieldType WRITE setFieldType)
            Q_ENUMS(fieldTypes)
            Q_PROPERTY(QString TableName READ tableName WRITE setTableName)
            Q_PROPERTY(QString FieldName READ fieldName WRITE setFieldName)

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
    fieldTypes fieldType() const {return m_fieldType;}
    void setFieldType(fieldTypes type) {m_fieldType = type;}
private:
    QString m_tableName;
    QString m_fieldName;
    fieldTypes m_fieldType;
};

#endif
