#include "fdglvallocation.h"
#include "ui_fdglvallocation.h"

fdglvallocation::fdglvallocation(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::fdglvallocation)
{
    ui->setupUi(this);
}

fdglvallocation::~fdglvallocation()
{
    //qDebug() << "Before destroy fdglvallocation UI";
    delete ui;
    //qDebug() << "After destroy fdglvallocation UI";
}
