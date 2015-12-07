#include "climatecover.h"
#include "ui_climatecover.h"
#include <QSvgWidget>
#include <QtSvg>
#include <QResizeEvent>

climateCover::climateCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::climateCover)
{
    ui->setupUi(this);
}

climateCover::~climateCover()
{
    delete ui;
}


void climateCover::resizeEvent ( QResizeEvent * /*event*/ )
{
    QString file;
    file = ":images/svg/climate.svg";

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
