#ifndef LIVESTOCKMAN_H
#define LIVESTOCKMAN_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class livestockman;
}

class livestockman : public QWidget
{
    Q_OBJECT

public:
    explicit livestockman(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~livestockman();
    void constructCustomDelegator();

private:
    Ui::livestockman *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_parentModel;
    managementModel *m_activityModel;
    managementSortModel *m_actSortModel;    
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
    bool applyAcyData;
    void createActions();
    void constructHeader1(QList<TmoduleFieldDef> keys);
    FlickCharm cropcharm;
    FlickCharm actcharm;
    void loadAndroidEffects();
private slots:
    void on_BitBtn1_clicked();
    void on_BitBtn5_clicked();
    void parentViewActivated(const QModelIndex &index);
    void activityChanged();
    void constructActPopUpMenu(QPoint mousepos, QModelIndex index);
    void loadYield();
    void loadLabour();
    void cropMenuRequested(QPoint mousepos);
    void actMenuRequested(QPoint mousepos);
    void on_cmddetails_clicked();
};

#endif // LIVESTOCKMAN_H
