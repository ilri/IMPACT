#ifndef DIETCOVER_H
#define DIETCOVER_H

#include <QWidget>

namespace Ui {
    class dietCover;
}

class dietCover : public QWidget
{
    Q_OBJECT

public:
    explicit dietCover(QWidget *parent = 0);
    ~dietCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::dietCover *ui;
};

#endif // DIETCOVER_H
