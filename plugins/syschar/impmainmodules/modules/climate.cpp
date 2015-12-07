#include "climate.h"
#include "ui_climate.h"
#include "delegates/imagecombodelegate.h"
#include <qsqlquery.h>
#include <QSqlError>
#include "moduleglobal.h"
#include <calcdialog.h>
#include <QPixmap>
#include <QDebug>

climate::climate(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::climate)
{
    ui->setupUi(this);
    dataBase = db;
    currentSystem = systemID;


    QSqlQuery sysinfo(dataBase);
    QString sql;
    sql = "SELECT numseasons FROM system WHERE sys_cod = '" + currentSystem + "'";
    sysinfo.exec(sql);
    sysinfo.first();
    numseasons = sysinfo.value(0).toInt();
    newNumseasons = numseasons;
    loadForm();
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

void climate::loadAndroidEffects()
{
    charm.activateOn(ui->tableView);
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalculator(QModelIndex)));
}

climate::~climate()
{
    //qDebug() << "Before destroy climate UI";
    delete ui;
    //qDebug() << "After destroy climate UI";
}

void climate::constructCustomDelegator()
{

    imageComboDelegate *ldelegate = new imageComboDelegate(this);
    ldelegate->setUndefinedValue("NA",QPixmap(":/images/nosundata.png"));
    if (numseasons == 2)
    {       
        ldelegate->insertItem("DR","Dry",QPixmap(":/images/sun.png"));
        ldelegate->insertItem("WE","Wet",QPixmap(":/images/wet.png"));
        ui->RadioButton1->setChecked(true);

        ui->Image11->setVisible(false);
        ui->Image12->setVisible(false);
        ui->Label12->setVisible(false);
        ui->Label13->setVisible(false);
        ui->Image9->setPixmap(QPixmap(":images/wet.png"));
        ui->Image10->setPixmap(QPixmap(":images/sun.png"));
        ui->Label10->setText("Wet");
        ui->Label11->setText("Dry");

    }
    else
    {
        ldelegate->insertItem("AT","Autum",QPixmap(":/images/autum.png"));
        ldelegate->insertItem("WI","Winter",QPixmap(":/images/winter.png"));
        ldelegate->insertItem("SP","Spring",QPixmap(":/images/spring.png"));
        ldelegate->insertItem("SM","Summer",QPixmap(":/images/summer.png"));
        ui->RadioButton2->setChecked(true);

        ui->Image11->setVisible(true);
        ui->Image12->setVisible(true);
        ui->Label12->setVisible(true);
        ui->Label13->setVisible(true);
        ui->Image9->setPixmap(QPixmap(":images/autum.png"));
        ui->Image10->setPixmap(QPixmap(":images/winter.png"));
        ui->Label10->setText("Autum");
        ui->Label11->setText("Winter");

    }
    ldelegate->addIgnoredColumn(0);
    ldelegate->addIgnoredColumn(1);
    ui->tableView->setItemDelegateForRow(1,ldelegate);

    imageComboDelegate *sdelegate = new imageComboDelegate(this);
    sdelegate->setUndefinedValue("NA",QPixmap(":/images/nosundata.png"));
    sdelegate->insertItem("1","Sun",QPixmap(":/images/fullsun.png"));
    sdelegate->insertItem("2","Some clouds",QPixmap(":/images/suncloud.png"));
    sdelegate->insertItem("3","Many clouds",QPixmap(":/images/clouds.png"));
    sdelegate->insertItem("4","Fully cloudy",QPixmap(":/images/nosun.png"));
    sdelegate->addIgnoredColumn(0);
    sdelegate->addIgnoredColumn(1);
    ui->tableView->setItemDelegateForRow(10,sdelegate);


    for (int pos =0; pos <= m_periodModel->columnCount()-1;pos++)
    {
        if (pos <= 1)
            ui->tableView->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->tableView->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }
}

void climate::loadForm()
{
    m_periodModel = new periodTableModel(this);
    m_periodModel->setDatabase(dataBase);
    //m_periodModel->setImpactModule(this);
    m_periodModel->setTableName("systemperiods");
    m_periodModel->addGroup("001","Season");
    m_periodModel->addField("season","Distribution","DR","001");
    m_periodModel->addGroup("002","Rainfall");
    m_periodModel->addField("rain","Mm.",0,"002");
    m_periodModel->addField("rday","Rainy days (no.)",0,"002");
    m_periodModel->addGroup("003","Temperature");
    m_periodModel->addField("mintemp","Minimum (°C)",0,"003");
    m_periodModel->addField("maxtemp","Maximum (°C)",0,"003");
    m_periodModel->addField("avetemp","Average (°C)",0,"003");
    m_periodModel->addGroup("004","Solar radiation");
    m_periodModel->addField("sun","Nubosity",1,"004");

    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_periodModel->loadData(keys);

    ui->tableView->setModel(m_periodModel);

//    m_periodModel->distributeColums();

    for (int rsize = 0; rsize <= m_periodModel->rowCount()-1;rsize++)
        ui->tableView->setRowHeight(rsize,listViewRowSize);

}

void climate::loadCalculator (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = false;
    colOk = false;

    switch (index.row())
    {
    case 3:
        rowOk = true;
        break;
    case 4:
        rowOk = true;
        break;
    case 6:
        rowOk = true;
        break;
    case 7:
        rowOk = true;
        break;
    case 8:
        rowOk = true;
        break;
    default:
        rowOk = false;
    }

    if (index.column() >= 2)
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(dataBase);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_periodModel->setData(index,calc.getCurrentDisplay());
    }
}

void climate::on_BitBtn45_clicked()
{
    m_periodModel->applyData();

    if (numseasons != newNumseasons)
    {
        QSqlQuery sysinfo(dataBase);
        QString sql;
        sql = "UPDATE system SET numseasons = " + QString::number(newNumseasons) + " WHERE sys_cod = '" + currentSystem + "'";
        if (sysinfo.exec(sql))
           numseasons =  newNumseasons;
        else
        {
            qWarning() << "Error: " << sysinfo.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
}

void climate::on_BitBtn43_clicked()
{
    m_periodModel->cancelData();

    if (numseasons == 2)
    {
        ui->RadioButton1->setChecked(true);

        ui->Image11->setVisible(false);
        ui->Image12->setVisible(false);
        ui->Label12->setVisible(false);
        ui->Label13->setVisible(false);
        ui->Image9->setPixmap(QPixmap(":images/wet.png"));
        ui->Image10->setPixmap(QPixmap(":images/sun.png"));
        ui->Label10->setText("Wet");
        ui->Label11->setText("Dry");

        imageComboDelegate *ldelegate = new imageComboDelegate(this);
        ldelegate->setUndefinedValue("NA",QPixmap(":/images/nosundata.png"));


        ldelegate->insertItem("DR","Dry",QPixmap(":/images/sun.png"));
        ldelegate->insertItem("WE","Wet",QPixmap(":/images/wet.png"));


        ldelegate->addIgnoredColumn(0);
        ldelegate->addIgnoredColumn(1);
        ui->tableView->setItemDelegateForRow(1,ldelegate);
    }
    else
    {
        ui->RadioButton2->setChecked(true);

        ui->Image11->setVisible(true);
        ui->Image12->setVisible(true);
        ui->Label12->setVisible(true);
        ui->Label13->setVisible(true);
        ui->Image9->setPixmap(QPixmap(":images/autum.png"));
        ui->Image10->setPixmap(QPixmap(":images/winter.png"));
        ui->Label10->setText("Autum");
        ui->Label11->setText("Winter");

        imageComboDelegate *ldelegate = new imageComboDelegate(this);
        ldelegate->setUndefinedValue("NA",QPixmap(":/images/nosundata.png"));

        ldelegate->insertItem("AT","Autum",QPixmap(":/images/autum.png"));
        ldelegate->insertItem("WI","Winter",QPixmap(":/images/winter.png"));
        ldelegate->insertItem("SP","Spring",QPixmap(":/images/spring.png"));
        ldelegate->insertItem("SM","Summer",QPixmap(":/images/sun.png"));

        ldelegate->addIgnoredColumn(0);
        ldelegate->addIgnoredColumn(1);
        ui->tableView->setItemDelegateForRow(1,ldelegate);
    }
}

void climate::on_RadioButton1_clicked()
{
    int pos;
    QModelIndex idx;

    newNumseasons = 2;

    imageComboDelegate *ldelegate = new imageComboDelegate(this);
    ldelegate->setUndefinedValue("NA",QPixmap(":/images/nosundata.png"));


    ldelegate->insertItem("DR","Dry",QPixmap(":/images/sun.png"));
    ldelegate->insertItem("WE","Wet",QPixmap(":/images/wet.png"));


    ldelegate->addIgnoredColumn(0);
    ldelegate->addIgnoredColumn(1);
    ui->tableView->setItemDelegateForRow(1,ldelegate);

    for (pos = 2; pos <= m_periodModel->columnCount()-1;pos++)
    {
        idx = m_periodModel->index(1,pos);
        m_periodModel->setData(idx,"NA");
    }
    ui->Image11->setVisible(false);
    ui->Image12->setVisible(false);
    ui->Label12->setVisible(false);
    ui->Label13->setVisible(false);
    ui->Image9->setPixmap(QPixmap(":images/wet.png"));
    ui->Image10->setPixmap(QPixmap(":images/sun.png"));
    ui->Label10->setText("Wet");
    ui->Label11->setText("Dry");


}

void climate::on_RadioButton2_clicked()
{
    int pos;
    QModelIndex idx;

    imageComboDelegate *ldelegate = new imageComboDelegate(this);
    ldelegate->setUndefinedValue("NA",QPixmap(":/images/nosundata.png"));

        ldelegate->insertItem("AT","Autum",QPixmap(":/images/autum.png"));
        ldelegate->insertItem("WI","Winter",QPixmap(":/images/winter.png"));
        ldelegate->insertItem("SP","Spring",QPixmap(":/images/spring.png"));
        ldelegate->insertItem("SM","Summer",QPixmap(":/images/sun.png"));

    ldelegate->addIgnoredColumn(0);
    ldelegate->addIgnoredColumn(1);
    ui->tableView->setItemDelegateForRow(1,ldelegate);

    for (pos = 2; pos <= m_periodModel->columnCount()-1;pos++)
    {
        idx = m_periodModel->index(1,pos);
        m_periodModel->setData(idx,"NA");
    }

    newNumseasons = 4;

    ui->Image11->setVisible(true);
    ui->Image12->setVisible(true);
    ui->Label12->setVisible(true);
    ui->Label13->setVisible(true);
    ui->Image9->setPixmap(QPixmap(":images/autum.png"));
    ui->Image10->setPixmap(QPixmap(":images/winter.png"));
    ui->Label10->setText("Autum");
    ui->Label11->setText("Winter");
}
