#ifndef CALENDARDIALOG_H
#define CALENDARDIALOG_H

#include <QDialog>
#include <QDate>
#include "export.h"

namespace Ui {
    class calendarDialog;
}

class IMPWIDGETSLIB_EXPORT calendarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit calendarDialog(QWidget *parent = 0);
    ~calendarDialog();
    QDate getCurrentDate();
    void setCurrentDate(QDate date);

private slots:
    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButton_clicked();

private:
    Ui::calendarDialog *ui;
    QDate currentDate;
};

#endif // CALENDARDIALOG_H
