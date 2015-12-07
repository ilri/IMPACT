#include "landcover.h"
#include "ui_landcover.h"
#include <QSvgWidget>
#include <QtSvg>

landCover::landCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::landCover)
{
    ui->setupUi(this);
}

landCover::~landCover()
{
    delete ui;
}

void landCover::resizeEvent ( QResizeEvent * /*event*/ )
{
    QString file;
    file = ":images/svg/crops.svg";

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
