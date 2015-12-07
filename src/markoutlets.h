#ifndef MARKOUTLETS_H
#define MARKOUTLETS_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class markoutlets;
}

class markoutlets : public impgenmaint
{
    Q_OBJECT

public:
    explicit markoutlets(QWidget *parent = 0);
    ~markoutlets();
    QTableView *list;

private:
    Ui::markoutlets *ui;
    //FlickCharm charm;
};

#endif // MARKOUTLETS_H
