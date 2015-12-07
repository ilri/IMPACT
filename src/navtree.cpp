#include "navtree.h"
#include "ui_navtree.h"
#include <QSqlQuery>
#include <QSqlError>
#include "systemsdialog.h"
#include "IMPACTGlobal.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>

navtree::navtree(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::navtree)
{
    ui->setupUi(this);
    currentSystem = "";
    currentLibrary = "";
    currentPlugin = "";
    currentSystemDesc = "";

    connect(ui->lstsystems,SIGNAL(clicked(QModelIndex)),this,SLOT(systemSelected(QModelIndex)));
    connect(ui->treeWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(pluginSelected(QModelIndex)));

}

navtree::~navtree()
{
    delete ui;
}

void navtree::loadTree(QSqlDatabase db)
{
    QSqlQuery modules(db);
    m_database = db;
    QString sql;
    sql = "SELECT module_id,module_library,";
    sql = sql + "module_plugin,module_pluginDesc,module_order,";
    sql = sql + "parent_module FROM moduletree ORDER BY module_order";
    if (modules.exec(sql))
    {
        while (modules.next())
        {
            if (modules.value(5).toString().isEmpty())
            {
                insertNodes(-1,modules.value(3).toString(),
                            modules.value(1).toString(),modules.value(2).toString());
            }
            else
            {
                insertNodes(modules.value(5).toInt()-1,modules.value(3).toString(),
                            modules.value(1).toString(),modules.value(2).toString());
            }
        }
        this->ui->treeWidget->insertTopLevelItems(0, items);
        this->ui->treeWidget->expandAll();
    }
    else
    {
        qWarning() << "Error: " << modules.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    if (items.count() == 0)
    {
        QMessageBox dlgBox;
        dlgBox.setText("Module tree");
        dlgBox.setInformativeText("The module tree is empty. Use the project settings to create a module tree / load the standard IMPACT tree");
        dlgBox.setStandardButtons(QMessageBox::Ok);
        dlgBox.exec();
    }
}



void navtree::loadSystems(QSqlDatabase dataBase)
{
    sysmodel = new systemsTableModel(this);
    sysmodel->setDatabase(dataBase);
    sysmodel->loadItems();
    ui->lstsystems->setModel(sysmodel);
    ui->lstsystems->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->lstsystems->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->lstsystems->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    ui->lstsystems->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->lstsystems->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    for (int pos =0; pos <= sysmodel->rowCount()-1;pos++)
    {
        ui->lstsystems->setRowHeight(pos,listViewRowSize);
    }

}

void navtree::systemSelected(QModelIndex index)
{
    currentSystem = sysmodel->getSystemCode(index.row());
    currentSystemDesc = sysmodel->getSystemDescription(index.row());
    if (!currentLibrary.isEmpty())
    {
        emit loadSystem(currentSystem,currentSystemDesc,currentLibrary,currentPlugin);
    }
}

void navtree::pluginSelected(QModelIndex index)
{
    currentLibrary = index.data(Qt::UserRole).toString();
    currentPlugin = index.data(Qt::UserRole+1).toString();
    if (!currentSystem.isEmpty())
    {
        emit loadSystem(currentSystem,currentSystemDesc,currentLibrary,currentPlugin);
    }
}

QString navtree::getModuleDescription(QString library, QString interface, QString plugin)
{
    //qWarning() << "Start: getModuleDescription";
    int pos;
    for (pos=0;pos <=plugins.count()-1;pos++)
    {
        if (plugins[pos]->getLibraryName() == library)
        {
            if (plugins[pos]->getInstanceName() == interface)
            {
                impInterface *iinstance = qobject_cast<impInterface *>(plugins[pos]->getInstance());
                return iinstance->getPluginDescription(plugin);
            }
        }
    }
    //qWarning() << "End: getModuleDescription";
    return QString();
}


int navtree::insertNodes(int parent, QString description, QString library, QString pluginCode)
{
    //qDebug() << caption;
    int res;
    QTreeWidgetItem *newItem;
    if (parent == -1)
    {
        newItem = new QTreeWidgetItem(QStringList(description));

        newItem->setText(0, description);
        newItem->setData(0,Qt::UserRole,library);
        newItem->setData(0,Qt::UserRole+1,pluginCode);
        newItem->setData(0,Qt::UserRole+2,description);
        if (getModuleDescription(library,"impInterface",pluginCode).isEmpty())
            newItem->setTextColor(0,QColor(Qt::red));
        items.append(newItem);
    }
    else
    {
        newItem = new QTreeWidgetItem(items[parent],QStringList(description));
        newItem->setText(0, description);
        newItem->setData(0,Qt::UserRole,library);
        newItem->setData(0,Qt::UserRole+1,pluginCode);
        newItem->setData(0,Qt::UserRole+2,description);
        if (getModuleDescription(library,"impInterface",pluginCode).isEmpty())
            newItem->setTextColor(0,QColor(Qt::red));
        items.append(newItem);
    }
    res = items.count()-1; //We get here the index of the last item
    //Change some flags and set the state of the node of unchecked
    items[res]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return res; //We return the new node so can be used to set childs
}

void navtree::setPlugins(QList<impPluginInstance *> lstPlugins)
{
    //qWarning() << "Start: setPlugins";
    plugins.append(lstPlugins);
     //qWarning() << "End: setPlugins";
}

void navtree::on_cmdedit_clicked()
{
    if (!currentSystem.isEmpty())
    {
        QModelIndex index = ui->lstsystems->currentIndex();
        systemsDialog sysdialog;
        sysdialog.setWindowTitle("General characteristics");
        sysdialog.loadData(m_database,currentSystem);
        sysdialog.exec();
        sysmodel->loadItems();
        ui->lstsystems->setCurrentIndex(index);
        ui->lstsystems->scrollTo(index);
        systemSelected(index);
    }
}

void navtree::on_cmdnew_clicked()
{
    systemsDialog sysdialog;
    sysdialog.setWindowTitle("General characteristics -- New system");
    sysdialog.createNewSystem(m_database);
    sysdialog.exec();
    if (!sysdialog.getSystemAdded().isEmpty())
    {
        currentSystem = "";
        QString newCode;
        newCode= sysdialog.getSystemAdded();
        sysmodel->loadItems();
        int idx;
        idx = sysmodel->getSystemIndex(newCode);
        if (idx > 0)
        {
            QModelIndex index;
            index = sysmodel->index(idx,0);
            ui->lstsystems->setCurrentIndex(index);
            ui->lstsystems->scrollTo(index);
            systemSelected(index);

            for (int pos =0; pos <= sysmodel->rowCount()-1;pos++)
            {
                ui->lstsystems->setRowHeight(pos,listViewRowSize);
            }
        }
    }
}

void navtree::on_pushButton_clicked()
{
    if (!currentSystem.isEmpty())
    {
        QMessageBox dlgBox;
        dlgBox.setText("You are going to delete the system " + currentSystem);
        dlgBox.setInformativeText("Do you want to continue?");
        dlgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        dlgBox.setDefaultButton(QMessageBox::Save);
        int ret = dlgBox.exec();
        if (ret == QMessageBox::Ok)
        {
            QSqlQuery dsystem(m_database);
            QString sql;
            sql = "DELETE FROM system WHERE sys_cod = '" + currentSystem + "'";
            if (!dsystem.exec(sql))
            {
                qWarning() << "Error: " << dsystem.lastError().databaseText();
                qWarning() << "Executing: " << sql;
                QMessageBox msgBox;
                msgBox.setText(tr("An error occurred while deleting the system. Contact support@qlands.com"));
                msgBox.exec();
            }
            else
            {
                currentSystem = "";
                sysmodel->loadItems();
                clearScreen();
            }
        }
    }
}
