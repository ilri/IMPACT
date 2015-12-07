#include "cropyield.h"
#include "ui_cropyield.h"

cropyield::cropyield(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::cropyield)
{
    ui->setupUi(this);
    connect(ui->chkfs,SIGNAL(clicked(bool)),this,SLOT(chgfs(bool)));
    connect(ui->chkhc,SIGNAL(clicked(bool)),this,SLOT(chghc(bool)));
    connect(ui->chklf,SIGNAL(clicked(bool)),this,SLOT(chglf(bool)));
    connect(ui->chklv,SIGNAL(clicked(bool)),this,SLOT(chglv(bool)));
    connect(ui->chkmm,SIGNAL(clicked(bool)),this,SLOT(chgmm(bool)));
    connect(ui->chkst,SIGNAL(clicked(bool)),this,SLOT(chgst(bool)));
    connect(ui->chkot,SIGNAL(clicked(bool)),this,SLOT(chgot(bool)));
    connect(ui->BitBtn18,SIGNAL(clicked()),this,SIGNAL(closeCalled()));
    connect(ui->BitBtn19,SIGNAL(clicked()),this,SLOT(applyToAll()));
    connect(ui->BitBtn16,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn17,SIGNAL(clicked()),this,SLOT(cancelChanges()));
}

cropyield::~cropyield()
{
    //qDebug() << "Before destroy cropyield UI";
    delete ui;
    //qDebug() << "After destroy cropyield UI";
}


void cropyield::setHeader1Description(QString description)
{
    ui->header1->setText(description);
}

void cropyield::setHeader2Description(QString description)
{
    ui->header2->setText(description);
}

void cropyield::loadMaintenance(QSqlDatabase db,QTableView *)
{
    impgenmaint::loadMaintenance(db);
    ui->txtfs->setEnabled(ui->chkfs->isChecked());
    ui->txthc->setEnabled(ui->chkhc->isChecked());
    ui->txtlf->setEnabled(ui->chklf->isChecked());
    ui->txtlv->setEnabled(ui->chklv->isChecked());
    ui->txtmm->setEnabled(ui->chkmm->isChecked());
    ui->txtst->setEnabled(ui->chkst->isChecked());
    ui->txtot->setEnabled(ui->chkot->isChecked());
}

void cropyield::chgfs(bool checked)
{
    ui->txtfs->setEnabled(checked);
    ui->txtfs->setText("0");
}

void cropyield::chghc(bool checked)
{
    ui->txthc->setEnabled(checked);
    ui->txthc->setText("0");
}

void cropyield::chglf(bool checked)
{
    ui->txtlf->setEnabled(checked);
    ui->txtlf->setText("0");
}

void cropyield::chglv(bool checked)
{
    ui->txtlv->setEnabled(checked);
    ui->txtlv->setText("0");
}

void cropyield::chgmm(bool checked)
{
    ui->txtmm->setEnabled(checked);
    ui->txtmm->setText("0");
}

void cropyield::chgst(bool checked)
{
    ui->txtst->setEnabled(checked);
    ui->txtst->setText("0");
}

void cropyield::chgot(bool checked)
{
    ui->txtot->setEnabled(checked);
    ui->txtot->setText("0");
}
