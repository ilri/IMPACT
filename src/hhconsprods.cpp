#include "hhconsprods.h"
#include "ui_hhconsprods.h"

hhconsprods::hhconsprods(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::hhconsprods)
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
    connect(ui->iMPEdit_10,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPComboBox,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    this->setSortColID(2);


    ui->iMPComboBox->addFixItem("Cereals","Cereals");
    ui->iMPComboBox->addFixItem("Eggs","Eggs");
    ui->iMPComboBox->addFixItem("Fish","Fish");
    ui->iMPComboBox->addFixItem("Fruits","Fruits");
    ui->iMPComboBox->addFixItem("Meat","Meat");
    ui->iMPComboBox->addFixItem("Milk","Milk");
    ui->iMPComboBox->addFixItem("Oil","Oil");
    ui->iMPComboBox->addFixItem("Pulses","Pulses");
    ui->iMPComboBox->addFixItem("Starches","Starches");
    ui->iMPComboBox->addFixItem("Sugar","Sugar");
    ui->iMPComboBox->addFixItem("Vegetables","Vegetables");


}
hhconsprods::~hhconsprods()
{
    this->unLoadMaintenance();
    delete ui;
}
