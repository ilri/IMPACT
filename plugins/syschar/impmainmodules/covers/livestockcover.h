#ifndef LIVESTOCKCOVER_H
#define LIVESTOCKCOVER_H

#include <QWidget>

namespace Ui {
    class livestockCover;
}

class livestockCover : public QWidget
{
    Q_OBJECT

public:
    explicit livestockCover(QWidget *parent = 0);
    ~livestockCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::livestockCover *ui;
};

#endif // LIVESTOCKCOVER_H
