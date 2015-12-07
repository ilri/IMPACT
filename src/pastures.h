#ifndef PASTURES_H
#define PASTURES_H
#include <impgenmaint.h>
#include "impclasses.h"
//#include "flickcharm.h"


namespace Ui {
    class pastures;
}

class pastures : public impgenmaint
{
    Q_OBJECT

public:
    explicit pastures(QWidget *parent = 0);
    ~pastures();
    QTableView *list;
    void loadMaintenance(QSqlDatabase db,QTableView *tableView);
protected:
    void applyChanges();
    void cancelChanges();
private:
    Ui::pastures *ui;
    childTableModel *MProducts;
    childTableModel *MForages;

    childTableSortModel *MSortProducts;
    childTableSortModel *MSortForgages;

    void setMasterStatus(bool status);
    void setChildStatus(bool status);
    //FlickCharm charm;

    QString activeCrop;
private slots:
    void on_BitBtn8_clicked();
    void on_BitBtn10_clicked();
    void on_BitBtn7_clicked();
    void on_BitBtn9_clicked();
    void chekitem(maintModel *model, QModelIndex index);
    void blockChildData();
    void productsDataChanged();
    void foragesDataChanged();
    void on_BitBtn6_clicked();
    void on_BitBtn5_clicked();
};

#endif // PASTURES_H
