#ifndef MNTDIALOG_H
#define MNTDIALOG_H

#include <QDialog>
#include <impgenmaint.h>

namespace Ui {
    class mntdialog;
}

class mntdialog : public QDialog
{
    Q_OBJECT

public:
    explicit mntdialog(QWidget *parent = 0);
    ~mntdialog();
    void setMaintWindow (impgenmaint *maint);

private:
    Ui::mntdialog *ui;
};

#endif // MNTDIALOG_H
