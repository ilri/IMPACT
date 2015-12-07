#ifndef IMPRADIOBUTTON_H
#define IMPRADIOBUTTON_H

#include <QRadioButton>
#include "export.h"

class IMPWIDGETSLIB_EXPORT IMPRadioButton : public QRadioButton
{
    Q_OBJECT

public:
    IMPRadioButton(QWidget *parent = 0);

    Q_PROPERTY(fieldTypes FieldType READ fieldType WRITE setFieldType)
    Q_ENUMS(fieldTypes)
    Q_PROPERTY(QString TableName READ tableName WRITE setTableName)
    Q_PROPERTY(QString FieldName READ fieldName WRITE setFieldName)
    Q_PROPERTY(QString FieldValue READ fieldValue WRITE setFieldValue)

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
    QString fieldValue();
    void setFieldValue(QString value);
private:
    QString m_tableName;
    QString m_fieldName;
    fieldTypes m_fieldType;
    QString m_fieldValue;
private slots:
    void valueHasChanged();
signals:
    void valueChanged(QString table, QString field, QString value);
};

#endif
