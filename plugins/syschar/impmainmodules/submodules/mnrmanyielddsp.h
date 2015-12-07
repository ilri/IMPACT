#ifndef MNRMANYIELDDSP_H
#define MNRMANYIELDDSP_H

#include <QDialog>
#include <QSqlDatabase>
#include "moduletypes.h"

namespace Ui {
    class mnrmanyielddsp;
}

class mnrmanyielddsp : public QDialog
{
    Q_OBJECT

public:
    explicit mnrmanyielddsp(QWidget *parent = 0);
    ~mnrmanyielddsp();
    void setDataBase(QSqlDatabase db);
    void loadData(QString table, QList <TmoduleFieldDef> keys);

private slots:
    void on_pushButton_clicked();

private:
    Ui::mnrmanyielddsp *ui;
    QSqlDatabase database;
};

#endif // MNRMANYIELDDSP_H
