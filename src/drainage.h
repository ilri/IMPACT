#ifndef DRAINAGE_H
#define DRAINAGE_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class drainage;
}

class drainage : public impgenmaint
{
    Q_OBJECT

public:
    explicit drainage(QWidget *parent = 0);
    ~drainage();
    QTableView *list;

private:
    Ui::drainage *ui;
    //FlickCharm charm;
};

#endif // DRAINAGE_H
