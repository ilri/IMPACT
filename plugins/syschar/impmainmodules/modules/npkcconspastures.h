#ifndef NPKCConsPastures_H
#define NPKCConsPastures_H

#include <QWidget>
#include <QSqlDatabase>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class NPKCConsPastures;
}

class NPKCConsPastures : public QWidget
{
    Q_OBJECT

public:
    explicit NPKCConsPastures(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~NPKCConsPastures();


private:
    Ui::NPKCConsPastures *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    yieldTableModel *m_cpprodsModel;
    int numseasons;
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

#endif // NPKCConsPastures_H
