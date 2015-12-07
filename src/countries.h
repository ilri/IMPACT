#ifndef COUNTRIES_H
#define COUNTRIES_H
#include <impgenmaint.h>



namespace Ui {
    class countries;
}

class countries : public impgenmaint
{
    Q_OBJECT

public:
    explicit countries(QWidget *parent = 0);
    ~countries();
    QTableView *list;

private:
    Ui::countries *ui;
    //FlickCharm charm;
};

#endif // COUNTRIES_H
