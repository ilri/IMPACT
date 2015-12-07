#include "activity.h"
#include "ui_activity.h"
#include "mntdialog.h"
#include "methods.h"
#include <QSqlRecord>

activities::activities(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::activities)
{
    ui->setupUi(this);
    list = ui->ListView1;

    this->setSortColID(2);

    connect(ui->BitBtn1,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->BitBtn2,SIGNAL(clicked()),this,SLOT(cancelChanges()));
    connect(ui->BitBtn3,SIGNAL(clicked()),this,SLOT(addNewItem()));
    connect(ui->BitBtn4,SIGNAL(clicked()),this,SLOT(removeItem()));
    connect(ui->BitBtn11,SIGNAL(clicked()),this,SLOT(closeForm()));

    connect(ui->Edit1,SIGNAL(valueChanged(QString,QString,QString)),this,SLOT(valueHasChanged(QString,QString,QString)));


    connect(this,SIGNAL(itemSelected(maintModel*,QModelIndex)),this,SLOT(chekitem(maintModel*,QModelIndex)));
    connect(this,SIGNAL(maintHasChanged()),this,SLOT(blockChildData()));


    MMethods = new childSelectTableModel(this);
    MUnits = new childSelectTableModel(this);

    MSortMethods = new childSelectTableSortModel(this);
    MSortUnits = new childSelectTableSortModel(this);

    //charm.activateOn(ui->ListView1);

    //Load the products table

    MMethods->setLkTableName("actymethods");
    MMethods->setLkKeyField("met_COD");
    MMethods->setLkDisplayColumn("met_DES");
    MMethods->setLkDisplayColumnDescription("Methods");
    MMethods->setUseCheckBoxes(false);

    //Load the forages table

    MUnits->setLkTableName("actyunits");
    MUnits->setLkKeyField("unit_COD");
    MUnits->setLkDisplayColumn("unit_DES");
    MUnits->setLkDisplayColumnDescription("Units");
    connect(MUnits,SIGNAL(childDataChanged()),this,SLOT(unitsDataChanged()));

    ui->txtunit->setVisible(false);

}
activities::~activities()
{
    //qDebug() << "Unloading maintenance";
    this->unLoadMaintenance();
    //qDebug() << "After unloading maintenance";
    delete ui;
    //qDebug() << "After delete ui";
}

void activities::chekitem(maintModel *model, QModelIndex index)
{
    TfieldDef keyfield;
    keyfield.fieldName = "acty_COD";
    keyfield.fieldValue = model->record(index.row()).value("acty_COD");
    MMethods->loadItems(keyfield);
    MUnits->checkItems(model->record(index.row()).value("unit_COD").toString());
    MSortMethods->sort(1);
    MSortUnits->sort(1);
    currAct = model->record(index.row()).value("acty_COD").toString();
}

void activities::blockChildData()
{
    setChildStatus(false);
}

void activities::loadMaintenance(QSqlDatabase db,QTableView *tableView)
{

    MMethods->setDatabase(db);
    MSortMethods->setSourceModel(MMethods);

    ui->ListView3->setModel(MSortMethods);
    ui->ListView3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);


    MUnits->setDatabase(db);
    MUnits->loadItems();
    MSortUnits->setSourceModel(MUnits);

    ui->ListView2->setModel(MSortUnits);
    ui->ListView2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);


    impgenmaint::loadMaintenance(db,tableView);
}



void activities::unitsDataChanged()
{
    ui->txtunit->setText(MUnits->getSelectedCode());
    valueHasChanged(ui->txtunit->tableName(),ui->txtunit->fieldName(),ui->txtunit->text());
}

void activities::setMasterStatus(bool status)
{
    ui->BitBtn1->setEnabled(status);
    ui->BitBtn2->setEnabled(status);
    ui->BitBtn3->setEnabled(status);
    ui->BitBtn4->setEnabled(status);
    ui->ListView1->setEnabled(status);
    this->setFieldsEnable(status);
}

void activities::setChildStatus(bool status)
{
    ui->ListView3->setEnabled(status);
    ui->BitBtn6->setEnabled(status);
    ui->ListView2->setEnabled(status);
    ui->BitBtn5->setEnabled(status); 
}


void activities::applyChanges()
{
    impgenmaint::applyChanges();
    setChildStatus(true);
}

void activities::cancelChanges()
{
    impgenmaint::cancelChanges();
    setChildStatus(true);
}



void activities::on_BitBtn6_clicked()
{
    mntdialog mntdlg;

    methods *mntwindow = new methods(0);
    mntwindow->setActivity(currAct);
    mntwindow->insertKeyField("acty_cod",currAct);
    mntwindow->loadMaintenance(database,mntwindow->list);
    mntdlg.setMaintWindow(mntwindow);
    mntdlg.setWindowTitle("Activity methods maintenance");
    mntdlg.exec();

    TfieldDef keyfield;
    keyfield.fieldName = "acty_COD";
    keyfield.fieldValue = currAct;
    MMethods->loadItems(keyfield);
    MSortMethods->sort(1);
}
