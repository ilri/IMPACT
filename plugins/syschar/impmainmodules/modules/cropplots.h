#ifndef cropplots_H
#define cropplots_H

#include <QWidget>
#include <QSqlDatabase>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class cropplots;
}

class cropplots : public QWidget
{
    Q_OBJECT

public:
    explicit cropplots(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~cropplots();

private:
    Ui::cropplots *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_plotsModel;
    fieldInColModel *m_seasonsModel;
    cropsTableModel *m_calendarModel;
    cropsTableSortModel *m_calendarSortModel;
    void loadForm();
    int nperiods;
    FlickCharm plotcharm;
    FlickCharm seasoncharm;
    FlickCharm cropcharm;
    void loadAndroidEffects();


private slots:
    void seasonsChanged();
    void loadSeasons(QModelIndex index);
    void loadCalendar(QModelIndex index);
    QString getSeasonType(QList<TmoduleFieldDef> keys);



    void on_pushButton_clicked();
    void on_BitBtn2_clicked();
    void on_BitBtn3_clicked();
    void on_BitBtn4_clicked();
    void on_BitBtn6_clicked();
    void on_BitBtn10_clicked();
    void on_BitBtn11_clicked();
    void loadCalculator (const QModelIndex &index);
};

#endif // cropplots_H
