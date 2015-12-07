#ifndef MODULETREE_H
#define MODULETREE_H

#include <QTreeWidget>
#include <QSqlDatabase>
#include "imppluginloader.h"

class moduletree : public QTreeWidget
{
    Q_OBJECT
public:
    moduletree(QWidget *parent = 0);
    int cntModules;
    void loadTree(QSqlDatabase db);
    void setPlugins(QList<impPluginInstance *> lstPlugins);
    void createStandardTree();

protected:
    void dragEnterEvent (QDragEnterEvent * event );
    void dragMoveEvent (QDragMoveEvent * event);
    void dropEvent (QDropEvent * event);

private:
    QList<impPluginInstance *> plugins;
    QString getModuleCode(int cnt);
    QList<QTreeWidgetItem *> items;
    int insertNodes(int parent, QString description, QString library, QString pluginCode);
    int insertNodes(int parent, QString description, QString library, QString pluginCode, QString code, QString parentCode, int order);
    QString getModuleDescription(QString library, QString interface, QString plugin);

signals:
    void treeChanged();

};

#endif // MODULETREE_H
