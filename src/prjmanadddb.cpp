#include "prjmanadddb.h"
#include "ui_prjmanadddb.h"
#include <QFileDialog>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

prjmanadddb::prjmanadddb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prjmanadddb)
{
    ui->setupUi(this);
    addDB = false;
}

prjmanadddb::~prjmanadddb()
{
    delete ui;
}

void prjmanadddb::on_cmdcancel_clicked()
{
    this->close();
}

bool prjmanadddb::validateDB(QString fileName)
{
    bool res;
    res = false;
    {
        QSqlDatabase database;
        database = QSqlDatabase::addDatabase("QSQLITE","testDB");

        database.setDatabaseName(fileName); //Set the databasename according to the textbox in the UI
        if (!database.open())
        {

            QMessageBox msgBox;
            msgBox.setText("The database cannot be opened.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();

        }
        else
        {
            QString sql;
            sql = "SELECT project_des FROM project";
            QSqlQuery projects(database);
            if (projects.exec(sql))
            {
                if (projects.first())
                {
                    res = true;
                    prjName = projects.value(0).toString();
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setText("The selected file is not an IMPACT database");
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setIcon(QMessageBox::Critical);
                    msgBox.exec();
                }
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText("The selected file is not an IMPACT database");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();

            }
        }

    }
    QSqlDatabase::removeDatabase("testDB");
    return res;
}

void prjmanadddb::on_cmdbrowse_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
         tr("Open database"), "~/home", tr("IMPACT database file (*.sqlite)"));
    if (!QFile::exists(fileName))
    {
        return;
    }

    ui->txtdb->setText(fileName);

    if (validateDB(fileName))
    {
        ui->txtprj->setText(prjName);
        ui->txtprj->setFocus();
    }
    else
        ui->txtdb->setText("");
}

void prjmanadddb::on_cmdadd_clicked()
{
    if (validateDB(ui->txtdb->text()))
    {
        if (!ui->txtprj->text().isEmpty())
        {
            prjName = ui->txtprj->text();
            prjDB = ui->txtdb->text();
            addDB = true;
            this->close();
        }
    }
}
