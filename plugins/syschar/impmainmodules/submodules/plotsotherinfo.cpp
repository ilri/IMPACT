#include "plotsotherinfo.h"
#include "ui_plotsotherinfo.h"

plotsotherinfo::plotsotherinfo(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::plotsotherinfo)
{
    ui->setupUi(this);
    connect(ui->BitBtn9,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn22,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->BitBtn28,SIGNAL(clicked()),this,SIGNAL(closeCalled()));
}

plotsotherinfo::~plotsotherinfo()
{
    //qDebug() << "Before destroy plotsotherinfo UI";
    delete ui;
    //qDebug() << "After destroy plotsotherinfo UI";
}
