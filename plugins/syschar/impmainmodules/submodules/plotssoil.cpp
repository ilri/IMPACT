#include "plotssoil.h"
#include "ui_plotssoil.h"

plotssoil::plotssoil(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::plotssoil)
{
    ui->setupUi(this);
    connect(ui->cmdapplyall,SIGNAL(clicked()),this,SLOT(applyToAll()));
    connect(ui->cmdapply,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->cmdcancel,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->cmdclose,SIGNAL(clicked()),this,SIGNAL(closeCalled()));
    connect(ui->cmbdrainage,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->cmbsoil,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->txtdepth,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->txtlname,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->txtpermea,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->txtslope,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    connect(ui->cmbcstruct,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->cmberosion,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->cmblandpos,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    connect(ui->txtcolour,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->txtfertclass,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
}

plotssoil::~plotssoil()
{
    //qDebug() << "Before destroy plotssoil UI";
    delete ui;
   // qDebug() << "After destroy plotssoil UI";
}

void plotssoil::loadMaintenance(QSqlDatabase db,QTableView *tableView)
{
    ui->cmbsoil->loadLkValues(db,"SOIL_COD","LOCA_DES","");
    ui->cmbdrainage->loadLkValues(db,"DRAIN_COD","DRAIN_DES","");

    ui->cmbcstruct->addFixItem("Terraces","Terraces");
    ui->cmbcstruct->addFixItem("Ditches","Ditches");
    ui->cmbcstruct->addFixItem("Other","Other");

    ui->cmberosion->addFixItem("No signs","No signs");
    ui->cmberosion->addFixItem("Moderate signs","Moderate signs");
    ui->cmberosion->addFixItem("Severe signs","Severe signs");
    ui->cmberosion->addFixItem("Other","Other");

    ui->cmblandpos->addFixItem("Uplad","Uplad");
    ui->cmblandpos->addFixItem("Mid slope","Mid slope");
    ui->cmblandpos->addFixItem("Bottomland","Bottomland");
    ui->cmblandpos->addFixItem("Other","Other");

    ui->tabWidget->setCurrentIndex(0);

    impgenmaint::loadMaintenance(db,tableView);
}

void plotssoil::applyChanges()
{
    ui->cmdapply->setEnabled(false);
    ui->cmdapplyall->setEnabled(false);
    ui->cmdcancel->setEnabled(false);
    impgenmaint::applyChanges();
}

void plotssoil::cancelChanges()
{
    ui->cmdapply->setEnabled(false);
    ui->cmdapplyall->setEnabled(false);
    ui->cmdcancel->setEnabled(false);
    impgenmaint::cancelChanges();
}

void plotssoil::valueHasChanged(QString, QString, QString)
{
    ui->cmdapply->setEnabled(true);
    ui->cmdapplyall->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void plotssoil::applyToAll()
{
    ui->cmdapply->setEnabled(false);
    ui->cmdapplyall->setEnabled(false);
    ui->cmdcancel->setEnabled(false);
    impgenmaint::applyToAll();
}
