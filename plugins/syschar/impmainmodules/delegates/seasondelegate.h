#ifndef seasonDelegate_H
#define seasonDelegate_H

#include <QStyledItemDelegate>
#include "moduletypes.h"
#include "moduleclasses.h"
#include <QSqlDatabase>
#include <QPointer>


class seasonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    seasonDelegate(QWidget *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
    void setDatabase(QSqlDatabase database);
    void setSeasonModel(QAbstractTableModel *model);
private:
    QSqlDatabase m_database;
    QPointer<QAbstractTableModel> m_seasonModel;
    QString getSeasonType(QList<TmoduleFieldDef> keys) const;
};

#endif // seasonDelegate_H
