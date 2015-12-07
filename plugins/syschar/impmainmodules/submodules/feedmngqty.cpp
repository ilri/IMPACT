#include "feedmngqty.h"
#include "ui_feedmngqty.h"

feedmngqty::feedmngqty(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::feedmngqty)
{
    ui->setupUi(this);
}

feedmngqty::~feedmngqty()
{
    delete ui;
}

void feedmngqty::on_BitBtn14_clicked()
{
    emit closeCalled();
}

void feedmngqty::on_BitBtn12_clicked()
{
    this->applyChanges();
}

void feedmngqty::on_BitBtn13_clicked()
{
    this->cancelChanges();
}

void feedmngqty::on_BitBtn15_clicked()
{
    this->applyToAll(" tim = 'T'");
}
