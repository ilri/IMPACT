#include "prchinscover.h"
#include "ui_prchinscover.h"
#include <QSvgWidget>
#include <QtSvg>

prchInsCover::prchInsCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::prchInsCover)
{
    ui->setupUi(this);
}

prchInsCover::~prchInsCover()
{
    delete ui;
}

void prchInsCover::resizeEvent ( QResizeEvent * /*event*/ )
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
