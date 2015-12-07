#ifndef MANUREMAN_H
#define MANUREMAN_H

#include <QWidget>
#include "moduleclasses.h"
#include <qsqlquery.h>
#include <QSqlError>
#include "flickcharm.h"

namespace Ui {
    class manureman;
}

class manureman : public QWidget
{
    Q_OBJECT

public:
    explicit manureman(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~manureman();



private:
    Ui::manureman *ui;
    fieldInColModel *m_heapModel;
    mnrManTableModel *m_mnrprodsModel;
    QList <TmoduleFieldDef> m_keys;
    QSqlDatabase database;
    QString currentSystem;
    void setChildStatus(bool status);
    void setParentStatus(bool status);
    void constructCustomDelegator();
    FlickCharm charmUnits;
    FlickCharm charmProds;
    void loadAndroidEffects();
private slots:
    void on_BitBtn3_clicked();
    void unitsDataChanged();
    void unitsDataChanged(int row, int column);
    void productsDataChanged();
    void loadChildData(QModelIndex index);
    void loadNPKC(QModelIndex index);
    void loadYield(QModelIndex index);
    void loadYieldAlloc(QModelIndex index);
    void loadNPKCAlloc(QModelIndex index);
    void addUnit();
    void deleteUnit();
    void applyUnits();
    void cancelUnits();
    void applyProducts();
    void cancelProducts();
    void loadUnitsCalc (const QModelIndex &index);
    void loadProdsCalc (const QModelIndex &index);

};

#endif // MANUREMAN_H
