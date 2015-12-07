#include "inputsallocation.h"
#include "ui_inputsallocation.h"
#include <qsqlquery.h>

inputsallocation::inputsallocation(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::inputsallocation)
{
    ui->setupUi(this);
    connect(ui->CheckBox4,SIGNAL(clicked(bool)),this,SLOT(farm(bool)));
    connect(ui->CheckBox5,SIGNAL(clicked(bool)),this,SLOT(exch(bool)));
    connect(ui->CheckBox6,SIGNAL(clicked(bool)),this,SLOT(prch(bool)));
    connect(ui->CheckBox14,SIGNAL(clicked(bool)),this,SLOT(machine(bool)));

    connect(ui->BitBtn14,SIGNAL(clicked()),this,SIGNAL(closeCalled()));
    connect(ui->BitBtn15,SIGNAL(clicked()),this,SLOT(applyToAll()));
    connect(ui->BitBtn12,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn13,SIGNAL(clicked()),this,SLOT(cancelChanges()));
}

inputsallocation::~inputsallocation()
{
    //qDebug() << "Before destroy inputsallocation UI";
    delete ui;
    //qDebug() << "After destroy inputsallocation UI";
}

void inputsallocation::setCurrAct(QString activity)
{
    currAct = activity;
}

void inputsallocation::loadMaintenance(QSqlDatabase db,QTableView *)
{
    //Load the combo!!!
    QSqlQuery tquery(db);
    QString sql;
    sql = "SELECT met_cod,met_des FROM actymethods WHERE acty_cod = '" + currAct + "'";
    tquery.exec(sql);
    while (tquery.next())
    {
        ui->ComboBox1->addFixItem(tquery.value(0).toString(),tquery.value(1).toString());
    }
    ui->ComboBox1->addFixItem("","None");
    impgenmaint::loadMaintenance(db);
    farm(ui->CheckBox4->isChecked());
    exch(ui->CheckBox5->isChecked());
    prch(ui->CheckBox6->isChecked());
    machine(ui->CheckBox14->isChecked());
}

void inputsallocation::farm(bool checked)
{
    ui->Edit9->setEnabled(checked);
    if (!checked)
    {
        ui->Edit9->setText("0");
    }
}

void inputsallocation::prch(bool checked)
{
    ui->Edit11->setEnabled(checked);
    if (!checked)
    {
        ui->Edit11->setText("0");
    }
}

void inputsallocation::exch(bool checked)
{
    ui->Edit10->setEnabled(checked);
    if (!checked)
    {
        ui->Edit10->setText("0");
    }
}

void inputsallocation::machine(bool checked)
{
    ui->Edit21->setEnabled(checked);
    ui->Edit22->setEnabled(checked);
    ui->iMPEdit->setEnabled(checked);

    if (!checked)
    {
        ui->Edit21->setText("0");
        ui->Edit22->setText("0");
        ui->iMPEdit->setText("");
    }
}

void inputsallocation::cancelChanges()
{
    impgenmaint::cancelChanges();
    farm(ui->CheckBox4->isChecked());
    exch(ui->CheckBox5->isChecked());
    prch(ui->CheckBox6->isChecked());
    machine(ui->CheckBox14->isChecked());
}

void inputsallocation::usePlanDensity(bool status)
{
    if (!status)
    {
        ui->Edit13->setTableName("NOTiNuSE");
        ui->Edit13->setVisible(false);
        ui->Label14->setVisible(false);
    }
}

void inputsallocation::setHeader1Description(QString description)
{
    ui->header1->setText(description);
}

void inputsallocation::setHeader2Description(QString description)
{
    ui->header2->setText(description);
}
