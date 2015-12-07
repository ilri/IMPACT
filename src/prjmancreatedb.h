#ifndef PRJMANCREATEDB_H
#define PRJMANCREATEDB_H

#include <QDialog>

namespace Ui {
    class prjmancreatedb;
}

class prjmancreatedb : public QDialog
{
    Q_OBJECT

public:
    explicit prjmancreatedb(QWidget *parent = 0);
    ~prjmancreatedb();
    bool crateDB;
    QString prjName;
    QString prjDB;
    int prjPeriods;

private slots:
    void on_cmdbrowse_clicked();

    void on_cmdcancel_clicked();

    void on_cmdcreate_clicked();

private:
    Ui::prjmancreatedb *ui;
};

#endif // PRJMANCREATEDB_H
