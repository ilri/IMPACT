#include "landusecover.h"
#include "ui_landusecover.h"
#include <QSvgWidget>
#include <QtSvg>

landUseCover::landUseCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::landUseCover)
{
    ui->setupUi(this);
}

landUseCover::~landUseCover()
{
    delete ui;
}

void landUseCover::resizeEvent ( QResizeEvent * /*event*/ )
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
