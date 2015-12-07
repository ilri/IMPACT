#ifndef PLOTSGPS_H
#define PLOTSGPS_H

#include <impgenmaint.h>
#include <qsqldatabase.h>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class plotsgps;
}

class plotsgps : public impgenmaint
{
    Q_OBJECT

public:
    explicit plotsgps(QWidget *parent = 0);
    ~plotsgps();
    void setKeyValues(QList<TmoduleFieldDef> keys);
    void loadModel(QSqlDatabase dataBase);

private:
    Ui::plotsgps *ui;
    fieldInColModel *m_colModel;
    QList<TmoduleFieldDef> m_keys;
    FlickCharm charm;
    void loadAndroidEffects();
private slots:
    void valueChanged();
    void deleteItem();
    void insertItem();
    void applyChanges();
    void cancelChanges();
    void loadCalc (const QModelIndex &index);
};

#endif // PLOTSGPS_H
