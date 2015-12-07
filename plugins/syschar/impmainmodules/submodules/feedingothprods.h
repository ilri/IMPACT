#ifndef FEEDINGOTHPRODS_H
#define FEEDINGOTHPRODS_H

#include <impgenmaint.h>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class feedingothprods;
}

class feedingothprods : public impgenmaint
{
    Q_OBJECT

public:
    explicit feedingothprods(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~feedingothprods();
    void constructCustomDelegator();
    void loadForm();
    bool changes;

private:
    Ui::feedingothprods *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    linkedTableModel *m_prodsModel;
    linkedTableSortModel * m_prodsSortModel;
    periodTableModel *m_periodModel;
    void setChildStatus(bool status);
    void setParentStatus(bool status);
    int numseasons;
    FlickCharm charmProds;
    FlickCharm charmDetail;
    void loadAndroidEffects();


private slots:
    void on_BitBtn12_clicked();
    void on_BitBtn11_clicked();
    void on_BitBtn8_clicked();
    void on_BitBtn10_clicked();
    void on_BitBtn9_clicked();
    void dataChanged();
    void childDataChanged();
    void loadChildData(QModelIndex index);
    void afterApply(QList<TmoduleInsertedKeys> insertedKeys);
    void loadDetCalc (const QModelIndex &index);
};

#endif // FEEDINGOTHPRODS_H
