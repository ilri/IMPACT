#ifndef CROPSALESCOVER_H
#define CROPSALESCOVER_H

#include <QWidget>

namespace Ui {
    class cropSalesCover;
}

class cropSalesCover : public QWidget
{
    Q_OBJECT

public:
    explicit cropSalesCover(QWidget *parent = 0);
    ~cropSalesCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::cropSalesCover *ui;
};

#endif // CROPSALESCOVER_H
