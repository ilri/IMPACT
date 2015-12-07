#ifndef LIVEACTCOVER_H
#define LIVEACTCOVER_H

#include <QWidget>

namespace Ui {
    class liveActCover;
}

class liveActCover : public QWidget
{
    Q_OBJECT

public:
    explicit liveActCover(QWidget *parent = 0);
    ~liveActCover();

protected:
    void resizeEvent ( QResizeEvent *event);

private:
    Ui::liveActCover *ui;
};

#endif // LIVEACTCOVER_H
