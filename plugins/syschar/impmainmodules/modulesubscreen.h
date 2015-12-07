#ifndef MODULESUBSCREEN_H
#define MODULESUBSCREEN_H

#include <QDialog>
#include <QSqlDatabase>
#include "moduleclasses.h"
#include <impgenmaint.h>

namespace Ui {
    class moduleSubScreen;
}

class moduleSubScreen : public QDialog
{
    Q_OBJECT

public:
    explicit moduleSubScreen(QWidget *parent = 0);
    ~moduleSubScreen();
    void loadSubScreen(impgenmaint *child);
private:
    Ui::moduleSubScreen *ui;
    impgenmaint *m_child;
};

#endif // MODULESUBSCREEN_H
