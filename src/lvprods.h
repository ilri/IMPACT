#ifndef LVPRODS_H
#define LVPRODS_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class lvprods;
}

class lvprods : public impgenmaint
{
    Q_OBJECT

public:
    explicit lvprods(QWidget *parent = 0);
    ~lvprods();
    QTableView *list;

private:
    Ui::lvprods *ui;
    //FlickCharm charm;
};

#endif // LVPRODS_H
