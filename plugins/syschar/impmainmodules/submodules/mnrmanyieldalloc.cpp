#include "mnrmanyieldalloc.h"
#include "ui_mnrmanyieldalloc.h"

mnrmanyieldalloc::mnrmanyieldalloc(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::mnrmanyieldalloc)
{
    ui->setupUi(this);

    connect(ui->BitBtn24,SIGNAL(clicked()),this,SIGNAL(closeCalled()));
}

mnrmanyieldalloc::~mnrmanyieldalloc()
{
    //qDebug() << "Before destroy mnrmanyieldalloc UI";
    delete ui;
    //qDebug() << "After destroy mnrmanyieldalloc UI";
}

void mnrmanyieldalloc::on_BitBtn17_clicked()
{
    this->applyChanges();
}

void mnrmanyieldalloc::on_BitBtn21_clicked()
{
    this->cancelChanges();
}
