#ifndef PLOTSOTHERINFO_H
#define PLOTSOTHERINFO_H

#include <impgenmaint.h>

namespace Ui {
    class plotsotherinfo;
}

class plotsotherinfo : public impgenmaint
{
    Q_OBJECT

public:
    explicit plotsotherinfo(QWidget *parent = 0);
    ~plotsotherinfo();

private:
    Ui::plotsotherinfo *ui;
};

#endif // PLOTSOTHERINFO_H
