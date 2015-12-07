#ifndef FEEDMNGQTY_H
#define FEEDMNGQTY_H

#include <impgenmaint.h>

namespace Ui {
    class feedmngqty;
}

class feedmngqty : public impgenmaint
{
    Q_OBJECT

public:
    explicit feedmngqty(QWidget *parent = 0);
    ~feedmngqty();

private:
    Ui::feedmngqty *ui;

private slots:
    void on_BitBtn15_clicked();
    void on_BitBtn13_clicked();
    void on_BitBtn12_clicked();
    void on_BitBtn14_clicked();
};

#endif // FEEDMNGQTY_H
