#ifndef SYSTEMSDIALOG_H
#define SYSTEMSDIALOG_H

#include <QDialog>
#include "general.h"
#include <QSqlDatabase>

namespace Ui {
    class systemsDialog;
}

class systemsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit systemsDialog(QWidget *parent = 0);
    ~systemsDialog();
    void loadData(QSqlDatabase db,QString systemCode);
    void createNewSystem(QSqlDatabase db);
    QString getSystemAdded();
    QString getCurrentDescription();

private:
    Ui::systemsDialog *ui;
    general *generalWindow;
};

#endif // SYSTEMSDIALOG_H
