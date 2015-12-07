#ifndef OTHLABOUR_H
#define OTHLABOUR_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class ohtlabour;
}

class ohtlabour : public QWidget
{
    Q_OBJECT

public:
    explicit ohtlabour(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~ohtlabour();


private:
    Ui::ohtlabour *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    managementModel *m_activityModel;
    managementSortModel *m_actSortModel;
    QString yieldTable;
    QList<TmoduleFieldDef> subModuleKeyFields;
    QString keyForAll;
    QString header1;
    QString header2;
    QString flabourTable;
    QString mlabourTable;
    //Actions
    QAction *showLabourAct;
    //Procedures
    void loadForm();
    void createActions();
    void constructCustomDelegator();
    FlickCharm charm;
    void loadAndroidEffects();
private slots:
    void on_BitBtn1_clicked();
    void on_BitBtn5_clicked();
    void activityChanged();
    void constructActPopUpMenu(QPoint mousepos, QModelIndex index);
    void loadLabour();

    void on_cmddetails_clicked();
};

#endif // OTHLABOUR_H
