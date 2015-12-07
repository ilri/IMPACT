#ifndef OTHERINSCOVER_H
#define OTHERINSCOVER_H

#include <QWidget>

namespace Ui {
    class otherInsCover;
}

class otherInsCover : public QWidget
{
    Q_OBJECT

public:
    explicit otherInsCover(QWidget *parent = 0);
    ~otherInsCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::otherInsCover *ui;
};

#endif // OTHERINSCOVER_H
