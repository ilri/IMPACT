#ifndef IMPGENMAINT_H
#define IMPGENMAINT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QStringList>
#include <QTableView>
#include <QPointer>
#include "maintclases.h"
#include "export.h"

//namespace Ui {
//    class impgenmaint;
//}

struct keyField
{
  QString code;
  QVariant value;
  bool useImApplyToAll;
};
typedef keyField TkeyField;

struct IMPlinkedValue
{
  QVariant parentValue;
  QVariant childValue;
};
typedef IMPlinkedValue TIMPlinkedValue;

struct IMPlinkedField
{
  QString parentField;
  QString childField;
  QList <TIMPlinkedValue> values;
  QVariant defaultValue;
};
typedef IMPlinkedField TIMPlinkedField;


class IMPWIDGETSLIB_EXPORT impgenmaint : public QWidget
{
    Q_OBJECT

public:
    explicit impgenmaint(QWidget *parent = 0);
    ~impgenmaint();

    Q_PROPERTY(QString TableName READ tableName WRITE setTableName)
    Q_PROPERTY(QString KeyField READ keyField WRITE setKeyField)
    Q_PROPERTY(QString DisplayColumn READ displayColumn WRITE setDisplayColumn)
    Q_PROPERTY(QString DisplayColumnDescription READ displayColumnDescription WRITE setDisplayColumnDescription)

    QString tableName(){return m_tableName;}
    void setTableName(QString name){m_tableName = name;}

    QString keyField(){return m_keyField;}
    void setKeyField(QString key){m_keyField = key;}

    QString displayColumn(){return m_displayColumn;}
    void setDisplayColumn(QString column){m_displayColumn = column;}

    QString displayColumnDescription(){return m_displayColumnDescription;}
    void setDisplayColumnDescription(QString description){m_displayColumnDescription = description;}

    virtual void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);
    void setFieldsEnable(bool status);

    void insertKeyField(QString code, QVariant value, bool useImApplyToAll = true);
    void clearKeyFields();

    void setFieldsToTable(QString table);

    void addLinkedField(QString parent, QString child, QVariant defaultValue);
    void addLinkedValue(QString parent, QString child, QVariant parentValue, QVariant childValue);

    void setDataBase(QSqlDatabase db);
    void hideField(QString table, QString field);
    void disableField(QString table, QString field);
    bool insertCurrentData(QSqlDatabase db,QList<TkeyField> otherData);

    void setSortColID(int colID){sortColID = colID;}
private:
    //Ui::impgenmaint *ui;

    maintModel *m_mainmodel;
    QPointer<QTableView> m_tableView;

    QString m_tableName;
    QString m_keyField;
    QString m_displayColumn;
    QString m_displayColumnDescription;

    QStringList fields;
    void goToFirst();
    bool maintUpdated;

    QList<TkeyField> m_keyFields;
    QList <TIMPlinkedField> m_linkedFields;

    int getFieldIndex(QSqlQuery query,QString fieldName);
    void updateTable();
    QList <TIMPlinkedField> getLinkedFields(QString parent);
    QVariant getLinkedValue(QString parent, QString child, QVariant parentValue);
    int sortColID;

protected slots:
    virtual void addNewItem();
    virtual void removeItem();
    virtual void applyChanges();
    virtual void applyToAll();
    virtual void applyToAll(QString whereClause);
    virtual void cancelChanges();
    virtual void closeForm();
    virtual void loadItem(const QModelIndex & index);
    virtual void valueHasChanged(QString table, QString field, QString value);
    virtual void valueHasChanged(QString table, QString field, bool value);

protected:
    QSqlDatabase database;
    void resizeEvent ( QResizeEvent * event );
    void unLoadMaintenance();
    void loadMaintTable();
signals:
    void closeCalled();
    void itemSelected(maintModel *model, QModelIndex index);
    void maintHasChanged();
};

#endif // IMPGENMAINT_H
