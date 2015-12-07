#include "imagecombodelegate.h"
#include "moduleglobal.h"
#include <QPainter>
#include <QComboBox>

imageComboDelegate::imageComboDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}


void imageComboDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{

    QString currValue;
    currValue = index.data().toString();
    QPixmap currpix;
    if (option.state & QStyle::State_Selected)
                 painter->fillRect(option.rect, option.palette.highlight());

    if (!isColumnIgnored(index.column()))
    {

        if (currValue == undefCode)
        {
            currpix = undefPix;
        }
        else
        {
            bool found;
            found = false;
            for (int pos = 0; pos <= m_items.count()-1; pos++)
            {
                if (m_items[pos].code == currValue)
                {
                    currpix = m_items[pos].image;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                currpix = undefPix;
            }
        }
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

        painter->drawPixmap(frame,currpix);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize imageComboDelegate::sizeHint(const QStyleOptionViewItem &option,
               const QModelIndex &index) const
{
    if (!isColumnIgnored(index.column()))
    {
        QPixmap currpix;
        bool found;
        found = false;
        for (int pos = 0; pos <= m_items.count()-1; pos++)
        {
            if (m_items[pos].code == index.data().toString())
            {
                currpix = m_items[pos].image;
                found = true;
                break;
            }
        }
        if (!found)
            currpix = undefPix;
        return currpix.size();
    }
    else
    {
        return option.rect.size();
    }
}

QWidget *imageComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,
                           const QModelIndex &) const
{
    QComboBox *editor = new QComboBox(parent);
    for (int pos = 0; pos <= m_items.count()-1; pos++)
    {
        editor->addItem(QIcon(m_items[pos].image),m_items[pos].description);
    }
    return editor;
}

void imageComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    int nvalue;
    nvalue = -1;
    for (int pos = 0; pos <= m_items.count()-1; pos++)
    {
        if (m_items[pos].code == value)
        {
            nvalue = pos;
            break;
        }
    }
    QComboBox *combo = static_cast<QComboBox*>(editor);
    combo->setCurrentIndex(nvalue);
}

void imageComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
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
    model->setData(index, value, Qt::EditRole);
}

void imageComboDelegate::updateEditorGeometry(QWidget *editor,
         const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}



void imageComboDelegate::insertItem(QString code, QString description, QPixmap image)
{
    TfixComboItem item;
    item.code = code;
    item.description = description;

    //qWarning() << "Scaling to: "<< listViewRowSize;

    QPixmap timage;
    timage = image.scaledToHeight(listViewRowSize);
    item.image = timage;
    m_items.append(item);
}

void imageComboDelegate::setUndefinedValue(QString code, QPixmap image)
{
    undefPix = image;
    undefCode = code;
}

void imageComboDelegate::addIgnoredColumn(int col)
{
    colsIgnored.append(col);
}

bool imageComboDelegate::isColumnIgnored(int col) const
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
