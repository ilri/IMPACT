#ifndef NPKCConsLive_H
#define NPKCConsLive_H

#include <QWidget>
#include <QSqlDatabase>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class NPKCConsLive;
}

class NPKCConsLive : public QWidget
{
    Q_OBJECT

public:
    explicit NPKCConsLive(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~NPKCConsLive();

private:
    Ui::NPKCConsLive *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    yieldTableModel *m_cpprodsModel;
    void loadForm();
    void loadProducts();
    FlickCharm charm;
    void loadAndroidEffects();
private slots:
    void valueChanged();
    void on_cmdapply_clicked();
    void on_cmdcancel_clicked();
    void loadCalc (const QModelIndex &index);
    void on_cmdgetvalues_clicked();
};

#endif // NPKCConsLive_H
