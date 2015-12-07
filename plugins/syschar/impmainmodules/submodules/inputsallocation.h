#ifndef INPUTSALLOCATION_H
#define INPUTSALLOCATION_H

#include <impgenmaint.h>

namespace Ui {
    class inputsallocation;
}

class inputsallocation : public impgenmaint
{
    Q_OBJECT

public:
    explicit inputsallocation(QWidget *parent = 0);
    ~inputsallocation();
    void setCurrAct(QString activity);
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);
    void usePlanDensity(bool status);
    void setHeader1Description(QString description);
    void setHeader2Description(QString description);

private:
    Ui::inputsallocation *ui;
    QString currAct;
private slots:
    void farm(bool checked);
    void prch(bool checked);
    void exch(bool checked);
    void machine(bool checked);
protected slots:
    void cancelChanges();
};

#endif // INPUTSALLOCATION_H
