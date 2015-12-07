#include "gendelegates.h"
#include <qpainter.h>
#include "impclasses.h"
#include <QLabel>
#include <QPainter>
#include <QComboBox>
#include <QEvent>
#include <QMouseEvent>

genChkDelegate::genChkDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    m_parentModel = 0;
    m_sortModel = 0;
}

genChkDelegate::~genChkDelegate()
{
}

void genChkDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{

    QString currValue;
    currValue = index.data().toString();
    if (!isColumnIgnored(index.column()))
    {

        QPixmap currpix;
        if ((currValue == "Y") ||
            (currValue == "T") ||
            (currValue == "1"))
            currpix = chkpix;
        else
            currpix = unchkpix;

        //Centers the image in the frame
        QSize pixSize;
        pixSize = currpix.size();
        QRect frame;
        frame = option.rect;
        int w = (int)frame.width()/2;
        int h = (int)frame.height()/2;
        int w2 = (int)pixSize.width()/2;
        int h2 = (int)pixSize.height()/2;
        frame.setX(frame.x()+(w-w2));
        frame.setY(frame.y()+(h-h2));
        frame.setSize(pixSize);


        if (!m_parentModel)
        {
            painter->drawPixmap(frame,currpix);
        }
        else
        {
            childTableModel *tm;
            tm = qobject_cast<childTableModel *>(m_parentModel);
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
                    painter->drawPixmap(frame,currpix);
                }
                else
                {
                    if (option.state & QStyle::State_Selected)
                                 painter->fillRect(option.rect, option.palette.highlight());
                    QStyledItemDelegate::paint(painter, option, index);
                }
            }
            else
            {
                if (option.state & QStyle::State_Selected)
                             painter->fillRect(option.rect, option.palette.highlight());
                QStyledItemDelegate::paint(painter, option, index);
            }
        }

    }
    else
    {
        if (option.state & QStyle::State_Selected)
                     painter->fillRect(option.rect, option.palette.highlight());
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize genChkDelegate::sizeHint(const QStyleOptionViewItem &option,
               const QModelIndex &index) const
{
    if (!isColumnIgnored(index.column()))
    {
        return chkpix.size();
    }
    else
    {
        return QStyledItemDelegate::sizeHint(option,index);
    }
}

bool genChkDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        if (!isColumnIgnored(index.column()))
        {
            QString currValue;
            currValue = index.data().toString();
            if ((currValue == "T") || (currValue == "F"))
            {
                if (currValue == "T")
                    currValue = "F";
                else
                    currValue = "T";
            }
            if ((currValue == "Y") || (currValue == "N"))
            {
                if (currValue == "Y")
                    currValue = "N";
                else
                    currValue = "Y";
            }
            if ((currValue == "1") || (currValue == "0"))
            {
                if (currValue == "1")
                    currValue = "0";
                else
                    currValue = "1";
            }
            model->setData(index, currValue, Qt::EditRole);
            return true;
        }
        else
        {
            return QStyledItemDelegate::editorEvent(event,model,option,index);
        }
    }
    QMouseEvent *mv;
    mv = (QMouseEvent *) event;
    if (mv->button() == Qt::RightButton)
    {
        emit requestPopUpMenu(mv->globalPos(),index);
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

void genChkDelegate::setCheckPixMap(QPixmap pix)
{
    chkpix = pix;
}

void genChkDelegate::setUnCheckPixMap(QPixmap pix)
{
    unchkpix = pix;
}

void genChkDelegate::addIgnoredColumn(int col)
{
    colsIgnored.append(col);
}

bool genChkDelegate::isColumnIgnored(int col) const
{
    bool res;
    res = false;
    for (int pos = 0; pos <= colsIgnored.count()-1;pos++)
    {
        if (colsIgnored[pos] == col)
        {
            res = true;
            break;
        }
    }
    return res;
}

void genChkDelegate::setParentModel(QAbstractTableModel *model)
{
    m_parentModel = model;
}

void genChkDelegate::setParentSortModel(QSortFilterProxyModel *sortmodel)
{
    m_sortModel = sortmodel;
}

//******************************************************





genCmbDelegate::genCmbDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
    m_readOnly = false;
    m_parentModel = 0;
    m_sortModel = 0;
    m_editable = false;
}

void genCmbDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
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
        childTableModel *tm;
        tm = qobject_cast<childTableModel *>(m_parentModel);
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

QSize genCmbDelegate::sizeHint(const QStyleOptionViewItem &,
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

QWidget *genCmbDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,
                           const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setEditable(m_editable);
    int pos;

        for (pos = 0; pos <= m_items.count()-1; pos++)
        {
            editor->addItem(m_items[pos].description);
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

void genCmbDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    int nvalue;
    nvalue = -1;

        for (int pos = 0; pos <= m_items.count()-1; pos++)
        {
            if (m_items[pos].code.toUpper() == value.toUpper())
            {
                nvalue = pos;
                break;
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

void genCmbDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
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

void genCmbDelegate::updateEditorGeometry(QWidget *editor,
         const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

void genCmbDelegate::insertItem(QString code, QString description)
{
    TgenCmbItem item;
    item.code = code;
    item.description = description;
    m_items.append(item);
}

void genCmbDelegate::setReadOnly(bool readyOnly)
{
    m_readOnly = readyOnly;
}

void genCmbDelegate::setParentModel(QAbstractTableModel *model)
{
    m_parentModel = model;
}

void genCmbDelegate::setParentSortModel(QSortFilterProxyModel *sortmodel)
{
    m_sortModel = sortmodel;
}


void genCmbDelegate::setEditable(bool editable)
{
    m_editable = editable;
}

void genCmbDelegate::addNoDrawValue(QString value)
{
    m_noDrawValues.append(value);
}

bool genCmbDelegate::isNoDrawValue(QString value) const
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


