#ifndef LANDMANAGEMENTCOVER_H
#define LANDMANAGEMENTCOVER_H

#include <QWidget>

namespace Ui {
    class landManagementCover;
}

class landManagementCover : public QWidget
{
    Q_OBJECT

public:
    explicit landManagementCover(QWidget *parent = 0);
    ~landManagementCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::landManagementCover *ui;
};

#endif // LANDMANAGEMENTCOVER_H
