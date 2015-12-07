#include "countries.h"
#include "ui_countries.h"
#include "maintclases.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

countries::countries(QWidget *parent, QSqlDatabase db) :
    QWidget(parent),
    ui(new Ui::countries)
{
    ui->setupUi(this);
    //Change this lines for each maintenance
    tableName = "country";
    keyField = "CNTY_COD";
    displayColumn = "CNTY_NAM";
    displayColumnDescription = tr("Countries");
    //-----------------------------------------

    database = db;
    m_mainmodel = new maintModel(this,db);
    m_mainmodel->setTable(tableName);
    m_mainmodel->setKeyColumn(keyField);
    m_mainmodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_mainmodel->setSort(1,Qt::AscendingOrder);
    if (m_mainmodel->select())
    {
        m_mainmodel->loadStatus();
        ui->ListView1->setModel(m_mainmodel);
        m_mainmodel->setView(ui->ListView1);
        m_mainmodel->setDisplayColumn(displayColumn,displayColumnDescription);
    }
    else
    {
        qDebug() << m_mainmodel->lastError().driverText();
    }
    connect(ui->ListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(itemSelected(QModelIndex)));
    //Connect the value changed signal of IMPWidgets to the slot
    connect(ui->iMPEdit,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_2,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_3,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit_4,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    //Connect the buttons
    connect(ui->cmdnew,SIGNAL(clicked()),this,SLOT(addNewItem()));
    connect(ui->cmdelete,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(ui->cmdapply,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->cmdcancel,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->cmdclose,SIGNAL(clicked()),this,SLOT(closeForm()));


    genericFunctions fnc;
    fields = fnc.getFields(tableName,this);

    if (m_mainmodel->rowCount() >0)
    {
        ui->ListView1->setCurrentIndex(m_mainmodel->index(0,1));
        itemSelected(m_mainmodel->index(0,1));
    }
    else
    {
        fnc.disableFields(tableName,this);
    }

}

void countries::resizeEvent ( QResizeEvent *)
{
    ui->ListView1->setColumnWidth(1,ui->ListView1->width()-65);
}

void countries::itemSelected(const QModelIndex & index)
{
    QVariant value;
    genericFunctions fnc;
    for (int pos = 0; pos <= fields.count()-1;pos++)
    {
        value = m_mainmodel->record(index.row()).value(fields[pos]);
        fnc.applyDataToField(this,tableName,fields[pos],value);
    }
}

countries::~countries()
{
    delete ui;
}


void countries::addNewItem()
{
    QSqlRecord newRecord;

    genericFunctions fnc;
    fnc.setFieldsToNew(tableName,this);
    ui->iMPEdit->setFocus(Qt::OtherFocusReason);
    newRecord = m_mainmodel->record();
    m_mainmodel->insertRecord(-1,newRecord);
    m_mainmodel->insertNewRecord();
    ui->ListView1->setCurrentIndex(m_mainmodel->index(m_mainmodel->rowCount()-1,1));
    itemSelected(m_mainmodel->index(m_mainmodel->rowCount()-1,1));
    ui->ListView1->scrollTo(m_mainmodel->index(m_mainmodel->rowCount()-1,1));
}

void countries::removeItem()
{
    QModelIndex index;
    index = ui->ListView1->currentIndex();
    m_mainmodel->updateRecordStatus(index.row(),"D");
    ui->ListView1->update(index);
}

void countries::applyChanges()
{
    if (m_mainmodel->applyChanges())
        goToFirst();
}

void countries::cancelChanges()
{
    m_mainmodel->cancelChanges();
    goToFirst();
}

void countries::closeForm()
{

}

void countries::goToFirst()
{
    m_mainmodel->setSort(1,Qt::AscendingOrder);
    QModelIndex index;
    index = m_mainmodel->index(0,1);
    ui->ListView1->setCurrentIndex(index);
    itemSelected(index);
    ui->ListView1->scrollTo(index);
    ui->ListView1->update(index);
}

void countries::valueHasChanged(QString, QString field, QString value)
{
    QModelIndex index;
    QSqlRecord currRecord;
    index = ui->ListView1->currentIndex();
    currRecord = m_mainmodel->record(index.row());
    currRecord.setValue(field,value);
    m_mainmodel->updateRecordStatus(index.row(),"M");
    m_mainmodel->setRecord(index.row(),currRecord);
    ui->ListView1->update(index);
}
