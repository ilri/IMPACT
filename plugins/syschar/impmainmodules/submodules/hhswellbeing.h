#ifndef HHSWELLBEING_H
#define HHSWELLBEING_H

#include <impgenmaint.h>

namespace Ui {
    class hhswellbeing;
}

class hhswellbeing : public impgenmaint
{
    Q_OBJECT

public:
    explicit hhswellbeing(QWidget *parent = 0);
    ~hhswellbeing();

private:
    Ui::hhswellbeing *ui;
};

#endif // HHSWELLBEING_H
