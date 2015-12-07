#ifndef UNITS_H
#define UNITS_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class units;
}

class units : public impgenmaint
{
    Q_OBJECT

public:
    explicit units(QWidget *parent = 0);
    ~units();
    QTableView *list;

private:
    Ui::units *ui;
    //FlickCharm charm;
};

#endif // UNITS_H
