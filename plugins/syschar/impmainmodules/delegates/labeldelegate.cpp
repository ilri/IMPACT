#include "labeldelegate.h"
#include <qlabel.h>
#include <qapplication.h>
#include <QPainter>
#include <QDebug>
#include "modulesubscreen.h"

labelDelegate::labelDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{
}


void labelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    if (isNoDrawValue(index.data().toString()))
    {
        QStyledItemDelegate::paint(painter,option,index);
        return;
    }
    QString description;
    description = "Something";
    for (int pos = 0; pos <= m_items.count()-1; pos++)
    {
        if (m_items[pos].code == index.data().toString())
        {
            description = m_items[pos].description;
            break;
        }
    }

    QLabel tlabel;
    QFont cfont;
    cfont = tlabel.font();
    tlabel.setText(description);
    cfont.setUnderline(true);

    if (option.state & QStyle::State_Selected)
                 painter->fillRect(option.rect, option.palette.highlight());

    painter->setFont(cfont);
    QPen tpen;
    tpen = painter->pen();
    tpen.setColor(QColor(Qt::blue));
    painter->setPen(tpen);


    painter->drawText(option.rect,Qt::AlignCenter,description);
}

QSize labelDelegate::sizeHint(const QStyleOptionViewItem &,
               const QModelIndex &index) const
{
    QString description;
    description = "Something";
    for (int pos = 0; pos <= m_items.count()-1; pos++)
    {
        if (m_items[pos].code == index.data().toString())
        {
            description = m_items[pos].description;
            break;
        }
    }
    QLabel tlabel;
    QFontMetrics fm(tlabel.font());
    return fm.size(Qt::TextSingleLine,description);
}

bool labelDelegate::editorEvent(QEvent *event, QAbstractItemModel *, const QStyleOptionViewItem &, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        if (!isNotShowValue(index.data().toString()))
        {
            emit loadSubModule(index);
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void labelDelegate::insertItem(QString code, QString description)
{
    TfixComboItem item;
    item.code = code;
    item.description = description;
    m_items.append(item);
}

void labelDelegate::addNotDialogValue(QString value)
{
    m_notDialogValues.append(value);
}

bool labelDelegate::isNotShowValue(QString value)
{
    for (int pos = 0; pos <= m_notDialogValues.count()-1;pos++)
    {
        if (m_notDialogValues[pos] == value)
        {
            return true;
        }
    }
    return false;
}

void labelDelegate::addNoDrawValue(QString value)
{
    m_noDrawValues.append(value);
}

bool labelDelegate::isNoDrawValue(QString value) const
{
    bool res;
    res = false;
    for (int pos = 0; pos <= m_noDrawValues.count()-1;pos++)
    {
        if (m_noDrawValues[pos] == value)
        {
            res = true;
            break;
        }
    }
    return res;
}
