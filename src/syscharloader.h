#ifndef SYSCHARLOADER_H
#define SYSCHARLOADER_H

#include <QWidget>
#include "imppluginloader.h"
#include "navtree.h"
#include <QMainWindow>
#include <QPointer>

namespace Ui {
    class syscharloader;
}

class syscharloader : public QMainWindow
{
    Q_OBJECT

public:
    explicit syscharloader(QWidget *parent = 0);
    ~syscharloader();
    void setPlugins(QList<impPluginInstance *> lstPlugins);
    void loadTree(QString xmlFile);
    void setDatabase(QSqlDatabase db);
private:
    Ui::syscharloader *ui;
    QList<impPluginInstance *> plugins;
    void loadModule(QString library, QString interface, QString plugin);
    QPointer<QWidget> currentWindow;
    QSqlDatabase dataBase;
    QString currentSystem;

    navtree *docNavTree;
private slots:
    void on_pushButton_clicked();
    void closeMe();
    void loadSystem(QString code, QString description, QString library, QString pluginCode);
    void clearScreen();
signals:
    void closeCalled();
};

#endif // SYSCHARLOADER_H
