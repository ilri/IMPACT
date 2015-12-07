#ifndef PRJSETTINGS_H
#define PRJSETTINGS_H

#include <QWidget>
#include "imppluginloader.h"
#include "plugintree.h"
#include "moduletree.h"
#include <QAbstractItemDelegate>
#include "genmodels.h"

namespace Ui {
    class prjsettings;
}

class prjsettings : public QWidget
{
    Q_OBJECT

public:
    explicit prjsettings(QWidget *parent = 0);
    ~prjsettings();
    void setPlugins(QList<impPluginInstance *> lstPlugins);
    void setDatabase(QSqlDatabase db);

private:
    Ui::prjsettings *ui;
    QList<QTreeWidgetItem *> plugitems;
    int insertPluginItem(int parent, QString library, QString plugin, QString pluginDescription);
    int getLibraryIndex(QString library);
    plugintree *m_plugintree;
    moduletree *m_moduletree;
    QSqlDatabase dataBase;
    genericFldModel *m_colModel;
    bool applyTree;
    bool applyCalendar;
    bool applyForm;
    void doApplyTree();
    void doApplyForm();
    void loadData();

signals:
    void closeCalled();
    void changeName(QString name);

private slots:
    void on_cmdapply_clicked();
    void on_pushButton_7_clicked();
    void treeChanged();
    void on_pushButton_clicked();
    void valueHasChanged(QString table,QString field,QString value);
    void modeldDataChanged();
    void on_cmdgenerate_clicked();
    void on_pushButton_2_clicked();
};

#endif // PRJSETTINGS_H
