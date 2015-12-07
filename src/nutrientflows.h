#ifndef NUTRIENTFLOWS_H
#define NUTRIENTFLOWS_H

#include <QWidget>

namespace Ui {
    class nutrientflows;
}

class nutrientflows : public QWidget
{
    Q_OBJECT

public:
    explicit nutrientflows(QWidget *parent = 0);
    ~nutrientflows();

private:
    Ui::nutrientflows *ui;
};

#endif // NUTRIENTFLOWS_H
