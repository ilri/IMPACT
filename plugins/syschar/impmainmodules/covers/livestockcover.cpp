#include "livestockcover.h"
#include "ui_livestockcover.h"
#include <QSvgWidget>
#include <QtSvg>

livestockCover::livestockCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::livestockCover)
{
    ui->setupUi(this);
}

livestockCover::~livestockCover()
{
    delete ui;
}

void livestockCover::resizeEvent ( QResizeEvent * /*event*/ )
{
    QString file;
    file = ":images/svg/livestock.svg";

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
