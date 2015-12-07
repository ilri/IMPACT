#include "nutrientflows.h"
#include "ui_nutrientflows.h"

nutrientflows::nutrientflows(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nutrientflows)
{
    ui->setupUi(this);
}

nutrientflows::~nutrientflows()
{
    delete ui;
}
