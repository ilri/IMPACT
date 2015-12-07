#ifndef DOCUMENTWINDOW_H
#define DOCUMENTWINDOW_H

#include <QMainWindow>
#include <impgenmaint.h>
#include <syscharloader.h>

namespace Ui {
    class documentWindow;
}

class documentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit documentWindow(QWidget *parent = 0);
    ~documentWindow();
    void setMaintenanceWindow(impgenmaint * window);
    void setSysCharWindow(syscharloader *window);
    void setWindow(QWidget *window);

private:
    Ui::documentWindow *ui;
    impgenmaint *child;
    QPointer<QWidget> childWindow;
    syscharloader *syschar;
private slots:

    void closeMe();
};

#endif // DOCUMENTWINDOW_H
