#ifndef MANUREALLOCATION_H
#define MANUREALLOCATION_H

#include <impgenmaint.h>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class manureallocation;
}

class manureallocation : public impgenmaint
{
    Q_OBJECT

public:
    explicit manureallocation(QWidget *parent = 0);
    ~manureallocation();
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);
    void setKeyFields(QList <TmoduleFieldDef> keyFields);
    void setModelTable(QString table);
    void constructCustomDelegator();
    void setHeader1Description(QString description);
    void setHeader2Description(QString description);

private:
    Ui::manureallocation *ui;
    linkedTableModel *m_manureModel;
    linkedTableSortModel * m_manureSortModel;
    QList <TmoduleFieldDef> m_keys;
    QSqlQuery *manureTable;
    QString ActManureCode;
    int getFieldIndex(QSqlQuery *query,QString fieldName);
    QString modelTable;
    QSqlDatabase database;
    FlickCharm charm;
    void loadAndroidEffects();
private slots:
    void dataChanged();
protected slots:
    void applyChanges();
    void cancelChanges();
    void applyToAll();
    void loadCalc (const QModelIndex &index);
};

#endif // MANUREALLOCATION_H
