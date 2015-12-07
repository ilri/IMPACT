#include "householdcover.h"
#include "ui_householdcover.h"
#include <QSvgWidget>
#include <QtSvg>

householdCover::householdCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::householdCover)
{
    ui->setupUi(this);
}

householdCover::~householdCover()
{
    delete ui;
}

void householdCover::resizeEvent ( QResizeEvent * /*event*/ )
{
    QString file;
    file = ":images/svg/human.svg";

    QSvgRenderer svgfile(file,0);
    QImage img(this->size().width(), this->size().height(), QImage::Format_ARGB32);
    img.fill(0);
    QPainter painter(&img);
    svgfile.render(&painter);

    QPalette palette;
    setAutoFillBackground(true);
    palette.setBrush(QPalette::Window,QPixmap::fromImage(img));

    setPalette(palette);

}
