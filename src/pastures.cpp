#include "pastures.h"
#include "ui_pastures.h"
#include "crpforages.h"
#include "crpprods.h"
#include "mntdialog.h"

#include <QSqlRecord>

pastures::pastures(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::pastures)
{
    ui->setupUi(this);
    list = ui->ListView1;
    connect(ui->BitBtn1,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn2,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->BitBtn3,SIGNAL(clicked()),this,SLOT(addNewItem()));
    connect(ui->BitBtn4,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(ui->BitBtn11,SIGNAL(clicked()),this,SLOT(closeForm()));

    connect(ui->Edit1,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));
    connect(ui->CheckBox1,SIGNAL(valueChanged(QString,QString,bool)),this,SLOT(valueHasChanged(QString,QString,bool)));

    connect(this,SIGNAL(itemSelected(maintModel*,QModelIndex)),this,SLOT(chekitem(maintModel*,QModelIndex)));
    connect(this,SIGNAL(maintHasChanged()),this,SLOT(blockChildData()));


    MProducts = new childTableModel(this);
    MForages = new childTableModel(this);

    MSortProducts = new childTableSortModel(this);
    MSortForgages = new childTableSortModel(this);

    //charm.activateOn(ui->ListView1);

    //Load the products table
    MProducts->setTableName("pastprods");
    MProducts->setKeyField("CROP_COD");
    MProducts->setLinkedKeyField("PROD_COD");
    MProducts->addField("DRYMATTER","DM (%)");
    MProducts->addField("NITROGEN","N (%)");
    MProducts->addField("PHOSPHORUS","P (%)");
    MProducts->addField("POTASSIUM","K (%)");
    MProducts->addField("CARBON","K (%)");
    MProducts->addField("ENERGY","Ener. (MJ/kg)",Qt::lightGray);
    MProducts->addField("PROTEIN","Prot. (g/kg)",Qt::lightGray);
    MProducts->addField("CPROTEIN","CP (%)",Qt::darkGray);
    MProducts->addField("NDF","NDF (%)",Qt::darkGray);
    MProducts->addField("DIGESTI","Dig. (%)",Qt::darkGray);
    MProducts->addField("ANENERGY","M.E. (MJ/kg)",Qt::darkGray);
    MProducts->setLkTableName("cpprods");
    MProducts->setLkKeyField("PROD_COD");
    MProducts->setLkDisplayColumn("PROD_DES");
    MProducts->setLkDisplayColumnDescription("Products");
    connect(MProducts,SIGNAL(childDataChanged()),this,SLOT(productsDataChanged()));

    //Load the forages table
    MForages->setTableName("pastfora");
    MForages->setKeyField("CROP_COD");
    MForages->setLinkedKeyField("FORA_COD");
    MForages->addField("DRYMATTER","DM (%)");
    MForages->addField("NITROGEN","N (%)");
    MForages->addField("PHOSPHORUS","P (%)");
    MForages->addField("POTASSIUM","K (%)");
    MForages->addField("CARBON","K (%)");
    MForages->addField("ENERGY","Ener. (MJ/kg)",Qt::lightGray);
    MForages->addField("PROTEIN","Prot. (g/kg)",Qt::lightGray);
    MForages->addField("CPROTEIN","CP (%)",Qt::darkGray);
    MForages->addField("NDF","NDF (%)",Qt::darkGray);
    MForages->addField("DIGESTI","Dig. (%)",Qt::darkGray);
    MForages->addField("ANENERGY","M.E. (MJ/kg)",Qt::darkGray);
    MForages->setLkTableName("forages");
    MForages->setLkKeyField("FORA_COD");
    MForages->setLkDisplayColumn("FORA_DES");
    MForages->setLkDisplayColumnDescription("Forages");
    connect(MForages,SIGNAL(childDataChanged()),this,SLOT(foragesDataChanged()));

}
pastures::~pastures()
{
    //qDebug() << "Unloading maintenance";
    this->unLoadMaintenance();
    //qDebug() << "After unloading maintenance";
    delete ui;
    //qDebug() << "After delete ui";
}

void pastures::chekitem(maintModel *model, QModelIndex index)
{
    MProducts->checkItems(model->record(index.row()).value("CROP_COD").toString());
    MForages->checkItems(model->record(index.row()).value("CROP_COD").toString());
    MSortProducts->sort(1);
    MSortForgages->sort(1);

    activeCrop = model->record(index.row()).value("CROP_COD").toString();
}

void pastures::blockChildData()
{
    setChildStatus(false);
}

void pastures::loadMaintenance(QSqlDatabase db,QTableView *tableView)
{

    MProducts->setDatabase(db);
    MProducts->loadItems();   
    MSortProducts->setSourceModel(MProducts);

    ui->ListView3->setModel(MSortProducts);
    ui->ListView3->resizeColumnsToContents();


    MForages->setDatabase(db);
    MForages->loadItems();
    MSortForgages->setSourceModel(MForages);

    ui->ListView2->setModel(MSortForgages);
    ui->ListView2->resizeColumnsToContents();


    impgenmaint::loadMaintenance(db,tableView);
}

void pastures::productsDataChanged()
{
    setMasterStatus(false);
    ui->BitBtn7->setEnabled(true);
    ui->BitBtn9->setEnabled(true);
}

void pastures::foragesDataChanged()
{
     setMasterStatus(false);
     ui->BitBtn8->setEnabled(true);
     ui->BitBtn10->setEnabled(true);
}

void pastures::setMasterStatus(bool status)
{
    ui->BitBtn1->setEnabled(status);
    ui->BitBtn2->setEnabled(status);
    ui->BitBtn3->setEnabled(status);
    ui->BitBtn4->setEnabled(status);
    ui->ListView1->setEnabled(status);
    this->setFieldsEnable(status);
}

void pastures::setChildStatus(bool status)
{
    ui->ListView3->setEnabled(status);
    ui->BitBtn9->setEnabled(status);
    ui->BitBtn7->setEnabled(status);
    ui->BitBtn6->setEnabled(status);

    ui->ListView2->setEnabled(status);
    ui->BitBtn5->setEnabled(status);
    ui->BitBtn8->setEnabled(status);
    ui->BitBtn10->setEnabled(status);
}


void pastures::applyChanges()
{
    impgenmaint::applyChanges();
    setChildStatus(true);
    ui->BitBtn7->setEnabled(false);
    ui->BitBtn9->setEnabled(false);
    ui->BitBtn8->setEnabled(false);
    ui->BitBtn10->setEnabled(false);
}

void pastures::cancelChanges()
{
    impgenmaint::cancelChanges();
    setChildStatus(true);
    ui->BitBtn7->setEnabled(false);
    ui->BitBtn9->setEnabled(false);
    ui->BitBtn8->setEnabled(false);
    ui->BitBtn10->setEnabled(false);
}

void pastures::on_BitBtn9_clicked()
{
    MProducts->cancelChanges();
    setMasterStatus(true);
    ui->BitBtn7->setEnabled(false);
    ui->BitBtn9->setEnabled(false);
    ui->ListView3->scrollToTop();
}

void pastures::on_BitBtn7_clicked()
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

void pastures::on_BitBtn10_clicked()
{
    MForages->cancelChanges();
    setMasterStatus(true);
    ui->BitBtn8->setEnabled(false);
    ui->BitBtn10->setEnabled(false);
    ui->ListView2->scrollToTop();
}

void pastures::on_BitBtn8_clicked()
{
    if (MForages->applyChanges())
    {
        setMasterStatus(true);
        MSortForgages->sort(1);
        ui->BitBtn8->setEnabled(false);
        ui->BitBtn10->setEnabled(false);
        ui->ListView2->scrollToTop();
    }
}

void pastures::on_BitBtn6_clicked()
{
    mntdialog mntdlg;

    crpprods *mntwindow = new crpprods(0);
    mntwindow->loadMaintenance(database,mntwindow->list);
    mntdlg.setMaintWindow(mntwindow);
    mntdlg.setWindowTitle("Pasture products maintenance");
    mntdlg.exec();

    MProducts->loadItems();
    MProducts->checkItems(activeCrop);
    MSortProducts->sort(1);
}

void pastures::on_BitBtn5_clicked()
{
    mntdialog mntdlg;

    crpforages *mntwindow = new crpforages(0);
    mntwindow->loadMaintenance(database,mntwindow->list);
    mntdlg.setMaintWindow(mntwindow);
    mntdlg.setWindowTitle("Pasture forages maintenance");
    mntdlg.exec();

    MForages->loadItems();
    MForages->checkItems(activeCrop);
    MSortForgages->sort(1);
}
