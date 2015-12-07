#include "errordialog.h"
#include "ui_errordialog.h"


errorDialog::errorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errorDialog)
{
    ui->setupUi(this);
    connect(ui->cmdclose,SIGNAL(clicked()),this,SLOT(closeForm()));
}

errorDialog::~errorDialog()
{
    delete ui;
}

void errorDialog::setErrorMessage(QString message)
{
    errorMsg = message;
    ui->textEdit->setText(errorMsg);
}

void errorDialog::setErrorMessage(QStringList messages)
{
    QString temp;
    for (int pos = 0; pos <= messages.count()-1;pos++)
    {
        temp = QString::number(pos+1) + " - ";
        temp = temp + messages[pos] + '\n';
        ui->textEdit->insertPlainText(temp);
    }
}

void errorDialog::setLastSQLCommand(QString command)
{
    lastSQLCommand = command;
}

void errorDialog::closeForm()
{
    this->close();
}
