#ifndef IMPPLUGINLOADER_H
#define IMPPLUGINLOADER_H


#include "plugins/interfaces/impinterface.h"

class impPlugin : public QObject
{
public:
    impPlugin(QString name);
    QString pluginName;
};

class impPluginInstance : public QObject
{
public:
    impPluginInstance(QObject *instanceRef, QString library);
    ~impPluginInstance();
    QObject *getInstance() {return instance;}
    QString getInstanceName() {return instanceName;}
    QString getLibraryName() {return libraryName;}
    int numPlugins() {return plugins.count();}
    QList<impPlugin *> getPlugins(){return plugins;}

private:
    QObject *instance;
    QString instanceName;
    QString libraryName;
    QList<impPlugin *> plugins;
};

#endif // IMPPLUGINLOADER_H
