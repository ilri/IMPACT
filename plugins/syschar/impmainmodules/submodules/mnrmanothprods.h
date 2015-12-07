#ifndef MNRMANOTHPRODS_H
#define MNRMANOTHPRODS_H

#include <impgenmaint.h>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class mnrmanothprods;
}

class mnrmanothprods : public impgenmaint
{
    Q_OBJECT

public:
    explicit mnrmanothprods(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~mnrmanothprods();
    void constructCustomDelegator();
    void loadForm();
    bool changes;

private:
    Ui::mnrmanothprods *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_colModel;
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
    void on_BitBtn15_clicked();
    void on_BitBtn14_clicked();
    void dataChanged();
    void childDataChanged();
    void loadChildData(QModelIndex index);
    void afterApply(QList<TmoduleInsertedKeys> insertedKeys);
    void loadDetCalc (const QModelIndex &index);
};

#endif // MNRMANOTHPRODS_H
