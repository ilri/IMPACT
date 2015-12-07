#ifndef FERTILISERS_H
#define FERTILISERS_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class fertilisers;
}

class fertilisers : public impgenmaint
{
    Q_OBJECT

public:
    explicit fertilisers(QWidget *parent = 0);
    ~fertilisers();
    QTableView *list;

private:
    Ui::fertilisers *ui;
    //FlickCharm charm;
};

#endif // FERTILISERS_H
