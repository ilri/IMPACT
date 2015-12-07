#ifndef CROPHARVEST_H
#define CROPHARVEST_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

class cropManagementModel : public managementModel
{
    Q_OBJECT
public:
    cropManagementModel(QObject *parent=0);

protected:
    void beforeInsert(QString table, QList<TmoduleFieldDef> keyFields);
};


namespace Ui {
    class cropharvest;
}

class cropharvest : public QWidget
{
    Q_OBJECT

public:
    explicit cropharvest(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~cropharvest();
    //void beforeInsert(QString table, QList<TmoduleFieldDef> keyFields);

private:
    Ui::cropharvest *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_parentModel;
    cropManagementModel *m_harvestModel;
    managementSortModel *m_hvrstSortModel;
    QString yieldTable;
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

    QString flabourTable;
    QString mlabourTable;
    void constructCustomDelegator();
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

#endif // cropharvest_H
