#ifndef HOUSESIZE_H
#define HOUSESIZE_H

#include <QWidget>
#include <QSqlDatabase>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class housesize;
}

class housesize : public QWidget
{
    Q_OBJECT

public:
    explicit housesize(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~housesize();



private:
    Ui::housesize *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_colModel;
    int numseasons;
    void loadForm();
    void constructCustomDelegator();
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
    void loadExtra(QModelIndex index);
    void afterApply(QList<TmoduleInsertedKeys> insertedKeys);
    void loadCalc (const QModelIndex &index);

};

#endif // HOUSESIZE_H
