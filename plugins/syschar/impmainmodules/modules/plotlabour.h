#ifndef PLOTLABOUR
#define PLOTLABOUR

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class plotlabour;
}

class plotlabour : public QWidget
{
    Q_OBJECT

public:
    explicit plotlabour(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~plotlabour();


private:
    Ui::plotlabour *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_parentModel;
    managementModel *m_activityModel;
    managementSortModel *m_actSortModel;
    QString yieldTable;
    QString mlabourTable;
    QString flabourTable;

    QString ffertTable;
    QString mfertTable;
    QString manureTable;
    bool usePlantingDensity;
    QString planTable;

    QAction *showInputsAct;
    QAction *showFertsAct;
    QAction *showManureAct;

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
    void cropMenuRequested(QPoint mousepos);
    void actMenuRequested(QPoint mousepos);
    bool isActyType(QList<TmoduleFieldDef> keys, QString type);
    void on_cmddetails_clicked();
};

#endif // PLOTLABOUR
