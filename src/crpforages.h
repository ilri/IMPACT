#ifndef CRPFORAGES_H
#define CRPFORAGES_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class crpforages;
}

class crpforages : public impgenmaint
{
    Q_OBJECT

public:
    explicit crpforages(QWidget *parent = 0);
    ~crpforages();
    QTableView *list;

private:
    Ui::crpforages *ui;
    //FlickCharm charm;
};

#endif // CRPFORAGES_H
