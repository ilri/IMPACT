#ifndef LIVESTOCKPRODUCTION_H
#define LIVESTOCKPRODUCTION_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class livestockproduction;
}

class livestockproduction : public QWidget
{
    Q_OBJECT

public:
    explicit livestockproduction(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~livestockproduction();


private:
    Ui::livestockproduction *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_parentModel;
    managementModel *m_harvestModel;
    managementSortModel *m_hvrstSortModel;
    QString yieldTable;
    QString mlabourTable;
    QString flabourTable;
    QList<TmoduleFieldDef> subModuleKeyFields;
    QString keyForAll;
    QString header1;
    QString header2;
    QString currentLiveCod;
    //Actions
    QAction *showYieldAct;
    QAction *showLabourAct;
    //Procedures
    void loadForm();
    bool appyHarvestData;
    void loadProdandFora(QString liveIcod);
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

#endif // livestockproduction_H
