#ifndef HOUSEHOLDCOVER_H
#define HOUSEHOLDCOVER_H

#include <QWidget>

namespace Ui {
    class householdCover;
}

class householdCover : public QWidget
{
    Q_OBJECT

public:
    explicit householdCover(QWidget *parent = 0);
    ~householdCover();

protected:
    void resizeEvent ( QResizeEvent * event );

private:
    Ui::householdCover *ui;
};

#endif // HOUSEHOLDCOVER_H
