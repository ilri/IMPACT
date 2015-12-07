#include "hhcover.h"
#include "ui_hhcover.h"
#include <QSvgWidget>
#include <QtSvg>

hhCover::hhCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hhCover)
{
    ui->setupUi(this);
}

hhCover::~hhCover()
{
    delete ui;
}

void hhCover::resizeEvent ( QResizeEvent * /*event*/ )
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
