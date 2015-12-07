#ifndef HHSOTHERINFO_H
#define HHSOTHERINFO_H

#include <impgenmaint.h>
#include "moduleclasses.h"

namespace Ui {
    class hhsotherinfo;
}

class hhsotherinfo : public impgenmaint
{
    Q_OBJECT

public:
    explicit hhsotherinfo(QWidget *parent = 0);
    ~hhsotherinfo();
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);
    void constructCustomDelegator();
    void setKeyValues(QList<TmoduleFieldDef> keys);

private:
    Ui::hhsotherinfo *ui;
    periodTableModel *m_periodModel;
    QList<TmoduleFieldDef> m_keys;

protected slots:
    void applyChanges();
    void cancelChanges();
};

#endif // HHSOTHERINFO_H
