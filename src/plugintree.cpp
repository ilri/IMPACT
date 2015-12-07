#include "plugintree.h"
#include <QDomDocument>
#include <QMouseEvent>
#include <QModelIndex>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QFontMetrics>
#include <QMimeData>
#include <QDrag>

plugintree::plugintree(QWidget *parent) :
    QTreeWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);

    setDragEnabled(true);
    setAcceptDrops(false);
}

void plugintree::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    const QModelIndex index = this->indexAt(event->pos());
    this->setCurrentIndex(index);
    if (index.isValid())
    {
        if (!index.data(Qt::UserRole+1).toString().isEmpty())
        {
            QByteArray itemData;

            QDomDocument doc("IMPMODULE");
            QDomElement root = doc.createElement("IMPMODULE");
            doc.appendChild(root);

            QString valor;
            QDomText t;

            QDomElement library = doc.createElement("LibraryName");
            root.appendChild(library);
            valor = index.data(Qt::UserRole).toString();
            t = doc.createTextNode(valor);
            library.appendChild(t);

            QDomElement plugin = doc.createElement("PluginName");
            root.appendChild(plugin);
            valor = index.data(Qt::UserRole+1).toString();
            t = doc.createTextNode(valor);
            plugin.appendChild(t);

            QDomElement pluginDesc = doc.createElement("PluginDescription");
            root.appendChild(pluginDesc);
            valor = index.data(Qt::UserRole+2).toString();
            t = doc.createTextNode(valor);
            pluginDesc.appendChild(t);

            itemData.append(doc.toString());

            QFont font;
            QFontMetrics met(font);
            QPixmap pixmap(met.boundingRect(valor).size());
            pixmap.fill();
            QPainter paint(&pixmap);
            paint.drawText(0,0-met.boundingRect(valor).y(),valor);

            mimeData = new QMimeData;
            mimeData->setData("object/x-IMPACT-plugin", itemData);            
            drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            drag->exec(Qt::MoveAction);            
        }
    }
}

void plugintree::dragEnterEvent(QDragEnterEvent *event)
{

    if (event->mimeData()->hasFormat("object/x-IMPACT-plugin"))
        event->accept();
    else
        event->ignore();
}

void plugintree::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("object/x-IMPACT-plugin")) {
             event->setDropAction(Qt::MoveAction);
             event->accept();
         } else
             event->ignore();
}
