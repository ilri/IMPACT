#ifndef GENMODELS_H
#define GENMODELS_H


#include <QSqlDatabase>
#include <QSortFilterProxyModel>
#include <QColor>

struct genModelFieldDef
{
  int colIndex;
  int newRecords;
  QString code;
  QString description;
  QVariant value;
  QVariant previousValue;
  QVariant defValue;
  bool key;
  bool autogen;
  bool show;
  bool editable;
  QColor Color;
};
typedef genModelFieldDef TgenModelFieldDef;

struct genModelextDataCol
{
  int colIndex;
  QString code;
  QString description;
  QVariant defaultValue;
  QVariant valueWhenNew;
  QVariant valueWhenModified;
  QVariant valueWhenDeleted;
  QColor Color;
};
typedef genModelextDataCol TgenModelextDataCol;

struct genModelItemDef
{
  QString currentStatus;
  QString previousStatus;
  QList <TgenModelFieldDef> fields;
  QList <TgenModelextDataCol> columns;
  bool error;
};
typedef genModelItemDef TgenModelItemDef;

struct genModelInsertedKeys
{
    int row;
    QList<TgenModelFieldDef> keys;
};
typedef genModelInsertedKeys TgenModelInsertedKeys;

struct genModelROField
{
  QString field;
  QString whenStatus;
};
typedef genModelROField TgenModelROField;

struct readOnlyFieldFlag
{
  QString field;
  QString whenField;
  QVariant whenFieldValue;
  QString whenStatus;
};
typedef readOnlyFieldFlag TgenModelROFieldFlag;

struct genModelFldShowValue
{
    QString field;
    QVariant value;
    QString whenField;
    QVariant whenValue;
};
typedef genModelFldShowValue TgenModelFldShowValue;

struct genModelErrorDef
{
  int row;
  QString error;
};
typedef genModelErrorDef TgenModelErrorDef;

struct systemTypeItem
{
  QString code;
  QString description;
  QString livestock;
  QString livestockCode;
  QString crop;
  QString cropCode;
  QString currentStatus;
};
typedef systemTypeItem TsystemTypeItem;

class genericFldModel :  public QAbstractTableModel
{
   Q_OBJECT
public:
   genericFldModel(QObject *parent=0);
   ~genericFldModel();

   int rowCount(const QModelIndex & parent = QModelIndex()) const;
   int columnCount(const QModelIndex &parent = QModelIndex()) const;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
   bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
   Qt::ItemFlags flags (const QModelIndex &index) const;
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
   void setDatabase (QSqlDatabase database);
   void setTableName(QString tablename);
   void insertField(QString code,
                    QString description,
                    QVariant value = 0,
                    QVariant defvalue = 0,
                    bool key = false,
                    bool autogen = false,
                    bool show = true,
                    bool editable = true,
                    QColor Color = QColor(Qt::white));
   void loadData(QList<TgenModelFieldDef> keyValues);
   QVariant getDataFromField(int row, QString field);
   QVariant getOldDataFromField(int row, QString field);
   void setDataToField(int row, QString field, QVariant value);
   void insertExtraColumn(QString code,
                          QString description,
                          QVariant defaultValue,
                          QVariant valueWhenNew,
                          QVariant valueWhenModified,
                          QVariant valueWhenDeleted,
                          QColor color = QColor(Qt::white));

   QList<TgenModelFieldDef> getRowKeyValues(int row);
   QString getRowStatus(int row);
   void setReadOnly(bool readOnly);
   void setWhereClause(QString clause);
   void addReadOnlyField(QString field, QString whenStatus);
   void addReadOnlyField(QString field, QString whenField, QVariant whenFieldValue, QString whenStatus);
   void addShowValue(QString field, QVariant value, QString whenField, QVariant whenValue);
   void addDistributionException(int column);
   void resetModelStructure();
public slots:
   bool applyChanges();
   void cancelChanges();
   void insertNewItem();
   void deleteItem(int row);
private:
   QSqlDatabase m_database;
   QString m_tableName;
   QList<TgenModelFieldDef> m_fields;
   QList<TgenModelInsertedKeys> m_insertedKeys;
   QList<TgenModelInsertedKeys> m_updatedKeys;
   QList<TgenModelROField> m_genModelROFields;
   QList<TgenModelROFieldFlag> m_genModelROFieldFlags;
   QList<TgenModelFldShowValue> m_showValues;
   QList<TgenModelextDataCol> m_columns;
   QList<TgenModelItemDef> m_items;
   QList<TgenModelFieldDef> m_keyValues;
   QList<int> m_distException;
   int nShowCols;
   int getFieldByIndex(int idx) const;
   int getColumnByIndex(int idx) const;
   int getFieldIndex(QSqlQuery query,QString fieldName);
   QList <TgenModelErrorDef> errors;
   void insertData(int row);
   void updateData(int row);
   void deleteData(int row);
   void updateRow(int row,QString status);
   QString genAutoGenKey(int fieldIndex);
   void removeDeleted();
   bool isReadOnly;
   QString m_whereClause;
   bool isFieldReadOnly(QString field, QString status) const;
   bool isFieldReadOnly(QString field, int itemIndex, QString status) const;
   QVariant getShowValue(int itemIndex,QString field, QVariant currentValue) const;
   bool isColInExceptions(int column) const;
signals:
   void modeldDataChanged();
   void modeldDataChanged(int row, int column);
   void afterInsert(int rowInserted);
   void afterApply(QList<TgenModelInsertedKeys> insertedKeys);
   void afterUpdate(QList<TgenModelInsertedKeys> insertedKeys);

};

class systemTypesModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    systemTypesModel(QObject *parent=0);
    ~systemTypesModel();
    void setDatabase(QSqlDatabase db){database = db;}
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void loadItems();
    void checkItems(QString code);
    QString getSortDescription(QModelIndex index);
    bool hasCheckedItems();
    void setReadOnly(bool readOnly);
    QString getCropCode();
    QString getLivestockCode();
    QString getSystemTypeCode();
private:
    QList <systemTypeItem> items;
    QSqlDatabase database;
    bool m_readOnly;
signals:
    void modelDataChanged();
};

class systemTypesSortModel : public QSortFilterProxyModel
 {
     Q_OBJECT
 public:
     systemTypesSortModel(QObject *parent = 0);

 protected:
     bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
 };

#endif // GENMODELS_H
