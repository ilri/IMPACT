#ifndef FIXCOMBODELEGATE_H
#define FIXCOMBODELEGATE_H

#include <QStyledItemDelegate>
#include "moduletypes.h"
#include <QSortFilterProxyModel>

class fixComboDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    fixComboDelegate(QWidget *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
             const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void insertItem(QString code, QString description);

    void setReadOnly(bool readyOnly);
    void setParentModel(QAbstractTableModel *model);
    void setParentSortModel(QSortFilterProxyModel *sortmodel);
    void setEditable(bool editable);
    void addNoDrawValue(QString value);
    void addShowContraint(QString code, QString whenField, QVariant whenValue);
private:
    QList<TfixComboItem> m_items;
    bool m_readOnly;
    QAbstractTableModel *m_parentModel;
    QSortFilterProxyModel *m_sortModel;
    bool m_editable;
    QStringList m_noDrawValues;
    bool isNoDrawValue(QString value) const;
    QList<TcmbdlgShowConstraint> showPattern;
    bool showItem(QString code, int index) const;

};

#endif // FIXCOMBODELEGATE_H
