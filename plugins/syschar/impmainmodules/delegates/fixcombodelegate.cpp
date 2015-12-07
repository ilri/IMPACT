#include "fixcombodelegate.h"
#include <QLabel>
#include <QPainter>
#include <QComboBox>
#include "moduleclasses.h"

fixComboDelegate::fixComboDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
    m_readOnly = false;
    m_parentModel = 0;
    m_sortModel = 0;
    m_editable = false;
}

void fixComboDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    if (isNoDrawValue(index.data().toString()))
    {
        QStyledItemDelegate::paint(painter,option,index);
        return;
    }

    QString description;
    description = "Set a value";
    for (int pos = 0; pos <= m_items.count()-1; pos++)
    {
        if (m_items[pos].code.toUpper() == index.data().toString().toUpper())
        {
            description = m_items[pos].description;
            break;
        }
    }
    if (m_editable)
    {
        description = index.data().toString();
    }

    QLabel tlabel;
    QFont cfont;
    cfont = tlabel.font();
    tlabel.setText(description);
    if (!m_readOnly)
        cfont.setUnderline(true);

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    QPen tpen;
    tpen = painter->pen();
    tpen.setColor(QColor(Qt::blue));

    if (!m_parentModel)
    {
        painter->setFont(cfont);
        if (!m_readOnly)
            painter->setPen(tpen);
        painter->drawText(option.rect,Qt::AlignCenter,description);
    }
    else
    {
        linkedTableModel *tm;
        tm = qobject_cast<linkedTableModel *>(m_parentModel);
        if (tm)
        {
            QModelIndex idx;
            if (m_sortModel)
                idx = m_sortModel->mapToSource(index);
            else
                idx = index;
            if ((tm->getItemStatus(idx.row()) == "C") ||
                (tm->getItemStatus(idx.row()) == "M"))
            {
                painter->setFont(cfont);
                if (!m_readOnly)
                    painter->setPen(tpen);
                painter->drawText(option.rect,Qt::AlignCenter,description);
            }
            else
            {
                QStyledItemDelegate::paint(painter,option,index);
            }
        }
        else
        {
            painter->setFont(cfont);
            if (!m_readOnly)
                painter->setPen(tpen);
            painter->drawText(option.rect,Qt::AlignCenter,description);
        }
    }

}

QSize fixComboDelegate::sizeHint(const QStyleOptionViewItem &,
               const QModelIndex &index) const
{
    QString description;
    description = "Something";
    for (int pos = 0; pos <= m_items.count()-1; pos++)
    {
        if (m_items[pos].code.toUpper() == index.data().toString().toUpper())
        {
            description = m_items[pos].description;
            break;
        }
    }
    QLabel tlabel;
    QFontMetrics fm(tlabel.font());
    return fm.size(Qt::TextSingleLine,description);
}

QWidget *fixComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,
                           const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setEditable(m_editable);
    int pos;
    if (showPattern.count() == 0)
    {
        for (pos = 0; pos <= m_items.count()-1; pos++)
        {
            editor->addItem(m_items[pos].description);
        }
    }
    else
    {
        fieldInColModel *fcmodel;
        fcmodel = qobject_cast<fieldInColModel *>(m_parentModel);
        if (fcmodel)
        {
            for (pos = 0; pos <= m_items.count()-1; pos++)
            {
                if (showItem(m_items[pos].code,index.row()))
                    editor->addItem(m_items[pos].description);
            }
        }
        else
        {
            for (pos = 0; pos <= m_items.count()-1; pos++)
            {
                editor->addItem(m_items[pos].description);
            }
        }
    }
    if (m_editable)
    {
        int nvalue;
        nvalue = -1;
        for (pos = 0; pos <= m_items.count()-1; pos++)
        {
            if (m_items[pos].code.toUpper() == index.data().toString().toUpper())
            {
                nvalue = pos;
                break;
            }
        }
        if (nvalue == -1)
        {
            editor->addItem(index.data().toString());
        }
    }
    return editor;
}

void fixComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    int nvalue;
    nvalue = -1;
    QList<TfixComboItem> s_items;
    if (showPattern.count() == 0)
    {
        for (int pos = 0; pos <= m_items.count()-1; pos++)
        {
            if (m_items[pos].code.toUpper() == value.toUpper())
            {
                nvalue = pos;
                break;
            }
        }
    }
    else
    {
        fieldInColModel *fcmodel;
        fcmodel = qobject_cast<fieldInColModel *>(m_parentModel);
        if (fcmodel)
        {
            for (int pos = 0; pos <= m_items.count()-1; pos++)
            {
                if (showItem(m_items[pos].code,index.row()))
                {
                    s_items.append(m_items[pos]);
                }
            }
            for (int pos = 0; pos <= s_items.count()-1; pos++)
            {
                if (s_items[pos].code.toUpper() == value.toUpper())
                {
                    nvalue = pos;
                    break;
                }
            }
        }
        else
        {
            for (int pos = 0; pos <= m_items.count()-1; pos++)
            {
                if (m_items[pos].code.toUpper() == value.toUpper())
                {
                    nvalue = pos;
                    break;
                }
            }
        }
    }

    QComboBox *combo = static_cast<QComboBox*>(editor);
    if (nvalue == -1)
    {
        if (m_editable)
        {
            combo->addItem(value);
            nvalue = combo->count()-1;
        }
    }
    combo->setCurrentIndex(nvalue);
}

void fixComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox*>(editor);
    int nvalue;
    nvalue = combo->currentIndex();
    QString value;
    if (nvalue >= 0)
        value = m_items[nvalue].code;
    else
        value = m_items[0].code;
    if (m_editable)
    {
        value = combo->currentText();
    }
    model->setData(index, value, Qt::EditRole);
}

void fixComboDelegate::updateEditorGeometry(QWidget *editor,
         const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

void fixComboDelegate::insertItem(QString code, QString description)
{
    TfixComboItem item;
    item.code = code;
    item.description = description;
    m_items.append(item);
}

void fixComboDelegate::setReadOnly(bool readyOnly)
{
    m_readOnly = readyOnly;
}

void fixComboDelegate::setParentModel(QAbstractTableModel *model)
{
    m_parentModel = model;
}

void fixComboDelegate::setParentSortModel(QSortFilterProxyModel *sortmodel)
{
    m_sortModel = sortmodel;
}


void fixComboDelegate::setEditable(bool editable)
{
    m_editable = editable;
}

void fixComboDelegate::addNoDrawValue(QString value)
{
    m_noDrawValues.append(value);
}

bool fixComboDelegate::isNoDrawValue(QString value) const
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

void fixComboDelegate::addShowContraint(QString code, QString whenField, QVariant whenValue)
{
    TcmbdlgShowConstraint cnt;
    cnt.code = code;
    cnt.whenField = whenField;
    cnt.whenValue = whenValue;
    showPattern.append(cnt);
}

bool fixComboDelegate::showItem(QString code, int index) const
{
    int pos;
    bool res;
    res = false;
    fieldInColModel *fcmodel;
    fcmodel = qobject_cast<fieldInColModel *>(m_parentModel);
    QString whenField;
    QVariant whenValue;
    for (pos = 0; pos <= showPattern.count()-1;pos++)
    {
        whenField = showPattern[pos].whenField;
        whenValue = showPattern[pos].whenValue;
        if ((showPattern[pos].code == code) &&
            (fcmodel->getDataFromField(index,whenField) == whenValue))
        {
            res = true;
            break;
        }
    }
    return res;
}
