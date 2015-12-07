#include "calendardialog.h"
#include "ui_calendardialog.h"

calendarDialog::calendarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::calendarDialog)
{
    ui->setupUi(this);
    ui->calendarWidget->setGridVisible(true);
}

calendarDialog::~calendarDialog()
{
    delete ui;
}

void calendarDialog::on_calendarWidget_clicked(const QDate &date)
{
    currentDate = date;
    ui->calendarWidget->setSelectedDate(currentDate);
}


QDate calendarDialog::getCurrentDate()
{
    return currentDate;
}

void calendarDialog::setCurrentDate(QDate date)
{
    currentDate = date;
    ui->calendarWidget->setSelectedDate(date);
}

void calendarDialog::on_pushButton_clicked()
{
    close();
}
