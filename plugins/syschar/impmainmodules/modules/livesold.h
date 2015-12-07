#ifndef livesold_H
#define livesold_H

#include "moduleclasses.h"
#include "flickcharm.h"
#include <QWidget>

namespace Ui {
    class livesold;
}

class livesold : public QWidget
{
    Q_OBJECT

public:
    explicit livesold(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~livesold();

    void loadForm();
    bool changes;

private:
    Ui::livesold *ui;
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
    void constructCustomDelegator();
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

#endif // livesold_H
