#include "ecnomics.h"
#include "ui_ecnomics.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <IMPACTGlobal.h>
#include <QDir>
#include <QSqlRecord>
#include <qglobal.h>

detailModel::detailModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    m_tipcambio = 1;
}

int detailModel::rowCount(const QModelIndex &) const
{
    return 19;
}
int detailModel::columnCount(const QModelIndex &) const
{
    return inscrops.count()+2;
}
QVariant detailModel::data(const QModelIndex &index, int role) const
{
    int total;
    int pos;
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.row() == 0)
        {
            if (index.column() == 0)
                return "Crops";
        }
        if (index.row() == 5)
        {
            if (index.column() == 0)
                return "Livestock";
        }
        if (index.row() == 10)
        {
            if (index.column() == 0)
                return "Other";
        }
        if (index.row() == 15)
        {
            if (index.column() == 0)
                return "Balance";
        }
        //Crops
        //Crop inputs
        if (index.row() == 1)
        {
            if (index.column() == 0)
                return "Income";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(inscrops[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= inscrops.count()-1;pos++)
                    total = total + inscrops[pos];
                return QString::number(total,'f',2);
            }
        }
        //Crop outputs
        if (index.row() == 2)
        {
            if (index.column() == 0)
                return "Expenses";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(outscrops[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outscrops.count()-1;pos++)
                    total = total + outscrops[pos];
                return QString::number(total,'f',2);
            }
        }
        //Crop balance
        if (index.row() == 3)
        {
            if (index.column() == 0)
                return "Balance";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(inscrops[index.column()-1] - outscrops[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outscrops.count()-1;pos++)
                    total = total + (inscrops[pos] - outscrops[pos]);
                return QString::number(total,'f',2);
            }
        }
        //Livestock
        //Livestock inputs
        if (index.row() == 6)
        {
            if (index.column() == 0)
                return "Income";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(inslives[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= inslives.count()-1;pos++)
                    total = total + inslives[pos];
                return QString::number(total,'f',2);
            }
        }
        //Livestock outputs
        if (index.row() == 7)
        {
            if (index.column() == 0)
                return "Expenses";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(outslives[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outslives.count()-1;pos++)
                    total = total + outslives[pos];
                return QString::number(total,'f',2);
            }
        }
        //Livestock balance
        if (index.row() == 8)
        {
            if (index.column() == 0)
                return "Balance";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(inslives[index.column()-1] - outslives[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outslives.count()-1;pos++)
                    total = total + (inslives[pos] - outslives[pos]);
                return QString::number(total,'f',2);
            }
        }
        //Other
        //Other inputs
        if (index.row() == 11)
        {
            if (index.column() == 0)
                return "Income";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(insothers[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= insothers.count()-1;pos++)
                    total = total + insothers[pos];
                return QString::number(total,'f',2);
            }
        }
        //other outputs
        if (index.row() == 12)
        {
            if (index.column() == 0)
                return "Expenses";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(outsothers[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outsothers.count()-1;pos++)
                    total = total + outsothers[pos];
                return QString::number(total,'f',2);
            }
        }
        //other balance
        if (index.row() == 13)
        {
            if (index.column() == 0)
                return "Balance";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(insothers[index.column()-1] - outsothers[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outsothers.count()-1;pos++)
                    total = total + (insothers[pos] - outsothers[pos]);
                return QString::number(total,'f',2);
            }
        }
        //Total
        //Total income
        if (index.row() == 16)
        {
            if (index.column() == 0)
                return "Income";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(inscrops[index.column()-1] + inslives[index.column()-1] + insothers[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= insothers.count()-1;pos++)
                    total = total + inscrops[pos] + inslives[pos] + insothers[pos];
                return QString::number(total,'f',2);
            }
        }
        //Total expenses
        if (index.row() == 17)
        {
            if (index.column() == 0)
                return "Expenses";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number(outscrops[index.column()-1] + outslives[index.column()-1] + outsothers[index.column()-1],'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outsothers.count()-1;pos++)
                    total = total + outscrops[pos] + outslives[pos] + outsothers[pos];
                return QString::number(total,'f',2);
            }
        }
        //other balance
        if (index.row() == 18)
        {
            if (index.column() == 0)
                return "Balance";
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                return QString::number((inscrops[index.column()-1] + inslives[index.column()-1] + insothers[index.column()-1] ) - (outscrops[index.column()-1] + outslives[index.column()-1] + outsothers[index.column()-1]),'f',2);
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outsothers.count()-1;pos++)
                    total = total + ((inscrops[pos] + inslives[pos] + insothers[pos]) - (outscrops[pos] + outslives[pos] + outsothers[pos]));
                return QString::number(total,'f',2);
            }
        }


    }
    if (role == Qt::BackgroundRole)
    {
        if ((index.row() == 4) || (index.row() == 9) || (index.row() == 14))
        {
            QBrush Background(Qt::lightGray);
            Background.setStyle(Qt::SolidPattern);
            return Background;
        }
        if (index.row() == 3)
        {
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                if ((inscrops[index.column()-1] - outscrops[index.column()-1]) < 0)
                {
                    QBrush Background(Qt::red);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outscrops.count()-1;pos++)
                    total = total + (inscrops[pos] - outscrops[pos]);
                if (total < 0)
                {
                    QBrush Background(Qt::red);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
            }
        }
        if (index.row() == 8)
        {
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                if ((inslives[index.column()-1] - outslives[index.column()-1]) < 0)
                {
                    QBrush Background(Qt::red);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outslives.count()-1;pos++)
                    total = total + (inslives[pos] - outslives[pos]);
                if (total < 0)
                {
                    QBrush Background(Qt::red);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
            }
        }
        if (index.row() == 13)
        {
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                if ((insothers[index.column()-1] - outsothers[index.column()-1]) < 0)
                {
                    QBrush Background(Qt::red);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outsothers.count()-1;pos++)
                    total = total + (insothers[pos] - outsothers[pos]);
                if (total < 0)
                {
                    QBrush Background(Qt::red);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
            }
        }
        if (index.row() == 18)
        {
            if ((index.column() > 0) && (index.column() < this->columnCount()-1))
            {
                if (((inscrops[index.column()-1] + inslives[index.column()-1] + insothers[index.column()-1]) - (outscrops[index.column()-1] + outslives[index.column()-1] + outsothers[index.column()-1])) < 0)
                {
                    QBrush Background(Qt::red);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
            }
            if (index.column() == this->columnCount()-1)
            {
                total = 0;
                for (pos = 0; pos <= outsothers.count()-1;pos++)
                    total = total + ((inscrops[pos] + inslives[pos] + insothers[pos]) - (outscrops[pos] + outslives[pos] + outsothers[pos]));
                if (total < 0)
                {
                    QBrush Background(Qt::red);
                    Background.setStyle(Qt::SolidPattern);
                    return Background;
                }
            }
        }
    }
    if (role == Qt::FontRole)
    {
        if (index.column() == 0)
        {
            if ((index.row() == 0) || (index.row() == 5) || (index.row() == 10) || (index.row() == 15))
            {
                QFont font;
                font.setBold(true);
                return font;
            }
        }
    }
    return QVariant();
}
bool detailModel::setData(const QModelIndex &,const QVariant &,int)
{
    return false;
}

Qt::ItemFlags detailModel::flags (const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
QVariant detailModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "";
            if (section > 0 and section <= m_periods.count())
                return m_periods[section-1];
            if (section > m_periods.count())
                return "Total";
        }
    }
    return QVariant();
}

void detailModel::setClasses(QVector<float> icrops, QVector<float> ocrops, QVector<float> ilives, QVector<float> olives, QVector<float> iothers, QVector<float> oothers)
{
    this->beginResetModel();
    inscrops = QVector<float>(icrops);
    outscrops = QVector<float>(ocrops);
    inslives = QVector<float>(ilives);
    outslives = QVector<float>(olives);
    insothers = QVector<float>(iothers);
    outsothers = QVector<float>(oothers);

    if (m_tipcambio == 0)
        m_tipcambio = 1;

    for (int pos = 0; pos <= inscrops.count()-1;pos++)
    {
        inscrops[pos] = inscrops[pos]*1.0/m_tipcambio;
        outscrops[pos] = outscrops[pos]*1.0/m_tipcambio;
        inslives[pos] = inslives[pos]*1.0/m_tipcambio;
        outslives[pos] = outslives[pos]*1.0/m_tipcambio;
        insothers[pos] = insothers[pos]*1.0/m_tipcambio;
        outsothers[pos] = outsothers[pos]*1.0/m_tipcambio;
    }

    this->endResetModel();
}

void detailModel::setPeriods(QStringList periods)
{
    m_periods.append(periods);
}

void detailModel::setTipCambio(float tipcambio)
{
    m_tipcambio = tipcambio;
}

//*********************************************

classesModel::classesModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
}

int classesModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}
int classesModel::columnCount(const QModelIndex &) const
{
    return 2;
}
QVariant classesModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return m_items[index.row()].name;
        if (index.column() == 1)
            return QString::number(m_items[index.row()].value,'f',2);
    }
    return QVariant();
}
bool classesModel::setData(const QModelIndex &,const QVariant &,int)
{
    return false;
}

Qt::ItemFlags classesModel::flags (const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
QVariant classesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Class";
            if (section == 1)
                return "Value";
        }
    }
    return QVariant();
}

void classesModel::loadClasses(QList<TchartSeries> classes)
{

    m_items.append(classes);
}


//**********************************************************************
economics::economics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::economics)
{
    ui->setupUi(this);
    tipcambio = 1;

    ui->tabWidget_2->setCurrentIndex(0);
    connect(ui->tabWidget_2,SIGNAL(currentChanged(int)),this,SLOT(tabchanged(int)));
}

economics::~economics()
{
    delete ui;
}

void economics::setDatabase(QSqlDatabase db)
{
    dataBase = db;

    QSqlQuery query(dataBase);
    QString sql;

    sql = "SELECT sys_cod,reg_desc,tipcambio FROM system";
    query.exec(sql);
    while (query.next())
    {
        ui->comboBox->addItem(query.value(1).toString());
        ui->comboBox->setItemData(ui->comboBox->count()-1,query.value(0).toString(),Qt::UserRole);
        ui->comboBox->setItemData(ui->comboBox->count()-1,query.value(2).toFloat(),Qt::UserRole+1);
    }
    if (ui->comboBox->count()> 0)
        ui->comboBox->setCurrentIndex(0);
}

int economics::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

QString economics::getTempPath()
{
    QDir dir;
    QString appPath;
#if IMPACT_OS_ANDROID == false
    appPath = dir.absolutePath()+dir.separator();
#else
    androidInterface *andint;
    andint = new androidInterface(this,m_javaVM);
    appPath = andint->getExternalStorageDirectory();
    delete andint;
    appPath = appPath+"/Android/data/org.ilri.CrossIMPACT/";
#endif
    QString tempPath;
    tempPath = appPath + "tmp" + dir.separator();
    if (!dir.exists(tempPath))
        if (!dir.mkdir(tempPath))
            return QString();
    return tempPath;
}

QString economics::saveChart(QString chartName, QStringList html,QStringList)
{
    QString tempPath;
    tempPath = getTempPath();
    if (tempPath.isEmpty())
        return QString();
    //Saves the html
    QString htmlFile;
    int pos;
    htmlFile = tempPath + chartName + ".html";
    QFile file(htmlFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return QString();
    QTextStream out(&file);
    for (pos = 0; pos <= html.count()-1; pos++)
    {
        out << html[pos] << "\n";
    }
    file.close();

    /*
    //Saves the xml
    file.setFileName(tempPath + chartName + ".xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return QString();
    out.setDevice(&file);
    for (pos = 0; pos <= xml.count()-1; pos++)
    {
        out << xml[pos] << "\n";
    }
    file.close();*/

#if defined(Q_OS_WIN)
    htmlFile = htmlFile.replace("\\","/");
    return "file:///" + htmlFile;
#else
    return "file://" + htmlFile;
#endif

}

QString economics::createBarChart(QString chartName, QString chartTitle, QString currSymbol,QList<TchartSeries> series, QWebView *view)
{
    QStringList html;

    QString jsFile;
    QString jqueryFile;

    int width;
    int height;
    int pos;
    QString temp;

    if (series.count() == 0)
        return QString();

    width = view->geometry().width()-20;
    height = view->geometry().height()-20;

    jsFile = "../3rdPartyTools/highcharts/js/highcharts.js";
    jqueryFile = "../3rdPartyTools/highcharts/js/jquery.min.js";


    //Creates the html

    html << "<!DOCTYPE HTML>";
    html << "<html>";
    html << "<head>";
    html << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">";
    html << "<title>Highcharts Example</title>";

    html << "<script type=\"text/javascript\" src=\"" + jqueryFile + "\"></script>";
    html << "<script type=\"text/javascript\">";

    html << "function addCommas(nStr)";
    html << "{";
    html << "nStr += '';";
    html << "x = nStr.split('.');";
    html << "x1 = x[0];";
    html << "x2 = x.length > 1 ? '.' + x[1] : '';";
    html << "var rgx = /(\\d+)(\\d{3})/;";
    html << "while (rgx.test(x1)) {";
    html << "x1 = x1.replace(rgx, '$1' + ',' + '$2');";
    html << "}";
    html << "return x1 + x2;";
    html << "}";

    html << "var chart;";
    html << "$(document).ready(function() {";
    html << "chart = new Highcharts.Chart({";
    html << "chart: {";
    html << "renderTo: 'container',";
    html << "defaultSeriesType: 'bar'";
    html << "},";
    html << "title: {";
    html << "text: '" + chartTitle + "'";
    html << "},";

    html << "xAxis: {";
    html << "categories: [''],";
    html << "title: {";
    html << "text: null";
    html << "}";
    html << "},";
    html << "yAxis: {";
    html << "title: {";
    html << "text: '',";
    html << "align: 'high'";
    html << "}";
    html << "},";
    html << "tooltip: {";
    html << "formatter: function() {";
    html << "return ''+";
    html << "this.series.name +': '+ addCommas(this.y) + ' " + currSymbol + "';";
    html << "}";
    html << "},";
    html << "plotOptions: {";
    html << "bar: {";
    html << "dataLabels: {";
    html << "enabled: true,";

    html << "formatter: function() {";
    html << "return addCommas(this.y) +' " + currSymbol + "';";
    html << "}";

    html << "}";
    html << "}";
    html << "},";
    html << "legend: {";
    html << "layout: 'vertical',";
    html << "align: 'right',";
    html << "verticalAlign: 'top',";
    html << "x: 0,";
    html << "y: 0,";
    html << "floating: true,";
    html << "borderWidth: 1,";
    html << "backgroundColor: '#FFFFFF',";
    html << "shadow: true";
    html << "},";
    html << "credits: {";
    html << "enabled: false";
    html << "},";
    html << "series: [";

    temp = "";

    pos = series.count();

    for (pos = 0; pos <= series.count()-1;pos++)
    {
        temp = temp + "{name: '" + series[pos].name + "',data: [";
        temp = temp + QString::number(series[pos].value,'f',2) + "]}, ";
    }
    temp = temp.left(temp.length()-2);

    html << temp;


    html << "]";
    html << "});";
    html << "});";

    html << "</script>";
    html << "</head>";
    html << "<body>";
    html << "<script type=\"text/javascript\" src=\"" + jsFile + "\"></script>";


    html << "<div id=\"container\" style=\"width: " + QString::number(width) + "px; height: " + QString::number(height) + "px; margin: 0 auto\"></div>";

    html << "</body>";
    html << "</html>";

    //**************End HTML of chart

    //Saves the chart
    return saveChart(chartName,html,QStringList());
}

QString economics::createPieChart(QString chartName, QString chartTitle, QString currSymbol,QList<TchartSeries> series, QWebView *view)
{

    QStringList html;

    QString jsFile;
    QString jqueryFile;

    int width;
    int height;
    int pos;


    if (series.count() == 0)
        return QString();

    width = view->geometry().width()-20;
    height = view->geometry().height()-20;

    //qDebug() << "W: " << width;
    //qDebug() << "H: " << height;


    jsFile = "../3rdPartyTools/highcharts/js/highcharts.js";
    jqueryFile = "../3rdPartyTools/highcharts/js/jquery.min.js";


    //Creates the html

    html << "<!DOCTYPE HTML>";
    html << "<html>";
    html << "<head>";
    html << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">";
    html << "<title>Highcharts Example</title>";

    html << "<script type=\"text/javascript\" src=\"" + jqueryFile + "\"></script>";
    html << "<script type=\"text/javascript\">";

    html << "function addCommas(nStr)";
    html << "{";
    html << "nStr += '';";
    html << "x = nStr.split('.');";
    html << "x1 = x[0];";
    html << "x2 = x.length > 1 ? '.' + x[1] : '';";
    html << "var rgx = /(\\d+)(\\d{3})/;";
    html << "while (rgx.test(x1)) {";
    html << "x1 = x1.replace(rgx, '$1' + ',' + '$2');";
    html << "}";
    html << "return x1 + x2;";
    html << "}";

    html << "var chart;";
    html << "$(document).ready(function() {";
    html << "chart = new Highcharts.Chart({";
    html << "chart: {";
    html << "renderTo: 'container',";
    html << "plotBackgroundColor: null,";
    html << "plotBorderWidth: null,";
    html << "plotShadow: false";
    html << "},";
    html << "title: {";
    html << "text: '" + chartTitle + "'";
    html << "},";
    html << "tooltip: {";
    html << "formatter: function() {";
    html << "return '<b>'+ this.point.name +'</b>: '+ addCommas(this.y) + ' " + currSymbol + "';";
    html << "}";
    html << "},";
    html << "plotOptions: {";
    html << "pie: {";
    html << "allowPointSelect: true,";
    html << "cursor: 'pointer',";
    html << "dataLabels: {";
    html << "enabled: true,";
    html << "color: '#000000',";
    html << "connectorColor: '#000000',";
    html << "formatter: function() {";
    html << "return addCommas(this.y) +' " + currSymbol + "';";
    html << "}";
    html << "},";
    html << "showInLegend: true";
    html << "}";
    html << "},";
    html << "series: [{";
    html << "type: 'pie',";
    html << "name: 'Browser share',";
    html << "data: [";

    for (pos = 0; pos <= series.count()-2;pos++)
    {
        html << "['" + series[pos].name + "', " + QString::number(series[pos].value,'f',2) + "],";
    }
    html << "['" + series[series.count()-1].name + "', " + QString::number(series[series.count()-1].value,'f',2) + "]";


    html << "]";
    html << "}]";
    html << "});";
    html << "});";

    html << "</script>";
    html << "</head>";
    html << "<body>";
    html << "<script type=\"text/javascript\" src=\"" + jsFile + "\"></script>";


    html << "<div id=\"container\" style=\"width: " + QString::number(width) + "px; height: " + QString::number(height) + "px; margin: 0 auto\"></div>";

    html << "</body>";
    html << "</html>";

    //**************End HTML of chart



    //Saves the chart
    return saveChart(chartName,html,QStringList());

}

QString economics::createLineChart(QString chartName, QString chartTitle, QString currSymbol,QStringList categories,
                        QList<TchartGroups> groups,
                        QList<TchartSeries> series,QWebView *view)
{
    QStringList html;

    QString jsFile;
    QString jqueryFile;

    QString subtitle;
    QString temp;
    QString temp2;

    int width;
    int height;
    int pos;
    int pos2;

    if (series.count() == 0)
        return QString();

    width = view->geometry().width()-20;
    height = view->geometry().height()-20;

    jsFile = "../3rdPartyTools/highcharts/js/highcharts.js";
    jqueryFile = "../3rdPartyTools/highcharts/js/jquery.min.js";


    //Creates the html

    html << "<!DOCTYPE HTML>";
    html << "<html>";
    html << "<head>";
    html << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">";
    html << "<title>Highcharts Example</title>";

    html << "<script type=\"text/javascript\" src=\"" + jqueryFile + "\"></script>";
    html << "<script type=\"text/javascript\">";

    html << "function addCommas(nStr)";
    html << "{";
    html << "nStr += '';";
    html << "x = nStr.split('.');";
    html << "x1 = x[0];";
    html << "x2 = x.length > 1 ? '.' + x[1] : '';";
    html << "var rgx = /(\\d+)(\\d{3})/;";
    html << "while (rgx.test(x1)) {";
    html << "x1 = x1.replace(rgx, '$1' + ',' + '$2');";
    html << "}";
    html << "return x1 + x2;";
    html << "}";

    html << "var chart;";
    html << "$(document).ready(function() {";
    html << "chart = new Highcharts.Chart({";
    html << "chart: {";
    html << "renderTo: 'container',";
    html << "defaultSeriesType: 'line',";
    html << "marginRight: 130,";
    html << "marginBottom: 25";
    html << "},";
    html << "title: {";
    html << "text: '" + chartTitle + "',";
    html << "x: -20 //center";
    html << "},";
    html << "subtitle: {";
    html << "text: '" + subtitle + "',";
    html << "x: -20";
    html << "},";
    html << "xAxis: {";

    html << "categories: [";

    //Add the categories
    temp = "";
    for (pos = 0; pos <= categories.count()-1;pos++)
    {
        temp = temp + "'" + categories[pos] + "', ";
    }
    temp = temp.left(temp.length()-2);
    html << temp;

    html << "]";

    html << "},";
    html << "yAxis: {";
    html << "title: {";
    html << "text: 'Total (" + currSymbol + ")'";
    html << "},";
    html << "plotLines: [{";
    html << "value: 0,";
    html << "width: 1,";
    html << "color: '#808080'";
    html << "}]";
    html << "},";
    html << "tooltip: {";
    html << "formatter: function() {";
    html << "return '<b>'+ this.series.name +'</b><br/>'+";
    html << "this.x +': '+ addCommas(this.y) +' " + currSymbol + "';";
    html << "}";
    html << "},";
    html << "legend: {";
    html << "layout: 'vertical',";
    html << "align: 'right',";
    html << "verticalAlign: 'top',";
    html << "x: -10,";
    html << "y: 100,";
    html << "borderWidth: 0";
    html << "},";
    html << "series: [";

    temp = "";
    for (pos = 0; pos <= groups.count()-1;pos++)
    {
        temp = temp + "{name: '" + groups[pos].name + "', data: [";
        temp2 = "";
        for (pos2 = 0; pos2 <= series.count()-1;pos2++)
        {
            if (series[pos2].group == groups[pos].code)
                temp2 = temp2 + QString::number(series[pos2].value,'f',2) + ", ";
        }
        temp2 = temp2.left(temp2.length()-2) + "]";
        temp = temp + temp2 + "}, ";
    }
    temp = temp.left(temp.length()-2);

    html << temp;

    html << "]";
    html << "});";
    html << "});";

    html << "</script>";
    html << "</head>";
    html << "<body>";
    html << "<script type=\"text/javascript\" src=\"" + jsFile + "\"></script>";


    html << "<div id=\"container\" style=\"width: " + QString::number(width) + "px; height: " + QString::number(height) + "px; margin: 0 auto\"></div>";

    html << "</body>";
    html << "</html>";

    //**************End HTML of chart



    //Saves the chart
    return saveChart(chartName,html,QStringList());
}

void economics::createChart1()
{


    QSqlQuery query(dataBase);
    QList<PProductData> lista;
    QString sql;
    int pos;
    QString ptype;
    int ind;
    int pos_end;
    //TListitems items;
    //TListitem item;


    ptype = "C";
    sql = "select crpprodmansoldperiods.crop_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum(qty * pri) as total";
    sql = sql + " from crpprodmansoldperiods,crop";
    sql = sql + " where crpprodmansoldperiods.crop_cod = crop.crop_cod";
    sql = sql + " and crpprodmansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by crpprodmansoldperiods.crop_cod,crop.crop_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == ptype) &&
                  (lista[pos].pCode == query.value(0).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            PProductData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(0).toString();
            gendata.pDesc = query.value(1).toString();
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }
    }


    //Crops Forages
    ptype = "C";
    sql = "select crpforamansoldperiods.crop_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum(qty * pri";
    sql = sql + ") as total";
    sql = sql + " from crpforamansoldperiods,crop";
    sql = sql + " where crpforamansoldperiods.crop_cod = crop.crop_cod";
    sql = sql + " and crpforamansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by crpforamansoldperiods.crop_cod,crop.crop_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if((lista[pos].ptype == ptype) &&
                 (lista[pos].pCode == query.value(0).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            PProductData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(0).toString();
            gendata.pDesc = query.value(1).toString();
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }



    //Vegetable products
    ptype = "V";
    sql = "select vegprodmansoldperiods.veg_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum(qty*pri";
    sql = sql + ") as total";
    sql = sql + " from vegprodmansoldperiods,crop";
    sql = sql + " where vegprodmansoldperiods.veg_cod = crop.crop_cod";
    sql = sql + " and vegprodmansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by vegprodmansoldperiods.veg_cod,crop.crop_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if((lista[pos].ptype == ptype) &&
                 (lista[pos].pCode == query.value(0).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            PProductData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(0).toString();
            gendata.pDesc = query.value(1).toString() + " (" + ptype + ")";
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
           lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }


    //Vegetable forages
    ptype = "V";
    sql = "select vegforamansoldperiods.veg_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum(qty*pri";
    sql = sql + ") as total";
    sql = sql + " from vegforamansoldperiods,crop";
    sql = sql + " where vegforamansoldperiods.veg_cod = crop.crop_cod";
    sql = sql + " and vegforamansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by vegforamansoldperiods.veg_cod,crop.crop_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if((lista[pos].ptype == ptype) &&
                 (lista[pos].pCode == query.value(0).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            PProductData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(0).toString();
            gendata.pDesc = query.value(1).toString() + " (" + ptype + ")";
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }


    //Grass products
    ptype = "G";
    sql = "select grassprodmansoldperiods.crop_cod as pcode,pastures.crop_des as pdesc,";
    sql = sql + "sum(qty*pri";
    sql = sql + ") as total";
    sql = sql + " from grassprodmansoldperiods,pastures";
    sql = sql + " where grassprodmansoldperiods.crop_cod = pastures.crop_cod";
    sql = sql + " and grassprodmansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by grassprodmansoldperiods.crop_cod,pastures.crop_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if((lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(0).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            PProductData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(0).toString();
            gendata.pDesc = query.value(1).toString();
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }
    }


    //Grass forages
    ptype = "G";
    sql = "select grassforamansoldperiods.crop_cod as pcode,pastures.crop_des as pdesc,";
    sql = sql + "sum(qty*pri";
    sql = sql + ") as total";
    sql = sql + " from grassforamansoldperiods,pastures";
    sql = sql + " where grassforamansoldperiods.crop_cod = pastures.crop_cod";
    sql = sql + " and grassforamansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by grassforamansoldperiods.crop_cod,pastures.crop_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if((lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(0).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            PProductData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(0).toString();
            gendata.pDesc = query.value(1).toString();
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }


    //Livestock with weight
    ptype = "L";
    sql = "select livehvstsoldperiods.live_cod as pcode,livestoc.live_des as pdesc,";
    sql = sql + "sum((qty*pri)";
    sql = sql + ") as total";
    sql = sql + " from livehvstsoldperiods,products,lvproducts,livestoc";
    sql = sql + " where livehvstsoldperiods.live_cod = lvproducts.live_cod";
    sql = sql + " and livehvstsoldperiods.prod_cod = lvproducts.prod_cod";
    sql = sql + " and livehvstsoldperiods.prod_cod = products.prod_cod";
    sql = sql + " and lvproducts.weight = 'Y'";
    sql = sql + " and livehvstsoldperiods.live_cod = livestoc.live_cod";
    sql = sql + " and livehvstsoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by livehvstsoldperiods.live_cod,livestoc.live_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if((lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(0).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            PProductData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(0).toString();
            gendata.pDesc = query.value(1).toString();
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }


    //Livestock with quantity
    ptype = "L";
    sql = "select livehvstsoldperiods.live_cod as pcode,livestoc.live_des as pdesc,";
    sql = sql + "sum((qty*pri)";
    sql = sql + ") as total";
    sql = sql + " from livehvstsoldperiods,products,lvproducts,livestoc";
    sql = sql + " where livehvstsoldperiods.live_cod = lvproducts.live_cod";
    sql = sql + " and livehvstsoldperiods.prod_cod = lvproducts.prod_cod";
    sql = sql + " and livehvstsoldperiods.prod_cod = products.prod_cod";
    sql = sql + " and lvproducts.weight = 'N'";
    sql = sql + " and livehvstsoldperiods.live_cod = livestoc.live_cod";
    sql = sql + " and livehvstsoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by livehvstsoldperiods.live_cod,livestoc.live_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if((lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(0).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            PProductData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(0).toString();
            gendata.pDesc = query.value(1).toString();
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }
    }


    //Livestock by category with weight
    ptype = "L";
    sql = "select livehvstsoldcatperiods.live_cod as pcode,livestoc.live_des as pdesc,";
    sql = sql + "sum((qty * pri)";
    sql = sql + ") as total";
    sql = sql + " from livehvstsoldcatperiods,products,lvproducts,livestoc";
    sql = sql + " where livehvstsoldcatperiods.live_cod = lvproducts.live_cod";
    sql = sql + " and livehvstsoldcatperiods.prod_cod = lvproducts.prod_cod";
    sql = sql + " and livehvstsoldcatperiods.prod_cod = products.prod_cod";
    sql = sql + " and lvproducts.weight = 'Y'";
    sql = sql + " and livehvstsoldcatperiods.live_cod = livestoc.live_cod";
    sql = sql + " and livehvstsoldcatperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by livehvstsoldcatperiods.live_cod,livestoc.live_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if((lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(0).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            PProductData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(0).toString();
            gendata.pDesc = query.value(1).toString();
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }


    //Livestock by category with quantity
    ptype = "L";
    sql = "select livehvstsoldcatperiods.live_cod as pcode,livestoc.live_des as pdesc,";
    sql = sql + "sum((qty * pri)";
    sql = sql + ") as total";
    sql = sql + " from livehvstsoldcatperiods,products,lvproducts,livestoc";
    sql = sql + " where livehvstsoldcatperiods.live_cod = lvproducts.live_cod";
    sql = sql + " and livehvstsoldcatperiods.prod_cod = lvproducts.prod_cod";
    sql = sql + " and livehvstsoldcatperiods.prod_cod = products.prod_cod";
    sql = sql + " and lvproducts.weight = 'N'";
    sql = sql + " and livehvstsoldcatperiods.live_cod = livestoc.live_cod";
    sql = sql + " and livehvstsoldcatperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by livehvstsoldcatperiods.live_cod,livestoc.live_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if((lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(0).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            PProductData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(0).toString();
            gendata.pDesc = query.value(1).toString();
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }

    pos_end = lista.count();

    QList<TchartSeries> series;
    for(pos = 0 ; pos < pos_end ; pos++)
    {
        TchartSeries serie;
        serie.name = lista[pos].pDesc;
        serie.value = lista[pos].pValue*1.0/tipcambio;
        series.append(serie);
    }
    QString htmlFile;
    htmlFile = createPieChart("eco01","Annual income from sale of produce by product",sysCurrSymbol,series,ui->view1);
    if (!htmlFile.isEmpty())
    {
        ui->view1->load(QUrl(htmlFile));
        ui->view1->show();
    }

    classesModel *model = new classesModel(this);
    model->loadClasses(series);
    ui->tableView_2->setModel(model);

    ui->tableView_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);


}

void economics::createChart2()
{
    QSqlQuery query(dataBase);
    QList<POutputData> lista;
    //
    QString sql;
    QString outdesc;
    int pos;
    int pos2;
    int ind;
    float totals[6];
    QString ptype;
    QString table;
    QString campo;
    int pos_end;



      //Crops products
    ptype = "C";
    sql = "select crpprodmansoldperiods.crop_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum((crpprodmansoldperiods.qty * crpprodmansoldperiods.pri)";
    sql = sql + ") as total,";
    sql = sql + "sum(crpprodmanperiods.qty";
    sql = sql + ") as hhcons";
    sql = sql + " from crpprodmansoldperiods,crop,crpprodmanperiods";
    sql = sql + " where crpprodmansoldperiods.crop_cod = crop.crop_cod";
    sql = sql + " and crpprodmansoldperiods.sys_cod = crpprodmanperiods.sys_cod";
    sql = sql + " and crpprodmansoldperiods.plot_cod = crpprodmanperiods.plot_cod";
    sql = sql + " and crpprodmansoldperiods.numseason = crpprodmanperiods.numseason";
    sql = sql + " and crpprodmansoldperiods.crop_cod = crpprodmanperiods.crop_cod";
    sql = sql + " and crpprodmansoldperiods.acty_cod = crpprodmanperiods.acty_cod";
    sql = sql + " and crpprodmansoldperiods.prod_cod = crpprodmanperiods.prod_cod";
    sql = sql + " and crpprodmansoldperiods.period_id = crpprodmanperiods.period_id";
    sql = sql + " and crpprodmansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by crpprodmansoldperiods.crop_cod,crop.crop_des";
    //qDebug() << sql;
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if( ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            if( query.value(getFieldIndex(query,"hhcons")).toFloat() > 0)
                gendata.pFood = true;
            else
                gendata.pFood = false;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"total")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"total")).toFloat();
            if( (lista[ind].pFood == false) && (query.value(getFieldIndex(query,"hhcons")).toFloat() > 0))
                lista[ind].pFood = true;
        }

    }

    //Crops Forages
    sql = "select crpforamansoldperiods.crop_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum(crpforamansoldperiods.qty * crpforamansoldperiods.pri";
    sql = sql + ") as total,";
    sql = sql + "sum(crpforamanperiods.qty";
    sql = sql + ") as hhcons";
    sql = sql + " from crpforamansoldperiods,crpforamanperiods,crop";
    sql = sql + " where crpforamansoldperiods.crop_cod = crop.crop_cod";
    sql = sql + " and crpforamansoldperiods.sys_cod = crpforamanperiods.sys_cod";
    sql = sql + " and crpforamansoldperiods.plot_cod = crpforamanperiods.plot_cod";
    sql = sql + " and crpforamansoldperiods.numseason = crpforamanperiods.numseason";
    sql = sql + " and crpforamansoldperiods.crop_cod = crpforamanperiods.crop_cod";
    sql = sql + " and crpforamansoldperiods.acty_cod = crpforamanperiods.acty_cod";
    sql = sql + " and crpforamansoldperiods.fora_cod = crpforamanperiods.fora_cod";
    sql = sql + " and crpforamansoldperiods.period_id = crpforamanperiods.period_id";
    sql = sql + " and crpforamansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by crpforamansoldperiods.crop_cod,crop.crop_des";
    //qDebug() << sql;
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count() ;
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if( ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            if( query.value(getFieldIndex(query,"hhcons")).toFloat() > 0)
                gendata.pFood = true;
            else
                gendata.pFood = false;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"total")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"total")).toFloat();
            if( (lista[ind].pFood == false) && (query.value(getFieldIndex(query,"hhcons")).toFloat() > 0))
                lista[ind].pFood = true;
        }

    }

    //Vegetable products
    sql = "select vegprodmansoldperiods.veg_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum((vegprodmansoldperiods.qty *  vegprodmansoldperiods.pri)";
    sql = sql + ") as total,";
    sql = sql + "sum(vegprodmanperiods.qty";
    sql = sql + ") as hhcons";
    sql = sql + " from vegprodmansoldperiods,vegprodmanperiods,crop";
    sql = sql + " where vegprodmansoldperiods.veg_cod = crop.crop_cod";
    sql = sql + " and vegprodmansoldperiods.sys_cod = vegprodmanperiods.sys_cod";
    sql = sql + " and vegprodmansoldperiods.plot_cod = vegprodmanperiods.plot_cod";
    sql = sql + " and vegprodmansoldperiods.numseason = vegprodmanperiods.numseason";
    sql = sql + " and vegprodmansoldperiods.crop_cod = vegprodmanperiods.crop_cod";
    sql = sql + " and vegprodmansoldperiods.veg_cod = vegprodmanperiods.veg_cod";
    sql = sql + " and vegprodmansoldperiods.acty_cod = vegprodmanperiods.acty_cod";
    sql = sql + " and vegprodmansoldperiods.prod_cod = vegprodmanperiods.prod_cod";
    sql = sql + " and vegprodmansoldperiods.period_id = vegprodmanperiods.period_id";
    sql = sql + " and vegprodmansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by vegprodmansoldperiods.veg_cod,crop.crop_des";
    //qDebug() << sql;
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if( ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            if( query.value(getFieldIndex(query,"hhcons")).toFloat() > 0)
                gendata.pFood = true;else
                gendata.pFood = false;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"total")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"total")).toFloat();
            if( (lista[ind].pFood == false) && (query.value(getFieldIndex(query,"hhcons")).toFloat() > 0))
                lista[ind].pFood = true;
        }

    }


    //Vegetable forages
    sql = "select vegforamansoldperiods.veg_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum((vegforamansoldperiods.qty * vegforamansoldperiods.pri)";
    sql = sql + ") as total,";
    sql = sql + "sum(vegforamanperiods.qty";
    sql = sql + ") as hhcons";
    sql = sql + " from vegforamansoldperiods,vegforamanperiods,crop";
    sql = sql + " where vegforamansoldperiods.veg_cod = crop.crop_cod";
    sql = sql + " and vegforamansoldperiods.sys_cod = vegforamanperiods.sys_cod";
    sql = sql + " and vegforamansoldperiods.plot_cod = vegforamanperiods.plot_cod";
    sql = sql + " and vegforamansoldperiods.numseason = vegforamanperiods.numseason";
    sql = sql + " and vegforamansoldperiods.crop_cod = vegforamanperiods.crop_cod";
    sql = sql + " and vegforamansoldperiods.veg_cod = vegforamanperiods.veg_cod";
    sql = sql + " and vegforamansoldperiods.acty_cod = vegforamanperiods.acty_cod";
    sql = sql + " and vegforamansoldperiods.fora_cod = vegforamanperiods.fora_cod";
    sql = sql + " and vegforamansoldperiods.period_id = vegforamanperiods.period_id";
    sql = sql + " and vegforamansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by vegforamansoldperiods.veg_cod,crop.crop_des";
    //qDebug() << sql;
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if( ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            if( query.value(getFieldIndex(query,"hhcons")).toFloat() > 0)
                gendata.pFood = true;else
                gendata.pFood = false;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"total")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"total")).toFloat();
            if( (lista[ind].pFood == false) && (query.value(getFieldIndex(query,"hhcons")).toFloat() > 0))
                lista[ind].pFood = true;
        }

    }


    ptype = "P";
    //Grass products
    sql = "select grassprodmansoldperiods.crop_cod as pcode,pastures.crop_des as pdesc,";
    sql = sql + "sum((grassprodmansoldperiods.qty * grassprodmansoldperiods.pri)";
    sql = sql + ") as total,";
    sql = sql + "sum(grassprodmanperiods.qty";
    sql = sql + ") as hhcons";
    sql = sql + " from grassprodmansoldperiods,grassprodmanperiods,pastures";
    sql = sql + " where grassprodmansoldperiods.crop_cod = pastures.crop_cod";
    sql = sql + " and grassprodmansoldperiods.sys_cod = grassprodmanperiods.sys_cod";
    sql = sql + " and grassprodmansoldperiods.plot_cod = grassprodmanperiods.plot_cod";
    sql = sql + " and grassprodmansoldperiods.numpadd = grassprodmanperiods.numpadd";
    sql = sql + " and grassprodmansoldperiods.crop_cod = grassprodmanperiods.crop_cod";
    sql = sql + " and grassprodmansoldperiods.acty_cod = grassprodmanperiods.acty_cod";
    sql = sql + " and grassprodmansoldperiods.prod_cod = grassprodmanperiods.prod_cod";
    sql = sql + " and grassprodmansoldperiods.period_id = grassprodmanperiods.period_id";
    sql = sql + " and grassprodmansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by grassprodmansoldperiods.crop_cod,pastures.crop_des";
    //qDebug() << sql;
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if( ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            if( query.value(getFieldIndex(query,"hhcons")).toFloat() > 0)
                gendata.pFood = true;else
                gendata.pFood = false;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"total")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"total")).toFloat();
            if( (lista[ind].pFood == false) && (query.value(getFieldIndex(query,"hhcons")).toFloat() > 0))
                lista[ind].pFood = true;
        }
    }


    //Grass forages
    sql = "select grassforamansoldperiods.crop_cod as pcode,pastures.crop_des as pdesc,";
    sql = sql + "sum((grassforamansoldperiods.qty * grassforamansoldperiods.pri)";
    sql = sql + ") as total,";
    sql = sql + "sum(grassforamanperiods.qty";
    sql = sql + ") as hhcons";
    sql = sql + " from grassforamansoldperiods,grassforamanperiods,pastures";
    sql = sql + " where grassforamansoldperiods.crop_cod = pastures.crop_cod";
    sql = sql + " and grassforamansoldperiods.sys_cod = grassforamanperiods.sys_cod";
    sql = sql + " and grassforamansoldperiods.plot_cod = grassforamanperiods.plot_cod";
    sql = sql + " and grassforamansoldperiods.numpadd = grassforamanperiods.numpadd";
    sql = sql + " and grassforamansoldperiods.crop_cod = grassforamanperiods.crop_cod";
    sql = sql + " and grassforamansoldperiods.acty_cod = grassforamanperiods.acty_cod";
    sql = sql + " and grassforamansoldperiods.fora_cod = grassforamanperiods.fora_cod";
    sql = sql + " and grassforamansoldperiods.period_id = grassforamanperiods.period_id";
    sql = sql + " and grassforamansoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by grassforamansoldperiods.crop_cod,pastures.crop_des";
    //qDebug() << sql;
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == ptype) &&
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if( ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            if( query.value(getFieldIndex(query,"hhcons")).toFloat() > 0)
                gendata.pFood = true;
            else
                gendata.pFood = false;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"total")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"total")).toFloat();
            if( (lista[ind].pFood == false) && (query.value(getFieldIndex(query,"hhcons")).toFloat() > 0))
                lista[ind].pFood = true;
        }

    }



    //Livestock with weight
    sql = "select livehvstsoldperiods.prod_cod as pcode,products.prod_des as pdesc,";
    sql = sql + "sum((livehvstsoldperiods.qty * livehvstsoldperiods.pri)";
    sql = sql + ") as total";
    sql = sql + " from livehvstsoldperiods,products,lvproducts";
    sql = sql + " where livehvstsoldperiods.live_cod = lvproducts.live_cod";
    sql = sql + " and livehvstsoldperiods.prod_cod = lvproducts.prod_cod";
    sql = sql + " and livehvstsoldperiods.prod_cod = products.prod_cod";
    sql = sql + " and lvproducts.weight = 'Y'";
    sql = sql + " and livehvstsoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by livehvstsoldperiods.prod_cod,products.prod_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == "L"))
            {
                ind = pos;
                break;
            }
        }
        if( ind == -1)
        {
            POutputData gendata;
            gendata.ptype = "L";
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }


    //Livestock with quantity
    sql = "select livehvstsoldperiods.prod_cod as pcode,products.prod_des as pdesc,";
    sql = sql + "sum((livehvstsoldperiods.qty * livehvstsoldperiods.pri)";
    sql = sql + ") as total";
    sql = sql + " from livehvstsoldperiods,products,lvproducts";
    sql = sql + " where livehvstsoldperiods.live_cod = lvproducts.live_cod";
    sql = sql + " and livehvstsoldperiods.prod_cod = lvproducts.prod_cod";
    sql = sql + " and livehvstsoldperiods.prod_cod = products.prod_cod";
    sql = sql + " and lvproducts.weight = 'N'";
    sql = sql + " and livehvstsoldperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by livehvstsoldperiods.prod_cod,products.prod_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == "L"))
            {
                ind = pos;
                break;
            }
        }
        if( ind == -1)
        {
            POutputData gendata;
            gendata.ptype = "L";
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }


    //Livestock by category with weight
    sql = "select livehvstsoldcatperiods.prod_cod as pcode,products.prod_des as pdesc,";
    sql = sql + "sum((livehvstsoldcatperiods.qty * livehvstsoldcatperiods.pri)";
    sql = sql + ") as total";
    sql = sql + " from livehvstsoldcatperiods,products,lvproducts";
    sql = sql + " where livehvstsoldcatperiods.live_cod = lvproducts.live_cod";
    sql = sql + " and livehvstsoldcatperiods.prod_cod = lvproducts.prod_cod";
    sql = sql + " and livehvstsoldcatperiods.prod_cod = products.prod_cod";
    sql = sql + " and lvproducts.weight = 'Y'";
    sql = sql + " and livehvstsoldcatperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by livehvstsoldcatperiods.prod_cod,products.prod_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == "L"))
            {
                ind = pos;
                break;
            }
        }
        if( ind == -1)
        {
            POutputData gendata;
            gendata.ptype = "L";
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }


    //Livestock by category with quantity
    sql = "select livehvstsoldcatperiods.prod_cod as pcode,products.prod_des as pdesc,";
    sql = sql + "sum((livehvstsoldcatperiods.qty * livehvstsoldcatperiods.pri)";
    sql = sql + ") as total";
    sql = sql + " from livehvstsoldcatperiods,products,lvproducts";
    sql = sql + " where livehvstsoldcatperiods.live_cod = lvproducts.live_cod";
    sql = sql + " and livehvstsoldcatperiods.prod_cod = lvproducts.prod_cod";
    sql = sql + " and livehvstsoldcatperiods.prod_cod = products.prod_cod";
    sql = sql + " and lvproducts.weight = 'N'";
    sql = sql + " and livehvstsoldcatperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by livehvstsoldcatperiods.prod_cod,products.prod_des";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    while(query.next())
    {
        ind = -1;
        pos_end = lista.count();
        for(pos = 0 ; pos < pos_end ; pos++)
        {
            if( (lista[pos].ptype == "L"))
            {
                ind = pos;
                break;
            }
        }
        if( ind == -1)
        {
            POutputData gendata;
            gendata.ptype = "L";
            gendata.pValue = query.value(2).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(2).toFloat();
        }

    }

    pos_end = lista.count();
    for(pos = 0 ; pos < pos_end ; pos++)
    {
        lista[pos].pFeed = false;
    }

    pos_end = lista.count();
    int pos2_end;
    for(pos = 0 ; pos < pos_end ; pos++)
    {
        if( lista[pos].ptype != "L")
        {
            if( lista[pos].ptype == "C")
            {
                pos2_end = 5 ;
                for(pos2 = 1 ; pos2 < pos2_end ; pos2++)
                {
                    switch(pos2) {
                    case 1:  {
                        table = "crpprodmanfeedperiods";
                        campo = "crop_cod";
                        break;
                    }
                    case 2:  {
                        table = "crpforamanfeedperiods";
                        campo = "crop_cod";
                        break;
                    }
                    case 3:  {
                        table = "vegprodmanfeedperiods";
                        campo = "veg_cod";
                        break;
                    }
                    case 4:  {
                        table = "vegforamanfeedperiods";
                        campo = "veg_cod";
                        break;
                    }
                    }
                    sql = "select " + campo + ",";
                    sql = sql + "sum(qty";
                    sql = sql + ") as tfeed";
                    sql = sql + " from " + table;
                    sql = sql + " where sys_cod = '" + sistema +  "'";
                    sql = sql + " and " + campo + " = '" + lista[pos].pCode +  "'";
                    sql = sql + " group by " + campo;
                    if (!query.exec(sql))
                    {
                        qDebug() << "Error: " << query.lastError().databaseText();
                        qDebug() << "Executing: " << sql;
                    }
                    if (query.first())
                        if( (lista[pos].pFeed == false) && (query.value(1).toFloat() > 0))
                            lista[pos].pFeed = true;


                }
            }
            else
            {
                pos2_end = 3 ;
                for(pos2 = 1 ; pos2 < pos2_end ; pos2++)
                {
                    switch(pos2)
                    {
                    case 1:
                    {
                        table = "grassprodmanfeedperiods";
                        campo = "crop_cod";
                        break;
                    }
                    case 2:
                    {
                        table = "grassforamanfeedperiods";
                        campo = "crop_cod";
                        break;
                    }
                    }
                    sql = "select " + campo + ",";
                    sql = sql + "sum(qty";
                    sql = sql + ") as tfeed";
                    sql = sql + " from " + table;
                    sql = sql + " where sys_cod = '" + sistema +  "'";
                    sql = sql + " and " + campo + " = '" + lista[pos].pCode +  "'";
                    sql = sql + " group by " + campo;
                    if (!query.exec(sql))
                    {
                        qDebug() << "Error: " << query.lastError().databaseText();
                        qDebug() << "Executing: " << sql;
                    }
                    if (query.first())
                    if( (lista[pos].pFeed == false) && (query.value(1).toFloat() > 0))
                        lista[pos].pFeed = true;
                }
            }
        }
    }
    pos_end = 6 ;
    for(pos = 1 ; pos < pos_end ; pos++)
        totals[pos] = 0;
    pos_end = lista.count();
    for(pos = 0 ; pos < pos_end ; pos++)
    {
        if( lista[pos].ptype != "L")
        {
            if( (lista[pos].pFood == false) && (lista[pos].pFeed == false))
                totals[1] = totals[1] + lista[pos].pValue;
            if( (lista[pos].pFood == true) && (lista[pos].pFeed == false))
                totals[2] = totals[2] + lista[pos].pValue;
            if( (lista[pos].pFood == false) && (lista[pos].pFeed == true))
                totals[3] = totals[3] + lista[pos].pValue;
            if( (lista[pos].pFood == true) && (lista[pos].pFeed == true))
                totals[4] = totals[4] + lista[pos].pValue;
        }
        else
            totals[5] = totals[5] + lista[pos].pValue;
    }


    QList<TchartSeries> series;

    //Creating the Graphic
    pos_end = 6 ;
    for(pos = 1 ; pos < pos_end ; pos++)
    {
        switch(pos) {
        case 1:  outdesc = "Cash Crop"; break;
        case 2:  outdesc = "Food Crop"; break;
        case 3:  outdesc = "Feed Crop"; break;
        case 4:  outdesc = "Food-Feed Crop"; break;
        case 5:  outdesc = "Livestock"; break;
        }
        TchartSeries serie;
        serie.name = outdesc;
        serie.value = totals[pos]*1.0/tipcambio;
        series.append(serie);

    }
    QString htmlFile;
    htmlFile = createPieChart("eco02","Annual income from sale of produce by output",sysCurrSymbol,series,ui->view2);
    if (!htmlFile.isEmpty())
    {
        ui->view2->load(QUrl(htmlFile));
        ui->view2->show();
    }

    classesModel *model = new classesModel(this);
    model->loadClasses(series);
    ui->tableView_3->setModel(model);

    ui->tableView_3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

void economics::createChart3()
{
    QList<TchartSeries> series;
    TchartSeries serie;

    serie.value = anexpsrc[ 1 ]*1.0/tipcambio;
    serie.name = "Livestock keeping";
    series.append(serie);


    serie.value = anexpsrc[ 2 ]*1.0/tipcambio;
    serie.name = "Cropping cost";
    series.append(serie);

    serie.value = anexpsrc[ 3 ]*1.0/tipcambio;
    serie.name = "Food for household";
    series.append(serie);


    serie.value = anexpsrc[ 4 ]*1.0/tipcambio;
    serie.name = "Other household expenses";
    series.append(serie);

    QString htmlFile;
    htmlFile = createPieChart("eco03","Annual expenditure sources",sysCurrSymbol,series,ui->view3);
    if (!htmlFile.isEmpty())
    {
        ui->view3->load(QUrl(htmlFile));
        ui->view3->show();
    }

    classesModel *model = new classesModel(this);
    model->loadClasses(series);
    ui->tableView_4->setModel(model);

    ui->tableView_4->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_4->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

}

void economics::createChart4()
{
    QStringList categories;
    QList<TchartGroups> groups;

    QList<TchartSeries> series;


    QString sql;
    QSqlQuery query(dataBase);
    sql = "SELECT period_des FROM periods";
    query.exec(sql);
    while (query.next())
    {
        categories.append(query.value(0).toString());
    }
    TchartGroups group;
    group.code = "INC";
    group.name = "Income";
    group.color = QColor("BLUE");
    groups.append(group);

    group.code = "EXP";
    group.name = "Expensed";
    group.color = QColor("RED");
    groups.append(group);

    int nperiods;
    sql = "SELECT project_nperiods FROM project";
    query.exec(sql);
    query.first();
    nperiods = query.value(0).toInt();
    int pos;
    float total;
    for (pos = 1; pos <= nperiods; pos++)
    {
        total = inscrops[pos-1];
        total = total + inslives[pos-1];
        total = total + insothers[pos-1];
        TchartSeries serie;
        serie.group = "INC";
        serie.name = "Income - " + categories[pos-1];
        serie.value = total*1.0/tipcambio;
        series.append(serie);
    }
    for (pos = 1; pos <= nperiods; pos++)
    {
        total = outscrops[pos-1];
        total = total + outslives[pos-1];
        total = total + outsothers[pos-1];
        TchartSeries serie;
        serie.group = "EXP";
        serie.name = "Expense - " + categories[pos-1];
        serie.value = total*1.0/tipcambio;
        series.append(serie);
    }

    QString htmlFile;
    htmlFile = createLineChart("eco04","Income and expenses per period",sysCurrSymbol,categories,groups,series,ui->view4);
    if (!htmlFile.isEmpty())
    {
        ui->view4->load(QUrl(htmlFile));
        ui->view4->show();
    }

    classesModel *model = new classesModel(this);
    model->loadClasses(series);
    ui->tableView_5->setModel(model);

    ui->tableView_5->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_5->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

}

void economics::createChart5()
{
    float totalCrops;
    float totalLive;
    float totalOther;
    float total;

    QString sql;
    QSqlQuery query(dataBase);
    int pos;


    int nperiods;
    sql = "SELECT project_nperiods FROM project";
    query.exec(sql);
    query.first();
    nperiods = query.value(0).toInt();

    totalCrops = 0;
    totalLive = 0;
    totalOther = 0;
    total = 0;
    for (pos = 1; pos <= nperiods; pos++)
    {
        totalCrops = totalCrops + (inscrops[pos-1] - outscrops[pos-1]);
        totalLive = totalLive + (inslives[pos-1] - outslives[pos-1]);
        totalOther = totalOther + (insothers[pos-1] - outsothers[pos-1]);
    }
    total = totalCrops + totalLive + totalOther;

    QList<TchartSeries> series;
    TchartSeries serie;
    serie.name = "Crops";
    serie.value = totalCrops*1.0/tipcambio;
    series.append(serie);

    serie.name = "Livestock";
    serie.value = totalLive*1.0/tipcambio;
    series.append(serie);

    serie.name = "Other";
    serie.value = totalOther*1.0/tipcambio;
    series.append(serie);

    serie.name = "Total";
    serie.value = total*1.0/tipcambio;
    series.append(serie);

    QString htmlFile;
    htmlFile = createBarChart("eco05","Annual expenditure sources",sysCurrSymbol,series,ui->view5);
    if (!htmlFile.isEmpty())
    {
        ui->view5->load(QUrl(htmlFile));
        ui->view5->show();
    }

    classesModel *model = new classesModel(this);
    model->loadClasses(series);
    ui->tableView_6->setModel(model);

    ui->tableView_6->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_6->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

}

void economics::getoutscrops(QString system, QString tipo)
{
    float valor;
    int mes;

    QString sqlSelect;
    QString sqlFrom;
    QString sqlWhere;
    QSqlQuery reader(dataBase);
    QString sql;

    sqlFrom = "";
    sqlSelect = "SELECT * ";
    //Labour
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    if(  tipo == "C" )
        sqlFrom = " FROM  croplabour";
    if(  tipo == "V" )
        sqlFrom = " FROM  veglabour";
    if(  tipo == "G" )
        sqlFrom = " FROM  grasslabour";
    if(  tipo == "CP" )
        sqlFrom = " FROM  crpprodlabour";
    if(  tipo == "CF" )
        sqlFrom = " FROM  crpforalabour";
    if(  tipo == "VP" )
        sqlFrom = " FROM  vegprodlabour";
    if(  tipo == "VF" )
        sqlFrom = " FROM  vegforalabour";
    if(  tipo == "GP" )
        sqlFrom = " FROM  grassprodlabour";
    if(  tipo == "GF" )
        sqlFrom = " FROM  grassforalabour";
    if (!sqlFrom.isEmpty())
    {
        sql = sqlSelect + sqlFrom + sqlWhere;
        if (!reader.exec(sql))
        {
            qDebug() << "Error: " << reader.lastError().databaseText();
            qDebug() << "Executing: " << sql;
        }

        while(reader.next())
        {
            valor = reader.value(getFieldIndex(reader, "frequency")).toFloat();
            valor = valor * reader.value(getFieldIndex(reader, "hoursday")).toFloat();
            valor = valor * reader.value(getFieldIndex(reader, "pperhour")).toFloat();
            mes = reader.value(getFieldIndex(reader, "nmonth")).toInt();
            outscrops[ mes-1 ] = outscrops[ mes-1 ] + valor;

        }
    }
    sqlFrom = "";
    //Machinery
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    if(  tipo == "C" )
        sqlFrom = " FROM  cropmantim";
    if(  tipo == "V" )
        sqlFrom = " FROM  vegmantim";
    if(  tipo == "P" )
        sqlFrom = " FROM  grassmantim";
    if(  tipo == "CP" )
        sqlFrom = " FROM  crpprodmantim";
    if(  tipo == "CF" )
        sqlFrom = " FROM  crpforamantim";
    if(  tipo == "VP" )
        sqlFrom = " FROM  vegprodmantim";
    if(  tipo == "VF" )
        sqlFrom = " FROM  vegforamantim";
    if(  tipo == "GP" )
        sqlFrom = " FROM  grassprodmantim";
    if(  tipo == "GF" )
        sqlFrom = " FROM  grassforamantim";
    if(!sqlFrom.isEmpty())
    {
        sql = sqlSelect + sqlFrom + sqlWhere;
        if (!reader.exec(sql))
        {
            qDebug() << "Error: " << reader.lastError().databaseText();
            qDebug() << "Executing: " << sql;
        }
        while(reader.next())
        {
            valor = reader.value(getFieldIndex(reader, "freqmach")).toFloat();
            valor = valor * reader.value(getFieldIndex(reader, "price")).toFloat();
            mes = reader.value(getFieldIndex(reader, "nmonth")).toInt();
            outscrops[ mes-1 ] = outscrops[ mes-1 ] + valor;
            anexpsrc[ 2 ] = anexpsrc[ 2 ] + valor;

        }
    }
    sqlFrom = "";
    //Machinery for fertilizers
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    if(  tipo == "C" )
        sqlFrom = " FROM  cropmanfert";
    if(  tipo == "V" )
        sqlFrom = " FROM  vegmanfert";
    if(  tipo == "P" )
        sqlFrom = " FROM  grassmanfert";
    if(!sqlFrom.isEmpty())
    {
        sql = sqlSelect + sqlFrom + sqlWhere;
        if (!reader.exec(sql))
        {
            qDebug() << "Error: " << reader.lastError().databaseText();
            qDebug() << "Executing: " << sql;
        }
        while(reader.next())
        {
            valor = reader.value(getFieldIndex(reader, "freqmach")).toFloat();
            valor = valor * reader.value(getFieldIndex(reader, "price")).toFloat();
            mes = reader.value(getFieldIndex(reader, "nmonth")).toInt();
            outscrops[ mes-1 ] = outscrops[ mes-1 ] + valor;
            anexpsrc[ 2 ] = anexpsrc[ 2 ] + valor;

        }
    }
    sqlFrom = "";
    //Purchased Inputs
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    if(  tipo == "C" )
        sqlFrom = " FROM  cropmanperiods";
    if(  tipo == "V" )
        sqlFrom = " FROM  vegmanperiods";
    if(  tipo == "P" )
        sqlFrom = " FROM  grassmanperiods";
    if(!sqlFrom.isEmpty())
    {
        sql = sqlSelect + sqlFrom + sqlWhere;
        if (!reader.exec(sql))
        {
            qDebug() << "Error: " << reader.lastError().databaseText();
            qDebug() << "Executing: " << sql;
        }
        while(reader.next())
        {

            valor = reader.value(getFieldIndex(reader, "QTY")).toFloat();
            valor = valor * reader.value(getFieldIndex(reader, "PRI")).toFloat();
            mes = reader.value(getFieldIndex(reader, "period_id")).toInt();
            outscrops[ mes-1 ] = outscrops[ mes-1 ] + valor;
            anexpsrc[ 2 ] = anexpsrc[ 2 ] + valor;
        }
    }
    sqlFrom = "";

    //Purchased Inputs for Fetilizers
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    if(  tipo == "C" )
        sqlFrom = " FROM  cropmanprchfertperiods";
    if(  tipo == "V" )
        sqlFrom = " FROM  vegmanprchfertperiods";
    if(  tipo == "P" )
        sqlFrom = " FROM  grassmanprchfertperiods";
    if(!sqlFrom.isEmpty())
    {
        sql = sqlSelect + sqlFrom + sqlWhere;
        if (!reader.exec(sql))
        {
            qDebug() << "Error: " << reader.lastError().databaseText();
            qDebug() << "Executing: " << sql;
        }
        while(reader.next())
        {
            valor = reader.value(getFieldIndex(reader, "QTY")).toFloat();
            valor = valor * reader.value(getFieldIndex(reader, "PRI")).toFloat();
            mes = reader.value(getFieldIndex(reader, "period_id")).toInt();
            outscrops[ mes-1 ] = outscrops[ mes-1 ] + valor;
            anexpsrc[ 2 ] = anexpsrc[ 2 ] + valor;
        }
    }

}

void economics::getoutslvandot(QString system)
{
    float valor;
    int mes;
    QSqlQuery reader(dataBase);
    QString sqlSelect;
    QString sqlFrom;
    QString sqlWhere;
    QString sql;


    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    sqlFrom = " FROM  liveavailresperiods";
    sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {

        valor = reader.value(getFieldIndex(reader,"QTY") ).toFloat();
        valor = valor * reader.value(getFieldIndex(reader,"PRI")).toFloat();
        mes = reader.value(getFieldIndex(reader,"period_id")).toInt();
        outslives[ mes-1 ] = outslives[ mes-1 ] + valor;
        anexpsrc[ 1 ] = anexpsrc[ 1 ] + valor;
    }

    //Manure products
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    sqlFrom = " FROM  mnrheapprodsperiods";
    sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        valor = reader.value(getFieldIndex(reader,"QTY")).toFloat();
        valor = valor * reader.value(getFieldIndex(reader,"PRI")).toFloat();
        mes = reader.value(getFieldIndex(reader,"period_id")).toInt();
        outsothers[ mes-1 ] = outsothers[ mes-1 ] + valor;
        anexpsrc[ 4 ] = anexpsrc[ 4 ] + valor;
    }

    //Livestock  other expenses
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    sqlFrom = " FROM  otherprodperiods";
    sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        valor = reader.value(getFieldIndex(reader,"PRI")).toFloat();
        mes = reader.value(getFieldIndex(reader,"period_id")).toInt();
        outslives[ mes-1 ] = outslives[ mes-1 ] + valor;
        anexpsrc[ 1 ] = anexpsrc[ 1 ] + valor;
    }

    //Livestock labour
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    sqlFrom = " FROM  livelabour";
    sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        valor = reader.value(getFieldIndex(reader,"frequency" )).toFloat();
        valor = valor * reader.value(getFieldIndex(reader,"hoursday" )).toFloat();
        valor = valor * reader.value(getFieldIndex(reader,"pperhour" )).toFloat();
        mes = reader.value(getFieldIndex(reader,"nmonth" )).toInt();
        anexpsrc[ 1 ] = anexpsrc[ 1 ] + valor;
        outslives[ mes-1 ] = outslives[ mes-1 ] + valor;

    }

    //Livestock harvest labour;
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    sqlFrom = " FROM  livehvstlabour";
    sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        valor = reader.value(getFieldIndex(reader,"frequency" )).toFloat();
        valor = valor * reader.value(getFieldIndex(reader,"hoursday" )).toFloat();
        valor = valor * reader.value(getFieldIndex(reader,"pperhour" )).toFloat();
        mes = reader.value(getFieldIndex(reader,"nmonth" )).toInt();
                outslives[ mes-1 ] = outslives[ mes-1 ] + valor;
        anexpsrc[ 1 ] = anexpsrc[ 1 ] + valor;

    }

    //Otherlabour
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    sqlFrom = " FROM  otherlabour";
    sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        valor = reader.value(getFieldIndex(reader,"frequency" )).toFloat();
        valor = valor * reader.value(getFieldIndex(reader,"hoursday" )).toFloat();
        valor = valor * reader.value(getFieldIndex(reader,"pperhour" )).toFloat();
        mes = reader.value(getFieldIndex(reader,"nmonth" )).toInt();
        outsothers[ mes-1 ] = outsothers[ mes-1 ] + valor;
        anexpsrc[ 4 ] = anexpsrc[ 4 ] + valor;
    }

    //Other household expenses
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    sqlFrom = " FROM  otherhhexpperiods";
    sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        valor = reader.value(getFieldIndex(reader,"PRI") ).toFloat();
        mes = reader.value(getFieldIndex(reader,"period_id" )).toInt();
        outsothers[ mes-1 ] = outsothers[ mes-1 ] + valor;
        anexpsrc[ 4 ] = anexpsrc[ 4 ] + valor;
    }

    //Other household expenses food
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    sqlFrom = " FROM  otherhhcpperiods";
    sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        valor = (reader.value(getFieldIndex(reader,"QTP")).toFloat() * reader.value(getFieldIndex(reader,"PRI")).toFloat());
        mes = reader.value(getFieldIndex(reader,"period_id" )).toInt();
        outsothers[ mes-1 ] = outsothers[ mes-1 ] + valor;
        anexpsrc[ 3 ] = anexpsrc[ 3 ] + valor;
    }

    //Other household incomes
    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    sqlFrom = " FROM  otherincomeperiods";
    sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        valor = reader.value(getFieldIndex(reader,"PRI")).toFloat();
        mes = reader.value(getFieldIndex(reader,"period_id" )).toInt();
        insothers[ mes-1 ] = insothers[ mes-1 ] + valor;
    }
}

void economics::getinscrops(QString system, QString tipo)
{
    float valor;
    int mes;
    QSqlQuery reader(dataBase);

    QString sql;
    QString sqlSelect;
    QString sqlFrom;
    QString sqlWhere;


    sqlSelect = "SELECT * ";
    sqlWhere = " WHERE sys_cod = '" + system + "'";
    //Sold crops
    if(  tipo == "CP" )
        sqlFrom = " FROM  crpprodmansoldperiods";
    if(  tipo == "CF" )
        sqlFrom = " FROM  crpforamansoldperiods";
    if(  tipo == "VP" )
        sqlFrom = " FROM  vegprodmansoldperiods";
    if(  tipo == "VF" )
        sqlFrom = " FROM  vegforamansoldperiods";
    if(  tipo == "GP" )
        sqlFrom = " FROM  grassprodmansoldperiods";
    if(  tipo == "GF" )
        sqlFrom = " FROM  grassforamansoldperiods";
    sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        valor = reader.value(getFieldIndex(reader,"QTY")).toFloat();
        valor = valor * reader.value(getFieldIndex(reader,"PRI") ).toFloat();
        mes = reader.value(getFieldIndex(reader,"period_id" )).toInt();
        inscrops[ mes-1 ] = inscrops[ mes-1 ] + valor;
    }
}

void economics::getinslives(QString system)
{
  float valor;
  int mes;
  QSqlQuery reader(dataBase);

  QString sql;
  QString sqlSelect;
  QString sqlFrom;
  QString sqlWhere;

  sqlSelect = "SELECT * ";
  sqlWhere = " WHERE sys_cod = '" + system + "'";
  sqlFrom = " FROM  livehvstsoldperiods";
  sql = sqlSelect + sqlFrom + sqlWhere;
    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
  while(reader.next())
  {
      valor = reader.value(getFieldIndex(reader,"QTY")).toFloat();
      valor = valor * reader.value(getFieldIndex(reader,"PRI")).toFloat();
      mes = reader.value(getFieldIndex(reader,"period_id" )).toInt();
      inslives[ mes-1 ] = inslives[ mes-1 ] + valor;
  }

  //Sold livestock by category

  sqlSelect = "SELECT * ";
  sqlWhere = " WHERE sys_cod = '" + system + "'";
  sqlFrom = " FROM  livehvstsoldcatperiods";
  sql = sqlSelect + sqlFrom + sqlWhere;
  if (!reader.exec(sql))
  {
      qDebug() << "Error: " << reader.lastError().databaseText();
      qDebug() << "Executing: " << sql;
  }
  while(reader.next())
  {

      valor = reader.value(getFieldIndex(reader,"QTY")).toFloat();
      valor = valor * reader.value(getFieldIndex(reader,"PRI")).toFloat();
      mes = reader.value(getFieldIndex(reader,"period_id" )).toInt();
      inslives[ mes-1 ] = inslives[ mes-1 ] + valor;
  }

}

void economics::on_cmdclode_clicked()
{
    emit closeCalled();
}

void economics::on_cmdgen_clicked()
{
    if (ui->comboBox->count()==0)
        return;

    sistema = ui->comboBox->itemData(ui->comboBox->currentIndex(),Qt::UserRole).toString(); //"KE20040913ABNDMZCTB"

    float exRate;
    exRate = ui->comboBox->itemData(ui->comboBox->currentIndex(),Qt::UserRole+1).toFloat();
    if (exRate == 0)
        exRate = 1;

    if (ui->chkusd->isChecked())
        tipcambio = exRate;
    else
        tipcambio = 1;

    sysCurrSymbol = "Ksh";
    int pos;

    QString sql;
    QSqlQuery query(dataBase);
    int nperiods;
    sql = "SELECT project_nperiods FROM project";
    query.exec(sql);
    query.first();
    nperiods = query.value(0).toInt();

    inscrops.resize(nperiods);
    outscrops.resize(nperiods);
    inslives.resize(nperiods);
    outslives.resize(nperiods);
    insothers.resize(nperiods);
    outsothers.resize(nperiods);

    for (pos = 1; pos <= nperiods; pos++)
    {
        inscrops[pos-1] = 0;
        outscrops[pos-1] = 0;
        inslives[pos-1] = 0;
        outslives[pos-1] = 0;
        insothers[pos-1] = 0;
        outsothers[pos-1] = 0;
    }

    for (pos = 0; pos <= 4; pos++)
        anexpsrc[pos] = 0;


    getoutscrops(sistema,"C");
    getoutscrops(sistema,"V");
    getoutscrops(sistema,"G");
    getoutscrops(sistema,"CP");
    getoutscrops(sistema,"CF");
    getoutscrops(sistema,"VP");
    getoutscrops(sistema,"VF");
    getoutscrops(sistema,"GP");
    getoutscrops(sistema,"GF");
    getoutslvandot(sistema);

    getinscrops( sistema, "CP" );
    getinscrops( sistema, "CF" );
    getinscrops( sistema, "VP" );
    getinscrops( sistema, "VF" );
    getinscrops( sistema, "GF" );
    getinscrops( sistema, "GF" );
    getinslives(sistema);



    QStringList categories;
    sql = "SELECT period_des FROM periods";
    query.exec(sql);
    while (query.next())
    {
        categories.append(query.value(0).toString());
    }

    if (categories.count() > 0)
    {
        detailModel *detmodel = new detailModel(this);
        detmodel->setPeriods(categories);
        detmodel->setTipCambio(tipcambio);
        detmodel->setClasses(inscrops,outscrops,inslives,outslives,insothers,outsothers);
        ui->tableView->setModel(detmodel);

        for (pos = 0; pos <= detmodel->columnCount()-1;pos++)
        {
            ui->tableView->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        }
    }

    tabchanged(ui->tabWidget_2->currentIndex());
}

void economics::tabchanged(int pos)
{
    if (sistema.isEmpty())
        return;
    if (pos == 0)
    {
        createChart1();
    }
    if (pos == 1)
    {
        createChart2();
    }
    if (pos == 2)
    {
        createChart3();
    }
    if (pos == 3)
    {
        createChart4();
    }
    if (pos == 4)
    {
        createChart5();
    }
}
