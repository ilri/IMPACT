#include "mnrmanyielddsp.h"
#include "ui_mnrmanyielddsp.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

mnrmanyielddsp::mnrmanyielddsp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mnrmanyielddsp)
{
    ui->setupUi(this);

    QStringList headers;
    headers << "Period";
    headers << "Yield (kg)";

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
}

mnrmanyielddsp::~mnrmanyielddsp()
{
    delete ui;

}

void mnrmanyielddsp::on_pushButton_clicked()
{
    this->close();
}

void mnrmanyielddsp::setDataBase(QSqlDatabase db)
{
    database = db;
}

void mnrmanyielddsp::loadData(QString table, QList <TmoduleFieldDef> keys)
{
    QSqlQuery query(database);
    QString sql;
    if (table != "mnrheapprods")
    {
        sql = "SELECT periods.period_id,periods.period_des," + table + ".pormm FROM periods," + table;
        sql = sql + " WHERE " + table + ".nmonth = periods.period_id AND ";
        for (int pos = 0; pos <= keys.count()-1;pos++)
        {
            sql = sql + table + "." + keys[pos].code + " = '";
            sql = sql + keys[pos].value.toString() + "' AND ";
        }
        sql = sql.left(sql.length()-5);
        //sql = sql + " ORDER BY periods.period_id";
        if (!query.exec(sql))
        {
            qDebug() << "Error: " << query.lastError().databaseText();
            qDebug() << "Executing:"  << sql;
        }
        else
        {
            int totrows;
            totrows = 0;
            while (query.next())
            {
                totrows++;
            }
            ui->tableWidget->setRowCount(totrows);
            query.first();
            totrows = -1;
            float value;
            while (query.isValid())
            {
                totrows++;
                QTableWidgetItem *period = new QTableWidgetItem(query.value(1).toString());
                value = query.value(2).toFloat();
                QTableWidgetItem *pvalue = new QTableWidgetItem(QString::number(value,'f',2));
                ui->tableWidget->setItem(totrows, 0, period);
                ui->tableWidget->setItem(totrows, 1, pvalue);
                query.next();
            }
        }
    }
}
