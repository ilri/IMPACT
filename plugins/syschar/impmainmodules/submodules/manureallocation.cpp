#include "manureallocation.h"
#include "ui_manureallocation.h"
#include "delegates/fixcombodelegate.h"
#include <qsqlquery.h>
#include <qsqlrecord.h>
#include <calcdialog.h>
#include "moduleglobal.h"

manureallocation::manureallocation(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::manureallocation)
{
    ui->setupUi(this);
    m_manureModel = new linkedTableModel(this);
    m_manureSortModel = new linkedTableSortModel(this);
    connect(ui->BitBtn24,SIGNAL(clicked()),this,SLOT(applyToAll()));
    connect(ui->BitBtn21,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn22,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->BitBtn23,SIGNAL(clicked()),this,SIGNAL(closeCalled()));
}

manureallocation::~manureallocation()
{
   // qDebug() << "Before destroy manureallocation UI";
    delete ui;
   // qDebug() << "After destroy manureallocation UI";
}

void manureallocation::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView6);
    connect(ui->AdvListView6,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalc(QModelIndex)));
}

int manureallocation::getFieldIndex(QSqlQuery *query,QString fieldName)
{
    return query->record().indexOf(fieldName); //Return the index of fieldName;
}

void manureallocation::loadMaintenance(QSqlDatabase db,QTableView *)
{
    database = db;
    manureTable = new QSqlQuery(db);
    connect(m_manureModel,SIGNAL(modelDataChanged()),this,SLOT(dataChanged()));
    //m_manureModel->setImpactModule(this);

    m_manureModel->setDatabase(db);
    m_manureModel->setTableName(modelTable);
    m_manureModel->setKeyFields(m_keys);
    m_manureModel->setLinkedKeyField("HEAP_COD");
    m_manureModel->addField("MET_COD","Method","NULL",Qt::white);
    m_manureModel->addField("QTYAPP","Quantity applied \n (kg)");
    m_manureModel->setLkTableName("mnrheap");
    m_manureModel->setLkKeyField("HEAP_COD");
    m_manureModel->setLkDisplayColumn("HEAP_TYP");
    m_manureModel->addLinkedField("MET_COD","ACTYM_COD",ActManureCode);
    m_manureModel->addLinkedValue("MET_COD","ACTYM_COD","NULL","NULL");
    m_manureModel->setLkUseCustomDescription(true);
    m_manureModel->setLkDisplayColumnDescription("Manure storage units");
    QList <TmoduleFieldDef> kfields;
    TmoduleFieldDef field;
    QString sql;
    sql = "SELECT * FROM mnrheap WHERE SYS_COD = '";
    for (int pos = 0; pos <= m_keys.count()-1;pos++)
    {
        if (m_keys[pos].code.toLower() == "sys_cod")
        {
            field.code = m_keys[pos].code;
            field.value = m_keys[pos].value;
            sql = sql + m_keys[pos].value.toString() + "'";
            kfields.append(field);
            break;
        }
    }
    manureTable->exec(sql);
    m_manureModel->loadItems(kfields);
    m_manureModel->checkItems();
    m_manureSortModel->setSourceModel(m_manureModel);
    m_manureSortModel->sort(1);
    ui->AdvListView6->setModel(m_manureSortModel);
    //m_manureModel->distributeColums();

    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}



void manureallocation::constructCustomDelegator()
{

    fixComboDelegate *ldelegate = new fixComboDelegate(this);
    ldelegate->setParentModel(m_manureModel);
    ldelegate->setParentSortModel(m_manureSortModel);
    ldelegate->insertItem("NULL","None");
    QSqlQuery tquery(database);
    QString sql;
    sql = "SELECT met_cod,met_des FROM actymethods WHERE acty_cod = '" + ActManureCode + "'";
    tquery.exec(sql);
    while (tquery.next())
    {
        ldelegate->insertItem(tquery.value(0).toString(),tquery.value(1).toString());
    }

    ui->AdvListView6->setItemDelegateForColumn(1,ldelegate);
    ui->AdvListView6->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView6->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView6->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void manureallocation::dataChanged()
{
    ui->BitBtn24->setEnabled(true);
    ui->BitBtn21->setEnabled(true);
    ui->BitBtn22->setEnabled(true);
}

void manureallocation::applyChanges()
{
    if (m_manureModel->applyChanges())
    {
        ui->BitBtn24->setEnabled(false);
        ui->BitBtn21->setEnabled(false);
        ui->BitBtn22->setEnabled(false);
    }
}

void manureallocation::cancelChanges()
{
    m_manureModel->cancelChanges();
    ui->BitBtn24->setEnabled(false);
    ui->BitBtn21->setEnabled(false);
    ui->BitBtn22->setEnabled(false);
}

void manureallocation::applyToAll()
{
    if (m_manureModel->applyChanges())
    {
        ui->BitBtn24->setEnabled(false);
        ui->BitBtn21->setEnabled(false);
        ui->BitBtn22->setEnabled(false);
    }
    //Need to do apply for all for the model
}

void manureallocation::setModelTable(QString table)
{
    modelTable = table;
}

void manureallocation::setKeyFields(QList <TmoduleFieldDef> keyFields)
{
    m_keys.append(keyFields);
    for (int pos = 0; pos <= m_keys.count()-1;pos++)
    {
        if (m_keys[pos].code == "ACTY_COD")
        {
            ActManureCode = m_keys[pos].value.toString();
            break;
        }
    }
}

void manureallocation::setHeader1Description(QString description)
{
    ui->header1->setText(description);
}

void manureallocation::setHeader2Description(QString description)
{
    ui->header2->setText(description);
}

void manureallocation::loadCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if (index.column() > 0)
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(database);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_manureModel->setData(index,calc.getCurrentDisplay());
    }
}
