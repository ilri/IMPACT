#include "otherinscover.h"
#include "ui_otherinscover.h"
#include <QSvgWidget>
#include <QtSvg>

otherInsCover::otherInsCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::otherInsCover)
{
    ui->setupUi(this);
}

otherInsCover::~otherInsCover()
{
    delete ui;
}

void otherInsCover::resizeEvent ( QResizeEvent * /*event*/ )
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
