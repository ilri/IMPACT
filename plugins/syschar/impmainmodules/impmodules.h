#ifndef IMPMODULES_H
#define IMPMODULES_H

#include <impinterface.h>
#include <QSqlDatabase>

class impMainModules : public QObject, public impInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ILRI.IMPACT.mainModules")
    Q_INTERFACES(impInterface)
public:
    QStringList pluginNames();
    QString getPluginDescription(QString pluginName);
    QWidget* loadScreen(QString luginName, QSqlDatabase dataBase, QString systemID);
};

#endif // IMPMODULES_H
