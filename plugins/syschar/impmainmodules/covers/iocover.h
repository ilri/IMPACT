#ifndef IOCOVER_H
#define IOCOVER_H

#include <QWidget>

namespace Ui {
    class ioCover;
}

class ioCover : public QWidget
{
    Q_OBJECT

public:
    explicit ioCover(QWidget *parent = 0);
    ~ioCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::ioCover *ui;
};

#endif // IOCOVER_H
