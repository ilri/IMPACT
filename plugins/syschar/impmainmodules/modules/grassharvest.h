#ifndef GRASSHARVEST_H
#define GRASSHARVEST_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

class grassManagementModel : public managementModel
{
    Q_OBJECT
public:
    grassManagementModel(QObject *parent=0);

protected:
    void beforeInsert(QString table, QList<TmoduleFieldDef> keyFields);
};

namespace Ui {
    class grassharvest;
}

class grassharvest : public QWidget
{
    Q_OBJECT

public:
    explicit grassharvest(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~grassharvest();
    //void beforeInsert(QString table, QList<TmoduleFieldDef> keyFields);

private:
    Ui::grassharvest *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_parentModel;
    grassManagementModel *m_harvestModel;
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

#endif // grassharvest_H
