#include "documentwindow.h"
#include "ui_documentwindow.h"
#include "prjsettings.h"

documentWindow::documentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::documentWindow)
{
    ui->setupUi(this);
}

documentWindow::~documentWindow()
{
}

void documentWindow::setMaintenanceWindow(impgenmaint * window)
{
    child = window;
    connect(child,SIGNAL(closeCalled()),this,SLOT(closeMe()));
    ui->mainlayout->addWidget(child);
}

void documentWindow::setWindow(QWidget *window)
{
    childWindow = window;
    if (qobject_cast<prjsettings *>(window))
        connect(qobject_cast<prjsettings *>(window),SIGNAL(closeCalled()),this,SLOT(closeMe()));
    ui->mainlayout->addWidget(childWindow);
}

void documentWindow::setSysCharWindow(syscharloader * window)
{
    //qWarning() << "Start: setSysCharWindow";
    syschar = window;
    connect(syschar,SIGNAL(closeCalled()),this,SLOT(closeMe()));
    ui->mainlayout->addWidget(syschar);
     //qWarning() << "End: setSysCharWindow";
}

void documentWindow::closeMe()
{
    //ui->mainlayout->removeWidget(child);
    this->parentWidget()->close();
}
