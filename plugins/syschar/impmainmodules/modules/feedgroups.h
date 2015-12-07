#ifndef FEEDGROUPS_H
#define FEEDGROUPS_H

#include <impgenmaint.h>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class feedgroups;
}

class feedgroups : public impgenmaint
{
    Q_OBJECT

public:
    explicit feedgroups(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~feedgroups();
    void loadMaintenance(QSqlDatabase db,QTableView *tableView = 0);
   void constructCustomDelegator();
private:
    Ui::feedgroups *ui;
    fieldInColModel *m_liveModel;
    fieldInColModel *m_fgroupsModel;
    QList <TmoduleFieldDef> m_keys;
    bool applyModel;
    bool applyForm;
    QSqlDatabase database;
    QString currentSystem;
    FlickCharm livecharm;
    FlickCharm groupcharm;
    void loadAndroidEffects();
private slots:
    void loadPattern(QModelIndex index);
    void setChildStatus(bool status);
protected slots:
    void applyChanges();
    void cancelChanges();
    void valueHasChanged(QString table, QString field, QString value);
    void dataChanged();
    void loadFeedGroups(QModelIndex index);
    void loadFeedGDetail(QModelIndex index);
    void enableFields(QString criteria);
    void disableFields();
    void insertNew();
    void deleteCurrent();

};

#endif // FEEDGROUPS_H
