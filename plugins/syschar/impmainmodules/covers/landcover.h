#ifndef LANDCOVER_H
#define LANDCOVER_H

#include <QWidget>

namespace Ui {
    class landCover;
}

class landCover : public QWidget
{
    Q_OBJECT

public:
    explicit landCover(QWidget *parent = 0);
    ~landCover();
protected:
    void resizeEvent ( QResizeEvent *event );

private:
    Ui::landCover *ui;
};

#endif // LANDCOVER_H
