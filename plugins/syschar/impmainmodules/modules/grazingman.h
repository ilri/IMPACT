#ifndef grazingman_H
#define grazingman_H

#include "moduleclasses.h"
#include "flickcharm.h"
#include <QWidget>

namespace Ui {
    class grazingman;
}

class grazingman : public QWidget
{
    Q_OBJECT

public:
    explicit grazingman(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~grazingman();
    void constructCustomDelegator();
    void loadForm();
    bool changes;

private:
    Ui::grazingman *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    int numseasons;
    fieldInColModel *m_paddocksModel;
    grazingTableModel *m_groupsModel;
    grazingTableSortModel * m_groupsSortModel;
    periodTableModel *m_periodDataModel;
    void setCalendarStatus(bool status);
    void setProductsStatus(bool status);
    void setMarketStatus(bool status);
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
    void loadFeedGData(QModelIndex index);
    void afterApply(QList<TmoduleInsertedKeys> insertedKeys);
    void loadDetCalc (const QModelIndex &index);
};

#endif // grazingman_H
