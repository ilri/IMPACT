#include "cropsalescover.h"
#include "ui_cropsalescover.h"
#include <QSvgWidget>
#include <QtSvg>

cropSalesCover::cropSalesCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cropSalesCover)
{
    ui->setupUi(this);
}

cropSalesCover::~cropSalesCover()
{
    delete ui;
}

void cropSalesCover::resizeEvent ( QResizeEvent * /*event*/ )
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
