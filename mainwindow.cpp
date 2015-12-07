#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "units.h"
#include "countries.h"
#include "systypes.h"
#include "drainage.h"
#include "crops.h"
#include "crpprods.h"
#include "crpforages.h"
#include "pastures.h"
#include "soil.h"
#include "activity.h"
#include "markoutlets.h"
#include "syscharloader.h"
#include "livestock.h"
#include "fertilisers.h"
#include "prjsettings.h"
#include "lvprods.h"
#include "prchfeeds.h"
#include "hhconsprods.h"
#include <QtGlobal>
#include "IMPACTGlobal.h"
#include <QSqlQuery>
#include <QSqlError>
#include "prjmanager.h"
#include "ecnomics.h"
#include "foodsecurity.h"
#include "about.h"
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QMessageBox>
#include <QSettings>
#include <QPluginLoader>

#if defined(Q_OS_LINUX)
#if IMPACT_OS_ANDROID == true
    #include "android_code/androidinterface.h"
    extern JavaVM *m_javaVM;
#endif
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowTitle(tr("Impact"));
    ui->setupUi(this);
    if (IMPACT_use_Android == true)
    {
        QFont fnt("Droid Sans Mono");
        QApplication::setFont(fnt);
    }
    ui->toolBar->addAction(ui->syschar);
    //ui->toolBar->addAction(ui->ExportSystem); //Todo
    //ui->toolBar->addAction(ui->ImportSystem); //Todo
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->Systemtypes1);
    ui->toolBar->addAction(ui->actionCrops);
    ui->toolBar->addAction(ui->actionPastures);
    ui->toolBar->addAction(ui->actionActivities);
    ui->toolBar->addAction(ui->Ferttypes1);
    ui->toolBar->addAction(ui->actionLivestock);
    ui->toolBar->addAction(ui->Purchasedinputs1);
    ui->toolBar->addAction(ui->Householdconsumptionproducts1);
    ui->toolBar->addAction(ui->Countries1);
    ui->toolBar->addAction(ui->Soilds1);
    ui->toolBar->addAction(ui->Drainage1);
    ui->toolBar->addAction(ui->Marketingoutlets1);

    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->InputsOutputs1);
    ui->toolBar->addAction(ui->Foodsecurity1);
    //ui->toolBar->addAction(ui->Nutrientflows1); //Todo
    //ui->toolBar->addAction(ui->Labourefficiency1); //Todo
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->ProjectSettings);
    ui->toolBar->addAction(ui->DBMan);

    //Connect the actions with the slots
    connect(ui->actionActivities,SIGNAL(triggered()),this,SLOT(loadActivity()));
    connect(ui->actionActivity_units,SIGNAL(triggered()),this,SLOT(loadUnits()));
    connect(ui->Drainage1,SIGNAL(triggered()),this,SLOT(loadDrainage()));
    connect(ui->Marketingoutlets1,SIGNAL(triggered()),this,SLOT(loadMarkOutlets()));
    connect(ui->Ferttypes1,SIGNAL(triggered()),this,SLOT(loadFertilisers()));
    connect(ui->Soilds1,SIGNAL(triggered()),this,SLOT(loadSoil()));
    connect(ui->actionPastures,SIGNAL(triggered()),this,SLOT(loadPastures()));
    connect(ui->actionLivestock,SIGNAL(triggered()),this,SLOT(loadLivestock()));
    connect(ui->actionLivestock_products,SIGNAL(triggered()),this,SLOT(loadLVProds()));

    connect(ui->Purchasedinputs1,SIGNAL(triggered()),this,SLOT(loadPrchFeeds()));
    connect(ui->Householdconsumptionproducts1,SIGNAL(triggered()),this,SLOT(loadHHConsProds()));

    connect(ui->actionCrop_products,SIGNAL(triggered()),this,SLOT(loadcrpProds()));
    connect(ui->actionCrop_forages,SIGNAL(triggered()),this,SLOT(loadcrpForages()));
    connect(ui->actionPasture_forages,SIGNAL(triggered()),this,SLOT(loadcrpForages()));
    connect(ui->Systemtypes1,SIGNAL(triggered()),this,SLOT(loadSysTypes()));
    connect(ui->Countries1,SIGNAL(triggered()),this,SLOT(loadCountries()));
    connect(ui->actionCrops,SIGNAL(triggered()),this,SLOT(loadCrops()));
    connect(ui->syschar,SIGNAL(triggered()),this,SLOT(loadSysChar()));
    connect(ui->ProjectSettings,SIGNAL(triggered()),this,SLOT(loadProjectSettings()));
    connect(ui->Itemexit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->DBMan,SIGNAL(triggered()),this,SLOT(loadProjectMan()));
    connect(ui->InputsOutputs1,SIGNAL(triggered()),this,SLOT(loadEconomics()));
    connect(ui->Foodsecurity1,SIGNAL(triggered()),this,SLOT(loadFoodSec()));

    connect(ui->About1,SIGNAL(triggered()),this,SLOT(loadAbout()));

    //Sets the database type
    database = QSqlDatabase::addDatabase("QSQLITE","dblite"); //Add the database connector to SQLite
    readSettings();

    //************************MDI settings

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    //connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
    //        this, SLOT(updateMenus()));
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));


    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeAllSubWindows()));

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()),
            mdiArea, SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(previousAct, SIGNAL(triggered()),
            mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    windowMenu = ui->MainMenu1->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    loadPlugins();
}

MainWindow::~MainWindow()
{
    delete ui;
    database.close();
}

void MainWindow::loadMaintenance(impgenmaint * window, QString title)
{
    documentWindow *child = createMdiChild();
    //child->setMaintenanceWindow(window);
    child->setCentralWidget(window);
    connect(window,SIGNAL(closeCalled()),child->parentWidget(),SLOT(close()));
    child->showMinimized();
    child->setWindowTitle(title);
    child->setWindowState(Qt::WindowMaximized);
}

void MainWindow::loadDrainage()
{
    drainage *crps = new drainage(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Drainage types maintenance"));
}

void MainWindow::loadFertilisers()
{
    fertilisers *crps = new fertilisers(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Fertiliser types maintenance"));
}

void MainWindow::loadSoil()
{
    soil *crps = new soil(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Soil types maintenance"));
}

void MainWindow::loadPrchFeeds()
{
    prchfeeds *crps = new prchfeeds(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Purchased feeds maintenance"));
}

void MainWindow::loadHHConsProds()
{
    hhconsprods *crps = new hhconsprods(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Household consumption products maintenance"));
}


void MainWindow::loadMarkOutlets()
{
    markoutlets *crps = new markoutlets(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Marketing outlets maintenance"));
}

void MainWindow::loadAbout()
{
    about aboutfrm;
    aboutfrm.setWindowTitle(tr("About IMPACT"));
    aboutfrm.exec();
}

void MainWindow::loadActivity()
{
    activities *crps = new activities(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Activity maintenance"));
}

void MainWindow::loadUnits()
{
    units *unitf = new units(this);
    unitf->loadMaintenance(database,unitf->list);
    loadMaintenance(unitf,tr("Activity units maintenance"));
}

void MainWindow::loadcrpForages()
{
    crpforages *crpf = new crpforages(this);
    crpf->loadMaintenance(database,crpf->list);
    loadMaintenance(crpf,tr("Crop/Pasture forages maintenance"));
}

void  MainWindow::loadcrpProds()
{
    crpprods *crpp = new crpprods(this);
    crpp->loadMaintenance(database,crpp->list);
    loadMaintenance(crpp,tr("Crop/Pasture products maintenance"));
}

void MainWindow::loadCountries()
{
    countries *cnt = new countries(this);
    cnt->loadMaintenance(database,cnt->list);
    loadMaintenance(cnt,tr("Countries maintenance"));
}

void MainWindow::loadSysTypes()
{
    systypes *syst = new systypes(this);
    syst->loadMaintenance(database,syst->list);
    loadMaintenance(syst,tr("Sytem types maintenance"));
}

void MainWindow::loadCrops()
{
    crops *crps = new crops(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Crops maintenance"));
}

void MainWindow::loadLivestock()
{
    livestock *crps = new livestock(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Livestock maintenance"));
}

void MainWindow::loadLVProds()
{
    lvprods *crps = new lvprods(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Livestock products maintenance"));
}

void MainWindow::loadPastures()
{
    pastures *crps = new pastures(this);
    crps->loadMaintenance(database,crps->list);
    loadMaintenance(crps,tr("Pastures maintenance"));
}

void MainWindow::loadSysChar()
{
    syscharloader *syschar = new syscharloader(this);
    syschar->setDatabase(database);
    syschar->setPlugins(plugins);
    syschar->loadTree("Some File here!!"); //---------------Todo!
    documentWindow *child = createMdiChild();
    //child->setSysCharWindow(syschar);
    child->setCentralWidget(syschar);
    connect(syschar,SIGNAL(closeCalled()),child->parentWidget(),SLOT(close()));
    child->showMaximized();
    child->setWindowTitle(tr("System Characterisation"));
}

void MainWindow::loadProjectSettings()
{
    prjsettings *pjsettings = new prjsettings(this);
    pjsettings->setPlugins(plugins);
    pjsettings->setDatabase(database);

    documentWindow *child = createMdiChild();

    //child->setWindow(pjsettings);
    child->setCentralWidget(pjsettings);
    connect(pjsettings,SIGNAL(closeCalled()),child->parentWidget(),SLOT(close()));
    connect(pjsettings,SIGNAL(changeName(QString)),this,SLOT(changeName(QString)));
    child->showMinimized();
    child->setWindowTitle(tr("Project settings"));
    child->setWindowState(Qt::WindowNoState);

}

void MainWindow::loadProjectMan()
{
    prjmanager *pjsettings = new prjmanager(this);

    documentWindow *child = createMdiChild();
    //child->setWindow(pjsettings);
    child->setCentralWidget(pjsettings);
    connect(pjsettings,SIGNAL(closeCalled()),child->parentWidget(),SLOT(close()));
    child->showMinimized();
    child->setWindowTitle(tr("Project manager"));
    child->setWindowState(Qt::WindowNoState);
}

void MainWindow::loadFoodSec()
{
    foodsecurity *pjsettings = new foodsecurity(this);
    pjsettings->setDatabase(database);

    documentWindow *child = createMdiChild();

    //child->setWindow(pjsettings);
    child->setCentralWidget(pjsettings);
    connect(pjsettings,SIGNAL(closeCalled()),child->parentWidget(),SLOT(close()));
    child->showMinimized();
    child->setWindowTitle(tr("Food security"));
    child->setWindowState(Qt::WindowMaximized);
}

void MainWindow::loadEconomics()
{
    economics *pjsettings = new economics(this);
    pjsettings->setDatabase(database);

    documentWindow *child = createMdiChild();

    //child->setWindow(pjsettings);
    child->setCentralWidget(pjsettings);
    connect(pjsettings,SIGNAL(closeCalled()),child->parentWidget(),SLOT(close()));
    child->showMinimized();
    child->setWindowTitle(tr("Economic analysis"));
    child->setWindowState(Qt::WindowMaximized);
}

void MainWindow::changeName(QString name)
{
    this->setWindowTitle("IMPACT (" + name + ")");
}

void MainWindow::readSettings()
{
    //qDebug() << "Reading settings";
    QSettings settings("ILRI", "IMPACT");
    QString currDB = settings.value("currentDB","").toString();
    if (currDB.isEmpty())
    {
        QDir dir;
        QString ilriDB;
        QString impactConf;
#if IMPACT_OS_ANDROID == false
        ilriDB = dir.absolutePath()+dir.separator()+"db"+dir.separator()+"impact.sqlite";
        impactConf = dir.absolutePath()+dir.separator()+"impact.cnf";
#else

        androidInterface *andint;
        andint = new androidInterface(this,m_javaVM);
        ilriDB = andint->getExternalStorageDirectory();
        delete andint;
        ilriDB = ilriDB+"/Android/data/org.ilri.CrossIMPACT/db/impact.sqlite";
        impactConf = ilriDB+"/Android/data/org.ilri.CrossIMPACT/impact.cnf";
#endif
        qWarning() << "No IMPACT settings. Creating settings" << ilriDB;
        if (QFile::exists(ilriDB))
        {
            settings.setValue("currentDB",ilriDB);

            {
                QSqlDatabase cnfdb;
                cnfdb = QSqlDatabase::addDatabase("QSQLITE","cnfcon");

                cnfdb.setDatabaseName(impactConf); //Set the databasename according to the textbox in the UI
                if (!cnfdb.open())
                {
                    qWarning() << "Error: Cannot open conf file";
                    return;
                }
                QString sql;
                sql = "CREATE TABLE projects (";
                sql = sql + "prj_cod VARCHAR(3) NOT NULL ,";
                sql = sql + "prj_des VARCHAR(60) NULL ,";
                sql = sql + "prj_db VARCHAR(255) NULL ,";
                sql = sql + "prj_def INT(11)  NULL default 0,";
                sql = sql + "prj_sel INT(11)  NULL default 0,";
                sql = sql + "PRIMARY KEY (prj_cod) )";

                QSqlQuery qry(cnfdb);
                if (!qry.exec(sql))
                {
                    qWarning() << "Error: Cannot create conf structure: " << qry.lastError().databaseText();
                    return;
                }
                sql = "INSERT INTO projects(prj_cod,prj_des,prj_db,prj_def,prj_sel)";
                sql = sql + " VALUES ('000','Default database',";
                sql = sql + "'" + ilriDB + "',";
                sql = sql + "1,1)";
                if (!qry.exec(sql))
                {
                    qWarning() << "Error: Cannot insert into conf structure";
                    return;
                }
            }
            QSqlDatabase::removeDatabase("cnfcon");


            openDatabase(ilriDB);
        }
        else
        {
            QMessageBox::critical(0, qApp->tr("Cannot open the default database"),
            qApp->tr("The file does not exist. Check your installation\n\n"
                             "Click ok to exit."), QMessageBox::Ok);
            close();
        }
    }
    else
    {
        if (QFile::exists(currDB))
        {
            openDatabase(currDB);
        }
        else
        {
            QMessageBox::information(0, qApp->tr("Cannot open the database in the settings"),
                                     qApp->tr("The file does not exist. Got to the project manager to confirm."
                                                      ), QMessageBox::Ok);

            QDir dir;
            QString ilriDB;
#if IMPACT_OS_ANDROID == false
            ilriDB = dir.absolutePath()+dir.separator()+"db"+dir.separator()+"impact.sqlite";
#else
            androidInterface *andint;
            andint = new androidInterface(this,m_javaVM);
            ilriDB = andint->getExternalStorageDirectory();
            delete andint;
            ilriDB = ilriDB +"/Android/data/org.ilri.CrossIMPACT/db/impact.sqlite";
#endif
            qDebug() << "Setting error DB:" << ilriDB;

            if (QFile::exists(ilriDB))
            {
                settings.setValue("currentDB",ilriDB);
                openDatabase(ilriDB);
            }
            else
            {
                QMessageBox::critical(0, qApp->tr("Cannot open the default database"),
                qApp->tr("The file does not exist. Check your installation\n\n"
                                 "Click ok to exit."), QMessageBox::Ok);
                close();
            }
        }
    }
}

void MainWindow::openDatabase(QString filename)
{
    //qWarning() << "Opening: " << filename;



    database.close();
    database.setDatabaseName(filename); //Set the databasename according to the textbox in the UI
    if (!database.open()) //Try to opens the database
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
        qApp->tr("Unable to establish a SQLite connection. Please check the database file\n\n"
                         "Click Cancel to exit."), QMessageBox::Cancel);
    }
    QString sql;
    sql = "SELECT project_des FROM project";

    QSqlQuery qry(database);
    if (qry.exec(sql))
    {
        qry.first();
        this->setWindowTitle("IMPACT (" + qry.value(0).toString() + ")");
    }

    //qWarning() << "Database opened";



    //We need to get the description of the database here!!! ---Todo
}

//**********************MDI settings*****************************

//void MainWindow::updateMenus()
//{
//
//}

void MainWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        documentWindow *child = qobject_cast<documentWindow *>(windows.at(i)->widget());

        QString text;
        text = child->windowTitle();

        QAction *action  = windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
}

documentWindow *MainWindow::createMdiChild()
{
    documentWindow *child = new documentWindow;
    mdiArea->addSubWindow(child);
    child->setAttribute(Qt::WA_DeleteOnClose,true);
    return child;
}

void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

documentWindow *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<documentWindow *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &title)
{
    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        documentWindow *mdiChild = qobject_cast<documentWindow *>(window->widget());
        if (mdiChild->windowTitle() == title)
            return window;
    }
    return 0;
}

void MainWindow::loadPlugins()
{
#if IMPACT_OS_ANDROID != true
    pluginsDir = QDir(qApp->applicationDirPath());
#else
    pluginsDir = "/data/data/org.ilri.CrossIMPACT/lib";
#endif

    //pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        if ((QFileInfo(fileName).completeSuffix() == "so") ||
                (QFileInfo(fileName).completeSuffix() == "dll"))
        {
            //qWarning() << pluginsDir.absoluteFilePath(fileName);
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = loader.instance();
            if (plugin)
                plugins.append(new impPluginInstance(loader.instance(),fileName));
            else
                qWarning() << loader.errorString();
            plugin = 0;
        }
    }
    int pos;
    int pos2;
    QList<impPlugin *> lstplugins;
    for (pos=0;pos <=plugins.count()-1;pos++)
    {
        if (plugins[pos]->getInstanceName() == "impInterface")
        {
            qWarning() << "Found plugin library:" << plugins[pos]->getLibraryName() << ". Containing the instance:" << plugins[pos]->getInstanceName() << " with the following plugins:";
            lstplugins = plugins[pos]->getPlugins();
            for (pos2=0;pos2<=lstplugins.count()-1;pos2++)
            {
                qWarning() << lstplugins[pos2]->pluginName;
            }
        }
    }
}
