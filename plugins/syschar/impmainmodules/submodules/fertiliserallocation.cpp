#include "fertiliserallocation.h"
#include "ui_fertiliserallocation.h"
#include "delegates/fixcombodelegate.h"
#include <qsqlquery.h>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QDebug>

fertiliserallocation::fertiliserallocation(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::fertiliserallocation)
{
    ui->setupUi(this);
    m_fertModel = new linkedTableModel(this);
    m_fertSortModel = new linkedTableSortModel(this);
    connect(ui->BitBtn11,SIGNAL(clicked()),this,SLOT(applyToAll()));
    connect(ui->BitBtn6,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn7,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->BitBtn8,SIGNAL(clicked()),this,SLOT(closeForm()));
    connect(m_fertModel,SIGNAL(modelDataChanged()),this,SLOT(dataChanged()));
    connect(m_fertModel,SIGNAL(afterInsert(QList<TmoduleInsertedKeys>)),this,SLOT(afterInsert(QList<TmoduleInsertedKeys>)));
    connect(m_fertModel,SIGNAL(afterDelete(QList<TmoduleInsertedKeys>)),this,SLOT(afterDelete(QList<TmoduleInsertedKeys>)));

    connect(ui->CheckBox14,SIGNAL(valueChanged(QString,QString,bool)),this,SLOT(valueHasChanged(QString,QString,bool)));
    connect(ui->Edit21,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->Edit22,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPEdit,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    connect(ui->CheckBox14,SIGNAL(clicked(bool)),this,SLOT(machine(bool)));

    applyModel = false;
    applyForm = false;
}

fertiliserallocation::~fertiliserallocation()
{
    //qDebug() << "Before destroy fertiliserallocation UI";
    delete ui;
    //qDebug() << "After destroy fertiliserallocation UI";
}

void fertiliserallocation::loadAndroidEffects()
{
    charm.activateOn(ui->ListView3);
    connect(ui->ListView3,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalc(QModelIndex)));
}

void fertiliserallocation::loadMaintenance(QSqlDatabase db,QTableView *)
{
    database = db;
    impgenmaint::loadMaintenance(db);
    //m_fertModel->setImpactModule(this);

    m_fertModel->setDatabase(db);
    m_fertModel->setTableName(modelTable);

    m_fertModel->setKeyFields(m_keys);
    m_fertModel->setLinkedKeyField("FERTYP_COD");
    m_fertModel->addField("MET_COD","Method","NULL",Qt::white);
    m_fertModel->addField("QTYFARM","On farm \n (kg/plot)",0);
    m_fertModel->addField("QTYEXCH","Exchange \n (kg/plot)",0);
    m_fertModel->addField("QTYPRCH","Purchased \n (kg/plot)",0);
    m_fertModel->setLkTableName("ferttype");
    m_fertModel->setLkKeyField("FERTYP_COD");
    m_fertModel->setLkDisplayColumn("FERTYP_DES");
    m_fertModel->setLkDisplayColumnDescription("Fertilisers");
    m_fertModel->addLinkedField("MET_COD","ACTYM_COD",ActFertCode);
    m_fertModel->addLinkedValue("MET_COD","ACTYM_COD","NULL","NULL");
    m_fertModel->addLinkedField("QTYFARM","FARM","T");
    m_fertModel->addLinkedValue("QTYFARM","FARM","0","F");
    m_fertModel->addLinkedValue("QTYFARM","FARM","NULL","F");
    m_fertModel->addLinkedValue("QTYFARM","FARM","","F");
    m_fertModel->addLinkedField("QTYEXCH","EXCH","T");
    m_fertModel->addLinkedValue("QTYEXCH","EXCH","0","F");
    m_fertModel->addLinkedValue("QTYEXCH","EXCH","NULL","F");
    m_fertModel->addLinkedValue("QTYEXCH","EXCH","","F");
    m_fertModel->addLinkedField("QTYPRCH","PRCH","T");
    m_fertModel->addLinkedValue("QTYPRCH","PRCH","0","F");
    m_fertModel->addLinkedValue("QTYPRCH","PRCH","NULL","F");
    m_fertModel->addLinkedValue("QTYPRCH","PRCH","","F");


    m_fertModel->loadItems();
    m_fertModel->checkItems();
    m_fertSortModel->setSourceModel(m_fertModel);
    m_fertSortModel->sort(1);
    ui->ListView3->setModel(m_fertSortModel);
    //m_fertModel->distributeColums();
    machine(ui->CheckBox14->isChecked());
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();

    for (int rsize = 0; rsize <= m_fertSortModel->rowCount()-1;rsize++)
        ui->ListView3->setRowHeight(rsize,listViewRowSize);


}

void fertiliserallocation::setKeyFields(QList <TmoduleFieldDef> keyFields)
{
    m_keys.append(keyFields);
    for (int pos = 0; pos <= m_keys.count()-1;pos++)
    {
        if (m_keys[pos].code == "ACTY_COD")
        {
            ActFertCode = m_keys[pos].value.toString();
            break;
        }
    }
}

void fertiliserallocation::setModelTable(QString table)
{
     modelTable = table;
}

void fertiliserallocation::constructCustomDelegator()
{

    fixComboDelegate *ldelegate = new fixComboDelegate(this);
    ldelegate->setParentModel(m_fertModel);
    ldelegate->setParentSortModel(m_fertSortModel);
    ldelegate->insertItem("NULL","None");
    QSqlQuery tquery(database);
    QString sql;
    sql = "SELECT met_cod,met_des FROM actymethods WHERE acty_cod = '" + ActFertCode + "'";
    tquery.exec(sql);
    while (tquery.next())
    {
        ldelegate->insertItem(tquery.value(0).toString(),tquery.value(1).toString());
    }
    ui->ListView3->setItemDelegateForColumn(1,ldelegate);

    ui->ListView3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->ListView3->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->ListView3->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->ListView3->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->ListView3->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
}

void fertiliserallocation::dataChanged()
{
    applyModel = true;
    ui->BitBtn11->setEnabled(true);
    ui->BitBtn6->setEnabled(true);
    ui->BitBtn7->setEnabled(true);
}

void fertiliserallocation::applyChanges()
{
    if (applyModel)
    {
        if (m_fertModel->applyChanges())
        {
            ui->BitBtn11->setEnabled(false);
            ui->BitBtn6->setEnabled(false);
            ui->BitBtn7->setEnabled(false);
            applyModel = false;
        }
        else
            return;
    }
    if (applyForm)
    {
        impgenmaint::applyChanges();
        ui->BitBtn11->setEnabled(false);
        ui->BitBtn6->setEnabled(false);
        ui->BitBtn7->setEnabled(false);
        applyForm = false;
    }
}

void fertiliserallocation::applyToAll()
{
    applyChanges(); //Add to make apply to all!
}

void fertiliserallocation::cancelChanges()
{
    if (applyModel)
    {
        m_fertModel->cancelChanges();
    }
    if (applyForm)
    {
        impgenmaint::cancelChanges();
    }
    ui->BitBtn11->setEnabled(false);
    ui->BitBtn6->setEnabled(false);
    ui->BitBtn7->setEnabled(false);
    applyModel = false;
    applyForm = false;
    machine(ui->CheckBox14->isChecked());
}

void fertiliserallocation::closeForm()
{
    emit closeCalled();
}

void fertiliserallocation::valueHasChanged(QString, QString, QString)
{
    applyForm = true;
    ui->BitBtn11->setEnabled(true);
    ui->BitBtn6->setEnabled(true);
    ui->BitBtn7->setEnabled(true);
}

void fertiliserallocation::valueHasChanged(QString, QString, bool)
{
    applyForm = true;
    ui->BitBtn11->setEnabled(true);
    ui->BitBtn6->setEnabled(true);
    ui->BitBtn7->setEnabled(true);
}

void fertiliserallocation::machine(bool checked)
{
    ui->Edit21->setEnabled(checked);
    ui->Edit22->setEnabled(checked);
    ui->iMPEdit->setEnabled(checked);

    if (!checked)
    {
        ui->Edit21->setText("0");
        ui->Edit22->setText("0");
        ui->iMPEdit->setText("");
    }
}

void fertiliserallocation::afterInsert(QList<TmoduleInsertedKeys> keys)
{
    QSqlQuery tquery(database);
    QString sql;
    int pos;
    int pos2;
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        if (modelTable == "cropmanfert")
        {
            sql = "INSERT INTO cropmanprchfert (";
        }
        if (modelTable == "grassmanfert")
        {
            sql = "INSERT INTO grassmanprchfert (";
        }
        if (modelTable == "vegmanfert")
        {
            sql = "INSERT INTO vegmanprchfert (";
        }
        for (pos2 = 0; pos2 <= keys[pos].keys.count()-1;pos2++)
        {
            if ( keys[pos].keys[pos2].code != "NMONTH")
            {
                sql = sql + keys[pos].keys[pos2].code + ",";
            }
        }
        sql = sql.left(sql.length()-1);
        sql = sql + ") VALUES (";
        for (pos2 = 0; pos2 <= keys[pos].keys.count()-1;pos2++)
        {
            if ( keys[pos].keys[pos2].code != "NMONTH")
            {
                sql = sql + "'" + keys[pos].keys[pos2].value.toString() + "',";
            }
        }
        sql = sql.left(sql.length()-1);
        sql = sql + ")";
        if (!tquery.exec(sql))
        {
            qWarning() << "Error: " << tquery.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
}


void fertiliserallocation::afterDelete(QList<TmoduleInsertedKeys> keys)
{
    QSqlQuery tquery(database);
    QString sql;
    int pos;
    int pos2;
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        if (modelTable == "cropmanfert")
        {
            sql = "DELETE FROM cropmanprchfert";
        }
        if (modelTable == "grassmanfert")
        {
            sql = "DELETE FROM grassmanprchfert";
        }
        if (modelTable == "vegmanfert")
        {
            sql = "DELETE FROM vegmanprchfert";
        }

        sql = sql + " WHERE ";
        for (pos2 = 0; pos2 <= keys[pos].keys.count()-1;pos2++)
        {
            if ( keys[pos].keys[pos2].code != "NMONTH")
            {
                sql = sql + keys[pos].keys[pos2].code + " = ";
                sql = sql + "'" + keys[pos].keys[pos2].value.toString() + "' AND ";
            }
        }
        sql = sql.left(sql.length()-5);
        if (!tquery.exec(sql))
        {
            qWarning() << "Error: " << tquery.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }
    }
}

void fertiliserallocation::setHeader1Description(QString description)
{
    ui->header1->setText(description);
}

void fertiliserallocation::setHeader2Description(QString description)
{
    ui->header2->setText(description);
}

void fertiliserallocation::loadCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = true;
    colOk = false;

    if (index.column() > 1)
        colOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(database);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_fertSortModel->setData(index,calc.getCurrentDisplay());
    }
}
