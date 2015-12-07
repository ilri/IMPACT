#ifndef CLIMATECOVER_H
#define CLIMATECOVER_H

#include <QWidget>

namespace Ui {
    class climateCover;
}

class climateCover : public QWidget
{
    Q_OBJECT

public:
    explicit climateCover(QWidget *parent = 0);
    ~climateCover();

protected:
    void resizeEvent ( QResizeEvent * event );

private:
    Ui::climateCover *ui;
};

#endif // CLIMATECOVER_H
