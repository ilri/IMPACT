#include "seasondelegate.h"
#include <QPainter>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QLabel>
#include "moduleclasses.h"

seasonDelegate::seasonDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{
}


void seasonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    if (m_seasonModel)
    {
        fieldInColModel *fcolModel;
        fcolModel = qobject_cast<fieldInColModel *>(m_seasonModel);
        if (fcolModel)
        {
            if (fcolModel->getRowStatus(index.row()) == "C")
            {
                QList<TmoduleFieldDef> keys;
                keys.append(fcolModel->getRowKeyValues(index.row()));
                painter->drawText(option.rect,Qt::AlignCenter,getSeasonType(keys));
            }
            else
            {
                if (fcolModel->getRowStatus(index.row()) == "N")
                    painter->drawText(option.rect,Qt::AlignCenter,"New season");
                else
                    painter->drawText(option.rect,Qt::AlignCenter,"To be deleted");
            }
        }
        else
        {
            QStyledItemDelegate::paint(painter,option,index);
        }
    }
    else
    {
        QStyledItemDelegate::paint(painter,option,index);
    }
}

QSize seasonDelegate::sizeHint(const QStyleOptionViewItem &option,
               const QModelIndex &index) const
{
    QString description;

    if (m_seasonModel)
    {
        fieldInColModel *fcolModel;
        fcolModel = qobject_cast<fieldInColModel *>(m_seasonModel);
        if (fcolModel)
        {
            if (fcolModel->getRowStatus(index.row()) == "C")
            {
                QList<TmoduleFieldDef> keys;
                keys.append(fcolModel->getRowKeyValues(index.row()));
                description = getSeasonType(keys);
            }
            else
            {
                if (fcolModel->getRowStatus(index.row()) == "N")
                    description = "New season";
                else
                    description = "To be deleted";
            }
            QLabel tlabel;
            QFontMetrics fm(tlabel.font());
            return fm.size(Qt::TextSingleLine,description);
        }
        else
        {
            return QStyledItemDelegate::sizeHint(option,index);
        }
    }
    else
        return QStyledItemDelegate::sizeHint(option,index);


}

void seasonDelegate::setDatabase(QSqlDatabase database)
{
    m_database = database;
}

 void seasonDelegate::setSeasonModel(QAbstractTableModel *model)
 {
     m_seasonModel = model;
 }

 QString seasonDelegate::getSeasonType(QList<TmoduleFieldDef> keys) const
 {
     QSqlQuery tquery(m_database);
     QString sql;
     int pos;
     //Check for crops
     sql = "SELECT COUNT(*) FROM owneduse WHERE ";
     for (pos = 0; pos <= keys.count()-1;pos++)
     {
         sql = sql + keys[pos].code + " = ";
         sql = sql + "'" + keys[pos].value.toString() + "' AND ";
     }
     sql = sql + "crop_cod not like '(V%'";

     if (tquery.exec(sql))
     {
         if (tquery.first())
         {
             if (tquery.value(0).toInt() == 1)
                 return "Single crop";
             if (tquery.value(0).toInt() > 1)
                 return "Intercropping";
         }
     }
     else
     {
         qWarning() << "Error: " << tquery.lastError().databaseText();
         qWarning() << "Executing: " << sql;
         return "Error!";
     }

     //Check for vegetables
     sql = "SELECT COUNT(*) FROM ownusedv WHERE ";
     for (pos = 0; pos <= keys.count()-1;pos++)
     {
         sql = sql + keys[pos].code + " = ";
         sql = sql + "'" + keys[pos].value.toString() + "' AND ";
     }
     sql = sql.left(sql.length()-5);
     if (tquery.exec(sql))
     {
         if (tquery.first())
         {
             if (tquery.value(0).toInt() > 0)
                 return "Vegetable orchard";
         }
     }
     else
     {
         qWarning() << "Error: " << tquery.lastError().databaseText();
         qWarning() << "Executing: " << sql;
         return "Error!";
     }


     //Empty vege orchard
     sql = "SELECT COUNT(*) FROM owneduse WHERE ";
     for (pos = 0; pos <= keys.count()-1;pos++)
     {
         sql = sql + keys[pos].code + " = ";
         sql = sql + "'" + keys[pos].value.toString() + "' AND ";
     }
     sql = sql + "crop_cod like '(V%'";

     if (tquery.exec(sql))
     {
         if (tquery.first())
         {
             if (tquery.value(0).toInt() > 0)
                 return "Empty vegetable orchard";
         }
     }
     else
     {
         qWarning() << "Error: " << tquery.lastError().databaseText();
         qWarning() << "Executing: " << sql;
         return "Error!";
     }
     return "Empty season";
 }
