#ifndef NKPCCOVER_H
#define NKPCCOVER_H

#include <QWidget>

namespace Ui {
    class NKPCCover;
}

class NKPCCover : public QWidget
{
    Q_OBJECT

public:
    explicit NKPCCover(QWidget *parent = 0);
    ~NKPCCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::NKPCCover *ui;
};

#endif // NKPCCOVER_H
