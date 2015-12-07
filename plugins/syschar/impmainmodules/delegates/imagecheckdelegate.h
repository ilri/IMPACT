#ifndef IMAGECHECKDELEGATE_H
#define IMAGECHECKDELEGATE_H

#include <QStyledItemDelegate>
#include <QPointer>
#include <QSortFilterProxyModel>


class imageCheckDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    imageCheckDelegate(QObject *parent = 0);
    ~imageCheckDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    void setCheckPixMap(QPixmap pix);
    void setUnCheckPixMap(QPixmap pix);

    void addIgnoredColumn(int col);
    void setManagementModel(QAbstractTableModel *model);
    void setSortModel(QSortFilterProxyModel *model);
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void requestPopUpMenu(QPoint mousepos, QModelIndex index);

private:
    QPixmap chkpix;
    QPixmap unchkpix;
    QList<int> colsIgnored;
    bool isColumnIgnored(int col) const;
    QPointer<QAbstractTableModel> p_model;
    QPointer<QSortFilterProxyModel> s_model;

};

#endif // IMAGECHECKDELEGATE_H
