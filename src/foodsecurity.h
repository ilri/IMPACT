#ifndef FOODSECURITY_H
#define FOODSECURITY_H

#include <QWidget>
#include <QSqlDatabase>
#include <QWebView>
#include <QAbstractTableModel>


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

class classesModel2 :  public QAbstractTableModel
{
    Q_OBJECT
public:
    classesModel2(QObject *parent=0);

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
    class foodsecurity;
}

class foodsecurity : public QWidget
{
    Q_OBJECT

public:
    explicit foodsecurity(QWidget *parent = 0);
    ~foodsecurity();
    void setDatabase(QSqlDatabase db);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::foodsecurity *ui;

    QSqlDatabase dataBase;

    int getFieldIndex(QSqlQuery query,QString fieldName);

    //int totfoods;
    QVector<float>totemes;
    QVector<float>totpmes;
    QStringList periods;
    float contribe[4];
    float contribp[4];

    QString sistema;
    float GetMin(QString sistema, QString tipo);
    void loadValues(QString system);
    void generateGraphs(QString gtype);
    void createChart2();

    QString getTempPath();
    QString saveChart(QString chartName, QStringList html,QStringList);
    QString createPieChart(QString chartName, QString chartTitle, QString currSymbol,QList<TchartSeries> series, QWebView *view);
    QString createBarChart(QString chartName, QString chartTitle, QString yAxisTitle, QList<TchartSeries> series, float minvalue, QWebView *view);
signals:
    void closeCalled();

};

#endif // FOODSECURITY_H
