#ifndef HHCONSPRODS_H
#define HHCONSPRODS_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class hhconsprods;
}

class hhconsprods : public impgenmaint
{
    Q_OBJECT

public:
    explicit hhconsprods(QWidget *parent = 0);
    ~hhconsprods();
    QTableView *list;

private:
    Ui::hhconsprods *ui;
    //FlickCharm charm;
};

#endif // HHCONSPRODS_H
