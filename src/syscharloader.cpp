#include "syscharloader.h"
#include "ui_syscharloader.h"
#include "pluginnotfound.h"

syscharloader::syscharloader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::syscharloader)
{
    //qWarning() << "Start: syscharloader";
    ui->setupUi(this);
    currentWindow = 0;
    connect(ui->cmdclose,SIGNAL(clicked()),this,SLOT(closeMe()));
    currentSystem = "Carlos";

    docNavTree = new navtree(this);
    docNavTree->setAllowedAreas(Qt::LeftDockWidgetArea);

    connect(docNavTree,SIGNAL(loadSystem(QString,QString,QString,QString)),this,SLOT(loadSystem(QString,QString,QString,QString)));
    connect(docNavTree,SIGNAL(clearScreen()),this,SLOT(clearScreen()));

    addDockWidget(Qt::LeftDockWidgetArea, docNavTree);


    //qWarning() << "End: syscharloader";
}

syscharloader::~syscharloader()
{
    //qWarning() << "Start: ~syscharloader";
    /*if (currentWindow)
    {
        //qWarning() << "Removing Widget";
        ui->mainlayout->removeWidget(currentWindow);
        delete currentWindow;
        currentWindow = 0;
    }*/
    for (int pos = 0; pos <= plugins.count()-1;pos++)
    {
        plugins[pos] = 0;
    }
    delete ui;
    plugins.clear();
    //qWarning() << "End: ~syscharloader";
}

void syscharloader::setPlugins(QList<impPluginInstance *> lstPlugins)
{
    //qWarning() << "Start: setPlugins";
    plugins.append(lstPlugins);
     //qWarning() << "End: setPlugins";
}

void syscharloader::loadTree(QString /*xmlFile*/)
{

    docNavTree->setPlugins(plugins);
    docNavTree->loadSystems(dataBase);
    docNavTree->loadTree(dataBase);
}

void syscharloader::setDatabase(QSqlDatabase db)
{
    dataBase = db;
}

void syscharloader::loadModule(QString library, QString interface, QString plugin)
{
   //qWarning() << "Start: loadModule";
    int pos;
    if (currentWindow)
    {
        ui->mainlayout->removeWidget(currentWindow);
        delete currentWindow;
        currentWindow = 0;
    }
    for (pos=0;pos <=plugins.count()-1;pos++)
    {
        if (plugins[pos]->getLibraryName() == library)
        {
            if (plugins[pos]->getInstanceName() == interface)
            {
                impInterface *iinstance = qobject_cast<impInterface *>(plugins[pos]->getInstance());
                currentWindow = iinstance->loadScreen(plugin,dataBase,currentSystem);
                break;
            }
        }
    }
    if (currentWindow)
    {
        ui->mainlayout->addWidget(currentWindow);
    }
    else
    {
        currentWindow = new pluginnotfound(0);
        qobject_cast<pluginnotfound *>(currentWindow)->setInfo(library,plugin);
        ui->mainlayout->addWidget(currentWindow);
    }
    //qWarning() << "End: loadModule";
}

void syscharloader::clearScreen()
{
    currentSystem = "";
    if (currentWindow)
    {
        ui->mainlayout->removeWidget(currentWindow);
        delete currentWindow;
        currentWindow = 0;
    }
}

void syscharloader::loadSystem(QString code, QString description, QString library, QString pluginCode)
{
    ui->lblsystem->setText(tr("Current system: ") + description);
    currentSystem = code;
    loadModule(library,"impInterface",pluginCode);
}

void syscharloader::closeMe()
{
    emit closeCalled();
}

void syscharloader::on_pushButton_clicked()
{
    if (docNavTree->isVisible())
        docNavTree->hide();
    else
        docNavTree->show();
}
