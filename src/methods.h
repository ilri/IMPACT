#ifndef METHODS_H
#define METHODS_H
#include <impgenmaint.h>
//#include "flickcharm.h"


namespace Ui {
    class methods;
}

class methods : public impgenmaint
{
    Q_OBJECT

public:
    explicit methods(QWidget *parent = 0);
    ~methods();
    QTableView *list;
    void setActivity(QString code){currAct = code;}

protected slots:
    void addNewItem();

private:
    Ui::methods *ui;
    //FlickCharm charm;
    QString currAct;
};

#endif // METHODS_H
