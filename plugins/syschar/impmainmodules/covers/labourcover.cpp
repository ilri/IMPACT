#include "labourcover.h"
#include "ui_labourcover.h"
#include <QSvgWidget>
#include <QtSvg>

labourCover::labourCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::labourCover)
{
    ui->setupUi(this);
}

labourCover::~labourCover()
{
    delete ui;
}

void labourCover::resizeEvent ( QResizeEvent * /*event*/ )
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
