#ifndef FERTILISERALLOCATION_H
#define FERTILISERALLOCATION_H

#include <impgenmaint.h>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class fertiliserallocation;
}

class fertiliserallocation : public impgenmaint
{
    Q_OBJECT

public:
    explicit fertiliserallocation(QWidget *parent = 0);
    ~fertiliserallocation();
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);
    void setKeyFields(QList <TmoduleFieldDef> keyFields);
    void constructCustomDelegator();
    void setModelTable(QString table);
    void setHeader1Description(QString description);
    void setHeader2Description(QString description);

private:
    Ui::fertiliserallocation *ui;
    linkedTableModel *m_fertModel;
    linkedTableSortModel * m_fertSortModel;
    QList <TmoduleFieldDef> m_keys;
    QString modelTable;
    QString ActFertCode;
    bool applyModel;
    bool applyForm;
    QSqlDatabase database;
    FlickCharm charm;
    void loadAndroidEffects();
private slots:
    void dataChanged();
protected slots:
    void applyChanges();
    void applyToAll();
    void cancelChanges();
    void closeForm();
    void valueHasChanged(QString table, QString field, QString value);
    void valueHasChanged(QString table, QString field, bool value);
    void machine(bool checked);
    void afterInsert(QList<TmoduleInsertedKeys> keys);
    void afterDelete(QList<TmoduleInsertedKeys> keys);
    void loadCalc (const QModelIndex &index);

};

#endif // FERTILISERALLOCATION_H
