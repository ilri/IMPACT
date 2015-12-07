#ifndef prchinscrops_H
#define prchinscrops_H

#include "moduleclasses.h"
#include "flickcharm.h"
#include <QWidget>

namespace Ui {
    class prchinscrops;
}

class prchinscrops : public QWidget
{
    Q_OBJECT

public:
    explicit prchinscrops(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~prchinscrops();
    void constructCustomDelegator();
    void loadForm();
    bool changes;

private:
    Ui::prchinscrops *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    inputsTableModel *m_cpprodsModel;
    periodTableModel *m_periodModel;
    void setCalendarStatus(bool status);
    void setProductsStatus(bool status);
    void loadProducts();
    FlickCharm charmProds;
    FlickCharm charmDetail;
    void loadAndroidEffects();
private slots:
    void on_BitBtn2_clicked();
    void on_BitBtn1_clicked();
    void calendarDataChanged();
    void loadCalendarData(QModelIndex index);
    void loadDetCalc (const QModelIndex &index);
};

#endif // prchinscrops_H
