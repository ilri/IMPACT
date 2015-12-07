#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "src/documentwindow.h"
#include <impgenmaint.h>
#include "imppluginloader.h"
#include <QMdiSubWindow>
#include <QWidget>
#include <QSignalMapper>
#include <QDir>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openDatabase(QString filename);

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    void readSettings();

    documentWindow *activeMdiChild();
    QMdiSubWindow *findMdiChild(const QString &title);
    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;

    QMenu *windowMenu;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;

    QList<impPluginInstance *> plugins;
    QDir pluginsDir;
    void loadPlugins();


private slots:
    void loadUnits();
    void loadFertilisers();
    void loadSoil();
    void loadPrchFeeds();
    void loadHHConsProds();
    void loadPastures();
    void loadCountries();
    void loadcrpProds();
    void loadcrpForages();
    void loadAbout();
    void loadActivity();
    void loadDrainage();
    void loadMarkOutlets();
    void loadSysTypes();
    void loadCrops();
    void loadSysChar();
    void loadProjectSettings();
    void loadProjectMan();
    void loadLivestock();
    void loadLVProds();
    void loadEconomics();
    void loadFoodSec();
    void changeName(QString name);

    //void updateMenus();
    void updateWindowMenu();
    documentWindow *createMdiChild();
    void switchLayoutDirection();
    void setActiveSubWindow(QWidget *window);
    void loadMaintenance(impgenmaint * window, QString title);

};

#endif // MAINWINDOW_H
