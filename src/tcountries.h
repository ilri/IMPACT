#ifndef COUNTRIES_H
#define COUNTRIES_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtGui>
#include <QStringList>
#include "maintclases.h"
#include <impedit.h>



namespace Ui {
    class countries;
}

class countries : public QWidget
{
    Q_OBJECT

public:
    explicit countries(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase());
    ~countries();

private:
    Ui::countries *ui;
    QSqlDatabase database;    
    maintModel *m_mainmodel;
    QString tableName;
    QString keyField;
    QString displayColumn;
    QString displayColumnDescription;
    QStringList fields;
    void goToFirst();

private slots:
    void addNewItem();
    void removeItem();
    void applyChanges();
    void cancelChanges();
    void closeForm();
    void itemSelected(const QModelIndex & index);
    void valueHasChanged(QString table, QString field, QString value);
protected:
    void resizeEvent ( QResizeEvent * event );
signals:
    void closeMe();
};

#endif // COUNTRIES_H
