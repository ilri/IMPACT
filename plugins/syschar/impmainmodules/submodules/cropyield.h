#ifndef CROPYIELD_H
#define CROPYIELD_H

#include <impgenmaint.h>
#include "moduletypes.h"

namespace Ui {
    class cropyield;
}

class cropyield : public impgenmaint
{
    Q_OBJECT

public:
    explicit cropyield(QWidget *parent = 0);
    ~cropyield();
    void setHeader1Description(QString description);
    void setHeader2Description(QString description);
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);

private:
    Ui::cropyield *ui;
private slots:
    void chgfs(bool checked);
    void chghc(bool checked);
    void chglf(bool checked);
    void chglv(bool checked);
    void chgmm(bool checked);
    void chgst(bool checked);
    void chgot(bool checked);
};

#endif // CROPYIELD_H
