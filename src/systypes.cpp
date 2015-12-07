#include "systypes.h"
#include "ui_systypes.h"
#include <QSqlRecord>

systypes::systypes(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::systypes)
{
    ui->setupUi(this);
    list = ui->ListView1;
    connect(ui->BitBtn1,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn2,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->BitBtn3,SIGNAL(clicked()),this,SLOT(addNewItem()));
    connect(ui->BitBtn4,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(ui->BitBtn11,SIGNAL(clicked()),this,SLOT(closeForm()));

    connect(ui->Edit1,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));

    connect(this,SIGNAL(itemSelected(maintModel*,QModelIndex)),this,SLOT(chekitem(maintModel*,QModelIndex)));



    MCrops = new childSelectTableModel(this);
    MLivestock = new childSelectTableModel(this);

    MSortCrops = new childSelectTableSortModel(this);
    MSortLivestock = new childSelectTableSortModel(this);

    //charm.activateOn(ui->ListView1);

    //Load the products table

    MCrops->setLkTableName("crop");
    MCrops->setLkKeyField("crop_cod");
    MCrops->setLkDisplayColumn("crop_DES");
    MCrops->setLkDisplayColumnDescription("Crops");
    connect(MCrops,SIGNAL(childDataChanged()),this,SLOT(cropsDataChanged()));

    //Load the forages table

    MLivestock->setLkTableName("livestoc");
    MLivestock->setLkKeyField("LIVE_COD");
    MLivestock->setLkDisplayColumn("LIVE_DES");
    MLivestock->setLkDisplayColumnDescription("Livestock");
    connect(MLivestock,SIGNAL(childDataChanged()),this,SLOT(livestockDataChanged()));

    ui->txtcrop->setVisible(false);
    ui->txtlive->setVisible(false);

}
systypes::~systypes()
{
    this->unLoadMaintenance();
    delete ui;
}

void systypes::chekitem(maintModel *model, QModelIndex index)
{
    MCrops->checkItems(model->record(index.row()).value("CROP_COD").toString());
    MLivestock->checkItems(model->record(index.row()).value("LIVE_COD").toString());
    MSortCrops->sort(0);
    MSortLivestock->sort(0);
}



void systypes::loadMaintenance(QSqlDatabase db,QTableView *tableView)
{

    MCrops->setDatabase(db);
    MCrops->loadItems();
    MSortCrops->setSourceModel(MCrops);

    ui->ListView3->setModel(MSortCrops);
    ui->ListView3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);


    MLivestock->setDatabase(db);
    MLivestock->loadItems();
    MSortLivestock->setSourceModel(MLivestock);

    ui->ListView2->setModel(MSortLivestock);
    ui->ListView2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);


    impgenmaint::loadMaintenance(db,tableView);

    MLivestock->checkItems(ui->txtlive->text());
    MCrops->checkItems(ui->txtcrop->text());

    MSortCrops->sort(0);
    MSortLivestock->sort(0);
}

void systypes::cropsDataChanged()
{
    ui->txtcrop->setText(MCrops->getSelectedCode());
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn2->setEnabled(true);
    valueHasChanged(ui->txtcrop->tableName(),ui->txtcrop->fieldName(),ui->txtcrop->text());
}

void systypes::livestockDataChanged()
{
    ui->txtlive->setText(MLivestock->getSelectedCode());
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn2->setEnabled(true);
    valueHasChanged(ui->txtlive->tableName(),ui->txtlive->fieldName(),ui->txtlive->text());
}


void systypes::applyChanges()
{
    ui->txtcrop->setText(MCrops->getSelectedCode());
    ui->txtlive->setText(MLivestock->getSelectedCode());

    impgenmaint::applyChanges();


    MSortCrops->sort(0);
    MSortLivestock->sort(0);
}

void systypes::cancelChanges()
{
    impgenmaint::cancelChanges();

    MCrops->checkItems(ui->txtcrop->text());
    MLivestock->checkItems(ui->txtlive->text());

    MSortCrops->sort(0);
    MSortLivestock->sort(0);
}
