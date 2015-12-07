#ifndef ACTIVITIES_H
#define ACTIVITIES_H
#include <impgenmaint.h>
#include "impclasses.h"



namespace Ui {
    class activities;
}

class activities : public impgenmaint
{
    Q_OBJECT

public:
    explicit activities(QWidget *parent = 0);
    ~activities();
    QTableView *list;
    void loadMaintenance(QSqlDatabase db,QTableView *tableView);
protected:
    void applyChanges();
    void cancelChanges();
private:
    Ui::activities *ui;
    childSelectTableModel *MMethods;
    childSelectTableModel *MUnits;

    childSelectTableSortModel *MSortMethods;
    childSelectTableSortModel *MSortUnits;

    void setMasterStatus(bool status);
    void setChildStatus(bool status);
    //FlickCharm charm;
    QString currAct;
private slots:

    void chekitem(maintModel *model, QModelIndex index);
    void blockChildData();
    void unitsDataChanged();
    void on_BitBtn6_clicked();
};

#endif // ACTIVITIES_H
