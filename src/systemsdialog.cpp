#include "systemsdialog.h"
#include "ui_systemsdialog.h"

systemsDialog::systemsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::systemsDialog)
{
    ui->setupUi(this);
    generalWindow = new general(this);
    ui->MainLayout->addWidget(generalWindow);
    connect(generalWindow,SIGNAL(closeCalled()),this,SLOT(close()));
}

systemsDialog::~systemsDialog()
{
    delete ui;
}

void systemsDialog::loadData(QSqlDatabase db,QString systemCode)
{
    generalWindow->setSystemCode(systemCode);
    generalWindow->loadMaintenance(db,0);
}

void systemsDialog::createNewSystem(QSqlDatabase db)
{
    generalWindow->setInsertMode(true);
    generalWindow->loadMaintenance(db,0);
}

QString systemsDialog::getSystemAdded()
{
    return generalWindow->getSystemAdded();
}

QString systemsDialog::getCurrentDescription()
{
    return generalWindow->getCurrentDescription();
}
