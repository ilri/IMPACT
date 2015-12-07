#ifndef LABOURALLOCATION_H
#define LABOURALLOCATION_H

#include <impgenmaint.h>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class labourallocation;
}

class labourallocation : public impgenmaint
{
    Q_OBJECT

public:
    explicit labourallocation(QWidget *parent = 0);
    ~labourallocation();
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);
    void setKeyFields(QList <TmoduleFieldDef> keyFields);
    void setModelTable(QString table);
    void setHeader1Description(QString description);
    void setHeader2Description(QString description);

private:
    Ui::labourallocation *ui;
    linkedTableModel *m_labourModel;
    linkedTableSortModel * m_labourSortModel;
    QList <TmoduleFieldDef> m_keys;
    QSqlQuery *labourTable;
    int getFieldIndex(QSqlQuery *query,QString fieldName);
    QString modelTable;
    FlickCharm charm;
    void loadAndroidEffects();
private slots:
    void on_radioButton_2_clicked();
    void on_radioButton_clicked();
    void dataChanged();

protected slots:
    void valueHasChanged(QString table, QString field, QString value);
    void applyChanges();
    void cancelChanges();
    void applyToAll();
    void loadCalc (const QModelIndex &index);
};

#endif // LABOURALLOCATION_H
