#include "foodsecurity.h"
#include "ui_foodsecurity.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <IMPACTGlobal.h>
#include <qglobal.h>

classesModel2::classesModel2(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
}

int classesModel2::rowCount(const QModelIndex &) const
{
    return m_items.count();
}
int classesModel2::columnCount(const QModelIndex &) const
{
    return 2;
}
QVariant classesModel2::data(const QModelIndex &index, int role) const
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
bool classesModel2::setData(const QModelIndex &,const QVariant &,int)
{
    return false;
}

Qt::ItemFlags classesModel2::flags (const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
QVariant classesModel2::headerData(int section, Qt::Orientation orientation, int role) const
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

void classesModel2::loadClasses(QList<TchartSeries> classes)
{

    m_items.append(classes);
}

//*****************************************************************************
foodsecurity::foodsecurity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::foodsecurity)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);
}

foodsecurity::~foodsecurity()
{
    delete ui;
}

void foodsecurity::setDatabase(QSqlDatabase db)
{
    dataBase = db;

    QSqlQuery query(dataBase);
    QString sql;

    sql = "SELECT system.sys_cod,system.reg_desc,system.tipcambio,";
    sql = sql + "system.cnty_cod,country.currcode,country.currdes";
    sql = sql + " FROM system,country";
    sql = sql + " WHERE system.cnty_cod = country.cnty_cod";
    query.exec(sql);
    while (query.next())
    {
        ui->comboBox->addItem(query.value(1).toString());
        ui->comboBox->setItemData(ui->comboBox->count()-1,query.value(0).toString(),Qt::UserRole);
        ui->comboBox->setItemData(ui->comboBox->count()-1,query.value(2).toFloat(),Qt::UserRole+1);
        ui->comboBox->setItemData(ui->comboBox->count()-1,query.value(4).toString(),Qt::UserRole+2);
        ui->comboBox->setItemData(ui->comboBox->count()-1,query.value(5).toString(),Qt::UserRole+3);
    }
    if (ui->comboBox->count()> 0)
        ui->comboBox->setCurrentIndex(0);
}

int foodsecurity::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

QString foodsecurity::getTempPath()
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

QString foodsecurity::saveChart(QString chartName, QStringList html,QStringList)
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

QString foodsecurity::createPieChart(QString chartName, QString chartTitle, QString currSymbol,QList<TchartSeries> series, QWebView *view)
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
    html << "return '<b>'+ this.point.name +'</b>: '+ this.y +' " + currSymbol + "';";
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
    html << "return this.y +'" + currSymbol + "';";
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

QString foodsecurity::createBarChart(QString chartName, QString chartTitle, QString yAxisTitle,QList<TchartSeries> series,float minvalue, QWebView *view)
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
    html << "var chart;";
    html << "$(document).ready(function() {";
    html << "chart = new Highcharts.Chart({";
    html << "chart: {";
    html << "renderTo: 'container'";
    html << "},";
    html << "title: {";
    html << "text: '" + chartTitle + "'";
    html << "},";
    html << "xAxis: {";
    html << "categories: [";

    temp = "";
    for (pos = 0; pos <= series.count()-1;pos++)
        temp = temp + "'" + series[pos].name + "', ";
    temp = temp.left(temp.length()-2);
    html << temp;

    html << "]";
    html << "},";
    html << "tooltip: {";
    html << "enabled: true";
    html << "},";

    html << "yAxis: {";
    html << "title: {";
    html << "text: '" + yAxisTitle + "'}";
    html << "},";

    html << "plotOptions: {";
    html << "series: {";
    html << "marker: {";
    html << "enabled: false";
    html << "}";
    html << "}";
    html << "},";

    html << "series: [";
    html << "{";
    html << "type: 'column',";
    html << "data: [";

    temp = "";
    for (pos = 0; pos <= series.count()-1;pos++)
        temp = temp + QString::number(series[pos].value,'f',2) + ", ";
    temp = temp.left(temp.length()-2);
    html << temp;

    html << "],";
    html << "name: 'Periods'";
    html << "},{";
    html << "type: 'line',";
    html << "data: [";

    temp = "";
    for (pos = 0; pos <= series.count()-1;pos++)
        temp = temp + QString::number(minvalue,'f',2) + ", ";
    temp = temp.left(temp.length()-2);
    html << temp;

    html << "],";
    html << "name: 'Minimum required'";
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

void foodsecurity::createChart2()
{
    QSqlQuery query(dataBase);
    QList<POutputData> lista;

    QString sql; QString outdesc;
    int pos; int pos2;
    int ind;
    float totals[5];
    QString ptype; QString table; QString campo; QString campo2;
    int pos_end;



    if(ui->radioButton->isChecked() == true)
    {
        campo2 = "energy";
        //AdvListView1.Columns[1].Caption = "Energy (MJ)";
    }
    else
    {
        campo2 = "protein";
        //AdvListView1.Columns[1].Caption = "Protein (g)";
    }

    //Crops products
    ptype = "C";
    sql = "select crpprodmanperiods.crop_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum((crpprodmanperiods.qty * crpprodman." + campo2 + ")";
    sql = sql + ") as hhcons";
    sql = sql + " from crpprodmanperiods,crop,crpprodman";
    sql = sql + " where crpprodmanperiods.crop_cod = crop.crop_cod";

    sql = sql + " and crpprodman.sys_cod = crpprodmanperiods.sys_cod";
    sql = sql + " and crpprodman.plot_cod = crpprodmanperiods.plot_cod";
    sql = sql + " and crpprodman.numseason = crpprodmanperiods.numseason";
    sql = sql + " and crpprodman.crop_cod = crpprodmanperiods.crop_cod";
    sql = sql + " and crpprodman.acty_cod = crpprodmanperiods.acty_cod";
    sql = sql + " and crpprodman.prod_cod = crpprodmanperiods.prod_cod";

    sql = sql + " and crpprodmanperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by crpprodmanperiods.crop_cod,crop.crop_des";

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
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            gendata.pFood = true;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"hhcons")).toFloat();
            lista.append(gendata);

        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"hhcons")).toFloat();
        }

    }


    //Crops Forages
    sql = "select crpforamanperiods.crop_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum((crpforamanperiods.qty * crpforaman." + campo2 + ")";
    sql = sql + ") as hhcons";
    sql = sql + " from crpforamanperiods,crop,crpforaman";
    sql = sql + " where crpforamanperiods.crop_cod = crop.crop_cod";

    sql = sql + " and crpforaman.sys_cod = crpforamanperiods.sys_cod";
    sql = sql + " and crpforaman.plot_cod = crpforamanperiods.plot_cod";
    sql = sql + " and crpforaman.numseason = crpforamanperiods.numseason";
    sql = sql + " and crpforaman.crop_cod = crpforamanperiods.crop_cod";
    sql = sql + " and crpforaman.acty_cod = crpforamanperiods.acty_cod";
    sql = sql + " and crpforaman.fora_cod = crpforamanperiods.fora_cod";

    sql = sql + " and crpforamanperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by crpforamanperiods.crop_cod,crop.crop_des";

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
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            gendata.pFood = true;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"hhcons")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"hhcons")).toFloat();
        }
    }

    //Vegetable products
    sql = "select vegprodmanperiods.veg_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum((vegprodmanperiods.qty * vegprodman." + campo2 + ")";
    sql = sql + ") as hhcons";
    sql = sql + " from vegprodmanperiods,crop,vegprodman";
    sql = sql + " where vegprodmanperiods.veg_cod = crop.crop_cod";

    sql = sql + " and vegprodman.sys_cod = vegprodmanperiods.sys_cod";
    sql = sql + " and vegprodman.plot_cod = vegprodmanperiods.plot_cod";
    sql = sql + " and vegprodman.numseason = vegprodmanperiods.numseason";
    sql = sql + " and vegprodman.crop_cod = vegprodmanperiods.crop_cod";
    sql = sql + " and vegprodman.veg_cod = vegprodmanperiods.veg_cod";
    sql = sql + " and vegprodman.acty_cod = vegprodmanperiods.acty_cod";
    sql = sql + " and vegprodman.prod_cod = vegprodmanperiods.prod_cod";

    sql = sql + " and vegprodmanperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by vegprodmanperiods.veg_cod,crop.crop_des";

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
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            gendata.pFood = true;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"hhcons")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"hhcons")).toFloat();
        }

    }


    //Vegetable forages
    sql = "select vegforamanperiods.veg_cod as pcode,crop.crop_des as pdesc,";
    sql = sql + "sum((vegforamanperiods.qty * vegforaman." + campo2 + ")";
    sql = sql + ") as hhcons";
    sql = sql + " from vegforamanperiods,crop,vegforaman";
    sql = sql + " where vegforamanperiods.veg_cod = crop.crop_cod";

    sql = sql + " and vegforaman.sys_cod = vegforamanperiods.sys_cod";
    sql = sql + " and vegforaman.plot_cod = vegforamanperiods.plot_cod";
    sql = sql + " and vegforaman.numseason = vegforamanperiods.numseason";
    sql = sql + " and vegforaman.crop_cod = vegforamanperiods.crop_cod";
    sql = sql + " and vegforaman.veg_cod = vegforamanperiods.veg_cod";
    sql = sql + " and vegforaman.acty_cod = vegforamanperiods.acty_cod";
    sql = sql + " and vegforaman.fora_cod = vegforamanperiods.fora_cod";

    sql = sql + " and vegforamanperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by vegforamanperiods.veg_cod,crop.crop_des";

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
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            gendata.pFood = true;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"hhcons")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"hhcons")).toFloat();
        }
    }


    ptype = "P";
    //Grass products
    sql = "select grassprodmanperiods.crop_cod as pcode,pastures.crop_des as pdesc,";
    sql = sql + "sum((grassprodmanperiods.qty * grassprodman." + campo2 + ")";
    sql = sql + ") as hhcons";
    sql = sql + " from grassprodmanperiods,pastures,grassprodman";
    sql = sql + " where grassprodmanperiods.crop_cod = pastures.crop_cod";

    sql = sql + " and grassprodman.sys_cod = grassprodmanperiods.sys_cod";
    sql = sql + " and grassprodman.plot_cod = grassprodmanperiods.plot_cod";
    sql = sql + " and grassprodman.numpadd = grassprodmanperiods.numpadd";
    sql = sql + " and grassprodman.crop_cod = grassprodmanperiods.crop_cod";
    sql = sql + " and grassprodman.acty_cod = grassprodmanperiods.acty_cod";
    sql = sql + " and grassprodman.prod_cod = grassprodmanperiods.prod_cod";

    sql = sql + " and grassprodmanperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by grassprodmanperiods.crop_cod,pastures.crop_des";

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
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            gendata.pFood = true;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"hhcons")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"hhcons")).toFloat();
        }

    }

    //Grass forages
    sql = "select grassforamanperiods.crop_cod as pcode,pastures.crop_des as pdesc,";
    sql = sql + "sum((grassforamanperiods.qty * grassforaman." + campo2 + ")";
    sql = sql + ") as hhcons";
    sql = sql + " from grassforamanperiods,pastures,grassforaman";
    sql = sql + " where grassforamanperiods.crop_cod = pastures.crop_cod";

    sql = sql + " and grassforaman.sys_cod = grassforamanperiods.sys_cod";
    sql = sql + " and grassforaman.plot_cod = grassforamanperiods.plot_cod";
    sql = sql + " and grassforaman.numpadd = grassforamanperiods.numpadd";
    sql = sql + " and grassforaman.crop_cod = grassforamanperiods.crop_cod";
    sql = sql + " and grassforaman.acty_cod = grassforamanperiods.acty_cod";
    sql = sql + " and grassforaman.fora_cod = grassforamanperiods.fora_cod";

    sql = sql + " and grassforamanperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " group by grassforamanperiods.crop_cod,pastures.crop_des";

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
                    (lista[pos].pCode == query.value(getFieldIndex(query,"pcode")).toString()))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            POutputData gendata;
            gendata.ptype = ptype;
            gendata.pCode = query.value(getFieldIndex(query,"pcode")).toString();
            gendata.pFood = true;
            gendata.pFeed = false;
            gendata.pValue = query.value(getFieldIndex(query,"hhcons")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"hhcons")).toFloat();
        }

    }

    ptype = "L";

    //Livestock with products by quanity
    sql = "select livehvstperiods.prod_cod as pcode,products.prod_des as pdesc,";
    sql = sql + "sum((livehvstperiods.qty * livehvst." + campo2 + ")";
    sql = sql + ") as total";
    sql = sql + " from livehvstperiods,products,lvproducts,livehvst";
    sql = sql + " where livehvstperiods.prod_cod = products.prod_cod";
    sql = sql + " and livehvstperiods.live_cod = lvproducts.live_cod";
    sql = sql + " and lvproducts.salesby = 'Q'";
    sql = sql + " and livehvstperiods.prod_cod = lvproducts.prod_cod";
    sql = sql + " and livehvstperiods.sys_cod = '" + sistema +  "'";

    sql = sql + " and livehvst.sys_cod = livehvstperiods.sys_cod";
    sql = sql + " and livehvst.live_icod = livehvstperiods.live_icod";
    sql = sql + " and livehvst.group_cod = livehvstperiods.group_cod";
    sql = sql + " and livehvst.live_cod = livehvstperiods.live_cod";
    sql = sql + " and livehvst.prod_cod = livehvstperiods.prod_cod";


    sql = sql + " group by livehvstperiods.prod_cod,products.prod_des";

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
            if((lista[pos].ptype == "L"))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            POutputData gendata;
            gendata.ptype = "L";
            gendata.pValue = query.value(getFieldIndex(query,"total")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"total")).toFloat();
        }

    }

    //Livestock with products by units
    sql = "select livehvstperiods.prod_cod as pcode,products.prod_des as pdesc,";
    sql = sql + "sum((livehvstperiods.qty * livehvstperiods.wei * livehvst." + campo2 + ")";

    sql = sql + ") as total";
    sql = sql + " from livehvstperiods,products,lvproducts,livehvst";
    sql = sql + " where livehvstperiods.prod_cod = products.prod_cod";
    sql = sql + " and livehvstperiods.live_cod = lvproducts.live_cod";
    sql = sql + " and lvproducts.salesby = 'U'";
    sql = sql + " and lvproducts.weight = 'Y'";
    sql = sql + " and livehvstperiods.prod_cod = lvproducts.prod_cod";
    sql = sql + " and livehvstperiods.sys_cod = '" + sistema +  "'";

    sql = sql + " and livehvst.sys_cod = livehvstperiods.sys_cod";
    sql = sql + " and livehvst.live_icod = livehvstperiods.live_icod";
    sql = sql + " and livehvst.group_cod = livehvstperiods.group_cod";
    sql = sql + " and livehvst.live_cod = livehvstperiods.live_cod";
    sql = sql + " and livehvst.prod_cod = livehvstperiods.prod_cod";

    sql = sql + " group by livehvstperiods.prod_cod,products.prod_des";

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
            if((lista[pos].ptype == "L"))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            POutputData gendata;
            gendata.ptype = "L";
            gendata.pValue = query.value(getFieldIndex(query,"total")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"total")).toFloat();
        }

    }

    ptype = "R";
    //Purchased imputs
    sql = "select otherhhcpperiods.prod_cod as pcode,otherhhcp.prod_desc as pdesc,";
    sql = sql + "((otherhhcpperiods.qtp * otherhhcp." + campo2 + ")";
    sql = sql + ") as total";
    sql = sql + " from otherhhcpperiods,otherhhcp";
    sql = sql + " where otherhhcpperiods.sys_cod = '" + sistema +  "'";
    sql = sql + " and otherhhcp.sys_cod = otherhhcpperiods.sys_cod";
    sql = sql + " and otherhhcp.prod_cod = otherhhcpperiods.prod_cod";

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
            if((lista[pos].ptype == "R"))
            {
                ind = pos;
                break;
            }
        }
        if(ind == -1)
        {
            POutputData gendata;
            gendata.ptype = "R";
            gendata.pValue = query.value(getFieldIndex(query,"total")).toFloat();
            lista.append(gendata);
        }
        else
        {
            lista[ind].pValue = lista[ind].pValue + query.value(getFieldIndex(query,"total")).toFloat();
        }

    }

    int pos2_end;

    pos_end = lista.count();
    for(pos = 0 ; pos < pos_end ; pos++)
    {
        if((lista[pos].ptype != "L") && (lista[pos].ptype != "R"))
        {
            if(lista[pos].ptype == "C")
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
                        if((lista[pos].pFeed == false) && (query.value(getFieldIndex(query,"tfeed")).toFloat() > 0)) lista[pos].pFeed = true;


                }
            }
            else
            {
                pos2_end = 3 ;
                for(pos2 = 1 ; pos2 < pos2_end ; pos2++)
                {
                    switch(pos2) {
                    case 1:  {
                        table = "grassprodmanfeedperiods";
                        campo = "crop_cod";
                        break;
                    }
                    case 2:  {
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
                        if((lista[pos].pFeed == false) && (query.value(getFieldIndex(query,"tfeed")).toFloat() > 0)) lista[pos].pFeed = true;

                }
            }
        }
    }

    pos_end = 5 ;

    for(pos = 1 ; pos < pos_end ; pos++)
        totals[pos] = 0;
    pos_end = lista.count();
    for(pos = 0 ; pos < pos_end ; pos++)
    {
        if((lista[pos].ptype != "L") && (lista[pos].ptype != "R"))
        {
            if(lista[pos].pFeed == false)
                totals[1] = totals[1] + lista[pos].pValue;else
                totals[2] = totals[2] + lista[pos].pValue;}
        else
        {
            if(lista[pos].ptype == "L")
                totals[3] = totals[3] + lista[pos].pValue;else
                totals[4] = totals[4] + lista[pos].pValue;
        }
    }

    QList<TchartSeries> series;

    for(pos = 1 ; pos < 5 ; pos++)
    {
        switch(pos) {
        case 1:  outdesc = "Food Crop"; break;
        case 2:  outdesc = "Food-Feed Crop"; break;
        case 3:  outdesc = "Livestock"; break;
        case 4:  outdesc = "Purchased food"; break;
        }
        TchartSeries serie;
        serie.group = "";
        serie.name = outdesc;
        serie.value = totals[pos];
        series.append(serie);
    }
    QString url;
    if(ui->radioButton->isChecked() == true)
    {

        url = createPieChart("food01","Energy sources","MJ",series,ui->view1);
    }
    else
    {
        url = createPieChart("food01","Protein sources","g",series,ui->view1);
    }
    ui->view1->load(QUrl(url));
    ui->view1->show();

    classesModel2 *clasmodel = new classesModel2(this);
    clasmodel->loadClasses(series);
    ui->tableView_3->setModel(clasmodel);
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

}


float foodsecurity::GetMin(QString sistema, QString tipo)
{
    float result;
    float reqener; float reqprot;
    int edad;
    QSqlQuery tblage(dataBase);
    QSqlQuery reader(dataBase);


    QString sql;
    sql = "SELECT * FROM labforce where sys_cod = '" + sistema + "' AND pers_typ = 'HH'";

    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }

    reqener = 0;
    reqprot = 0;
    while( reader.next())
    {
        edad = reader.value(getFieldIndex(reader,"age" )).toInt();
        if(  edad >= 18 )
        {
            if(  reader.value(getFieldIndex(reader,"sex" )).toString() == "M" )
            {
                sql = "SELECT * FROM agegrps WHERE vfrom <= " + QString::number(edad);
                sql = sql + " AND vto >= "  + QString::number(edad);
                sql = sql + " AND typcod = 'man'";
            }
            else
            {
                sql = "SELECT * FROM agegrps WHERE vfrom <= " + QString::number(edad);
                sql = sql + " AND vto >= "  + QString::number(edad);
                sql = sql + " AND typcod = 'woman'";
            }
        }
        else
        {
            sql = "SELECT * FROM agegrps WHERE vfrom <= " + QString::number(edad);
            sql = sql + " AND vto >= "  + QString::number(edad);
        }
        if (!tblage.exec(sql))
        {
            qDebug() << "Error: " << tblage.lastError().databaseText();
            qDebug() << "Executing: " << sql;
        }
        if(tblage.first())
        {
            reqener = reqener + tblage.value(getFieldIndex(tblage,"energy")).toFloat();
            reqprot = reqprot + tblage.value(getFieldIndex(tblage,"protein")).toFloat();
        }
        else
        {
            qDebug() << "Age group not found!";
            qDebug() << "Executing: " << sql;
        }

    }

    if(  tipo == "E" )
        result = reqener * 30.41;
    else
        result = reqprot * 30.41;
    return result;
}

void foodsecurity::loadValues(QString system)
{
    QSqlQuery reader(dataBase);

    float energy;
    float protein;
    float valor;
    int pos2;
    int mes;

    QString sql;

    for( pos2 = 1 ; pos2 <=6 ; pos2++)
    {

        if (pos2 == 1)
        {
            sql = "SELECT crpprodman.energy,crpprodman.protein,crpprodmanperiods.period_id,crpprodmanperiods.qty";
            sql = sql + " FROM crpprodman,crpprodmanperiods";

            sql = sql + " WHERE crpprodman.sys_cod = crpprodmanperiods.sys_cod";
            sql = sql + " and crpprodman.plot_cod = crpprodmanperiods.plot_cod";
            sql = sql + " and crpprodman.numseason = crpprodmanperiods.numseason";
            sql = sql + " and crpprodman.crop_cod = crpprodmanperiods.crop_cod";
            sql = sql + " and crpprodman.acty_cod = crpprodmanperiods.acty_cod";
            sql = sql + " and crpprodman.prod_cod = crpprodmanperiods.prod_cod";
            sql = sql + " and crpprodman.sys_cod = '" + system + "'";
        }
        if (pos2 == 2)
        {
            sql = "SELECT crpforaman.energy,crpforaman.protein,crpforamanperiods.period_id,crpforamanperiods.qty";
            sql = sql + " FROM crpforaman,crpforamanperiods";

            sql = sql + " WHERE crpforaman.sys_cod = crpforamanperiods.sys_cod";
            sql = sql + " and crpforaman.plot_cod = crpforamanperiods.plot_cod";
            sql = sql + " and crpforaman.numseason = crpforamanperiods.numseason";
            sql = sql + " and crpforaman.crop_cod = crpforamanperiods.crop_cod";
            sql = sql + " and crpforaman.acty_cod = crpforamanperiods.acty_cod";
            sql = sql + " and crpforaman.fora_cod = crpforamanperiods.fora_cod";
            sql = sql + " and crpforaman.sys_cod = '" + system + "'";
        }
        if (pos2 == 3)
        {
            sql = "SELECT vegprodman.energy,vegprodman.protein,vegprodmanperiods.period_id,vegprodmanperiods.qty";
            sql = sql + " FROM vegprodman,vegprodmanperiods";

            sql = sql + " WHERE vegprodman.sys_cod = vegprodmanperiods.sys_cod";
            sql = sql + " and vegprodman.plot_cod = vegprodmanperiods.plot_cod";
            sql = sql + " and vegprodman.numseason = vegprodmanperiods.numseason";
            sql = sql + " and vegprodman.crop_cod = vegprodmanperiods.crop_cod";
            sql = sql + " and vegprodman.veg_cod = vegprodmanperiods.veg_cod";
            sql = sql + " and vegprodman.acty_cod = vegprodmanperiods.acty_cod";
            sql = sql + " and vegprodman.prod_cod = vegprodmanperiods.prod_cod";
            sql = sql + " and vegprodman.sys_cod = '" + system + "'";
        }
        if (pos2 == 4)
        {
            sql = "SELECT vegforaman.energy,vegforaman.protein,vegforamanperiods.period_id,vegforamanperiods.qty";
            sql = sql + " FROM vegforaman,vegforamanperiods";

            sql = sql + " WHERE vegforaman.sys_cod = vegforamanperiods.sys_cod";
            sql = sql + " and vegforaman.plot_cod = vegforamanperiods.plot_cod";
            sql = sql + " and vegforaman.numseason = vegforamanperiods.numseason";
            sql = sql + " and vegforaman.crop_cod = vegforamanperiods.crop_cod";
            sql = sql + " and vegforaman.veg_cod = vegforamanperiods.veg_cod";
            sql = sql + " and vegforaman.acty_cod = vegforamanperiods.acty_cod";
            sql = sql + " and vegforaman.fora_cod = vegforamanperiods.fora_cod";
            sql = sql + " and vegforaman.sys_cod = '" + system + "'";
        }
        if (pos2 == 5)
        {
            sql = "SELECT grassprodman.energy,grassprodman.protein,grassprodmanperiods.period_id,grassprodmanperiods.qty";
            sql = sql + " FROM grassprodman,grassprodmanperiods";

            sql = sql + " WHERE grassprodman.sys_cod = grassprodmanperiods.sys_cod";
            sql = sql + " and grassprodman.plot_cod = grassprodmanperiods.plot_cod";
            sql = sql + " and grassprodman.numpadd = grassprodmanperiods.numpadd";
            sql = sql + " and grassprodman.crop_cod = grassprodmanperiods.crop_cod";
            sql = sql + " and grassprodman.acty_cod = grassprodmanperiods.acty_cod";
            sql = sql + " and grassprodman.prod_cod = grassprodmanperiods.prod_cod";
            sql = sql + " and grassprodman.sys_cod = '" + system + "'";
        }
        if (pos2 == 6)
        {
            sql = "SELECT grassforaman.energy,grassforaman.protein,grassforamanperiods.period_id,grassforamanperiods.qty";
            sql = sql + " FROM grassforaman,grassforamanperiods";

            sql = sql + " WHERE grassforaman.sys_cod = grassforamanperiods.sys_cod";
            sql = sql + " and grassforaman.plot_cod = grassforamanperiods.plot_cod";
            sql = sql + " and grassforaman.numpadd = grassforamanperiods.numpadd";
            sql = sql + " and grassforaman.crop_cod = grassforamanperiods.crop_cod";
            sql = sql + " and grassforaman.acty_cod = grassforamanperiods.acty_cod";
            sql = sql + " and grassforaman.fora_cod = grassforamanperiods.fora_cod";
            sql = sql + " and grassforaman.sys_cod = '" + system + "'";
        }
        if (!reader.exec(sql))
        {
            qDebug() << "Error: " << reader.lastError().databaseText();
            qDebug() << "Executing: " << sql;
        }
        while(reader.next())
        {
            energy = reader.value(getFieldIndex(reader,"energy" )).toFloat();
            protein = reader.value(getFieldIndex(reader,"protein" )).toFloat();

            valor = reader.value(getFieldIndex(reader,"QTY")).toFloat();
            mes = reader.value(getFieldIndex(reader,"period_id")).toInt();
            totemes[ mes-1 ] = totemes[ mes-1 ] + ( energy * valor );
            totpmes[ mes-1 ] = totpmes[ mes-1 ] + ( protein * valor );
            contribe[ 0 ] = contribe[ 0 ] + ( energy * valor );
            contribp[ 0 ] = contribp[ 0 ] + ( protein * valor );
        }
    }

    //Livestock with products by quantity
    sql = "SELECT livehvst.energy,livehvst.protein,livehvstperiods.period_id,livehvstperiods.qty";
    sql = sql + " FROM livehvstperiods,lvproducts,livehvst";
    sql = sql + " WHERE livehvst.live_cod = lvproducts.live_cod";
    sql = sql + " AND livehvst.prod_cod = lvproducts.prod_cod";
    sql = sql + " AND lvproducts.salesby = 'Q'";
    sql = sql + " AND livehvst.sys_cod = livehvstperiods.sys_cod";
    sql = sql + " AND livehvst.live_icod = livehvstperiods.live_icod";
    sql = sql + " AND livehvst.group_cod = livehvstperiods.group_cod";
    sql = sql + " AND livehvst.live_cod = livehvstperiods.live_cod";
    sql = sql + " AND livehvst.prod_cod = livehvstperiods.prod_cod";
    sql = sql + " AND livehvst.sys_cod = '" + sistema +  "'";

    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        energy = reader.value(getFieldIndex(reader,"energy" )).toFloat();
        protein = reader.value(getFieldIndex(reader,"protein" )).toFloat();

        valor = reader.value(getFieldIndex(reader,"QTY")).toFloat();
        mes = reader.value(getFieldIndex(reader,"period_id")).toInt();
        totemes[ mes-1 ] = totemes[ mes-1 ] + ( energy * valor );
        totpmes[ mes-1 ] = totpmes[ mes-1 ] + ( protein * valor );
        contribe[ 1 ] = contribe[ 1 ] + ( energy * valor );
        contribp[ 1 ] = contribp[ 1 ] + ( protein * valor );
    }



    //Livestock with products by weight
    sql = "SELECT livehvst.energy,livehvst.protein,livehvstperiods.period_id,(livehvstperiods.qty * livehvstperiods.wei) as qtot";
    sql = sql + " FROM livehvstperiods,lvproducts,livehvst";
    sql = sql + " WHERE livehvst.live_cod = lvproducts.live_cod";
    sql = sql + " AND livehvst.prod_cod = lvproducts.prod_cod";
    sql = sql + " AND lvproducts.salesby = 'U'";
    sql = sql + " AND lvproducts.weight = 'Y'";
    sql = sql + " AND livehvst.sys_cod = livehvstperiods.sys_cod";
    sql = sql + " AND livehvst.live_icod = livehvstperiods.live_icod";
    sql = sql + " AND livehvst.group_cod = livehvstperiods.group_cod";
    sql = sql + " AND livehvst.live_cod = livehvstperiods.live_cod";
    sql = sql + " AND livehvst.prod_cod = livehvstperiods.prod_cod";
    sql = sql + " AND livehvst.sys_cod = '" + sistema +  "'";

    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        energy = reader.value(getFieldIndex(reader,"energy" )).toFloat();
        protein = reader.value(getFieldIndex(reader,"protein" )).toFloat();

        valor = reader.value(getFieldIndex(reader,"qtot")).toFloat();
        mes = reader.value(getFieldIndex(reader,"period_id")).toInt();
        totemes[ mes-1 ] = totemes[ mes-1 ] + ( energy * valor );
        totpmes[ mes-1 ] = totpmes[ mes-1 ] + ( protein * valor );
        contribe[ 1 ] = contribe[ 1 ] + ( energy * valor );
        contribp[ 1 ] = contribp[ 1 ] + ( protein * valor );
    }

    //Other products

    sql = "SELECT otherhhcp.energy,otherhhcp.protein,otherhhcpperiods.period_id,otherhhcpperiods.QTP";
    sql = sql + " FROM otherhhcp, otherhhcpperiods";
    sql = sql + " WHERE otherhhcp.sys_cod = otherhhcpperiods.sys_cod";
    sql = sql + " AND otherhhcp.prod_cod = otherhhcpperiods.prod_cod";
    sql = sql + " AND otherhhcp.sys_cod = '" + sistema +  "'";

    if (!reader.exec(sql))
    {
        qDebug() << "Error: " << reader.lastError().databaseText();
        qDebug() << "Executing: " << sql;
    }
    while(reader.next())
    {
        energy = reader.value(getFieldIndex(reader,"energy" )).toFloat();
        protein = reader.value(getFieldIndex(reader,"protein" )).toFloat();

        valor = reader.value(getFieldIndex(reader,"QTP")).toFloat();
        mes = reader.value(getFieldIndex(reader,"period_id")).toInt();
        totemes[ mes-1 ] = totemes[ mes-1 ] + ( energy * valor );
        totpmes[ mes-1 ] = totpmes[ mes-1 ] + ( protein * valor );
        contribe[ 2 ] = contribe[ 2 ] + ( energy * valor );
        contribp[ 2 ] = contribp[ 2 ] + ( protein * valor );
    }

}

void foodsecurity::generateGraphs(QString gtype)
{
    float max;
    int pos;
    float min;

    QList<TchartSeries> series;

    if(  gtype == "E" )
    {

        max = 0;
        for( pos = 0 ; pos < totemes.count() ; pos++ )
        {
            if(  totemes[ pos ] >= max )
                max = totemes[ pos ];
        }
        min = GetMin( sistema, "E" );
        if(  min >= max ) max = min;

        for( pos = 0 ; pos < totemes.count() ; pos++ )
        {
            TchartSeries serie;
            serie.group = "";
            serie.value = totemes[ pos ];
            serie.name = periods[pos];
            series.append(serie);
        }        
    }
    else
    {

        max = 0;
        for( pos = 0 ; pos < totpmes.count() ; pos++ )
        {
            if(  totpmes[ pos ] >= max )
                max = totpmes[ pos ];
        }
        min = GetMin( sistema, "P" );
        if(  min >= max ) max = min;

        for( pos = 0 ; pos < totpmes.count() ; pos++ )
        {
            TchartSeries serie;
            serie.group = "";
            serie.value = totpmes[ pos ];
            serie.name = periods[pos];
            series.append(serie);
        }        
    }

    QString url;
    if(ui->radioButton->isChecked() == true)
    {

        url = createBarChart("food02","Energy values per period","MJ/period",series,min,ui->view2);
    }
    else
    {
        url = createBarChart("food02","Protein values per period","g/period",series,min,ui->view2);
    }
    ui->view2->load(QUrl(url));
    ui->view2->show();

    classesModel2 *clasmodel = new classesModel2(this);
    clasmodel->loadClasses(series);
    ui->tableView_4->setModel(clasmodel);
    ui->tableView_4->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_4->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    ui->lineEdit->setText(QString::number(min,'f',2));

    createChart2();
}

void foodsecurity::on_pushButton_clicked()
{
    QString sql;
    QSqlQuery query(dataBase);
    int nperiods;
    sql = "SELECT project_nperiods FROM project";
    query.exec(sql);
    query.first();
    nperiods = query.value(0).toInt();

    totemes.resize(nperiods);
    totpmes.resize(nperiods);

    sql = "SELECT period_des FROM periods";
    query.exec(sql);

    periods.clear();
    while (query.next())
    {
        periods << query.value(0).toString();
    }


    int pos;

    for (pos = 1; pos <= nperiods; pos++)
    {
        totemes[pos-1] = 0;
        totpmes[pos-1] = 0;
    }

    for( pos = 0 ; pos < 3 ; pos++ )
    {
        contribe[ pos ] = 0;
        contribp[ pos ] = 0;
    }
    sistema = ui->comboBox->itemData(ui->comboBox->currentIndex(),Qt::UserRole).toString();
    loadValues( sistema );
    if( ui->radioButton->isChecked() == true )
        generateGraphs( "E" );
    else
        generateGraphs( "P" );
}

void foodsecurity::on_pushButton_2_clicked()
{
    emit closeCalled();
}

void foodsecurity::on_radioButton_clicked()
{
    ui->label_3->setText("MJ/period");
}

void foodsecurity::on_radioButton_2_clicked()
{
    ui->label_3->setText("g/period");
}

void foodsecurity::on_tabWidget_currentChanged(int)
{
    if (!sistema.isEmpty())
        on_pushButton_clicked();
}
