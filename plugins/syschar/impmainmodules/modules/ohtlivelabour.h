#ifndef OTHLIVELABOUR
#define OTHLIVELABOUR

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class ohtlivelabour;
}

class ohtlivelabour : public QWidget
{
    Q_OBJECT

public:
    explicit ohtlivelabour(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~ohtlivelabour();


private:
    Ui::ohtlivelabour *ui;
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
    QAction *showLabourAct;
    //Procedures
    void loadForm();
    bool applyAcyData;
    bool appyHarvestData;
    void createActions();
    void constructHeader1(QList<TmoduleFieldDef> keys);
    void constructCustomDelegator();
    FlickCharm charm;
    FlickCharm charmact;
    void loadAndroidEffects();
private slots:
    void on_BitBtn1_clicked();
    void on_BitBtn5_clicked();
    void parentViewActivated(const QModelIndex &index);
    void activityChanged();
    void constructActPopUpMenu(QPoint mousepos, QModelIndex index);
    void loadLabour();
    void cropMenuRequested(QPoint mousepos);
    void actMenuRequested(QPoint mousepos);
    void on_cmddetails_clicked();
};

#endif // OTHLIVELABOUR
