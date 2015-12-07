#include "prjsettings.h"
#include "ui_prjsettings.h"
#include "impinterface.h"
#include <QTreeWidgetItemIterator>
#include <QSqlQuery>
#include <QSqlError>
#include "gendelegates.h"
#include <QMessageBox>
#include <QDebug>

prjsettings::prjsettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::prjsettings)
{
    ui->setupUi(this);

    m_plugintree = new plugintree(this);
    ui->verticalLayout_2->addWidget(m_plugintree);

    m_plugintree->setHeaderLabel(tr("Available plugins"));

    m_moduletree = new moduletree(this);
    ui->lyttree->addWidget(m_moduletree);
    m_moduletree->setHeaderLabel(tr("Current module tree"));

    applyTree = false;
    applyCalendar = false;
    applyForm = false;

    connect(m_moduletree,SIGNAL(treeChanged()),this,SLOT(treeChanged()));

    connect(ui->txtname,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->txtedate,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->txtsdate,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->txtnperiods,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->txtperioddesc,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->cmblanguage,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    ui->cmblanguage->addFixItem("ENG","English");
}

prjsettings::~prjsettings()
{
    delete ui;
}

void prjsettings::loadData()
{
    m_colModel = new genericFldModel(this);
    m_colModel->setDatabase(dataBase);
    m_colModel->setTableName("periods");
    m_colModel->insertExtraColumn("STS","  ","","","","");
    m_colModel->insertField("period_id","Period ID","000","000",true,true,false);
    m_colModel->insertField("period_des","Description","","",false,false,true,true);
    m_colModel->insertField("period_jan","Jan","T","T",false,false,true,true);
    m_colModel->insertField("period_feb","Feb","T","T",false,false,true,true);
    m_colModel->insertField("period_mar","Mar","T","T",false,false,true,true);
    m_colModel->insertField("period_apr","Apr","T","T",false,false,true,true);
    m_colModel->insertField("period_may","May","T","T",false,false,true,true);
    m_colModel->insertField("period_jun","Jun","T","T",false,false,true,true);
    m_colModel->insertField("period_jul","Jul","T","T",false,false,true,true);
    m_colModel->insertField("period_aug","Aug","T","T",false,false,true,true);
    m_colModel->insertField("period_sep","Sep","T","T",false,false,true,true);
    m_colModel->insertField("period_oct","Oct","T","T",false,false,true,true);
    m_colModel->insertField("period_nov","Nov","T","T",false,false,true,true);
    m_colModel->insertField("period_dec","Dec","T","T",false,false,true,true);
    m_colModel->addReadOnlyField("STS","ALL");
    m_colModel->addDistributionException(0);
    connect(m_colModel,SIGNAL(modeldDataChanged()),this,SLOT(modeldDataChanged()));

    QList<TgenModelFieldDef> keys;
    m_colModel->loadData(keys);
    ui->tableView->setModel(m_colModel);
    int pos;
    for (pos = 0; pos <= m_colModel->columnCount()-1;pos++)
    {
        if (pos <= 1)
            ui->tableView->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->tableView->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }
    genChkDelegate *chkDelegate;
    for (pos = 2; pos <= m_colModel->columnCount()-1;pos++)
    {
        chkDelegate = new genChkDelegate(this);
        chkDelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
        chkDelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
        ui->tableView->setItemDelegateForColumn(pos,chkDelegate);
    }

    QSqlQuery modules(dataBase);
    QString sql;

    sql = "SELECT COUNT(sys_cod) FROM system";
    if (modules.exec(sql))
    {
        modules.first();
        if (modules.value(0).toInt() > 0)
        {
            ui->txtnperiods->setEnabled(false);
        }
    }
    else
    {
        qWarning() << "Error: " << modules.lastError().number() << " - " << modules.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        ui->txtnperiods->setText("12");
        ui->txtnperiods->setEnabled(false);
    }


    sql = "SELECT project_id,project_des,project_sdate,project_edate,";
    sql = sql + "project_language,project_nperiods,project_perioddesc FROM project";

    if (modules.exec(sql))
    {
        while (modules.next())
        {
            ui->txtname->setText(modules.value(1).toString());
            ui->txtsdate->setText(modules.value(2).toString());
            ui->txtedate->setText(modules.value(3).toString());
            ui->txtnperiods->setText(modules.value(5).toString());
            ui->txtperioddesc->setText(modules.value(6).toString());
            ui->cmblanguage->selectValue(modules.value(4).toString());
        }
    }
    else
    {
        qWarning() << "Error: " << modules.lastError().number() << " - " << modules.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

void prjsettings::modeldDataChanged()
{
    applyCalendar = true;
    ui->cmdapply->setEnabled(true);
}

void prjsettings::valueHasChanged(QString ,QString ,QString)
{
    applyForm = true;
     ui->cmdapply->setEnabled(true);
}

int prjsettings::getLibraryIndex(QString library)
{
    int pos;
    for (pos =0; pos <= plugitems.count()-1;pos++)
    {
        if (plugitems[pos]->data(0,Qt::UserRole).toString() == library)
        {
            return pos;
        }
    }
    return -1;
}

void prjsettings::setPlugins(QList<impPluginInstance *> lstPlugins)
{
    m_moduletree->setPlugins(lstPlugins);
    QString library;
    QList<impPlugin *> lplugins;
    QString plugin;
    QString pluginDesc;
    QString instance;
    int pos;
    int pos2;
    int node;
    impInterface *iinstance;
    for (pos=0;pos <=lstPlugins.count()-1;pos++)
    {
        library = lstPlugins[pos]->getLibraryName();
        instance = lstPlugins[pos]->getInstanceName();
        if (instance == "impInterface")
        {
            node = getLibraryIndex(library);
            if (node < 0)
                node = insertPluginItem(-1,library,"","");
            lplugins = lstPlugins[pos]->getPlugins();
            for (pos2 = 0; pos2 <= lplugins.count()-1;pos2++)
            {
                plugin = lplugins[pos2]->pluginName;

                iinstance = qobject_cast<impInterface *>(lstPlugins[pos]->getInstance());
                pluginDesc = iinstance->getPluginDescription(plugin);

                insertPluginItem(node,library,plugin,pluginDesc);
            }
        }
    }
    iinstance = 0;
    m_plugintree->insertTopLevelItems(0, plugitems);
    m_plugintree->expandAll();
}

int prjsettings::insertPluginItem(int parent, QString library, QString plugin,QString pluginDescription)
{
    //qDebug() << caption;
    int res;
    QTreeWidgetItem *newItem;
    if (parent == -1)
    {
        newItem = new QTreeWidgetItem(QStringList(library));
        newItem->setData(0,Qt::UserRole,library);
        newItem->setData(0,Qt::UserRole+1,"");
        newItem->setData(0,Qt::UserRole+2,"");
        plugitems.append(newItem);
    }
    else
    {
        newItem = new QTreeWidgetItem(plugitems[parent],QStringList(pluginDescription));
        newItem->setData(0,Qt::UserRole,library);
        newItem->setData(0,Qt::UserRole+1,plugin);
        newItem->setData(0,Qt::UserRole+2,pluginDescription);
        plugitems.append(newItem);
    }
    res = plugitems.count()-1; //We get here the index of the last item
    //Change some flags and set the state of the node of unchecked
    plugitems[res]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return res; //We return the new node so can be used to set childs
}

void prjsettings::setDatabase(QSqlDatabase db)
{
    dataBase = db;
    m_moduletree->loadTree(dataBase);
    loadData();
}

void prjsettings::on_pushButton_7_clicked()
{
    QTreeWidgetItem *item;
    item = m_moduletree->currentItem();
    if (item)
    {
        delete item;
        treeChanged();
    }
}

void prjsettings::treeChanged()
{
    applyTree = true;
    ui->cmdapply->setEnabled(true);
}

void prjsettings::doApplyForm()
{
    QSqlQuery modules(dataBase);
    QString sql;

    sql = "INSERT INTO project (project_id,project_des,project_sdate,project_edate,";
    sql = sql + "project_language,project_nperiods,project_perioddesc) VALUES (";
    sql = sql + "1,";
    sql = sql + "'" + ui->txtname->text() + "',";
    sql = sql + "'" + ui->txtsdate->text() + "',";
    sql = sql + "'" + ui->txtedate->text() + "',";
    sql = sql + "'" + ui->cmblanguage->getCurrentValue() + "',";
    sql = sql + ui->txtnperiods->text() + ",";
    sql = sql + "'" + ui->txtperioddesc->text() + "')";
    if (!modules.exec(sql))
    {
        if (modules.lastError().number() == 19)
        {
            sql = "UPDATE project SET ";
            sql = sql + "project_des = ";
            sql = sql + "'" + ui->txtname->text() + "',";
            sql = sql + "project_sdate = ";
            sql = sql + "'" + ui->txtsdate->text() + "',";
            sql = sql + "project_edate = ";
            sql = sql + "'" + ui->txtedate->text() + "',";
            sql = sql + "project_language = ";
            sql = sql + "'" + ui->cmblanguage->getCurrentValue() + "',";
            sql = sql + "project_nperiods = ";
            sql = sql + ui->txtnperiods->text() + ",";
            sql = sql + "project_perioddesc = ";
            sql = sql + "'" + ui->txtperioddesc->text() + "'";
            sql = sql + " WHERE project_id = 1";
            if (!modules.exec(sql))
            {
                qWarning() << "Error: " << modules.lastError().number() << " - " << modules.lastError().databaseText();
                qWarning() << "Executing: " << sql;
            }
        }
    }
    emit changeName(ui->txtname->text());
}

void prjsettings::doApplyTree()
{
    QSqlQuery modules(dataBase);
    QString sql;
    sql = "DELETE FROM moduletree";
    if (modules.exec(sql))
    {
        QTreeWidgetItemIterator it(m_moduletree);
        while (*it)
        {
            sql = "INSERT INTO moduletree(module_id,module_library,";
            sql = sql + "module_plugin,module_pluginDesc,module_order,parent_module)";
            sql = sql + " VALUES (";
            sql = sql + "'" + (*it)->data(0,Qt::UserRole+3).toString() + "',";
            sql = sql + "'" + (*it)->data(0,Qt::UserRole).toString() + "',";
            sql = sql + "'" + (*it)->data(0,Qt::UserRole+1).toString() + "',";
            sql = sql + "'" + (*it)->data(0,Qt::UserRole+2).toString() + "',";
            sql = sql + (*it)->data(0,Qt::UserRole+5).toString() + ",";
            if ((*it)->data(0,Qt::UserRole+4).toString() != "NULL")
                sql = sql + "'" + (*it)->data(0,Qt::UserRole+4).toString() + "')";
            else
                sql = sql + "NULL)";

            if (!modules.exec(sql))
            {
                qWarning() << "Error: " << modules.lastError().databaseText();
                qWarning() << "Executing: " << sql;
            }
            ++it;
        }
        applyTree = false;
    }
    else
    {
        qWarning() << "Error: " << modules.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
}

void prjsettings::on_cmdapply_clicked()
{
    if (applyTree)
    {
        doApplyTree();
    }
    if (applyCalendar)
    {
        m_colModel->applyChanges();
    }
    if (applyForm)
    {
        doApplyForm();
    }
    ui->cmdapply->setEnabled(false);
}

void prjsettings::on_pushButton_clicked()
{
    m_moduletree->createStandardTree();
    applyTree = true;
    ui->cmdapply->setEnabled(true);
}

void prjsettings::on_cmdgenerate_clicked()
{
    QSqlQuery modules(dataBase);
    QString sql;

    sql = "SELECT COUNT(sys_cod) FROM system";
    if (modules.exec(sql))
    {
        modules.first();
        if (modules.value(0).toInt() == 0)
        {
            int totperiods;
            totperiods = ui->txtnperiods->text().toInt();
            if (totperiods > 1)
            {
                sql = "DELETE FROM periods";
                if (modules.exec(sql))
                {
                    int pos;
                    bool error;
                    error = false;
                    for (pos = 1; pos <= totperiods; pos++)
                    {
                        sql = "INSERT INTO periods (period_id,period_des) VALUES (";
                        sql = sql + "'" + QString::number(pos) + "',";
                        sql = sql + "'Set a description for period " + QString::number(pos) + "')";
                        if (!modules.exec(sql))
                        {
                            error = true;
                            qWarning() << "Error: " << modules.lastError().number() << " - " << modules.lastError().databaseText();
                            qWarning() << "Executing: " << sql;
                        }
                    }
                    if (!error)
                    {
                        QList<TgenModelFieldDef> keys;
                        m_colModel->loadData(keys);
                        for (pos = 0; pos <= m_colModel->columnCount()-1;pos++)
                        {
                            if (pos <= 1)
                                ui->tableView->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
                            else
                                ui->tableView->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
                        }
                        ui->tabWidget->setCurrentIndex(1);
                    }
                    else
                    {
                        QMessageBox msgBox;
                        msgBox.setText(tr("An error ocurred while creating the new definition. Report the error to support@qlands.com"));
                        msgBox.exec();
                    }
                }
                else
                {
                    qWarning() << "Error: " << modules.lastError().number() << " - " << modules.lastError().databaseText();
                    qWarning() << "Executing: " << sql;
                    QMessageBox msgBox;
                    msgBox.setText(tr("An error ocurred while deleting the previous definition"));
                    msgBox.exec();
                }
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText(tr("You need to indicate two or more periods"));
                msgBox.exec();
            }
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText(tr("You can only generate period information if there are no farming systems in the database"));
            msgBox.exec();
        }
    }
    else
    {
        qWarning() << "Error: " << modules.lastError().number() << " - " << modules.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        QMessageBox msgBox;
        msgBox.setText(tr("An error ocurred while checking for farming systems."));
        msgBox.exec();
    }
}

void prjsettings::on_pushButton_2_clicked()
{
    emit closeCalled();
}
