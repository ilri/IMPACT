#ifndef OTHEREXPENSES_H
#define OTHEREXPENSES_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class otherexpenses;
}

class otherexpenses : public QWidget
{
    Q_OBJECT

public:
    explicit otherexpenses(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~otherexpenses();
    void constructCustomDelegator();
    void loadForm();

private:
    Ui::otherexpenses *ui;
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

#endif // OTHEREXPENSES_H
