#ifndef economics_H
#define economics_H

#include <QWidget>
#include <QSqlDatabase>
#include <QWebView>
#include <QAbstractTableModel>

struct ProductData
{
  QString ptype;
  QString pCode;
  QString pDesc;
  float pValue;
};
typedef ProductData PProductData;

#ifndef v_OutputData
#define v_OutputData
struct OutputData
{
  QString ptype;
  QString pCode;
  bool pFood;
  bool pFeed;
  float pValue;
};
typedef OutputData POutputData;
#endif

#ifndef v_chartSeries
#define v_chartSeries
struct chartSeries
{
  QString group;
  QString name;
  float value;
};
typedef chartSeries TchartSeries;
#endif

struct chartGroups
{
  QString code;
  QString name;
  QColor color;
};
typedef chartGroups TchartGroups;

class detailModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    detailModel(QObject *parent=0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void setClasses(QVector<float> icrops, QVector<float> ocrops, QVector<float> ilives, QVector<float> olives, QVector<float> iothers, QVector<float> oothers);
    void setPeriods(QStringList periods);
    void setTipCambio(float tipcambio);
private:
    QVector<float> inscrops;
    QVector<float> outscrops;
    QVector<float> inslives;
    QVector<float> outslives;
    QVector<float> insothers;
    QVector<float> outsothers;
    QStringList m_periods;
    float m_tipcambio;

};

class classesModel :  public QAbstractTableModel
{
    Q_OBJECT
public:
    classesModel(QObject *parent=0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void loadClasses(QList<TchartSeries> classes);

private:
    QList<TchartSeries> m_items;

};


namespace Ui {
    class economics;
}

class economics : public QWidget
{
    Q_OBJECT

public:
    explicit economics(QWidget *parent = 0);
    ~economics();
    void setDatabase(QSqlDatabase db);

private:
    Ui::economics *ui;
    QString sistema;
    QSqlDatabase dataBase;
    float tipcambio;
    QString sysCurrSymbol;



    int getFieldIndex(QSqlQuery query,QString fieldName);

    void createChart1();
    void createChart2();
    void createChart3();
    void createChart4();
    void createChart5();
    QString createPieChart(QString chartName, QString chartTitle, QString currSymbol,QList<TchartSeries> series, QWebView *view);
    QString createLineChart(QString chartName, QString chartTitle, QString currSymbol,QStringList categories,
                            QList<TchartGroups> groups,
                            QList<TchartSeries> series,QWebView *view);

    QString createBarChart(QString chartName, QString chartTitle, QString currSymbol,QList<TchartSeries> series, QWebView *view);

    void getoutscrops(QString system, QString tipo);
    void getoutslvandot(QString system);
    void getinscrops(QString system, QString tipo);
    void getinslives(QString system);

    QString getTempPath();
    QString saveChart(QString chartName, QStringList html,QStringList xml);

    float anexpsrc[5];

    QVector<float> inscrops;
    QVector<float> outscrops;
    QVector<float> inslives;
    QVector<float> outslives;
    QVector<float> insothers;
    QVector<float> outsothers;

signals:
    void closeCalled();

private slots:
    void on_cmdclode_clicked();
    void on_cmdgen_clicked();
    void tabchanged(int pos);
};

#endif // economics_H
