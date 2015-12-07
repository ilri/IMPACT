#include "liveactcover.h"
#include "ui_liveactcover.h"
#include <QSvgWidget>
#include <QtSvg>

liveActCover::liveActCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::liveActCover)
{
    ui->setupUi(this);
}

liveActCover::~liveActCover()
{
    delete ui;
}

void liveActCover::resizeEvent ( QResizeEvent * /*event*/ )
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
