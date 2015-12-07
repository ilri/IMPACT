#ifndef MODULETYPES_H
#define MODULETYPES_H

#include <QVariant>
#include <QString>
#include <QColor>
#include <QPixmap>
#include <QFont>

struct periodGroup
{
  QString code;
  QString description;
  QColor color;
  int rowIndex;
};
typedef periodGroup TperiodGroup;

struct periodField
{
  QString code;
  QString description;
  QVariant defaultValue;
  QList<QVariant> values;
  QList<QVariant> previousValues;
  QString status;
  QString groupCode;
  QColor color;
  int rowIndex;
};
typedef periodField TperiodField;

struct fixComboItem
{
  QString code;
  QString description;
  QPixmap image;
};
typedef fixComboItem TfixComboItem;

struct linkedValue
{
  QVariant parentValue;
  QVariant childValue;
};
typedef linkedValue TlinkedValue;

struct linkedField
{
  QString parentField;
  QString childField;
  QList <TlinkedValue> values;
  QVariant defaultValue;
};
typedef linkedField TlinkedField;

struct moduleFieldDef
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
typedef moduleFieldDef TmoduleFieldDef;

struct moduleInsertedKeys
{
    int row;
    QList<TmoduleFieldDef> keys;
};
typedef moduleInsertedKeys TmoduleInsertedKeys;

struct moduleFldShowValue
{
    QString field;
    QVariant value;
    QString whenField;
    QVariant whenValue;
};
typedef moduleFldShowValue TmoduleFldShowValue;

struct cmbdlgShowConstraint
{
    QString code;
    QString whenField;
    QVariant whenValue;
};
typedef cmbdlgShowConstraint TcmbdlgShowConstraint;

struct readOnlyField
{
  QString field;
  QString whenStatus;
};
typedef readOnlyField TreadOnlyField;

struct readOnlyFieldFlag
{
  QString field;
  QString whenField;
  QVariant whenFieldValue;
  QString whenStatus;
};
typedef readOnlyFieldFlag TreadOnlyFieldFlag;

struct linkeditemDef
{
  QString code;
  QString description;
  QString currentStatus;
  QString previousStatus;
  QList <moduleFieldDef> fields;
  bool error;
};
typedef linkeditemDef TlinkeditemDef;

struct grazingItem
{
  QString liveCode;
  QString LiveDesc;
  QString LivePurpose;
  QString LiveBreed;
  QString FeedCode;
  QString FeedDesc;
  QString currentStatus;
  QString previousStatus;
  bool error;
};
typedef grazingItem TgrazingItem;

struct extDataCol
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
typedef extDataCol TextDataCol;

struct moduleItemDef
{
  QString currentStatus;
  QString previousStatus;
  QList <TmoduleFieldDef> fields;
  QList <TextDataCol> columns;
  bool error;
};
typedef moduleItemDef TmoduleItemDef;

struct moduleErrorDef
{
  int row;
  QString error;
};
typedef moduleErrorDef TmoduleErrorDef;

struct yieldManTable
{
  QString name;
  QList<TmoduleFieldDef> keysFields;
  QString lookUpTable;
  QList<TmoduleFieldDef> lkKeysFields;
  QString lookUpTableType;
  QString yieldTable;
  QString yieldField;
  QString percField;
  QList<TmoduleFieldDef > fields;
  QString allocationTable;
  QString allocationField;
};
typedef yieldManTable TyieldManTable;

struct mnrManTable
{
  QString name;
  QList<TmoduleFieldDef> keysFields;
  QString lookUpTable;
  QList<TmoduleFieldDef> lkKeysFields;
  QString lookUpTableType;
  QString yieldTable;
  QString yieldField;
  QString percField;
};
typedef mnrManTable TmnrManTable;

struct yieldManItem
{
  QString masterTable;
  QList<TmoduleFieldDef> keyValues;
  QString description;
  QString itemType;
  QString currentStatus;
  QString previousStatus;
  QList<TmoduleFieldDef > currentValues;
  QList<TmoduleFieldDef > previousValues;
  QVariant totalYield;
  QVariant totalAllocated;
  bool error;
};
typedef yieldManItem TyieldManItem;

struct feedingManItem
{
  QString masterTable;
  QList<TmoduleFieldDef> keyValues;
  QString description;
  QString itemType;
  QString currentStatus;
  QString previousStatus;
  QList<QVariant > currentValues;
  QList<QVariant > previousValues;
  QVariant totalYield;
  bool error;
};
typedef feedingManItem TfeedingManItem;

struct mnrMamItem
{
  QString masterTable;
  QList<TmoduleFieldDef> keyValues;
  QString description;
  QString itemType;
  QString currentStatus;
  QString previousStatus;
  QVariant currentAllocation;
  QVariant previousAllocation;
  QVariant totalYield;
  QVariant totalAvailable;
  bool error;
};
typedef mnrMamItem TmnrMamItem;

struct managementTable
{
  QString name;
  QList<TmoduleFieldDef> keysFields;
  QString lookUpLinkedField;
  QString periodTable;
  QList<TmoduleFieldDef> periodKeysFields;
  QString periodField;
  QString periodStatusField;
  QVariant periodDefaultValue;
  QString lookUpTable;
  QString lookUpCodeField;
  QString lookUpDescField;
  QString lookUpWhereClause;
  QString lookUpColOrder;
  QColor lookUpChkColor;
  QColor lookUpUnChkColor;
};
typedef managementTable TmanagementTable;

struct managementItem
{
  QString masterTable;
  QString order;
  QString code;
  QString description;
  QString currentStatus;
  QString previousStatus;
  QList<QVariant> currentValues;
  QList<QVariant> previousValues;
  QList<TmoduleFieldDef> periodKeysFields;
  bool error;
  bool isEditable;
};
typedef managementItem TmanagementItem;

struct periodHeaderData
{
    QString code;
    QString description;
    QFont font;
    QColor color;
};
typedef periodHeaderData TperiodHeaderData;



struct cropItem
{
  QString code;
  QString description;
  QString currentStatus;
  QVariant currentLandCover;
  QVariant previousLandCover;
  QString previousStatus;
  QList<QVariant > currentValues;
  QList<QVariant > previousValues;
  bool error;
};
typedef cropItem TcropItem;

#endif // MODULETYPES_H
