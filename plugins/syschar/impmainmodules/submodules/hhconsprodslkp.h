#ifndef HHCONSPRODSLKP_H
#define HHCONSPRODSLKP_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSortFilterProxyModel>

struct hhProd
{
  QString code;
  QString type;
  QString desc;
  float energy;
  float protein;
};
typedef hhProd ThhProd;

class hhProdsModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    hhProdsModel(QObject *parent=0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void setDatabase(QSqlDatabase database);
    void loadItems();
    float getEnergy(int row);
    float getProtein(int row);
private:
    QSqlDatabase m_database;
    QList<ThhProd> m_items;
    int getFieldIndex(QSqlQuery query,QString fieldName);
};


namespace Ui {
    class hhconsprodslkp;
}

class hhconsprodslkp : public QDialog
{
    Q_OBJECT

public:
    explicit hhconsprodslkp(QWidget *parent = 0);
    ~hhconsprodslkp();
    void setDatabase(QSqlDatabase database);
    void loadData();
    float energy;
    float protein;
    bool selected;

private slots:
    void on_pushButton_3_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::hhconsprodslkp *ui;
    hhProdsModel * m_prodsmodel;
    QSqlDatabase m_database;
    QSortFilterProxyModel *m_sortModel;
};

#endif // HHCONSPRODSLKP_H
