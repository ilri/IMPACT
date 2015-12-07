#ifndef HERDSTR_H
#define HERDSTR_H

#include <impgenmaint.h>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class herdstr;
}

class herdstr : public impgenmaint
{
    Q_OBJECT

public:
    explicit herdstr(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~herdstr();
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);

private:
    Ui::herdstr *ui;
    fieldInColModel *m_liveModel;
    fieldInColModel *m_femaleModel;
    fieldInColModel *m_maleModel;
    QList <TmoduleFieldDef> m_keys;
    bool applyMaleModel;
    bool applyFemaleModel;
    bool applyForm;
    QSqlDatabase database;
    QString currentSystem;
    FlickCharm charm;
    void loadAndroidEffects();
private slots:
    void on_Button5_clicked();
    void loadPattern(QModelIndex index);
protected slots:
    void applyChanges();
    void cancelChanges();
    void valueHasChanged(QString table, QString field, QString value);
    void maleDataChanged();
    void femaleDataChanged();
    void loadChildData(QModelIndex index);
    void setChildStatus(bool status);
    void constructCustomDelegator();
    void loadFemaleCalc (const QModelIndex &index);
    void loadMaleCalc (const QModelIndex &index);
};

#endif // HERDSTR_H
