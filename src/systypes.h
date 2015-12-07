#ifndef SYSTYPES_H
#define SYSTYPES_H
#include <impgenmaint.h>
#include "impclasses.h"
//#include "flickcharm.h"


namespace Ui {
    class systypes;
}

class systypes : public impgenmaint
{
    Q_OBJECT

public:
    explicit systypes(QWidget *parent = 0);
    ~systypes();
    QTableView *list;
    void loadMaintenance(QSqlDatabase db,QTableView *tableView);
protected:
    void applyChanges();
    void cancelChanges();
private:
    Ui::systypes *ui;
    childSelectTableModel *MCrops;
    childSelectTableModel *MLivestock;

    childSelectTableSortModel *MSortCrops;
    childSelectTableSortModel *MSortLivestock;


    //FlickCharm charm;
private slots:




    void chekitem(maintModel *model, QModelIndex index);

    void cropsDataChanged();
    void livestockDataChanged();
};

#endif // SYSTYPES_H
