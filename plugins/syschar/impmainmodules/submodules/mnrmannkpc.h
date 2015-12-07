#ifndef MNRMANNKPC_H
#define MNRMANNKPC_H

#include <impgenmaint.h>

namespace Ui {
    class mnrmannkpc;
}

class mnrmannkpc : public impgenmaint
{
    Q_OBJECT

public:
    explicit mnrmannkpc(QWidget *parent = 0);
    ~mnrmannkpc();

private:
    Ui::mnrmannkpc *ui;

private slots:
    void on_BitBtn23_clicked();
    void on_BitBtn19_clicked();
    void on_BitBtn16_clicked();
};

#endif // MNRMANNKPC_H
