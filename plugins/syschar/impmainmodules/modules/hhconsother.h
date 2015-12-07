#ifndef HHCONSOTHER_H
#define HHCONSOTHER_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class hhconsother;
}

class hhconsother : public QWidget
{
    Q_OBJECT

public:
    explicit hhconsother(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~hhconsother();

    void loadForm();

private:
    Ui::hhconsother *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_colModel;
    periodTableModel *m_periodModel;
    void setChildStatus(bool status);
    void setParentStatus(bool status);
    int numseasons;
    void constructCustomDelegator();
    FlickCharm charmprod;
    FlickCharm charmdetail;
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

    void loadProdCalc (const QModelIndex &index);
    void loadDetCalc (const QModelIndex &index);
    void on_pushButton_clicked();
};

#endif // HHCONSOTHER_H
