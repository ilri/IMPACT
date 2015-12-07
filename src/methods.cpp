#include "methods.h"
#include "ui_methods.h"

methods::methods(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::methods)
{
    ui->setupUi(this);
    list = ui->ListView1;
    connect(ui->cmdapply,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->cmdcancel,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->cmdnew,SIGNAL(clicked()),this,SLOT(addNewItem()));
    connect(ui->cmdelete,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(ui->cmdclose,SIGNAL(clicked()),this,SLOT(closeForm()));

    connect(ui->iMPEdit_5,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    ui->txtacty->setVisible(false);
}
methods::~methods()
{
    this->unLoadMaintenance();
    delete ui;
}

void methods::addNewItem()
{
    impgenmaint::addNewItem();
    ui->txtacty->setText(currAct);
    valueHasChanged(ui->txtacty->tableName(),ui->txtacty->fieldName(),ui->txtacty->text());
}
