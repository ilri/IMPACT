#include "iocover.h"
#include "ui_iocover.h"
#include <QSvgWidget>
#include <QtSvg>

ioCover::ioCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ioCover)
{
    ui->setupUi(this);
}

ioCover::~ioCover()
{
    delete ui;
}

void ioCover::resizeEvent ( QResizeEvent * /*event*/ )
{
    QString file;
    file = ":images/svg/economic.svg";

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
