#include "prjmancreatedb.h"
#include "ui_prjmancreatedb.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QResource>
#include <QTextStream>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

prjmancreatedb::prjmancreatedb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prjmancreatedb)
{
    ui->setupUi(this);
    crateDB = false;
}

prjmancreatedb::~prjmancreatedb()
{
    delete ui;
}

void prjmancreatedb::on_cmdbrowse_clicked()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,
         tr("Create database"), "~/myProject.sqlite", tr("IMPACT database file (*.sqlite)"));
    if (!fileName.isEmpty())
    {
        ui->txtdir->setText(fileName);
        ui->txtprj->setFocus();
    }
}

void prjmancreatedb::on_cmdcancel_clicked()
{
    this->close();
}

void prjmancreatedb::on_cmdcreate_clicked()
{
    QMessageBox msgBox;
    if (ui->txtprj->text().isEmpty())
    {
        msgBox.setText("You need to indicate a project name");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }
    if (!((ui->spinBox->value() >= 2) && (ui->spinBox->value() <= 12)))
    {
        msgBox.setText("You need to indicate a period number between 2 and 12");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }
    if (QFile::exists(ui->txtdir->text()))
    {
        if (!QFile::remove(ui->txtdir->text()))
        {
            msgBox.setText("Cannot remove the previous project file");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
        }
    }

    bool error;
    error = false;
    QFile sqlfile(":/db/baseDB.sql");
    if (!sqlfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    {
        if (QFile::exists(ui->txtdir->text()))
            QFile::remove(ui->txtdir->text());

        QSqlDatabase database;
        database = QSqlDatabase::addDatabase("QSQLITE","newDB");

        database.setDatabaseName(ui->txtdir->text()); //Set the databasename according to the textbox in the UI
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

            QSqlQuery stms(database);
            QTextStream in(&sqlfile);

            int pos;
            pos = 0;
            while (!in.atEnd())
            {
                in.readLine();
                pos++;
            }
            ui->progressBar->setMaximum(pos);
            pos = 0;
            in.seek(0);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                pos++;
                ui->progressBar->setValue(pos);
                ui->progressBar->repaint();
                if (!stms.exec(line))
                {
                    error = true;
                    qDebug() << stms.lastError().databaseText();
                    break;
                }
            }
            QString sql;
            sql = "DELETE FROM periods";
            if (!stms.exec(sql))
            {
                error = true;
                qDebug() << stms.lastError().databaseText();
            }
            sql = "INSERT INTO project (project_id,project_des,project_nperiods)";
            sql = sql + " VALUES (1,'" + ui->txtprj->text() + "',";
            sql = sql + QString::number(ui->spinBox->value()) + ")";
            if (!stms.exec(sql))
            {
                error = true;
                qDebug() << stms.lastError().databaseText();
            }
            for (pos = 1; pos <= ui->spinBox->value();pos++)
            {
                sql = "INSERT INTO periods (period_id,period_des) VALUES (";
                sql = sql + "'" + QString::number(pos) + "',";
                sql = sql + "'Period " + QString::number(pos) + "')";
                if (!stms.exec(sql))
                {
                    error = true;
                    qDebug() << stms.lastError().databaseText();
                    break;
                }
            }
        }
    }

    QSqlDatabase::removeDatabase("newDB");

    if (!error)
    {
        crateDB = true;
        prjName = ui->txtprj->text();
        prjDB = ui->txtdir->text();
        prjPeriods = ui->spinBox->value();
        this->close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("There was an error creating the new database.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        crateDB = false;
        this->close();
    }
}
