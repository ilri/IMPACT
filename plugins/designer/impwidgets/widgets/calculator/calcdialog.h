#ifndef CALCDIALOG_H
#define CALCDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QVariant>
#include <QAbstractTableModel>
#include "widgets/calculator/calculator.h"
#include "export.h"

struct unitFieldDef
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
typedef unitFieldDef TunitFieldDef;

struct unitItemDef
{
  QString currentStatus;
  QString previousStatus;
  QList <TunitFieldDef> fields;
  bool error;
};
typedef unitItemDef TunitItemDef;

class indUnitsModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    indUnitsModel(QObject *parent=0);
    ~indUnitsModel();

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
    void loadData();
    QVariant getDataFromField(int row, QString field);
    QList<TunitFieldDef> getRowKeyValues(int row);
    QString getRowStatus(int row);
    void resetModelStructure();
    void setMaxKeySize(int size);
public slots:
    bool applyChanges();
    void cancelChanges();
    void insertNewItem();
    void deleteItem(int row);
private:
    QSqlDatabase m_database;
    QString m_tableName;
    QList<TunitFieldDef> m_fields;
    QList<TunitItemDef> m_items;
    QList<TunitFieldDef> m_keyValues;
    int nShowCols;
    int maxKeySize;
    int getFieldByIndex(int idx) const;
    int getFieldIndex(QSqlQuery query,QString fieldName);
    void insertData(int row);
    void updateData(int row);
    void deleteData(int row);
    void updateRow(int row,QString status);
    QString genAutoGenKey(int fieldIndex);
    void removeDeleted();
    bool isReadOnly;
signals:
    void modeldDataChanged();
    void modeldDataChanged(int row, int column);
};


namespace Ui {
    class calcDialog;
}

class IMPWIDGETSLIB_EXPORT calcDialog : public QDialog
{
    Q_OBJECT

public:
    explicit calcDialog(QWidget *parent = 0);
    ~calcDialog();
    void setDatabase(QSqlDatabase db);
    QString getCurrentDisplay();
    void setCurrentDisplay(double value);
    void readSettings();

private slots:
    void on_cmdkgs_clicked();

    void on_cmdlts_clicked();

    void on_cmdunitdef_clicked();

    void on_cmdnew_clicked();

    void on_cmddelete_clicked();

    void on_cmdapply_clicked();

    void on_cmdcancel_clicked();

    void valueChanged();

    void on_cmdchange_clicked();

    void on_cmdback_clicked();

    void on_cmdselect_clicked();

    void on_cmddefault_clicked();

private:
    indUnitsModel *m_colModel;
    Ui::calcDialog *ui;
    QSqlDatabase m_database;
    Calculator *calWidget;
    float kgsfactor;
    float ltsfactor;
    QString currIndUnit;
    QString currIndUnitDesc;
    void loadUnit(QString code); //QString currDB
    void loadUnits();//QString currDB
    //bool closeUnitsDB;
};

#endif // CALCDIALOG_H
