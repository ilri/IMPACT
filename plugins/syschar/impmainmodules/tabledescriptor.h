#ifndef TABLEDESCRIPTOR_H
#define TABLEDESCRIPTOR_H

#include <QWidget>
#include <QSqlDatabase>
#include "moduletypes.h"

class tableDescriptor : public QObject
{
    Q_OBJECT
public:
    explicit tableDescriptor(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    QString getCustomDescription(QString table, QList<TmoduleFieldDef> keyFields);
    QString getCustomDescription(QString table, QString code, QString system);

private:
    QSqlDatabase m_database;
    QString getPlotDesc(QString sysCode, QString plotId);
    QString getRotationDesc(QString sysCode, QString plotId, QString rotationId);
    QString getPaddockDesc(QString sysCode, QString plotId, QString paddpockId);
    QString getCropDesc(QString cropId);
    QString getPastureDesc(QString pastureId);
    QString getVegeDesc(QString vegeId);

    QString getCropProdDesc(QString cropId, QString prodId);
    QString getCropForaDesc(QString cropId, QString foraId);

    QString getPastureProdDesc(QString pastureId, QString prodId);
    QString getPastureForaDesc(QString pastureId, QString foraId);

    QString getLiveDesc(QString sysCode, QString liveId);
    QString getLiveProdDesc(QString prodId);

    QString getFeedGroupDesc(QString sysCode, QString liveId, QString feedGId);
    QString getPrchFeedDesc(QString feedId);
    QString getPrchMrnProdDesc(QString sysCode, QString prodId);

    QString getActivityDesc(QString ActyId);
    QString getFertDesc(QString fertId);

};

#endif // TABLEDESCRIPTOR_H
