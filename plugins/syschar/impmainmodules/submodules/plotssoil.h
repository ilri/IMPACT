#ifndef PLOTSSOIL_H
#define PLOTSSOIL_H
#include <impgenmaint.h>

namespace Ui {
    class plotssoil;
}

class plotssoil : public impgenmaint
{
    Q_OBJECT

public:
    explicit plotssoil(QWidget *parent = 0);
    ~plotssoil();
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);

private:
    Ui::plotssoil *ui;
private slots:
    void applyChanges();
    void cancelChanges();
    void valueHasChanged(QString table, QString field, QString value);
    void applyToAll();
};

#endif // PLOTSSOIL_H
