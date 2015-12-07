#ifndef hhconscrops_H
#define hhconscrops_H

#include "moduleclasses.h"
#include "flickcharm.h"
#include <QWidget>

namespace Ui {
    class hhconscrops;
}

class hhconscrops : public QWidget
{
    Q_OBJECT

public:
    explicit hhconscrops(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~hhconscrops();


    void loadForm();
    bool changes;

private:
    Ui::hhconscrops *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    int numseasons;
    yieldTableModel *m_cpprodsModel;

    periodTableModel *m_periodModel;
    void setCalendarStatus(bool status);
    void setProductsStatus(bool status);
    void loadProducts();
    void constructCustomDelegator();
    FlickCharm charmprod;
    FlickCharm charmdetail;
    void loadAndroidEffects();


private slots:

    void on_BitBtn2_clicked();
    void on_BitBtn1_clicked();
    void on_BitBtn4_clicked();
    void on_BitBtn3_clicked();    
    void calendarDataChanged();
    void parentDataChanged();
    void loadCalendarData(QModelIndex index);
    void loadProdCalc (const QModelIndex &index);
    void loadDetCalc (const QModelIndex &index);
    void on_pushButton_clicked();
};

#endif // hhconscrops_H
