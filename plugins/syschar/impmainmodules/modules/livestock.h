#ifndef LIVESTOCK_H
#define LIVESTOCK_H

#include <impgenmaint.h>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class livestock;
}

class livestock : public impgenmaint
{
    Q_OBJECT

public:
    explicit livestock(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~livestock();
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);
    void constructCustomDelegator();

private:
    Ui::livestock *ui;
    fieldInColModel *m_liveModel;
    QList <TmoduleFieldDef> m_keys;
    bool applyModel;
    bool applyForm;
    QSqlDatabase database;
    QString currentSystem;
    FlickCharm charm;
    void loadAndroidEffects();
private slots:
    void loadExtra(QModelIndex index);
protected slots:
    void applyChanges();
    void cancelChanges();
    void valueHasChanged(QString table, QString field, QString value);
    void dataChanged();
    void loadChildData(QModelIndex index);
    void setChildStatus(bool status);
    void afterApply(QList<TmoduleInsertedKeys> keys);
    void setMilkStatus(bool status);
    void setRuminantStatus(bool status);
    void deleteItem();
    void loadCalc (const QModelIndex &index);

};

#endif // LIVESTOCK_H
