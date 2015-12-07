#include "markoutlets.h"
#include "ui_markoutlets.h"

markoutlets::markoutlets(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::markoutlets)
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
markoutlets::~markoutlets()
{
    this->unLoadMaintenance();
    delete ui;
}
