#include "mnrmannkpc.h"
#include "ui_mnrmannkpc.h"

mnrmannkpc::mnrmannkpc(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::mnrmannkpc)
{
    ui->setupUi(this);
}

mnrmannkpc::~mnrmannkpc()
{
    //qDebug() << "Before destroy mnrmannkpc UI";
    delete ui;
   // qDebug() << "After destroy mnrmannkpc UI";
}

void mnrmannkpc::on_BitBtn16_clicked()
{
    this->applyChanges();
}

void mnrmannkpc::on_BitBtn19_clicked()
{
    this->cancelChanges();
}

void mnrmannkpc::on_BitBtn23_clicked()
{
    emit closeCalled();
}
