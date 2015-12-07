#ifndef IMAGECOMBODELEGATE_H
#define IMAGECOMBODELEGATE_H

#include <QStyledItemDelegate>
#include "moduletypes.h"

class imageComboDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    imageComboDelegate(QObject *parent = 0);
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

    void insertItem(QString code, QString description, QPixmap image);


    void setUndefinedValue(QString code, QPixmap image);

    void addIgnoredColumn(int col);
private:
    QList<TfixComboItem> m_items;
    QPixmap undefPix;
    QString undefCode;
    QList<int> colsIgnored;

    bool isColumnIgnored(int col) const;

};

#endif // IMAGECOMBODELEGATE_H
