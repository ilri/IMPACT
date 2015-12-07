#ifndef VEGHARVEST_H
#define VEGHARVEST_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

class vegManagementModel : public managementModel
{
    Q_OBJECT
public:
    vegManagementModel(QObject *parent=0);

protected:
    void beforeInsert(QString table, QList<TmoduleFieldDef> keyFields);
};

namespace Ui {
    class vegharvest;
}

class vegharvest : public QWidget
{
    Q_OBJECT

public:
    explicit vegharvest(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~vegharvest();
    void constructCustomDelegator();
    //void beforeInsert(QString table, QList<TmoduleFieldDef> keyFields);

private:
    Ui::vegharvest *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_parentModel;
    vegManagementModel *m_harvestModel;
    managementSortModel *m_hvrstSortModel;
    QString yieldTable;
    QString mlabourTable;
    QString flabourTable;
    QList<TmoduleFieldDef> subModuleKeyFields;
    QString keyForAll;
    QString header1;
    QString header2;
    //Actions
    QAction *showYieldAct;
    QAction *showLabourAct;
    //Procedures
    void loadForm();
    bool appyHarvestData;
    void loadProdandFora(QString crop);
    void createActions();
    void constructHeader1(QList<TmoduleFieldDef> keys);
    FlickCharm cropcharm;
    FlickCharm actcharm;
    void loadAndroidEffects();
private slots:
    void on_BitBtn1_clicked();
    void on_BitBtn5_clicked();
    void parentViewActivated(const QModelIndex &index);
    void harvestingChanged();
    void constructPorFPopUpMenu(QPoint mousepos, QModelIndex index);
    void loadYield();
    void loadLabour();
    void cropMenuRequested(QPoint mousepos);
    void on_cmddetails_clicked();
};

#endif // vegharvest_H
