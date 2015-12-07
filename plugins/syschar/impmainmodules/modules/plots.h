#ifndef PLOTS_H
#define PLOTS_H

#include <QWidget>
#include <QSqlDatabase>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class plots;
}

class plots : public QWidget
{
    Q_OBJECT

public:
    explicit plots(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~plots();
   void constructCustomDelegator();


private:
    Ui::plots *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_colModel;
    void loadForm();
    FlickCharm charm;
    void loadAndroidEffects();
private slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void valueChanged();
    void deleteItem();
    void insertItem();
    void applyChanges();
    void cancelChanges();
    void valueInserted(int row);
    void loadSoil(QModelIndex index);
    void loadGPS(QModelIndex index);
    void loadExtra(QModelIndex index);
    void loadCalc (const QModelIndex &index);
};

#endif // PLOTS_H
