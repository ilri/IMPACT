#include "mntdialog.h"
#include "ui_mntdialog.h"

mntdialog::mntdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mntdialog)
{
    ui->setupUi(this);
}

mntdialog::~mntdialog()
{
    delete ui;
}

void mntdialog::setMaintWindow (impgenmaint *maint)
{
    connect(maint,SIGNAL(closeCalled()),this,SLOT(close()));
    ui->mainlayout->addWidget(maint);
}
