#include "nkpccover.h"
#include "ui_nkpccover.h"
#include <QSvgWidget>
#include <QtSvg>

NKPCCover::NKPCCover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NKPCCover)
{
    ui->setupUi(this);
}

NKPCCover::~NKPCCover()
{
    delete ui;
}

void NKPCCover::resizeEvent ( QResizeEvent * /*event*/ )
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
