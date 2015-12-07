#ifndef NAVTREE_H
#define NAVTREE_H

#include <QDockWidget>
#include <QTreeWidgetItem>
#include "imppluginloader.h"
#include "impclasses.h"
//#include "flickcharm.h"

namespace Ui {
    class navtree;
}

class navtree : public QDockWidget
{
    Q_OBJECT

public:
    explicit navtree(QWidget *parent = 0);
    ~navtree();
    void setPlugins(QList<impPluginInstance *> lstPlugins);
    void loadTree(QSqlDatabase db);
    void loadSystems(QSqlDatabase dataBase);

private:
    Ui::navtree *ui;
    QList<QTreeWidgetItem *> items; //List of items in the table tree. Because is declared here we the see the list from any function of mainWindow
    QList<impPluginInstance *> plugins;
    int insertNodes(int parent, QString description, QString library, QString pluginCode);
    QString getModuleDescription(QString library, QString interface, QString plugin);
    QString currentSystem;
    QString currentSystemDesc;
    QString currentLibrary;
    QString currentPlugin;
    systemsTableModel *sysmodel;
    QSqlDatabase m_database;
    //FlickCharm charm1;
    //FlickCharm charm2;
private slots:
    void systemSelected(QModelIndex index);
    void pluginSelected(QModelIndex index);

    void on_cmdedit_clicked();

    void on_cmdnew_clicked();

    void on_pushButton_clicked();

signals:
    void loadSystem(QString code, QString description, QString library, QString pluginCode);
    void clearScreen();
};

#endif // NAVTREE_H
