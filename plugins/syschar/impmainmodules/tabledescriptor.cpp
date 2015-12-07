#include "tabledescriptor.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

tableDescriptor::tableDescriptor(QObject *parent, QSqlDatabase db) :
    QObject(parent)
{
    m_database = db;
}

QString tableDescriptor::getCustomDescription(QString /*table*/, QList<TmoduleFieldDef> keyFields)
{
    int pos;

    QString res;
    QVector<QString> keys(13);

    for (pos = 0; pos <= 12; pos++)
        keys[pos] = "";

    for (pos = 0; pos <= keyFields.count()-1;pos++)
    {
        if (keyFields[pos].code.toUpper() == "SYS_COD")
        {
            keys[0] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "PLOT_COD")
        {
            keys[1] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "NUMSEASON")
        {
            keys[2] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "NUMPADD")
        {
            keys[3] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "CROP_COD")
        {
            keys[4] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "VEG_COD")
        {
            keys[5] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "PROD_COD")
        {
            keys[6] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "FORA_COD")
        {
            keys[7] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "GROUP_COD")
        {
            keys[8] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "LIVE_ICOD")
        {
            keys[9] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "FEED_COD")
        {
            keys[10] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "ACTY_COD")
        {
            keys[11] = keyFields[pos].value.toString();
        }
        if (keyFields[pos].code.toUpper() == "FERTYP_COD")
        {
            keys[12] = keyFields[pos].value.toString();
        }
    }
    if (!keys[1].isEmpty())
    {
        res = "PLT (" + getPlotDesc(keys[0],keys[1]) + ")";
        if (!keys[2].isEmpty())
        {
            res = res + " - ROT (" + getRotationDesc(keys[0],keys[1],keys[2]) + ")";
            res = res + " - CRP (" + getCropDesc(keys[4]) + ")";
            if (!keys[5].isEmpty())
                res = res + " - VEG (" + getCropDesc(keys[5]) + ")";
            if (!keys[6].isEmpty())
                res = res + " - PRD (" + getCropProdDesc("",keys[6]) + ")";
            else
            {
                if (!keys[7].isEmpty())
                    res = res + " - FOR (" + getCropForaDesc("",keys[7]) + ")";
                else
                {
                    res = res + " - ACT (" + getActivityDesc(keys[11]) + ")";
                    if (!keys[12].isEmpty())
                        res = res + " - FRT (" + getFertDesc(keys[12]) + ")";
                }
            }
        }
        else
        {
            res = res + " - PDK (" + getPaddockDesc(keys[0],keys[1],keys[3]) + ")";
            res = res + " - PST (" + getPastureDesc(keys[4]) + ")";
            if (!keys[6].isEmpty())
            {
                res = res + " - PRD (" + getCropProdDesc("",keys[6]) + ")";
            }
            else
            {
                if (!keys[7].isEmpty())
                    res = res + " - FOR (" + getCropForaDesc("",keys[7]) + ")";
                else
                {
                    res = res + " - ACT (" + getActivityDesc(keys[11]) + ")";
                    if (!keys[12].isEmpty())
                        res = res + " - FRT (" + getFertDesc(keys[12]) + ")";
                }
            }
        }
    }
    else
    {
        if (!keys[9].isEmpty())
        {
            res = "LVS (" + getLiveDesc(keys[0],keys[9]) + ")";
            if (!keys[8].isEmpty())
                res = res + " - GRP (" + getFeedGroupDesc(keys[0],keys[9],keys[8]) + ")";
            if (!keys[6].isEmpty())
                res = res + " - PRD (" + getLiveProdDesc(keys[6]) + ")";
        }
        else
        {
            if (!keys[6].isEmpty())
                res = "HIP (" + getPrchMrnProdDesc(keys[0],keys[6]) + ")";
            else
                res = "PRF (" + getPrchFeedDesc(keys[10]) + ")";
        }
    }
    return res;


}

QString tableDescriptor::getCustomDescription(QString table, QString code, QString system)
{
    QString res;
    if (table == "labforce")
    {
        QString sql;
        QSqlQuery query(m_database);
        sql = "SELECT pers_typ,age,sex,SHORTDESC FROM labforce WHERE sys_cod = '" + system + "'";
        sql = sql + " AND PERS_COD = '" + code + "'";
        if (query.exec(sql))
        {
            if (query.first())
            {
                if (query.value(0).toString() == "HH")
                {
                    res = query.value(3).toString();
                    res = res + " - Age: " + query.value(1).toString();
                    if (query.value(2).toString() == "M")
                        res = res + " - Male";
                    else
                        res = res + " - Female";
                    return res;
                }
                else
                {
                    return "Hired labour";
                }
            }
            else
            {
                qWarning() << "Error: Descriptor has no data!";
            }
        }
        else
        {
            qWarning() << "Error: " << query.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
    if (table == "mnrheap")
    {
        return "Manure heap - " + code;
    }
    res = table + " : " + code;
    return res;
}

QString tableDescriptor::getPlotDesc(QString sysCode, QString plotId)
{
    QString res;
    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT plot_id,area FROM spadstown";
    sql = sql + " WHERE sys_cod = '" + sysCode + "'";
    sql = sql + " AND plot_cod = '" + plotId + "'";

    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString() + " " + lktable.value(1).toString() + " ha.";
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    return res;
}

QString tableDescriptor::getRotationDesc(QString sysCode, QString plotId, QString rotationId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT numseason,inter FROM plotseasons";
    sql = sql + " WHERE sys_cod = '" + sysCode + "'";
    sql = sql + " AND plot_cod = '" + plotId + "'";
    sql = sql + " AND numseason = '" + rotationId + "'";

    if (lktable.exec(sql))
    {
        lktable.first();
        if ((lktable.value(1).toString() == "Y") || (lktable.value(1).toString() == "Y"))
            res = lktable.value(0).toString() + " intercroping";
        else
            res = lktable.value(0).toString() + " Single crop";
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getPaddockDesc(QString sysCode, QString plotId, QString paddpockId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT numpadd,cutcarry FROM plotpaddspe";
    sql = sql + " WHERE sys_cod = '" + sysCode + "'";
    sql = sql + " AND plot_cod = '" + plotId + "'";
    sql = sql + " AND numpadd = '" + paddpockId + "'";

    if (lktable.exec(sql))
    {
        lktable.first();
        if ((lktable.value(1).toString() == "Y") || (lktable.value(1).toString() == "Y"))
            res = lktable.value(0).toString() + " cut&carry";
        else
            res = lktable.value(0).toString() + " Pasture";
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getCropDesc(QString cropId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT crop_des FROM crop";
    sql = sql + " WHERE crop_cod = '" + cropId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getPastureDesc(QString pastureId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT crop_des FROM pastures";
    sql = sql + " WHERE crop_cod = '" + pastureId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getVegeDesc(QString vegeId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT crop_des FROM crop";
    sql = sql + " WHERE crop_cod = '" + vegeId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getCropProdDesc(QString /*cropId*/, QString prodId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT prod_des FROM cpprods";
    sql = sql + " WHERE prod_cod = '" + prodId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getCropForaDesc(QString /*cropId*/, QString foraId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT fora_des FROM forages";
    sql = sql + " WHERE fora_cod = '" + foraId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getPastureProdDesc(QString /*pastureId*/, QString prodId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT prod_des FROM cpprods";
    sql = sql + " WHERE prod_cod = '" + prodId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getPastureForaDesc(QString /*pastureId*/, QString foraId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT fora_des FROM forages";
    sql = sql + " WHERE fora_cod = '" + foraId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}


QString tableDescriptor::getLiveDesc(QString sysCode, QString liveId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT livestoc.live_des FROM lvstosys,livestoc";
    sql = sql + " WHERE lvstosys.live_cod = livestoc.live_cod";
    sql = sql + " AND sys_cod = '" + sysCode + "'";
    sql = sql + " AND live_icod = '" + liveId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getLiveProdDesc(QString prodId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT prod_des FROM products";
    sql = sql + " WHERE prod_cod = '" + prodId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getActivityDesc(QString ActyId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT acty_des FROM activity";
    sql = sql + " WHERE acty_cod = '" + ActyId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

 QString tableDescriptor::getFertDesc(QString fertId)
 {
     QString res;

     QString sql;
     QSqlQuery lktable(m_database);

     sql = "SELECT FERTYP_DES FROM ferttype";
     sql = sql + " WHERE FERTYP_cod = '" + fertId + "'";


     if (lktable.exec(sql))
     {
         lktable.first();
         res = lktable.value(0).toString();
     }
     else
     {
         res = "Error getting description!";
         qWarning() << "Error: " << lktable.lastError().databaseText();
         qWarning() << "Executing: " << sql;
     }

     return res;
 }

QString tableDescriptor::getFeedGroupDesc(QString sysCode, QString liveId, QString feedGId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT group_des FROM feedgroups";
    sql = sql + " WHERE sys_cod = '" + sysCode + "'";
    sql = sql + " AND live_icod = '" + liveId + "'";
    sql = sql + " AND group_cod = '" + feedGId + "'";


    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getPrchFeedDesc(QString feedId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT res_des FROM restypes";
    sql = sql + " WHERE res_cod = '" + feedId + "'";

    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}

QString tableDescriptor::getPrchMrnProdDesc(QString sysCode, QString prodId)
{
    QString res;

    QString sql;
    QSqlQuery lktable(m_database);

    sql = "SELECT prod_des FROM mnrheapprods";
    sql = sql + " WHERE sys_cod = '" + sysCode + "'";
    sql = sql + " AND prod_cod = '" + prodId + "'";

    if (lktable.exec(sql))
    {
        lktable.first();
        res = lktable.value(0).toString();
    }
    else
    {
        res = "Error getting description!";
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    return res;
}
