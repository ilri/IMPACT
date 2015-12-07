#include "prchinscrops.h"
#include "ui_prchinscrops.h"
#include "delegates/imagecheckdelegate.h"
#include "delegates/fixcombodelegate.h"
#include <qsqlquery.h>
#include <QSqlError>
#include <calcdialog.h>
#include "moduleglobal.h"

prchinscrops::prchinscrops(QWidget *parent, QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::prchinscrops)
{
    ui->setupUi(this);
    dataBase = db;
    currentSystem = systemID;

    loadForm();
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

prchinscrops::~prchinscrops()
{
    //qDebug() << "Before destroy prchinscrops UI";
    delete ui;
    //qDebug() << "After destroy prchinscrops UI";
}

void prchinscrops::loadAndroidEffects()
{
    charmProds.activateOn(ui->AdvListView1);
    charmDetail.activateOn(ui->AdvListView2);
    connect(ui->AdvListView2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(loadDetCalc(QModelIndex)));
}

void prchinscrops::loadForm()
{

    m_cpprodsModel = new inputsTableModel(this);
    //m_cpprodsModel->setImpactModule(this);

    m_cpprodsModel->setDatabase(dataBase);
    loadProducts();
    ui->AdvListView1->setModel(m_cpprodsModel);
    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(loadCalendarData(QModelIndex)));
    //m_cpprodsModel->distributeColums();

    m_periodModel = new periodTableModel(this);
    m_periodModel->setDatabase(dataBase);
    //m_periodModel->setImpactModule(this);


    m_periodModel->addField("tim","Purchased in","N","");
    m_periodModel->addField("qty","Quantity (kg)","0","");
    m_periodModel->addField("pri","Price (KSH)","0",""); //Add currency!!!!
    connect(m_periodModel,SIGNAL(modeldDataChanged()),this,SLOT(calendarDataChanged()));
    ui->AdvListView2->setModel(m_periodModel);

    if (m_cpprodsModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_cpprodsModel->index(0,0);
        loadCalendarData(start);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
    {
        setCalendarStatus(false);
    }

    for (int rsize = 0; rsize <= m_cpprodsModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

}

void prchinscrops::loadProducts()
{
    QList<TmoduleFieldDef> masterKeys;
    TmoduleFieldDef key;
    //Add crops
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    m_cpprodsModel->addMasterTable("cropman",masterKeys,"Crop input");
    m_cpprodsModel->addYieldTable("cropman","cropmantim","QTYPRCH");

    //Add fertilizers in crops
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "FERTYP_COD";
    masterKeys.append(key);
    m_cpprodsModel->addMasterTable("cropmanprchfert",masterKeys,"Crop fertilizer");
    m_cpprodsModel->addYieldTable("cropmanprchfert","cropmanfert","QTYPRCH");

    //Add pastures
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMPADD";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    m_cpprodsModel->addMasterTable("grassman",masterKeys,"Crop input");
    m_cpprodsModel->addYieldTable("grassman","grassmantim","QTYPRCH");

    //Add fertilizers in pastures
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMPADD";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "FERTYP_COD";
    masterKeys.append(key);
    m_cpprodsModel->addMasterTable("grassmanprchfert",masterKeys,"Crop fertilizer");
    m_cpprodsModel->addYieldTable("grassmanprchfert","grassmanfert","QTYPRCH");

    //Add vegetables
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "VEG_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    m_cpprodsModel->addMasterTable("vegman",masterKeys,"Crop input");
    m_cpprodsModel->addYieldTable("vegman","vegmantim","QTYPRCH");

    //Add fertilizers in pastures
    masterKeys.clear();
    key.code = "SYS_COD";
    masterKeys.append(key);
    key.code = "PLOT_COD";
    masterKeys.append(key);
    key.code = "NUMSEASON";
    masterKeys.append(key);
    key.code = "CROP_COD";
    masterKeys.append(key);
    key.code = "VEG_COD";
    masterKeys.append(key);
    key.code = "ACTY_COD";
    masterKeys.append(key);
    key.code = "FERTYP_COD";
    masterKeys.append(key);
    m_cpprodsModel->addMasterTable("vegmanprchfert",masterKeys,"Crop fertilizer");
    m_cpprodsModel->addYieldTable("vegmanprchfert","vegmanfert","QTYPRCH");


    masterKeys.clear();
    key.code = "SYS_COD";
    key.value = currentSystem;
    masterKeys.append(key);
    m_cpprodsModel->loadItems(masterKeys);


}

void prchinscrops::constructCustomDelegator()
{

    fixComboDelegate *ldelegate = new fixComboDelegate(this);
    QSqlQuery tquery(dataBase);
    QString sql;
    sql = "SELECT ACTY_COD,ACTY_DES FROM activity";
    tquery.exec(sql);
    while (tquery.next())
    {
        ldelegate->insertItem(tquery.value(0).toString(),tquery.value(1).toString());
    }
    ldelegate->setReadOnly(true);
    ui->AdvListView1->setItemDelegateForColumn(2,ldelegate);

    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate->addIgnoredColumn(0);
    ui->AdvListView2->setItemDelegateForRow(0,ckhdelegate);

    for (int pos =0; pos <= m_periodModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView2->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

}

void prchinscrops::loadCalendarData(QModelIndex index)
{
    if (!index.isValid())
        return;
    m_periodModel->setTableName(m_cpprodsModel->getMasterTable(index.row())+"periods");
    m_periodModel->loadData(m_cpprodsModel->getRowKeyValues(index.row()));
    //m_periodModel->distributeColums();
    ui->AdvListView2->setEnabled(true);

    for (int rsize = 0; rsize <= m_periodModel->rowCount()-1;rsize++)
        ui->AdvListView2->setRowHeight(rsize,listViewRowSize);
}

void prchinscrops::setProductsStatus(bool status)
{
    ui->AdvListView1->setEnabled(status);
}

void prchinscrops::setCalendarStatus(bool status)
{
    ui->AdvListView2->setEnabled(status);
    ui->BitBtn1->setEnabled(status);
    ui->BitBtn2->setEnabled(status);
}

void prchinscrops::calendarDataChanged()
{
    setProductsStatus(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn2->setEnabled(true);
}

void prchinscrops::on_BitBtn1_clicked()
{
    m_periodModel->applyData();
    setProductsStatus(true);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
}

void prchinscrops::on_BitBtn2_clicked()
{
    m_periodModel->cancelData();
    setProductsStatus(true);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn2->setEnabled(false);
}

void prchinscrops::loadDetCalc (const QModelIndex &index)
{
    bool rowOk;
    bool colOk;
    rowOk = false;
    colOk = false;

    if (index.column() > 0)
        colOk = true;

    if (index.row() > 0)
        rowOk = true;

    if (rowOk && colOk)
    {
        calcDialog calc;
        calc.setDatabase(dataBase);
        calc.setCurrentDisplay(index.data().toFloat());
        calc.exec();
        m_periodModel->setData(index,calc.getCurrentDisplay());
    }
}
