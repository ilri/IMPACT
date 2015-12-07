#include "prjmanager.h"
#include "ui_prjmanager.h"
#include <QDir>
#include <QSqlDatabase>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include "prjmanadddb.h"
#include "prjmancreatedb.h"
#include <QMessageBox>



projectsModel::projectsModel(QObject *parent)
    :QAbstractTableModel(parent)
{

}

projectsModel::~projectsModel()
{
    //qDebug() << "Destroy: systemTypesModel";
}

int projectsModel::rowCount(const QModelIndex &) const
{
    return items.count();
}

int projectsModel::columnCount(const QModelIndex &) const
{
    return 2;
}

Qt::ItemFlags projectsModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant projectsModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return items[index.row()].description;
        if (index.column() == 1)
            return items[index.row()].databaseFile;
    }

    if (role == Qt::BackgroundRole)
    {
        if (!items[index.row()].error)
        {
            if (items[index.row()].selected)
            {
                QBrush Background(QColor(191,255,191));
                return Background;
            }
            else
            {
                if (!items[index.row()].defDB)
                    return QVariant();
                else
                {
                    QBrush Background(QColor(255,211,32));
                    return Background;
                }
            }
        }
        else
        {
            QBrush Background(QColor(166,202,240));
            return Background;
        }
    }
    return QVariant();
}

bool projectsModel::setData(const QModelIndex &,const QVariant &,int)
{
    return false;
}

QVariant projectsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Project";
            if (section == 1)
                return "Database";
        }
    }
    return QVariant();
}

void projectsModel::loadItems()
{
    this->beginResetModel();
    items.clear();

    QDir dir;
    QString cnfFile;
    cnfFile = dir.absolutePath()+dir.separator()+"impact.cnf";

    if (!QFile::exists(cnfFile))
    {
        qWarning() << "Error: conf file does not exists";
        return;
    }

    {
        QSqlDatabase database;
        database = QSqlDatabase::addDatabase("QSQLITE","cnfcon");

        database.setDatabaseName(cnfFile); //Set the databasename according to the textbox in the UI
        if (!database.open())
        {
            qWarning() << "Error: Cannot open conf file";
        }
        else
        {
            QString sql;
            sql = "SELECT prj_cod,prj_des,prj_db,prj_def,prj_sel FROM projects";
            QSqlQuery projects(database);
            if (projects.exec(sql))
            {
                while (projects.next())
                {
                    TprojectItem item;
                    item.code = projects.value(0).toString();
                    item.description = projects.value(1).toString();
                    item.databaseFile = projects.value(2).toString();
                    item.defDB = projects.value(3).toBool();
                    item.selected = projects.value(4).toBool();
                    if (QFile::exists(item.databaseFile))
                        item.error = false;
                    else
                        item.error = true;
                    items.append(item);
                }
            }
            else
            {
                qWarning() << "Error: " << projects.lastError().databaseText();
            }
        }

    }
    QSqlDatabase::removeDatabase("cnfcon");


    this->endResetModel();
}

void projectsModel::activateProject(QString code)
{

    QDir dir;
    QString cnfFile;
    cnfFile = dir.absolutePath()+dir.separator()+"impact.cnf";

    if (!QFile::exists(cnfFile))
    {
        qWarning() << "Error: conf file does not exists";
    }

    {
        QSqlDatabase database;
        database = QSqlDatabase::addDatabase("QSQLITE","cnfcon");

        database.setDatabaseName(cnfFile); //Set the databasename according to the textbox in the UI
        if (!database.open())
        {
            qWarning() << "Error: Cannot open conf file";
        }
        else
        {
            QString sql;
            sql = "UPDATE projects SET prj_sel = 0";
            QSqlQuery projects(database);
            projects.exec(sql);


            sql = "UPDATE projects SET prj_sel = 1 WHERE prj_cod = '" + code + "'";
            projects.exec(sql);
        }

    }
    QSqlDatabase::removeDatabase("cnfcon");

}


bool projectsModel::removeDatabase(int row)
{
    bool res;
    res = false;

    if (items[row].defDB)
    {
        return res;
    }

    if (items[row].selected)
    {
        return res;
    }


    QDir dir;
    QString cnfFile;
    cnfFile = dir.absolutePath()+dir.separator()+"impact.cnf";

    if (!QFile::exists(cnfFile))
    {
        qWarning() << "Error: conf file does not exists";
        return res;
    }

    {
        QSqlDatabase database;
        database = QSqlDatabase::addDatabase("QSQLITE","cnfcon");

        database.setDatabaseName(cnfFile); //Set the databasename according to the textbox in the UI
        if (!database.open())
        {
            qWarning() << "Error: Cannot open conf file";
        }
        else
        {
            QString sql;
            sql = "DELETE FROM projects WHERE prj_cod = '" + items[row].code + "'";
            QSqlQuery projects(database);
            if (projects.exec(sql))
            {
                res = true;
            }
            else
            {
                qWarning() << "Error: " << projects.lastError().databaseText();
            }
        }

    }
    QSqlDatabase::removeDatabase("cnfcon");
    return res;
}

bool projectsModel::addDatabase(QString name, QString fileName)
{
    bool res;
    res = false;
    QDir dir;
    QString cnfFile;
    cnfFile = dir.absolutePath()+dir.separator()+"impact.cnf";

    if (!QFile::exists(cnfFile))
    {
        qWarning() << "Error: conf file does not exists";
        return res;
    }

    {
        QSqlDatabase database;
        database = QSqlDatabase::addDatabase("QSQLITE","cnfcon");

        database.setDatabaseName(cnfFile); //Set the databasename according to the textbox in the UI
        if (!database.open())
        {
            qWarning() << "Error: Cannot open conf file";
        }
        else
        {
            QString sql;
            sql = "SELECT max(prj_cod) FROM projects";
            QSqlQuery projects(database);
            if (projects.exec(sql))
            {
                projects.first();
                int maxcod,pos;
                QString newCode,temp;
                maxcod = projects.value(0).toInt();
                maxcod++;

                temp = QString::number(maxcod);
                newCode = temp;
                for (pos = 1; pos <= 3 - temp.length();pos++)
                    newCode = "0" + newCode;

                sql = "INSERT INTO projects (prj_cod,prj_des,prj_db,prj_def,prj_sel)";
                sql = sql + " VALUES (";
                sql = sql + "'" + newCode + "',";
                sql = sql + "'" + name + "',";
                sql = sql + "'" + fileName + "',";
                sql = sql + "0,0)";

                if (projects.exec(sql))
                {
                    res = true;
                }
                else
                {
                    qWarning() << "Error: " << projects.lastError().databaseText();
                }
            }
            else
            {
                qWarning() << "Error: " << projects.lastError().databaseText();
            }
        }

    }
    QSqlDatabase::removeDatabase("cnfcon");
    return res;
}

void projectsModel::setActive(int row)
{
    this->beginResetModel();
    if (QFile::exists(items[row].databaseFile))
    {
        QSettings settings("ILRI", "IMPACT");
        QString currDB = settings.value("currentDB","").toString();
        if (currDB.isEmpty())
        {
            return;
        }

        for (int pos = 0; pos <= items.count()-1;pos++)
            items[pos].selected = false;
        items[row].selected = true;
        settings.setValue("currentDB",items[row].databaseFile);

        activateProject(items[row].code);

        QMessageBox msgBox;
        msgBox.setText("Changing the project will take effect after restarting IMPACT");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();

    }
    this->endResetModel();
}

//**************************************************

prjmanager::prjmanager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::prjmanager)
{
    ui->setupUi(this);

    m_prkmodel = new projectsModel();

    ui->tableView->setModel(m_prkmodel);
    m_prkmodel->loadItems();

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

prjmanager::~prjmanager()
{
    delete ui;
}


void prjmanager::on_pushButton_5_clicked()
{
    emit closeCalled();
}

void prjmanager::on_cmdadd_clicked()
{
    prjmanadddb addDlg;
    addDlg.setWindowTitle("Add project from file");
    addDlg.exec();

    if (addDlg.addDB)
    {
        if (m_prkmodel->addDatabase(addDlg.prjName,addDlg.prjDB))
        {
            m_prkmodel->loadItems();
        }
    }
}

void prjmanager::on_cmduse_clicked()
{
    if (ui->tableView->currentIndex().isValid())
    {
        m_prkmodel->setActive(ui->tableView->currentIndex().row());
    }
}

void prjmanager::on_cmdremove_clicked()
{
    if (ui->tableView->currentIndex().isValid())
    {
        QMessageBox msgDel;
        msgDel.setText("Delete project.");
        msgDel.setInformativeText("Do you want to delete the selected project?");
        msgDel.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgDel.setDefaultButton(QMessageBox::Cancel);
        int ret = msgDel.exec();
        if (ret == QMessageBox::Ok)
        {

            if (m_prkmodel->removeDatabase(ui->tableView->currentIndex().row()))
                m_prkmodel->loadItems();
            else
            {
                QMessageBox msgBox;
                msgBox.setText("You cannot remove the selected or default project");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
            }
        }
    }
}

void prjmanager::on_cmdnew_clicked()
{
    prjmancreatedb crtDialog;
    crtDialog.setWindowTitle("Create new project");
    crtDialog.exec();
    if (crtDialog.crateDB)
    {

        if (m_prkmodel->addDatabase(crtDialog.prjName,crtDialog.prjDB))
        {
            m_prkmodel->loadItems();
        }

    }
}
