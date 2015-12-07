#ifndef MAINTCLASES_H
#define MAINTCLASES_H

#include <QSqlTableModel>
#include <QStringList>
#include <QTableView>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QSqlDatabase>
//#include <imptypes.h>
#include <imptypes.h>

struct fieldStatus
{
  QString code;
  QString status;
};
typedef fieldStatus TfieldStatus;

class maintModel : public QSqlTableModel
{
    Q_OBJECT
public:
    maintModel(QObject *parent, QSqlDatabase db = QSqlDatabase());
    ~maintModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void loadStatus();
    void setDisplayColumn(QString column, QString description);
    void setView(QTableView *view);
    void insertNewRecord();
    void updateRecordStatus(int index,QString value);
    void setKeyColumn(QString column);
    int getLastNewRecord();
    bool applyChanges();
    void cancelChanges();
private:
    //QStringList status;
    QList <TfieldStatus> status;
    QString displayColumnCode;
    QString displayColumnDesc;
    QString keyColumn;
    QTableView *parentView;
    int newRecords;
    void deleteMarkedRecords();
};


class genericFunctions : public QObject
{
    Q_OBJECT
public:
    genericFunctions();
    QStringList getFields(QString tableName, QWidget *parent);
    void applyDataToField(QWidget *parent, QString tableName, QString fieldName, QVariant value);
    QVariant getDataFromField(QWidget *parent, QString tableName, QString fieldName);
    void enableFields(QString tableName, QWidget *parent);
    void disableFields(QString tableName, QWidget *parent);
    void setFieldsToNew(QString tableName, QWidget *parent);
    void setFieldsToTable(QString tableName, QWidget *parent);
    void hideField(QString tableName, QString fieldName, QWidget *parent);
    void disableField(QString tableName, QString fieldName, QWidget *parent);
    //void loadData(QSqlDatabase db);
    void setDataBase(QSqlDatabase db,QWidget *parent);

private:
    QStringList fields;
    void BrowseChildren(QString tableName, QWidget * parent, int role);
    void applyDataToChildren(QWidget *parent, QString tableName, QString fieldName, QVariant value);
    QVariant getDataFromChildren(QWidget *parent, QString tableName, QString fieldName);
};



#endif // MAINTCLASES_H
