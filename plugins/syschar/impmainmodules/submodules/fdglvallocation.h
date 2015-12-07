#ifndef FDGLVALLOCATION_H
#define FDGLVALLOCATION_H

#include <impgenmaint.h>

namespace Ui {
    class fdglvallocation;
}

class fdglvallocation : public impgenmaint
{
    Q_OBJECT

public:
    explicit fdglvallocation(QWidget *parent = 0);
    ~fdglvallocation();

private:
    Ui::fdglvallocation *ui;
};

#endif // FDGLVALLOCATION_H
