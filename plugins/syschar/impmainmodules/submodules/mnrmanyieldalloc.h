#ifndef MNRMANYIELDALLOC_H
#define MNRMANYIELDALLOC_H

#include <impgenmaint.h>

namespace Ui {
    class mnrmanyieldalloc;
}

class mnrmanyieldalloc : public impgenmaint
{
    Q_OBJECT

public:
    explicit mnrmanyieldalloc(QWidget *parent = 0);
    ~mnrmanyieldalloc();

private:
    Ui::mnrmanyieldalloc *ui;

private slots:
    void on_BitBtn21_clicked();
    void on_BitBtn17_clicked();
};

#endif // MNRMANYIELDALLOC_H
