#ifndef MODULECLASSES_H
#define MODULECLASSES_H

#include <QSqlDatabase>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include "moduletypes.h"

//class impModuleForm
// {
// public:
//     virtual ~impModuleForm(){}
//     //virtual void beforeInsert(QString table, QList<TmoduleFieldDef> keyFields);
// };

class inputsTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    inputsTableModel(QObject *parent=0);
    ~inputsTableModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //void setImpactModule(impModuleForm *module);
    void setDatabase(QSqlDatabase db);
    void addMasterTable(QString tableName, QList<TmoduleFieldDef> keyFields, QString tableType);
    void addYieldTable(QString masterTable, QString yieldTable, QString yieldField);
    void loadItems(QList<TmoduleFieldDef> keyFields);
    QString getSortDescription(QModelIndex index);
    QString getMasterTable(int row);
    QList<TmoduleFieldDef> getRowKeyValues(int row);
private:
    //impModuleForm *m_impModule;
    QSqlDatabase database;
    QList<mnrManTable> m_manTables;
    QList<TfeedingManItem> m_items;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    int getMasterIndex(QString masterTable) const;
};

class yieldTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    yieldTableModel(QObject *parent=0);
    ~yieldTableModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //void setImpactModule(impModuleForm *module);
    void setDatabase(QSqlDatabase db);
    void addMasterTable(QString tableName, QList<TmoduleFieldDef> keyFields, QString tableType);
    void addFieldToMaster(QString masterTable, QString field, QString description, QVariant value);
    void addYieldTable(QString masterTable, QString yieldTable, QString yieldField, QString percField);
    void addAllocationTable(QString masterTable, QString allocationTable, QString allocationField);
    void loadItems(QList<TmoduleFieldDef> keyFields);
    bool applyData();
    void cancelData();
    QString getSortDescription(QModelIndex index);
    QString getMasterTable(int row);
    QList<TmoduleFieldDef> getRowKeyValues(int row);
    void updateAllocation();
    void setCalculateYield(bool calculateYield);
private:
    //impModuleForm *m_impModule;
    QSqlDatabase database;
    QList<TyieldManTable> m_manTables;
    QList<TyieldManItem> m_items;
    bool m_calculateYield;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    int getMasterIndex(QString masterTable) const;
    QVariant getTotalAllocation(QString table, QString field, QList<TmoduleFieldDef> keyFields);
signals:
    void modeldDataChanged();
};

class cropsTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    cropsTableModel(QObject *parent=0);
    ~cropsTableModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void setDatabase(QSqlDatabase db);
    void loadItems();
    void chkitems(QList<TmoduleFieldDef> keyFields);
    QString getSortDescription(QModelIndex index);
    bool getCheckedStatusForRow(int row);
    QString getCurrentStatusForRow(int row);
    bool applyData();
    void cancelData();
    void setIsVegOrchad(bool isVegOrchad);
    void setTableName(QString table);
    QColor getColorForRow(int row);
private:
    QSqlDatabase database;
    QList<TcropItem> m_items;
    QList <TmoduleErrorDef> errors;
    QList<TperiodHeaderData> m_periods;
    int nperiods;
    bool m_isVegOrchad;
    QString m_tableName;
    QList<TmoduleFieldDef> m_keyFields;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    void insertData(int row);
    void updateData(int row);
    void deleteData(int row);
    bool isPeriodUsed(QString period) const;
    void loadPeriodData(int index, QList<TmoduleFieldDef> keyFields);
    void updateRow(int row,QString status);
    bool hasCheckedItems();
signals:
    void modeldDataChanged();
};

class cropsTableSortModel : public QSortFilterProxyModel
 {
     Q_OBJECT
 public:
     cropsTableSortModel(QObject *parent = 0);

 protected:
     bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
 };

class feedingTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    feedingTableModel(QObject *parent=0);
    ~feedingTableModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //void setImpactModule(impModuleForm *module);
    void setDatabase(QSqlDatabase db);
    void addMasterTable(QString tableName, QList<TmoduleFieldDef> keyFields);
    void addLookUpTable(QString masterTable, QString lkTableName, QString lkTableType,QList<TmoduleFieldDef> keyFields);
    void addYieldTable(QString masterTable, QString yieldTable, QString yieldField, QString percField);

    void setlkUpColDescription(QString description);
    void loadItems(QList<TmoduleFieldDef> keyFields);
    void chkitems(QList<TmoduleFieldDef> keyFields);

    void setPeriodValues(int row, QString sql);

    QString getSortDescription(QModelIndex index);
    bool getCheckedStatusForRow(int row);
    QString getCurrentStatusForRow(int row);
    QVariant getPeriodValueForRow(int row, int period);
    QString getLookUpTable(int row);
    bool applyData();
    void cancelData();
    QList<TmoduleFieldDef> getMasterKeyFields(int row);
    QList<TmoduleFieldDef> getLookUpFields(QModelIndex index);
    QString getMasterTable(int row);
    QList<TmoduleFieldDef> getRowKeyValues(int row);

private:
    //impModuleForm *m_impModule;
    QSqlDatabase database;
    QList<mnrManTable> m_manTables;
    QList<TfeedingManItem> m_items;
    QList <TmoduleErrorDef> errors;
    QList<TmoduleFieldDef> checkedKeyValues;
    QList<TperiodHeaderData> m_periods;
    int nperiods;
    QString lookUpColDescription;

    int getFieldIndex(QSqlQuery query,QString fieldName);
    int getMasterIndex(QString masterTable) const;
    void updateRow(int row,QString status);
    void insertData(int row);
    void updateData(int row);
    void deleteData(int row);

signals:
    void modeldDataChanged();
};

class mnrManTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    mnrManTableModel(QObject *parent=0);
    ~mnrManTableModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //void setImpactModule(impModuleForm *module);
    void setDatabase(QSqlDatabase db);
    void addMasterTable(QString tableName, QList<TmoduleFieldDef> keyFields);
    void addLookUpTable(QString masterTable, QString lkTableName, QString lkTableType,QList<TmoduleFieldDef> keyFields);
    void addYieldTable(QString masterTable, QString yieldTable, QString yieldField, QString percField);

    void setlkUpColDescription(QString description);
    void loadItems(QList<TmoduleFieldDef> keyFields);
    void chkitems(QList<TmoduleFieldDef> keyFields);
    QString getSortDescription(QModelIndex index);
    bool getCheckedStatusForRow(int row);
    QString getCurrentStatusForRow(int row);
    QString getLookUpTable(int row);
    bool applyData();
    void cancelData();
    QList<TmoduleFieldDef> getMasterKeyFields(int row);
    QList<TmoduleFieldDef> getLookUpFields(QModelIndex index);
    QString getMasterTable(int row);
    QList<TmoduleFieldDef> getRowKeyValues(int row);
    float getTotalUsage();

private:
    //impModuleForm *m_impModule;
    QSqlDatabase database;
    QList<mnrManTable> m_manTables;
    QList<mnrMamItem> m_items;
    QList <TmoduleErrorDef> errors;
    QList<TmoduleFieldDef> checkedKeyValues;
    QString lookUpColDescription;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    int getMasterIndex(QString masterTable) const;
    float getUsage(QString materTable,QList<TmoduleFieldDef> keyFields);
    void updateRow(int row,QString status);
    void insertData(int row);
    void updateData(int row);
    void deleteData(int row);
    void updateMaster();
    void updateAvailable();
signals:
    void modeldDataChanged();
};


class grazingTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    grazingTableModel(QObject *parent=0);
    ~grazingTableModel();
    void setDatabase(QSqlDatabase db){database = db;}
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void loadItems(QString system);
    void checkItems(QList<TmoduleFieldDef> keys);
    bool applyChanges();
    void cancelChanges();
    QString getSortDescription(QModelIndex index);
    QList<TmoduleFieldDef> getRowKeyValues(int row);
    QString getItemStatus(int row);
private:
    QList<TmoduleInsertedKeys> m_insertedKeys;
    QList <TgrazingItem> items;
    QList<TmoduleFieldDef> checkedKeys;
    QSqlDatabase database;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    void insertData(int row);
    void updateData(int row);
    void deleteData(int row);
    QList <TmoduleErrorDef> errors;
    void updateRow(int row,QString status);
signals:
    void modelDataChanged();
    void afterInsert(QList<TmoduleInsertedKeys> insertedKeys);

};

class grazingTableSortModel : public QSortFilterProxyModel
 {
     Q_OBJECT
 public:
     grazingTableSortModel(QObject *parent = 0);

 protected:
     bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
 };


class linkedTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    linkedTableModel(QObject *parent=0);
    ~linkedTableModel();
    //void setImpactModule(impModuleForm *module);
    void setDatabase(QSqlDatabase db){database = db;}
    void setTableName(QString table){tableName = table;}
    void setKeyFields(QList<TmoduleFieldDef> keys);
    void setLinkedKeyField(QString field);
    void addField(QString name, QString description, QVariant defaultValue = QVariant() ,QColor color = QColor(Qt::white));
    void setLkTableName(QString table){lkTableName = table;}
    void setLkKeyField(QString field){lkKeyField = field;}
    void setLkDisplayColumn(QString column){lkDisplayColumn = column;}
    void setLkDisplayColumnDescription(QString description){lkDisplayColumnDescription = description;}
    void setLkUseCustomDescription(bool use);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void loadItems();
    void loadItems(QList<TmoduleFieldDef> keyFields);
    void insertItem(QString code, QString description);
    void clearItems();
    void resetModel();
    void checkItems();
    bool applyChanges();
    void cancelChanges();
    QString getSortDescription(QModelIndex index);
    QString getItemStatus(int row);
    void addLinkedField(QString parent, QString child, QVariant defaultValue);
    void addLinkedValue(QString parent, QString child, QVariant parentValue, QVariant childValue);
    bool hasCheckedItems();
    QList<TmoduleFieldDef> getRowKeyValues(int row);
    QColor getColorForRow(int row);
private:
    QString m_currSystem;
    QString tableName;
    QList <TlinkedField> m_linkedFields;
    QList <TmoduleFieldDef> keyFields;
    QList<TmoduleInsertedKeys> m_insertedKeys;
    QList<TmoduleInsertedKeys> m_deletedKeys;
    QString linkedKeyField;
    QList <TmoduleFieldDef> fields;
    QString lkTableName;
    QString lkKeyField;
    QString lkDisplayColumn;
    QString lkDisplayColumnDescription;
    bool m_useCustomDescription;
    QList <TlinkeditemDef> items;
    QSqlDatabase database;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    void insertData(int row);
    void updateData(int row);
    void deleteData(int row);
    QList <TmoduleErrorDef> errors;
    void updateRow(int row,QString status);
    QList <TlinkedField> getLinkedFields(QString parent);
    QVariant getLinkedValue(QString parent, QString child, QVariant parentValue);
signals:
    void modelDataChanged();
    void afterInsert(QList<TmoduleInsertedKeys> insertedKeys);
    void afterDelete(QList<TmoduleInsertedKeys> insertedKeys);
};

class linkedTableSortModel : public QSortFilterProxyModel
 {
     Q_OBJECT
 public:
     linkedTableSortModel(QObject *parent = 0);

 protected:
     bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
 };


class managementSortModel : public QSortFilterProxyModel
 {
     Q_OBJECT
 public:
     managementSortModel(QObject *parent = 0);

 protected:
     bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
 };


class managementModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    managementModel(QObject *parent=0);
    ~managementModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //void setImpactModule(impModuleForm *module);
    void setDatabase(QSqlDatabase db);
    void addMasterTable(QString tableName);
    void setKeysFields(QString masterTable, QList<TmoduleFieldDef> keyFields);
    void setLookUpLinkedField(QString masterTable, QString field);
    void setPeriodTable(QString masterTable,QString tableName);
    void setPeriodKeyFields(QString masterTable, QList<TmoduleFieldDef> keyFields);
    void setPeriodField(QString masterTable, QString field);
    void setPeriodStatusField(QString masterTable, QString field);
    void setPeriodDefaultValue(QString masterTable, QVariant value);
    void setlkUpTable(QString masterTable, QString tableName);
    void setlkUpCodeFields(QString masterTable, QString field);
    void setlkUpDescFields(QString masterTable, QString field);
    void setlkUpWhereClause(QString masterTable, QString clause);
    void setlkUpColDescription(QString description);
    void setlkUpColOrder(QString masterTable, QString order);
    void setlkUpChkColor(QString masterTable, QColor color);
    void setlkUpUnChkColor(QString masterTable, QColor color);
    void clearItems();
    void loadItems();
    void insertItem(QString masterTable, QString code, QString description);
    void chkitems(QList<TmoduleFieldDef> keyFields);
    void setPeriodDescription(int period,QString description);
    void setPeriodFont(int period,QFont font);
    void setPeriodColor(int period,QColor color);
    QString getSortDescription(QModelIndex index);
    void setItemEdtStatus(QString masterTable, QString itemCode, bool status);
    QColor getColorForRow(int row);
    bool getCheckedStatusForRow(int row);
    QString getCurrentStatusForRow(int row);
    bool getEditableStatusForRow(int row);
    bool applyData();
    bool applyData(QList<TmoduleFieldDef> fixedKeyValues);
    void cancelData();
    void resetData();
    QString getMasterTable(int row);
    QString getPeriodTable(int row);
    QList<TmoduleFieldDef> getMasterKeyFields(int row);
    QList<TmoduleFieldDef> getPeriodKeyFields(QModelIndex index);
    QList<TmoduleFieldDef> getItemKeyFields(int row);
    QString getPeriodDescription(QModelIndex index);
    QVariant getPeriodValue(QModelIndex index);
private:
    //impModuleForm *m_impModule;
    QList<TmanagementTable> m_manTables;
    int nperiods;
    QList<TperiodHeaderData> m_periods;
    QList<TmanagementItem> m_items;
    QList <TmoduleErrorDef> errors;
    QString lookUpColDescription;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    int getMasterIndex(QString masterTable) const;
    int getItemIndex(QString masterTable, QString itemCode);
    void updateRow(int row,QString status);
    void insertDataToMaster(int row,QList<TmoduleFieldDef> fixedKeyValues);
    void DeleteDataFromMaster(int row,QList<TmoduleFieldDef> fixedKeyValues);
    bool insertDataToPeriod(int row);
    void updateDataInPeriod(int row,QList<TmoduleFieldDef> fixedKeyValues);
signals:
    void modeldDataChanged();
protected:
    QSqlDatabase database;
    virtual void beforeInsert(QString table, QList<TmoduleFieldDef> keyFields);

};

class periodTableModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    periodTableModel(QObject *parent=0);
    ~periodTableModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //void setImpactModule(impModuleForm *module);
    void setDatabase(QSqlDatabase db);
    void setTableName(QString tableName);
    QString getTableName();
    void loadData(QList<TmoduleFieldDef> keyFields);
    void clearData();

    void addField(QString code, QString description, QVariant defaultValue = QVariant(QString()), QString group = "", QColor color = QColor(Qt::white));
    void addGroup(QString code, QString description, QColor color = QColor(Qt::lightGray));
    void setReadOnly(bool readOnly);

public slots:
    void applyData();
    void cancelData();

private:
    //impModuleForm *m_impModule;
    QSqlDatabase database;
    QString m_tableName;
    QList<TmoduleFieldDef> m_keyFields;
    QList<TperiodField> m_fields;
    QStringList m_periods;
    QList<TperiodGroup> m_groups;
    int nShowRows;
    bool m_readOnly;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    bool isRowGroup(int row) const;
    int getFieldByIndex(int idx) const;
    int getGroupByIndex(int idx) const;
    void modifyItem(int index);
signals:
    void modeldDataChanged();
};

//*********************************fieldInColModel***********************************************

class fieldInColModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    fieldInColModel(QObject *parent=0);
    ~fieldInColModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //void setImpactModule(impModuleForm *module);
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
    void loadData(QList<TmoduleFieldDef> keyValues);
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

    QList<TmoduleFieldDef> getRowKeyValues(int row);
    QString getRowStatus(int row);
    void setReadOnly(bool readOnly);
    void setWhereClause(QString clause);
    void addReadOnlyField(QString field, QString whenStatus);
    void addReadOnlyField(QString field, QString whenField, QVariant whenFieldValue, QString whenStatus);
    void addShowValue(QString field, QVariant value, QString whenField, QVariant whenValue);
    void addDistributionException(int column);
    void resetModelStructure();
    void setMaxKeySize(int size);
    void setBeforeInsertTable(QString table);
    void addBeforeInsertIgnoreField(QString field);
public slots:
    bool applyChanges();
    void cancelChanges();
    void insertNewItem();
    void deleteItem(int row);
private:
    //impModuleForm *m_impModule;
    QSqlDatabase m_database;
    QString m_tableName;
    QList<TmoduleFieldDef> m_fields;
    QList<TmoduleInsertedKeys> m_insertedKeys;
    QList<TmoduleInsertedKeys> m_updatedKeys;
    QList<TmoduleInsertedKeys> m_deletedKeys;
    QList<TreadOnlyField> m_readOnlyfields;
    QList<TreadOnlyFieldFlag> m_readOnlyfieldFlags;
    QList<TmoduleFldShowValue> m_showValues;
    QList<TextDataCol> m_columns;
    QList<TmoduleItemDef> m_items;
    QList<TmoduleFieldDef> m_keyValues;
    QList<int> m_distException;
    QString beforeInsertTable;
    QList<TmoduleFieldDef> beforeInsertIngKeys;
    int nShowCols;
    int maxKeySize;
    int getFieldByIndex(int idx) const;
    int getColumnByIndex(int idx) const;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    QList <TmoduleErrorDef> errors;
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
    void insertInParentFirst(int row);
signals:
    void modeldDataChanged();
    void modeldDataChanged(int row, int column);
    void afterInsert(int rowInserted);
    void afterApply(QList<TmoduleInsertedKeys> insertedKeys);
    void afterUpdate(QList<TmoduleInsertedKeys> updatedKeys);
    void afterDelete(QList<TmoduleInsertedKeys> deletedKeys);

};


#endif // MODULECLASSES_H
