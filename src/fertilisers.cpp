#include "fertilisers.h"
#include "ui_fertilisers.h"

fertilisers::fertilisers(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::fertilisers)
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
    connect(ui->iMPCheckBox,SIGNAL(valueChanged(QString,QString,bool)),this,SLOT(valueHasChanged(QString,QString,bool)));

}
fertilisers::~fertilisers()
{
    this->unLoadMaintenance();
    delete ui;
}
