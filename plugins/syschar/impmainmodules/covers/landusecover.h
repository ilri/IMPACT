#ifndef LANDUSECOVER_H
#define LANDUSECOVER_H

#include <QWidget>

namespace Ui {
    class landUseCover;
}

class landUseCover : public QWidget
{
    Q_OBJECT

public:
    explicit landUseCover(QWidget *parent = 0);
    ~landUseCover();
protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::landUseCover *ui;
};

#endif // LANDUSECOVER_H
