#include "landmanagementcover.h"
#include "ui_landmanagementcover.h"
#include <QSvgWidget>
#include <QtSvg>

landManagementCover::landManagementCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::landManagementCover)
{
    ui->setupUi(this);
}

landManagementCover::~landManagementCover()
{
    delete ui;
}

void landManagementCover::resizeEvent ( QResizeEvent * /*event*/ )
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
