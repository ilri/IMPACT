#ifndef PRCHFEEDS_H
#define PRCHFEEDS_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class prchfeeds;
}

class prchfeeds : public impgenmaint
{
    Q_OBJECT

public:
    explicit prchfeeds(QWidget *parent = 0);
    ~prchfeeds();
    QTableView *list;

private:
    Ui::prchfeeds *ui;
    //FlickCharm charm;
};

#endif // PRCHFEEDS_H
