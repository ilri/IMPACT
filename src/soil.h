#ifndef SOIL_H
#define SOIL_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class soil;
}

class soil : public impgenmaint
{
    Q_OBJECT

public:
    explicit soil(QWidget *parent = 0);
    ~soil();
    QTableView *list;

private:
    Ui::soil *ui;
    //FlickCharm charm;
};

#endif // SOIL_H
