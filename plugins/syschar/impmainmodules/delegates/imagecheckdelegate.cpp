#include "imagecheckdelegate.h"
#include <qpainter.h>
#include "moduleclasses.h"
#include "moduleglobal.h"
#include <QEvent>
#include <QMouseEvent>

imageCheckDelegate::imageCheckDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    p_model = 0;
    s_model = 0;
}

imageCheckDelegate::~imageCheckDelegate()
{
    //qDebug() << "Destroy imageCheckDelegate";
}

void imageCheckDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
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
            currpix = chkpix.scaledToHeight(listViewRowSize);
        else
            currpix = unchkpix.scaledToHeight(listViewRowSize);

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

        if (!p_model)
        {
            painter->drawPixmap(frame,currpix);
            return;
        }

        managementModel *m_model;
        m_model = qobject_cast<managementModel *>(p_model);
        if (m_model)
        {
            QModelIndex idx;
            if (!s_model)
                idx = index;
            else
                idx = s_model->mapToSource(index);

            QColor color;
            color = m_model->getColorForRow(idx.row());
            bool checked;
            bool editable;
            checked = m_model->getCheckedStatusForRow(idx.row());
            editable = m_model->getEditableStatusForRow(idx.row());

            if (checked && editable)
            {
                painter->fillRect(option.rect, color);
                if (option.state & QStyle::State_Selected)
                             painter->fillRect(option.rect, option.palette.highlight());
                painter->drawPixmap(frame,currpix);
            }
            else
            {
                if (option.state & QStyle::State_Selected)
                             painter->fillRect(option.rect, option.palette.highlight());
                QStyledItemDelegate::paint(painter, option, index);
            }
            return;
        }
        linkedTableModel *l_model;
        l_model = qobject_cast<linkedTableModel *>(p_model);
        if (l_model)
        {
            QModelIndex idx;
            if (!s_model)
                idx = index;
            else
                idx = s_model->mapToSource(index);
            QColor color;
            color = l_model->getColorForRow(idx.row());
            painter->fillRect(option.rect, color);
            if (option.state & QStyle::State_Selected)
                         painter->fillRect(option.rect, option.palette.highlight());
            painter->drawPixmap(frame,currpix);
            return;
        }
        cropsTableModel *c_model;
        c_model = qobject_cast<cropsTableModel *>(p_model);
        if (c_model)
        {
            QModelIndex idx;
            if (!s_model)
                idx = index;
            else
                idx = s_model->mapToSource(index);
            QColor color;
            color = c_model->getColorForRow(idx.row());
            painter->fillRect(option.rect, color);
            if (option.state & QStyle::State_Selected)
                         painter->fillRect(option.rect, option.palette.highlight());
            painter->drawPixmap(frame,currpix);
            return;
        }

        feedingTableModel *f_model;
        f_model = qobject_cast<feedingTableModel *>(p_model);
        if (f_model)
        {
            QModelIndex idx;
            if (!s_model)
                idx = index;
            else
                idx = s_model->mapToSource(index);

            QColor color;
            color = QColor(Qt::white);//f_model->getColorForRow(idx.row());
            bool checked;
            bool editable;
            checked = f_model->getCheckedStatusForRow(idx.row());
            editable = true;//f_model->getEditableStatusForRow(idx.row());


                painter->fillRect(option.rect, color);
                if (option.state & QStyle::State_Selected)
                             painter->fillRect(option.rect, option.palette.highlight());
                painter->drawPixmap(frame,currpix);

            return;
        }



    }
    else
    {
        if (option.state & QStyle::State_Selected)
                     painter->fillRect(option.rect, option.palette.highlight());
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize imageCheckDelegate::sizeHint(const QStyleOptionViewItem &option,
               const QModelIndex &index) const
{
    if (!isColumnIgnored(index.column()))
    {
        return chkpix.scaledToHeight(listViewRowSize).size();
    }
    else
    {
        return QStyledItemDelegate::sizeHint(option,index);
    }
}

bool imageCheckDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
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

void imageCheckDelegate::setCheckPixMap(QPixmap pix)
{
    chkpix = pix;
}

void imageCheckDelegate::setUnCheckPixMap(QPixmap pix)
{
    unchkpix = pix;
}

void imageCheckDelegate::addIgnoredColumn(int col)
{
    colsIgnored.append(col);
}

bool imageCheckDelegate::isColumnIgnored(int col) const
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

void imageCheckDelegate::setManagementModel(QAbstractTableModel *model)
{
    p_model = model;
}

void imageCheckDelegate::setSortModel(QSortFilterProxyModel *model)
{
    s_model = model;
}
