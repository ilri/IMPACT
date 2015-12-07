#ifndef PRCHINSCOVER_H
#define PRCHINSCOVER_H

#include <QWidget>

namespace Ui {
    class prchInsCover;
}

class prchInsCover : public QWidget
{
    Q_OBJECT

public:
    explicit prchInsCover(QWidget *parent = 0);
    ~prchInsCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::prchInsCover *ui;
};

#endif // PRCHINSCOVER_H
