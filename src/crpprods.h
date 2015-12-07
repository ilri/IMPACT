#ifndef CRPPRODS_H
#define CRPPRODS_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class crpprods;
}

class crpprods : public impgenmaint
{
    Q_OBJECT

public:
    explicit crpprods(QWidget *parent = 0);
    ~crpprods();
    QTableView *list;

private:
    Ui::crpprods *ui;
    //FlickCharm charm;
};

#endif // CRPPRODS_H
