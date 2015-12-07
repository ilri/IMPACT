#ifndef HHCOVER_H
#define HHCOVER_H

#include <QWidget>

namespace Ui {
    class hhCover;
}

class hhCover : public QWidget
{
    Q_OBJECT

public:
    explicit hhCover(QWidget *parent = 0);
    ~hhCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::hhCover *ui;
};

#endif // HHCOVER_H
