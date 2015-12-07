#ifndef grassplots_H
#define grassplots_H

#include <QWidget>
#include <QSqlDatabase>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class grassplots;
}

class grassplots : public QWidget
{
    Q_OBJECT

public:
    explicit grassplots(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~grassplots();



private:
    Ui::grassplots *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_plotsModel;
    fieldInColModel *m_seasonsModel;
    cropsTableModel *m_calendarModel;
    cropsTableSortModel *m_calendarSortModel;
    void loadForm();
    void constructCustomDelegator();
    FlickCharm charmprod;
    FlickCharm charmmarks;
    void loadAndroidEffects();
private slots:

    void loadPaddocks(QModelIndex index);
    void loadExtra(QModelIndex index);

    void on_BitBtn2_clicked();
    void on_BitBtn3_clicked();
    void afterDelete(QList<TmoduleInsertedKeys> keys);
    void on_BitBtn4_clicked();
    void on_BitBtn6_clicked();

};

#endif // grassplots_H
