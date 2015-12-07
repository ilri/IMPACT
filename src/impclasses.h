#ifndef IMPCLASSES_H
#define IMPCLASSES_H

#include "imptypes.h"
#include <QSqlDatabase>
#include <QSortFilterProxyModel>

class systemsTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    systemsTableModel(QObject *parent=0);
    ~systemsTableModel();
    void setDatabase(QSqlDatabase db){database = db;}
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void loadItems();
    QString getSystemCode(int row);
    QString getSystemDescription(int row);
    int getSystemIndex(QString code);
private:
    QList <TsystemDef> items;
    QSqlDatabase database;
    int getFieldIndex(QSqlQuery query,QString fieldName);
};

class childTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    childTableModel(QObject *parent=0);
    ~childTableModel();
    void setDatabase(QSqlDatabase db){database = db;}
    void setTableName(QString table){tableName = table;}
    void setKeyField(QString field){keyField = field;}
    void setLinkedKeyField(QString field){linkedKeyField = field;}
    void addField(QString name, QString description, QColor color = QColor("WHITE"));
    void setLkTableName(QString table){lkTableName = table;}
    void setLkKeyField(QString field){lkKeyField = field;}
    void setLkDisplayColumn(QString column){lkDisplayColumn = column;}
    void setLkDisplayColumnDescription(QString description){lkDisplayColumnDescription = description;}
    QString getItemStatus(int row);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void loadItems();
    void checkItems(QString keyValue);
    bool applyChanges();
    void cancelChanges();
    QString getSortDescription(QModelIndex index);

private:
    QString tableName;
    QString keyField;
    QString linkedKeyField;
    QString keyFieldValue;
    QList <TfieldDef> fields;
    QString lkTableName;
    QString lkKeyField;
    QString lkDisplayColumn;
    QString lkDisplayColumnDescription;
    QList <TitemDef> items;
    QSqlDatabase database;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    void insertData(int row);
    void updateData(int row);
    void deleteData(int row);
    QList <TerrorDef> errors;
    void updateRow(int row,QString status);
signals:
    void childDataChanged();
};

//This sort model takes into consideration the checked status and the name
class childTableSortModel : public QSortFilterProxyModel
{
     Q_OBJECT
 public:
     childTableSortModel(QObject *parent = 0);

 protected:
     bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};


//*******************************************************************

class childSelectTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    childSelectTableModel(QObject *parent=0);
    ~childSelectTableModel();
    void setDatabase(QSqlDatabase db){database = db;}
    void setLkTableName(QString table){lkTableName = table;}
    void setLkKeyField(QString field){lkKeyField = field;}
    void setLkDisplayColumn(QString column){lkDisplayColumn = column;}
    void setLkDisplayColumnDescription(QString description){lkDisplayColumnDescription = description;}
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void loadItems(TfieldDef keyfield);
    void loadItems();
    void checkItems(QString code);
    QString getSortDescription(QModelIndex index);
    QString getSelectedCode();
    void setUseCheckBoxes(bool checkBoxes){useCheckBoxes = checkBoxes;}

private:
    QString lkTableName;
    QString lkKeyField;
    QString lkDisplayColumn;
    QString lkDisplayColumnDescription;
    bool useCheckBoxes;
    QList <TitemDef> items;
    QSqlDatabase database;
    int getFieldIndex(QSqlQuery query,QString fieldName);
signals:
    void childDataChanged();
};

//This sort model takes into consideration the checked status and the name
class childSelectTableSortModel : public QSortFilterProxyModel
 {
     Q_OBJECT
 public:
     childSelectTableSortModel(QObject *parent = 0);

 protected:
     bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
 };

#endif // IMPCLASSES_H
