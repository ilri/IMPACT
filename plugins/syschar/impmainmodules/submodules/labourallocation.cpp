#include "labourallocation.h"
#include "ui_labourallocation.h"
#include <qsqlquery.h>
#include <qsqlrecord.h>
#include <calcdialog.h>
#include "moduleglobal.h"
#include <QMessageBox>

labourallocation::labourallocation(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::labourallocation)
{
    ui->setupUi(this);
    m_labourModel = new linkedTableModel(this);
    m_labourSortModel = new linkedTableSortModel(this);
    connect(ui->BitBtn9,SIGNAL(clicked()),this,SLOT(applyToAll()));
    connect(ui->BitBtn2,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn3,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->BitBtn4,SIGNAL(clicked()),this,SIGNAL(closeCalled()));

    connect(ui->iMPEdit,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
}

labourallocation::~labourallocation()
{
    //qDebug() << "Before destroy labourallocation UI";
    delete ui;
    //qDebug() << "After destroy labourallocation UI";
    //qDebug() << "Before destroy labourTable";
    delete labourTable;
    labourTable = 0;
    // qDebug() << "After destroy labourTable";
}

void labourallocation::loadAndroidEffects()
{
    charm.activateOn(ui->ListView1);
    connect(ui->ListView1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadCalc(QModelIndex)));
}

void labourallocation::loadMaintenance(QSqlDatabase db,QTableView *)
{
    labourTable = new QSqlQuery(db);
    connect(m_labourModel,SIGNAL(modelDataChanged()),this,SLOT(dataChanged()));
   // m_labourModel->setImpactModule(this);

    m_labourModel->setDatabase(db);
    m_labourModel->setTableName(modelTable);
    m_labourModel->setKeyFields(m_keys);
    m_labourModel->setLinkedKeyField("PERS_COD");
    m_labourModel->addField("FREQUENCY","Frequency \n (days/month)");
    m_labourModel->addField("HOURSDAY","Duration \n (hours/day)");
    m_labourModel->addField("PPERHOUR","Cost \n (CURR/Hour)"); //Change to currency!
    m_labourModel->setLkTableName("labforce");
    m_labourModel->setLkKeyField("PERS_COD");
    m_labourModel->setLkDisplayColumn("AGE");
    m_labourModel->setLkUseCustomDescription(true);
    m_labourModel->setLkDisplayColumnDescription("Household and \n hired labour");

    QList <TmoduleFieldDef> kfields;
    TmoduleFieldDef field;
    QString sql;
    sql = "SELECT * FROM labforce WHERE SYS_COD = '";
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
    labourTable->exec(sql);
    m_labourModel->loadItems(kfields);
    m_labourModel->checkItems();
    m_labourSortModel->setSourceModel(m_labourModel);
    m_labourSortModel->sort(1);
    ui->ListView1->setModel(m_labourSortModel);
    //m_labourModel->distributeColums();
    impgenmaint::loadMaintenance(db);

    if (m_labourModel->hasCheckedItems())
    {
        ui->radioButton->setChecked(true);
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        if ((ui->iMPEdit->text().toInt() != 0 ) ||
            (ui->iMPEdit_2->text().toInt() != 0 ))
        {
            ui->radioButton_2->setChecked(true);
            ui->stackedWidget->setCurrentIndex(1);
        }
        else
        {
            ui->radioButton->setChecked(true);
            ui->stackedWidget->setCurrentIndex(0);
        }
    }

    ui->ListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->ListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->ListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->ListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);


    if (modules_use_Android == true)
        loadAndroidEffects();

    for (int rsize = 0; rsize <= m_labourSortModel->rowCount()-1;rsize++)
        ui->ListView1->setRowHeight(rsize,listViewRowSize);
}

void labourallocation::setKeyFields(QList <TmoduleFieldDef> keyFields)
{
    m_keys.append(keyFields);
}

int labourallocation::getFieldIndex(QSqlQuery *query,QString fieldName)
{
    return query->record().indexOf(fieldName); //Return the index of fieldName;
}

void labourallocation::dataChanged()
{
    ui->BitBtn2->setEnabled(true);
    ui->BitBtn3->setEnabled(true);
    ui->BitBtn9->setEnabled(true);
}

void labourallocation::applyChanges()
{
    if (m_labourModel->applyChanges())
    {
        ui->BitBtn2->setEnabled(false);
        ui->BitBtn3->setEnabled(false);
        ui->BitBtn9->setEnabled(false);
    }
    impgenmaint::applyChanges();
}

void labourallocation::cancelChanges()
{
    m_labourModel->cancelChanges();
    ui->BitBtn2->setEnabled(false);
    ui->BitBtn3->setEnabled(false);
    ui->BitBtn9->setEnabled(false);
    impgenmaint::cancelChanges();
}

void labourallocation::applyToAll()
{
    if (m_labourModel->applyChanges())
    {
        ui->BitBtn2->setEnabled(false);
        ui->BitBtn3->setEnabled(false);
        ui->BitBtn9->setEnabled(false);
    }
    //Need to do apply for all for the model
    impgenmaint::applyToAll();
}

void labourallocation::on_radioButton_clicked()
{
    if ((ui->iMPEdit->text().toInt() == 0 ) &&
        (ui->iMPEdit_2->text().toInt() == 0 ))
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui->radioButton_2->setChecked(true);
        ui->radioButton->setChecked(false);
        QMessageBox msgBox;
        msgBox.setText(tr("You can't allocate detailed labour while having information by manday"));
        msgBox.exec();
    }
}

void labourallocation::on_radioButton_2_clicked()
{
    if (!m_labourModel->hasCheckedItems())
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        ui->radioButton_2->setChecked(false);
        ui->radioButton->setChecked(true);
        QMessageBox msgBox;
        msgBox.setText(tr("You can't allocate labour by mandays while having detail information"));
        msgBox.exec();
    }
}

void labourallocation::setModelTable(QString table)
{
    modelTable = table;
}

void labourallocation::valueHasChanged(QString , QString , QString )
{
    ui->BitBtn2->setEnabled(true);
    ui->BitBtn3->setEnabled(true);
    ui->BitBtn9->setEnabled(true);
}

void labourallocation::setHeader1Description(QString description)
{
    ui->header1->setText(description);
}

void labourallocation::setHeader2Description(QString description)
{
    ui->header2->setText(description);
}

void labourallocation::loadCalc (const QModelIndex &index)
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
        m_labourSortModel->setData(index,calc.getCurrentDisplay());
    }
}

