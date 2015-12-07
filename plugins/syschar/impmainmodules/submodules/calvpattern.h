#ifndef CALVPATTERN_H
#define CALVPATTERN_H

#include <impgenmaint.h>
#include "moduletypes.h"
#include "moduleclasses.h"

namespace Ui {
    class calvpattern;
}

class calvpattern : public impgenmaint
{
    Q_OBJECT

public:
    explicit calvpattern(QWidget *parent = 0);
    ~calvpattern();
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);
    void setKeyValues(QList<TmoduleFieldDef> keys);
    void setLvType(QString lvtype);

private:
    Ui::calvpattern *ui;
    periodTableModel *m_periodModel;
    QList<TmoduleFieldDef> m_keys;
    QString m_lvType;
protected slots:
    void applyChanges();
    void cancelChanges();
private slots:
    void on_BitBtn13_clicked();
};

#endif // CALVPATTERN_H
