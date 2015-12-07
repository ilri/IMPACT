#ifndef IMPINTERFACE_H
#define IMPINTERFACE_H
#include <QSqlDatabase>
#include <QWidget>

class impInterface
 {
 public:
     virtual ~impInterface(){}
     virtual QStringList pluginNames() = 0;
     virtual QString getPluginDescription(QString pluginName) = 0;
     virtual QWidget* loadScreen(QString pluginName,QSqlDatabase dataBase, QString systemID) = 0;
 };

Q_DECLARE_INTERFACE(impInterface,"com.ILRI.IMPACT.impInterface/1.0")

#endif // IMPINTERFACE_H
