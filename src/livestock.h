#ifndef LIVESTOCK_H
#define LIVESTOCK_H
#include <impgenmaint.h>
#include "impclasses.h"
//#include "flickcharm.h"


namespace Ui {
    class livestock;
}

class livestock : public impgenmaint
{
    Q_OBJECT

public:
    explicit livestock(QWidget *parent = 0);
    ~livestock();
    QTableView *list;
    void loadMaintenance(QSqlDatabase db,QTableView *tableView);
protected:
    void applyChanges();
    void cancelChanges();
private:
    Ui::livestock *ui;
    childTableModel *MProducts;
    childTableSortModel *MSortProducts;


    void setMasterStatus(bool status);
    void setChildStatus(bool status);
    //FlickCharm charm;

    QString activeLive;
private slots:


    void on_BitBtn7_clicked();
    void on_BitBtn9_clicked();
    void chekitem(maintModel *model, QModelIndex index);
    void blockChildData();
    void productsDataChanged();
    void on_BitBtn6_clicked();

};

#endif // LIVESTOCK_H
