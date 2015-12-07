#include "livestock.h"
#include "ui_livestock.h"
#include "mntdialog.h"
#include "gendelegates.h"
#include "lvprods.h"

#include <QSqlRecord>

livestock::livestock(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::livestock)
{
    ui->setupUi(this);
    list = ui->ListView1;
    connect(ui->BitBtn1,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn2,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->BitBtn3,SIGNAL(clicked()),this,SLOT(addNewItem()));
    connect(ui->BitBtn4,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(ui->BitBtn11,SIGNAL(clicked()),this,SLOT(closeForm()));

    connect(ui->Edit1,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    connect(ui->iMPRadioButton,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPRadioButton_2,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->iMPRadioButton_3,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    connect(this,SIGNAL(itemSelected(maintModel*,QModelIndex)),this,SLOT(chekitem(maintModel*,QModelIndex)));
    connect(this,SIGNAL(maintHasChanged()),this,SLOT(blockChildData()));


    MProducts = new childTableModel(this);


    MSortProducts = new childTableSortModel(this);


    //charm.activateOn(ui->ListView1);

    //Load the products table
    MProducts->setTableName("lvproducts");
    MProducts->setKeyField("live_COD");
    MProducts->setLinkedKeyField("PROD_COD");

    MProducts->addField("SALESBY","Sold by");
    MProducts->addField("WEIGHT","Ask for \nunit's weight");
    MProducts->addField("HHCONS","Household \nconsumption");
    MProducts->addField("HERD","Sold by herd \nstructure");
    MProducts->addField("MANURE","Is \nmanure");
    MProducts->addField("DRYMATTER","DM (%)");
    MProducts->addField("NITROGEN","N (%)");
    MProducts->addField("PHOSPHORUS","P (%)");
    MProducts->addField("POTASSIUM","K (%)");
    MProducts->addField("CARBON","C (%)");
    MProducts->addField("ENERGY","Ener. (MJ/kg)",Qt::lightGray);
    MProducts->addField("PROTEIN","Prot. (g/kg)",Qt::lightGray);


    MProducts->setLkTableName("products");
    MProducts->setLkKeyField("PROD_COD");
    MProducts->setLkDisplayColumn("PROD_DES");
    MProducts->setLkDisplayColumnDescription("Products");
    connect(MProducts,SIGNAL(childDataChanged()),this,SLOT(productsDataChanged()));



}
livestock::~livestock()
{
    //qDebug() << "Unloading maintenance";
    this->unLoadMaintenance();
    //qDebug() << "After unloading maintenance";
    delete ui;
    //qDebug() << "After delete ui";
}

void livestock::chekitem(maintModel *model, QModelIndex index)
{
    MProducts->checkItems(model->record(index.row()).value("LIVE_COD").toString());
    MSortProducts->sort(1);
    activeLive = model->record(index.row()).value("LIVE_COD").toString();
}

void livestock::blockChildData()
{
    setChildStatus(false);
}

void livestock::loadMaintenance(QSqlDatabase db,QTableView *tableView)
{

    MProducts->setDatabase(db);
    MProducts->loadItems();   
    MSortProducts->setSourceModel(MProducts);

    ui->ListView3->setModel(MSortProducts);
    ui->ListView3->resizeColumnsToContents();



    impgenmaint::loadMaintenance(db,tableView);

    genChkDelegate *chkDelegate;
    int pos;
    for (pos = 2; pos <= 5;pos++)
    {
        chkDelegate = new genChkDelegate(this);
        chkDelegate->setParentModel(MProducts);
        chkDelegate->setParentSortModel(MSortProducts);
        chkDelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
        chkDelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
        ui->ListView3->setItemDelegateForColumn(pos,chkDelegate);
    }

    genCmbDelegate *ldelegate = new genCmbDelegate(this);
    ldelegate->setParentModel(MProducts);
    ldelegate->setParentSortModel(MSortProducts);
    ldelegate->insertItem("U","Unit");
    ldelegate->insertItem("Q","kg or lt");

    ui->ListView3->setItemDelegateForColumn(1,ldelegate);
}

void livestock::productsDataChanged()
{
    setMasterStatus(false);
    ui->BitBtn7->setEnabled(true);
    ui->BitBtn9->setEnabled(true);
}


void livestock::setMasterStatus(bool status)
{
    ui->BitBtn1->setEnabled(status);
    ui->BitBtn2->setEnabled(status);
    ui->BitBtn3->setEnabled(status);
    ui->BitBtn4->setEnabled(status);
    ui->ListView1->setEnabled(status);
    this->setFieldsEnable(status);
}

void livestock::setChildStatus(bool status)
{
    ui->ListView3->setEnabled(status);
    ui->BitBtn9->setEnabled(status);
    ui->BitBtn7->setEnabled(status);
    ui->BitBtn6->setEnabled(status);
}


void livestock::applyChanges()
{
    impgenmaint::applyChanges();
    setChildStatus(true);
    ui->BitBtn7->setEnabled(false);
    ui->BitBtn9->setEnabled(false);
}

void livestock::cancelChanges()
{
    impgenmaint::cancelChanges();
    setChildStatus(true);
    ui->BitBtn7->setEnabled(false);
    ui->BitBtn9->setEnabled(false);
}

void livestock::on_BitBtn9_clicked()
{
    MProducts->cancelChanges();
    setMasterStatus(true);
    ui->BitBtn7->setEnabled(false);
    ui->BitBtn9->setEnabled(false);
    ui->ListView3->scrollToTop();
}

void livestock::on_BitBtn7_clicked()
{
    if (MProducts->applyChanges())
    {
        setMasterStatus(true);
        MSortProducts->sort(1);
        ui->BitBtn7->setEnabled(false);
        ui->BitBtn9->setEnabled(false);
        ui->ListView3->scrollToTop();
    }
}


void livestock::on_BitBtn6_clicked()
{
    mntdialog mntdlg;

    lvprods *mntwindow = new lvprods(0);
    mntwindow->loadMaintenance(database,mntwindow->list);
    mntdlg.setMaintWindow(mntwindow);
    mntdlg.setWindowTitle("Livestock products maintenance");
    mntdlg.exec();

    MProducts->loadItems();
    MProducts->checkItems(activeLive);
    MSortProducts->sort(1);
}
