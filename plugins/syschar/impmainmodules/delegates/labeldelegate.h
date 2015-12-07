#ifndef LABELDELEGATE_H
#define LABELDELEGATE_H

#include <QStyledItemDelegate>
#include "moduletypes.h"
#include "moduleclasses.h"
#include <QSqlDatabase>


class labelDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    labelDelegate(QWidget *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    void insertItem(QString code, QString description);
    void addNotDialogValue(QString value);
    void addNoDrawValue(QString value);
private:
    QList<TfixComboItem> m_items;
    QStringList m_notDialogValues;
    bool isNotShowValue(QString value);
    QStringList m_noDrawValues;
    bool isNoDrawValue(QString value) const;

signals:
    void loadSubModule(QModelIndex index);
};

#endif // LABELDELEGATE_H
