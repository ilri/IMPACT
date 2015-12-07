#ifndef STALLFEEDING_H
#define STALLFEEDING_H

#include <QWidget>
#include "moduleclasses.h"
#include <qsqlquery.h>
#include <QSqlError>
#include "flickcharm.h"

namespace Ui {
    class stallfeeding;
}

class stallfeeding : public QWidget
{
    Q_OBJECT

public:
    explicit stallfeeding(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~stallfeeding();
    void constructCustomDelegator();
private:
    Ui::stallfeeding *ui;
    fieldInColModel *m_feedGroupsModel;
    feedingTableModel *m_feedingModel;
    QList <TmoduleFieldDef> m_keys;
    QSqlDatabase database;
    QString currentSystem;
    void setParentStatus(bool status);
    int numseasons;
    QAction *showYieldAct;
    QString yieldTable;
    int yieldPeriod;
    QList<TmoduleFieldDef> subModuleKeyFields;
    FlickCharm charmUnits;
    FlickCharm charmProds;
    void loadAndroidEffects();
private slots:
    void loadOtherProducts();
    void productsDataChanged();
    void loadChildData(QModelIndex index);
    void setChildStatus(bool status);
    void applyProducts();
    void cancelProducts();
    void requestPopUpMenu(QPoint pos,QModelIndex index);
    void loadYield();

    void on_cmddetails_clicked();
};

#endif // STALLFEEDING_H
