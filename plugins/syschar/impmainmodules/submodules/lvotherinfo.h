#ifndef LVOTHERINFO_H
#define LVOTHERINFO_H

#include <impgenmaint.h>

namespace Ui {
    class lvotherinfo;
}

class lvotherinfo : public impgenmaint
{
    Q_OBJECT

public:
    explicit lvotherinfo(QWidget *parent = 0);
    ~lvotherinfo();

private:
    Ui::lvotherinfo *ui;
};

#endif // LVOTHERINFO_H
