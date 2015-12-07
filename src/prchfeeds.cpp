#include "prchfeeds.h"
#include "ui_prchfeeds.h"

prchfeeds::prchfeeds(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::prchfeeds)
{
    ui->setupUi(this);
    list = ui->ListView1;
    connect(ui->cmdapply,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->cmdcancel,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->cmdnew,SIGNAL(clicked()),this,SLOT(addNewItem()));
    connect(ui->cmdelete,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(ui->cmdclose,SIGNAL(clicked()),this,SLOT(closeForm()));

    connect(ui->iMPEdit_5,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_2,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_3,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_4,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_6,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_7,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_8,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_9,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_10,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPComboBox,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    this->setSortColID(2);




    ui->iMPComboBox->addFixItem("Energy feed","Energy feed");
    ui->iMPComboBox->addFixItem("Forage","Forage");
    ui->iMPComboBox->addFixItem("Hay/straw","Hay/straw");
    ui->iMPComboBox->addFixItem("Minerals","Mineral");
    ui->iMPComboBox->addFixItem("Protein suplement","Protein suplement");
    ui->iMPComboBox->addFixItem("Silage","Silage");
    ui->iMPComboBox->addFixItem("Stover","Stover");
    ui->iMPComboBox->addFixItem("Other","Other");


}
prchfeeds::~prchfeeds()
{
    this->unLoadMaintenance();
    delete ui;
}
