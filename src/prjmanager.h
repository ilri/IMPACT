#ifndef PRJMANAGER_H
#define PRJMANAGER_H

#include <QWidget>
#include <QAbstractTableModel>

struct projectItem
{
  QString code;
  QString description;
  QString databaseFile;
  bool selected;
  bool defDB;
  bool error;
};
typedef projectItem TprojectItem;

class projectsModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    projectsModel(QObject *parent=0);
    ~projectsModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void loadItems();
    bool addDatabase(QString name, QString fileName);
    bool removeDatabase(int row);
    void setActive(int row);
private:
    QList <TprojectItem> items;
    void activateProject(QString code);
};

namespace Ui {
    class prjmanager;
}

class prjmanager : public QWidget
{
    Q_OBJECT

public:
    explicit prjmanager(QWidget *parent = 0);
    ~prjmanager();

signals:
    void closeCalled();

private slots:
    void on_pushButton_5_clicked();

    void on_cmdadd_clicked();

    void on_cmduse_clicked();

    void on_cmdremove_clicked();

    void on_cmdnew_clicked();

private:
    Ui::prjmanager *ui;
    projectsModel *m_prkmodel;
};

#endif // PRJMANAGER_H
