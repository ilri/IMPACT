#ifndef VEGMAN_H
#define VEGMAN_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class vegman;
}

class vegman : public QWidget
{
    Q_OBJECT

public:
    explicit vegman(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~vegman();
    void constructCustomDelegator();

private:
    Ui::vegman *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_parentModel;
    managementModel *m_activityModel;
    managementSortModel *m_actSortModel;
    QString mlabourTable;
    QString flabourTable;
    QString mfertTable;
    QString ffertTable;
    QString planTable;
    QString manureTable;
    bool usePlantingDensity;
    QList<TmoduleFieldDef> subModuleKeyFields;
    QString keyForAll;
    QString header1;
    QString header2;
    //Actions
    QAction *showLabourAct;
    QAction *showInputsAct;
    QAction *showFertsAct;
    QAction *showManureAct;
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
    void loadLabour();
    void loadInputs();
    void loadFerts();
    void loadMenure();
    bool isActyType(QList<TmoduleFieldDef> keys, QString type);
    void cropMenuRequested(QPoint mousepos);
    void actMenuRequested(QPoint mousepos);
    void on_cmddetails_clicked();
};

#endif // VEGMAN_H
