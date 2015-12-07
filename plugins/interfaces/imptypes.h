#ifndef IMPTYPES_H
#define IMPTYPES_H

#include <QString>
#include <QVariant>
#include <QColor>

struct systemDef
{
  QString systemCode;
  QString systemDescription;
};
typedef systemDef TsystemDef;

struct fieldDef
{
  QString fieldName;
  QString fieldDescription;
  QColor fieldColor;
  QVariant fieldValue;
  QVariant previousValue;
};
typedef fieldDef TfieldDef;

struct itemDef
{
  QString code;
  QString description;
  QString currentStatus;
  QString previousStatus;
  QList <TfieldDef> fields;
  bool error;
};
typedef itemDef TitemDef;

struct errorDef
{
  int row;
  QString error;
};
typedef errorDef TerrorDef;

#endif // IMPTYPES_H
