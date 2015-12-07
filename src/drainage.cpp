#include "drainage.h"
#include "ui_drainage.h"

drainage::drainage(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::drainage)
{
    ui->setupUi(this);
    list = ui->ListView1;
    connect(ui->cmdapply,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->cmdcancel,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->cmdnew,SIGNAL(clicked()),this,SLOT(addNewItem()));
    connect(ui->cmdelete,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(ui->cmdclose,SIGNAL(clicked()),this,SLOT(closeForm()));

    connect(ui->iMPEdit_5,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

}
drainage::~drainage()
{
    this->unLoadMaintenance();
    delete ui;
}
