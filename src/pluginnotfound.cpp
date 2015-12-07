#include "pluginnotfound.h"
#include "ui_pluginnotfound.h"

pluginnotfound::pluginnotfound(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pluginnotfound)
{
    ui->setupUi(this);
}

pluginnotfound::~pluginnotfound()
{
    delete ui;
}

void pluginnotfound::setInfo(QString file, QString name)
{
   ui->lineEdit->setText(file);
   ui->lineEdit_2->setText(name);
}
