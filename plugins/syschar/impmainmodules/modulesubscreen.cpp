#include "modulesubscreen.h"
#include "ui_modulesubscreen.h"

moduleSubScreen::moduleSubScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::moduleSubScreen)
{
    ui->setupUi(this);
}

moduleSubScreen::~moduleSubScreen()
{
    //qDebug() << "Before destroy moduleSubScreen UI";
    delete ui;
    //qDebug() << "After destroy moduleSubScreen UI";
    m_child = 0;
    //qDebug() << "After m_child = 0";
}

void moduleSubScreen::loadSubScreen(impgenmaint *child)
{
    m_child = child;
    connect(m_child,SIGNAL(closeCalled()),this,SLOT(close()));
    ui->MainLayout->addWidget(child);
}
