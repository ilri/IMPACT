#ifndef PRJMANADDDB_H
#define PRJMANADDDB_H

#include <QDialog>

namespace Ui {
    class prjmanadddb;
}

class prjmanadddb : public QDialog
{
    Q_OBJECT

public:
    explicit prjmanadddb(QWidget *parent = 0);
    ~prjmanadddb();
    bool addDB;
    QString prjName;
    QString prjDB;

private slots:
    void on_cmdcancel_clicked();

    void on_cmdbrowse_clicked();

    void on_cmdadd_clicked();

private:
    Ui::prjmanadddb *ui;
    bool validateDB(QString fileName);
};

#endif // PRJMANADDDB_H
