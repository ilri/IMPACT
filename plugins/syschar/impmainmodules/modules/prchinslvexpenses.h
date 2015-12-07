#ifndef prchinslvexpenses_H
#define prchinslvexpenses_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class prchinslvexpenses;
}

class prchinslvexpenses : public QWidget
{
    Q_OBJECT

public:
    explicit prchinslvexpenses(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~prchinslvexpenses();
    void constructCustomDelegator();
    void loadForm();

private:
    Ui::prchinslvexpenses *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_parentModel;
    fieldInColModel *m_colModel;
    periodTableModel *m_periodModel;
    void setChildStatus(bool status);
    void setParentStatus(bool status);
    int numseasons;
    FlickCharm charmProds;
    FlickCharm charmLive;
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
    void parentViewActivated(const QModelIndex &index);
    void loadDetCalc (const QModelIndex &index);
};

#endif // prchinslvexpenses_H
