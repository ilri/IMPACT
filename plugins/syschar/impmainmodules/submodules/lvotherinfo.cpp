#include "lvotherinfo.h"
#include "ui_lvotherinfo.h"

lvotherinfo::lvotherinfo(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::lvotherinfo)
{
    ui->setupUi(this);

    ui->cbobodycond->addFixItem("Good","Good");
    ui->cbobodycond->addFixItem("Medium","Medium");
    ui->cbobodycond->addFixItem("Poor","Poor");

    ui->cbofeedsys->addFixItem("0","Free grazing year round (communal land)");
    ui->cbofeedsys->addFixItem("1","Free grazing during certain months");
    ui->cbofeedsys->addFixItem("2","Free range");
    ui->cbofeedsys->addFixItem("3","Tethered in communal areas");
    ui->cbofeedsys->addFixItem("4","Tethered within the farm/compound");
    ui->cbofeedsys->addFixItem("5","Zero grazing (stalling)");
    ui->cbofeedsys->addFixItem("6","Other");

    ui->cbocorrsys->addFixItem("1","Night corralling in open kraals");
    ui->cbocorrsys->addFixItem("2","Night corralling under roof");
    ui->cbocorrsys->addFixItem("3","Zero grazing in open kraal");
    ui->cbocorrsys->addFixItem("4","Zero grazing in roofed boma");
    ui->cbocorrsys->addFixItem("5","Zero grazing in roofed and hard floored");
    ui->cbocorrsys->addFixItem("6","Other");

    connect(ui->BitBtn2,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn4,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->BitBtn5,SIGNAL(clicked()),this,SIGNAL(closeCalled()));

}

lvotherinfo::~lvotherinfo()
{
    //qDebug() << "Before destroy lvotherinfo UI";
    delete ui;
    //qDebug() << "After destroy lvotherinfo UI";
}
