#ifndef SOLDPRODS_H
#define SOLDPRODS_H

#include "moduleclasses.h"
#include "flickcharm.h"
#include <QWidget>

namespace Ui {
    class soldprods;
}

class soldprods : public QWidget
{
    Q_OBJECT

public:
    explicit soldprods(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~soldprods();
    void constructCustomDelegator();
    void loadForm();
    bool changes;

private:
    Ui::soldprods *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    int numseasons;
    yieldTableModel *m_cpprodsModel;
    linkedTableModel *m_marksModel;
    linkedTableSortModel * m_marksSortModel;
    periodTableModel *m_periodModel;
    void setCalendarStatus(bool status);
    void setProductsStatus(bool status);
    void setMarketStatus(bool status);
    void loadProducts();
    FlickCharm charmprod;
    FlickCharm charmmarks;
    FlickCharm charmdetail;
    void loadAndroidEffects();

private slots:

    void on_BitBtn2_clicked();
    void on_BitBtn1_clicked();
    void on_BitBtn4_clicked();
    void on_BitBtn3_clicked();
    void marketDataChanged();
    void calendarDataChanged();

    void loadCalendarData(QModelIndex index);
    void loadMarketData(QModelIndex index);
    void afterApply(QList<TmoduleInsertedKeys> insertedKeys);
    void loadMarkCalc (const QModelIndex &index);
    void loadDetCalc (const QModelIndex &index);
};

#endif // SOLDPRODS_H
