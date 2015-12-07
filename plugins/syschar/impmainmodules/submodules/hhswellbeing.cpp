#include "hhswellbeing.h"
#include "ui_hhswellbeing.h"

hhswellbeing::hhswellbeing(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::hhswellbeing)
{
    ui->setupUi(this);
}

hhswellbeing::~hhswellbeing()
{
    //qDebug() << "Before destroy hhswellbeing UI";
    delete ui;
    //qDebug() << "After destroy hhswellbeing UI";
}
