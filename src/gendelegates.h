#ifndef GENDELEGATES_H
#define GENDELEGATES_H

#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>


class genChkDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    genChkDelegate(QObject *parent = 0);
    ~genChkDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    void setCheckPixMap(QPixmap pix);
    void setUnCheckPixMap(QPixmap pix);

    void addIgnoredColumn(int col);
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

    void setParentModel(QAbstractTableModel *model);
    void setParentSortModel(QSortFilterProxyModel *sortmodel);

signals:
    void requestPopUpMenu(QPoint mousepos, QModelIndex index);

private:
    QPixmap chkpix;
    QPixmap unchkpix;
    QList<int> colsIgnored;
    bool isColumnIgnored(int col) const;
    QAbstractTableModel *m_parentModel;
    QSortFilterProxyModel *m_sortModel;

};

//************************************

struct genCmbItem
{
  QString code;
  QString description;
  QPixmap image;
};
typedef genCmbItem TgenCmbItem;


class genCmbDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    genCmbDelegate(QWidget *parent = 0);
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

private:
    QList<TgenCmbItem> m_items;
    bool m_readOnly;
    QAbstractTableModel *m_parentModel;
    QSortFilterProxyModel *m_sortModel;
    bool m_editable;
    QStringList m_noDrawValues;
    bool isNoDrawValue(QString value) const;


};

#endif // GENDELEGATES_H
