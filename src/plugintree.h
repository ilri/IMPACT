#ifndef PLUGINTREE_H
#define PLUGINTREE_H

#include <QTreeWidget>

class plugintree : public QTreeWidget
{
    Q_OBJECT
public:
     plugintree(QWidget *parent = 0);

protected:
     void mousePressEvent(QMouseEvent *event);
     void dragEnterEvent(QDragEnterEvent *event);
     void dragMoveEvent(QDragMoveEvent *event);
private:
     QMimeData *mimeData;
     QDrag *drag;

};

#endif // PLUGINTREE_H
