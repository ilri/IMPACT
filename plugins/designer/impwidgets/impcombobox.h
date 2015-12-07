#ifndef IMPCOMBOBOX_H
#define IMPCOMBOBOX_H

#include <QComboBox>
#include <QSqlDatabase>
#include "export.h"

struct comboItem
{
  QString code;
  QString description;
};
typedef comboItem TcomboItem;

class IMPWIDGETSLIB_EXPORT IMPComboBox : public QComboBox
{
    Q_OBJECT

public:
    IMPComboBox(QWidget *parent = 0);

    Q_PROPERTY(fieldTypes FieldType READ fieldType WRITE setFieldType)
            Q_ENUMS(fieldTypes)
            Q_PROPERTY(QString TableName READ tableName WRITE setTableName)
            Q_PROPERTY(QString FieldName READ fieldName WRITE setFieldName)
            Q_PROPERTY(bool UseLkTable READ useLkTable WRITE setUseLkTable)
            Q_PROPERTY(QString LkTableName READ lkTableName WRITE setLkTableName)

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

    bool useLkTable();
    void setUseLkTable(bool use);

    QString lkTableName();
    void setLkTableName(QString name);

    fieldTypes fieldType() const {return m_fieldType;}
    void setFieldType(fieldTypes type) {m_fieldType = type;}
    void addFixItem(QString code,QString description);
    void selectValue(QString value);
    QString getCurrentValue();
    void loadLkValues(QSqlDatabase db,QString fieldForCode,QString fieldForDescription, QString whereClause);
private:
    QString m_tableName;
    QString m_fieldName;
    fieldTypes m_fieldType;
    QList <TcomboItem>m_items;
    bool user_changed;
    bool m_useLkTable;
    QString m_lkTableName;
    int getFieldIndex(QSqlQuery query,QString fieldName);
private slots:
    void valueHasChanged(QString text);
signals:
    void valueChanged(QString table, QString field, QString value);
};

#endif
