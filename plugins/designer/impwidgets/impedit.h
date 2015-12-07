#ifndef IMPEDIT_H
#define IMPEDIT_H

#include <QLineEdit>
#include <QSqlDatabase>
#include "export.h"

const bool IMPWidgets_use_Android = false; //True for android

class IMPWIDGETSLIB_EXPORT IMPEdit : public QLineEdit
{
    Q_OBJECT
public:
    IMPEdit(QWidget *parent = 0);

    Q_PROPERTY(fieldTypes FieldType READ fieldType WRITE setFieldType)
    Q_ENUMS(fieldTypes)
    Q_PROPERTY(QString TableName READ tableName WRITE setTableName)
    Q_PROPERTY(QString FieldName READ fieldName WRITE setFieldName)
    Q_PROPERTY(QString DateFormat READ dateFormat WRITE setDateFormat)

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
    void setDateFormat(QString format) {m_dateFormat = format;}
    QString dateFormat();
    void setDataBase(QSqlDatabase db);
private:
    QSqlDatabase dataBase;
    QString m_tableName;
    QString m_fieldName;
    fieldTypes m_fieldType;
    QString m_dateFormat;
protected:
    void mousePressEvent (QMouseEvent * event);
private slots:
    void valueHasChanged(QString text);
signals:
    void valueChanged(QString table, QString field, QString value);
};

#endif
