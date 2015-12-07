#include "dietcover.h"
#include "ui_dietcover.h"
#include <QSvgWidget>
#include <QtSvg>

dietCover::dietCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dietCover)
{
    ui->setupUi(this);
}

dietCover::~dietCover()
{
    delete ui;
}

void dietCover::resizeEvent ( QResizeEvent * /*event*/ )
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
