#ifndef LABOURCOVER_H
#define LABOURCOVER_H

#include <QWidget>

namespace Ui {
    class labourCover;
}

class labourCover : public QWidget
{
    Q_OBJECT

public:
    explicit labourCover(QWidget *parent = 0);
    ~labourCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::labourCover *ui;
};

#endif // LABOURCOVER_H
