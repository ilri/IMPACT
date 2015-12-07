#include "grassman.h"
#include "ui_grassman.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qpixmap.h>
#include "submodules/cropyield.h"
#include "submodules/labourallocation.h"
#include "submodules/fertiliserallocation.h"
#include "submodules/inputsallocation.h"
#include "submodules/manureallocation.h"
#include "modulesubscreen.h"
#include "moduleglobal.h"
#include <QMenu>
#include <QDebug>

grassman::grassman(QWidget *parent,QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::grassman)
{
    dataBase = db;
    currentSystem = systemID;
    ui->setupUi(this);
    loadForm();
    createActions();
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

grassman::~grassman()
{
    //qDebug() << "Before destroy grassman UI";
    delete ui;
    //qDebug() << "After destroy grassman UI";
}

void grassman::loadAndroidEffects()
{
    cropcharm.activateOn(ui->AdvListView1);
    actcharm.activateOn(ui->AdvListView4);
}

void grassman::loadForm()
{
    connect(ui->AdvListView1,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(cropMenuRequested(QPoint)));

    ui->AdvListView1->setContextMenuPolicy(Qt::CustomContextMenu);

    //Creates and load the parent model
    m_parentModel = new fieldInColModel(this);
    m_parentModel->setReadOnly(true);
    m_parentModel->setDatabase(dataBase);
    //m_parentModel->setImpactModule(this);
    m_parentModel->setTableName("plotpaddspe");


    m_parentModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_parentModel->insertField("PLOT_COD","Plot ID","000","000",true,false,true,true);
    m_parentModel->insertField("NUMPADD","Paddock","000","000",true,false,true);
    m_parentModel->insertField("CROP_COD","Pasture","000","000",true,false,true,true);
    //m_parentModel->setWhereClause(" CROP_COD NOT LIKE ')%' AND VEGETABLE = 'N'");

    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);
    m_parentModel->loadData(keys);
    ui->AdvListView1->setModel(m_parentModel);
    //m_parentModel->distributeColums();
    connect(ui->AdvListView1,SIGNAL(clicked(QModelIndex)),this,SLOT(parentViewActivated(QModelIndex)));
    ui->AdvListView1->setSelectionBehavior(QAbstractItemView::SelectRows);


    //Creates and loads activity model
    m_activityModel = new managementModel(this);
    m_actSortModel = new managementSortModel(this);
    connect(m_activityModel,SIGNAL(modeldDataChanged()),this,SLOT(activityChanged()));


    m_activityModel->setDatabase(dataBase);
    //m_activityModel->setImpactModule(this);

    QList<TmoduleFieldDef> cpmankeys;
    key.code = "SYS_COD";
    cpmankeys.append(key);
    key.code = "PLOT_COD";
    cpmankeys.append(key);
    key.code = "NUMPADD";
    cpmankeys.append(key);
    key.code = "CROP_COD";
    cpmankeys.append(key);


    m_activityModel->addMasterTable("grassman");
    m_activityModel->setKeysFields("grassman",cpmankeys);
    m_activityModel->setLookUpLinkedField("grassman","ACTY_COD");
    m_activityModel->setPeriodTable("grassman","grassmantim");
    key.code = "ACTY_COD";
    cpmankeys.append(key);
    m_activityModel->setPeriodKeyFields("grassman",cpmankeys);
    m_activityModel->setPeriodField("grassman","NMONTH");
    m_activityModel->setPeriodStatusField("grassman","CHECKED");
    m_activityModel->setPeriodDefaultValue("grassman","N");



    m_activityModel->setlkUpTable("grassman","activity");
    m_activityModel->setlkUpCodeFields("grassman","ACTY_COD");
    m_activityModel->setlkUpDescFields("grassman","ACTY_DES");
    m_activityModel->setlkUpWhereClause("grassman"," ACTY_TYP = 'C'");
    m_activityModel->setlkUpColDescription("Activities");
    m_activityModel->setlkUpUnChkColor("grassman",QColor(230,255,255));
    m_activityModel->setlkUpChkColor("grassman",QColor(199,255,255));

    m_actSortModel->setSourceModel(m_activityModel);
    m_activityModel->loadItems();
    //m_activityModel->setItemEdtStatus("grassman","006",false);
    m_actSortModel->sort(1);
    ui->AdvListView4->setModel(m_actSortModel);
    //m_activityModel->distributeColums();

    if (m_parentModel->rowCount() > 0)
    {
        QModelIndex start;
        start = m_parentModel->index(0,0);
        parentViewActivated(start);
        ui->AdvListView1->setCurrentIndex(start);
    }
    else
    {
        ui->AdvListView4->setEnabled(false);
    }

    ui->AdvListView4->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->AdvListView4,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(actMenuRequested(QPoint)));

    for (int rsize = 0; rsize <= m_parentModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);
}

void grassman::constructCustomDelegator()
{
    fixComboDelegate *pdelegate = new fixComboDelegate(this);
    pdelegate->setReadOnly(true);
    QSqlQuery plots(dataBase);
    QString sql;
    QString desc;
    sql = "SELECT PLOT_COD,PLOT_ID,AREA FROM spadstown WHERE sys_cod = '" + currentSystem + "'";
    plots.exec(sql);
    while(plots.next())
    {
        desc = plots.value(1).toString() + " (" + plots.value(2).toString() + "ha)";
        pdelegate->insertItem(plots.value(0).toString(),desc);
    }
    ui->AdvListView1->setItemDelegateForColumn(0,pdelegate);


    fixComboDelegate *cdelegate = new fixComboDelegate(this);
    cdelegate->setReadOnly(true);
    sql = "SELECT CROP_COD,CROP_DES FROM pastures";
    plots.exec(sql);
    while(plots.next())
    {
        desc = plots.value(1).toString();
        cdelegate->insertItem(plots.value(0).toString(),desc);
    }
    ui->AdvListView1->setItemDelegateForColumn(2,cdelegate);


    imageCheckDelegate *ckhdelegate = new imageCheckDelegate(this);
    ckhdelegate->addIgnoredColumn(0);
    ckhdelegate->setCheckPixMap(QPixmap(":/images/ok.png"));
    ckhdelegate->setUnCheckPixMap(QPixmap(":/images/nocheck.png"));
    ckhdelegate->setManagementModel(m_activityModel);
    ckhdelegate->setSortModel(m_actSortModel);
    connect(ckhdelegate,SIGNAL(requestPopUpMenu(QPoint,QModelIndex)),this,SLOT(constructActPopUpMenu(QPoint,QModelIndex)));

    int pos;
    for (pos = 0; pos <= m_actSortModel->columnCount()-1;pos++)
    {
        if (pos > 0)
        {
            ui->AdvListView4->setItemDelegateForColumn(pos,ckhdelegate);
        }
    }

    for (pos =0; pos <= m_actSortModel->columnCount()-1;pos++)
    {
        if (pos <= 0)
            ui->AdvListView4->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::ResizeToContents);
        else
            ui->AdvListView4->horizontalHeader()->setSectionResizeMode(pos, QHeaderView::Stretch);
    }

    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

}

void grassman::constructHeader1(QList<TmoduleFieldDef> keys)
{
    header1 = "";
    QString plot = "";
    QString season = "";
    QString paddock = "";
    QString crop = "";
    QString vege = "";
    QSqlQuery query(dataBase);
    QString sql;
    int pos;
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        if (keys[pos].code.toUpper() == "PLOT_COD")
            plot = keys[pos].value.toString();
        if (keys[pos].code.toUpper() == "NUMSEASON")
            season = keys[pos].value.toString();
        if (keys[pos].code.toUpper() == "NUMPADD")
            paddock = keys[pos].value.toString();
        if (keys[pos].code.toUpper() == "CROP_COD")
            crop = keys[pos].value.toString();
        if (keys[pos].code.toUpper() == "VEG_COD")
            vege = keys[pos].value.toString();
    }
    sql = "SELECT PLOT_ID,AREA FROM spadstown WHERE sys_cod = '" + currentSystem + "'";
    sql = sql + " AND PLOT_COD = '" + plot + "'";
    if (query.exec(sql))
    {
        if (query.first())
        {
            header1 = "Plot: " + query.value(0).toString() + "(";
            header1 = header1 + query.value(1).toString() + " ha)";
        }
    }
    if (!vege.isEmpty())
    {
        header1 = header1 + " Season: " + season;
        sql = "SELECT CROP_COD,CROP_DES FROM crop WHERE CROP_COD = '" + vege + "'";
        if (query.exec(sql))
        {
            if (query.first())
            {
                header1 = header1 + " Vegetable: " + query.value(1).toString();
            }
        }
        return;
    }
    if (!paddock.isEmpty())
    {
        header1 = header1 + " Paddock: " + paddock;
        sql = "SELECT CROP_COD,CROP_DES FROM pastures WHERE CROP_COD = '" + crop + "'";
        if (query.exec(sql))
        {
            if (query.first())
            {
                header1 = header1 + " Crop: " + query.value(1).toString();
            }
        }
        return;
    }
    header1 = header1 + " Season: " + season;
    sql = "SELECT CROP_COD,CROP_DES FROM crop WHERE CROP_COD = '" + crop + "'";
    if (query.exec(sql))
    {
        if (query.first())
        {
            header1 = header1 + " Crop: " + query.value(1).toString();
        }
    }
}

void grassman::parentViewActivated(const QModelIndex &index)
{
    QList<TmoduleFieldDef> keys;
    keys = m_parentModel->getRowKeyValues(index.row());
    constructHeader1(keys);
    //qWarning() << keys.count();
    if (keys.count() > 0)
    {
        m_activityModel->chkitems(keys);
        m_actSortModel->sort(1);
        applyAcyData = false;

        for (int rsize = 0; rsize <= m_actSortModel->rowCount()-1;rsize++)
            ui->AdvListView4->setRowHeight(rsize,listViewRowSize);

    }
}

void grassman::activityChanged()
{
    ui->AdvListView1->setEnabled(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn5->setEnabled(true);
    applyAcyData = true;
}

void grassman::on_BitBtn5_clicked()
{
    if (applyAcyData)
    {
        if (m_activityModel->applyData())
        {
            ui->BitBtn1->setEnabled(false);
            ui->BitBtn5->setEnabled(false);
            applyAcyData = false;
            m_actSortModel->sort(1);
            ui->AdvListView4->scrollToTop();
            ui->AdvListView1->setEnabled(true);
        }
    }
}

void grassman::on_BitBtn1_clicked()
{
    if (applyAcyData)
    {
        m_activityModel->cancelData();
        m_actSortModel->sort(1);
        ui->BitBtn1->setEnabled(false);
        ui->BitBtn5->setEnabled(false);
        applyAcyData = false;
        ui->AdvListView1->setEnabled(true);
    }
}

void grassman::constructActPopUpMenu(QPoint mousepos, QModelIndex index)
{
    QModelIndex idx;
    idx = m_actSortModel->mapToSource(index);

    if (idx.column() == 0)
    {
        if (m_activityModel->getCheckedStatusForRow(idx.row()) == true)
        {
            QMenu menu(this);
            subModuleKeyFields = m_activityModel->getMasterKeyFields(idx.row());
            flabourTable = m_activityModel->getMasterTable(idx.row());
            mlabourTable = "grassmanlabour";
            menu.addAction(showLabourAct);
            menu.exec(mousepos);
        }
    }
    else
    {
        if (((m_activityModel->getPeriodValue(idx).toString() == "T") ||
             (m_activityModel->getPeriodValue(idx).toString() == "Y")) &&
            (m_activityModel->getCheckedStatusForRow(idx.row()) == true))
        {
            QMenu menu(this);
            header2 = m_activityModel->getPeriodDescription(idx);
            header2 = "Activity: " + header2;


            ffertTable = m_activityModel->getPeriodTable(idx.row());
            subModuleKeyFields = m_activityModel->getPeriodKeyFields(idx);
            flabourTable = m_activityModel->getPeriodTable(idx.row());
            mlabourTable = "grasslabour";
            menu.addAction(showLabourAct);
            if (isActyType(subModuleKeyFields,"F"))
            {
                mfertTable = "grassmanfert";
                manureTable = "grassmanheap";
                menu.addAction(showFertsAct);
                menu.addAction(showManureAct);
            }
            else
            {
                usePlantingDensity = true;
                planTable = m_activityModel->getPeriodTable(idx.row());
                menu.addAction(showInputsAct);
            }
            menu.exec(mousepos);
        }
    }
}

void grassman::cropMenuRequested(QPoint mousepos)
{
    QModelIndex pos;
    pos = ui->AdvListView1->currentIndex();
    if (pos.isValid())
    {
        QMenu menu(this);
        subModuleKeyFields = m_parentModel->getRowKeyValues(pos.row());
        flabourTable = "plotpaddspe";
        mlabourTable = "plotpaddspelabour";
        menu.addAction(showLabourAct);
        menu.exec(this->ui->AdvListView1->mapToGlobal(mousepos));
    }
}

void grassman::actMenuRequested(QPoint mousepos)
{
    //qWarning() << "In here!";
    QModelIndex pos;
    pos = ui->AdvListView4->indexAt(mousepos);
    if (pos.isValid())
    {
        //qWarning() << "Is valid!";
        pos = m_actSortModel->mapToSource(pos);
        if (pos.column() == 0)
        {
            //qWarning() << "is 0";
            if (m_activityModel->getCheckedStatusForRow(pos.row()) == true)
            {
                QMenu menu(this);
                header2 = "Activity: " + pos.data().toString();
                subModuleKeyFields = m_activityModel->getItemKeyFields(pos.row());
                flabourTable = "grassman";
                mlabourTable = "grassmanlabour";
                menu.addAction(showLabourAct);
                menu.exec(ui->AdvListView4->mapToGlobal(mousepos));
            }
        }
    }
}

void grassman::createActions()
{
    showLabourAct = new QAction(tr("Labour allocation"), this);
    connect(showLabourAct,SIGNAL(triggered()),this,SLOT(loadLabour()));
    showInputsAct = new QAction(tr("Inputs allocation"), this);
    connect(showInputsAct,SIGNAL(triggered()),this,SLOT(loadInputs()));
    showFertsAct = new QAction(tr("Fertilizer allocation"), this);
    connect(showFertsAct,SIGNAL(triggered()),this,SLOT(loadFerts()));
    showManureAct = new QAction(tr("Manure allocation"), this);
    connect(showManureAct,SIGNAL(triggered()),this,SLOT(loadMenure()));
}

void grassman::loadLabour()
{
    labourallocation *labourDialog = new labourallocation(0);
    labourDialog->setTableName(mlabourTable);
    labourDialog->setHeader2Description(header2);
    labourDialog->setHeader1Description(header1);
    labourDialog->setModelTable(mlabourTable);
    labourDialog->setTableName(flabourTable);
    labourDialog->setFieldsToTable(flabourTable);
    labourDialog->setKeyFields(subModuleKeyFields);
    for (int pos =0; pos <= subModuleKeyFields.count()-1;pos++)
    {
        if (subModuleKeyFields[pos].code.toLower() != "nmonth")
            labourDialog->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,true);
        else
            labourDialog->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,false);
    }
    labourDialog->loadMaintenance(dataBase);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(labourDialog);
    m_dialogWindow.setWindowTitle(tr("Labour allocation"));
    m_dialogWindow.exec();
}

void grassman::loadInputs()
{
    QString ActFertCode;
    inputsallocation *impDialog = new inputsallocation(0);
    impDialog->setTableName(planTable);
    impDialog->setHeader2Description(header2);
    impDialog->setHeader1Description(header1);
    for (int pos =0; pos <= subModuleKeyFields.count()-1;pos++)
    {
        if (subModuleKeyFields[pos].code.toLower() != "nmonth")
            impDialog->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,true);
        else
            impDialog->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,false);
        if (subModuleKeyFields[pos].code.toLower() == "acty_cod")
        {
            ActFertCode = subModuleKeyFields[pos].value.toString();
        }
    }
    impDialog->setFieldsToTable(planTable);
    impDialog->usePlanDensity(usePlantingDensity);
    impDialog->addLinkedField("MET_COD","ACTYM_COD",ActFertCode);
    impDialog->addLinkedValue("MET_COD","ACTYM_COD","","NULL");
    impDialog->setCurrAct(ActFertCode);
    impDialog->loadMaintenance(dataBase);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(impDialog);
    m_dialogWindow.setWindowTitle(tr("Input allocation"));
    m_dialogWindow.exec();
}

void grassman::loadFerts() //fertiliserallocation
{
    fertiliserallocation *fertDialog = new fertiliserallocation(0);
    fertDialog->setHeader2Description(header2);
    fertDialog->setHeader1Description(header1);
    fertDialog->setModelTable(mfertTable);
    fertDialog->setTableName(ffertTable);
    fertDialog->setFieldsToTable(ffertTable);
    fertDialog->setKeyFields(subModuleKeyFields);
    for (int pos =0; pos <= subModuleKeyFields.count()-1;pos++)
    {
        if (subModuleKeyFields[pos].code.toLower() != "nmonth")
            fertDialog->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,true);
        else
            fertDialog->insertKeyField(subModuleKeyFields[pos].code,
                                    subModuleKeyFields[pos].value,false);
    }
    fertDialog->loadMaintenance(dataBase);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(fertDialog);
    m_dialogWindow.setWindowTitle(tr("Fertiliser allocation"));
    m_dialogWindow.exec();
}

void grassman::loadMenure()
{
    manureallocation *manureDialog = new manureallocation(0);
    manureDialog->setModelTable(manureTable);
    manureDialog->setHeader2Description(header2);
    manureDialog->setHeader1Description(header1);
    manureDialog->setKeyFields(subModuleKeyFields);
    manureDialog->loadMaintenance(dataBase);
    moduleSubScreen m_dialogWindow;
    m_dialogWindow.loadSubScreen(manureDialog);
    m_dialogWindow.setWindowTitle(tr("Manure allocation"));
    m_dialogWindow.exec();
}

bool grassman::isActyType(QList<TmoduleFieldDef> keys, QString type)
{
    QString actyValue;
    for (int pos = 0; pos <= keys.count()-1;pos++)
    {
        if (keys[pos].code.toLower() == "acty_cod")
        {
            actyValue = keys[pos].value.toString();
            break;
        }
    }
    QSqlQuery lktable(dataBase);
    QString sql;
    sql = "SELECT count(acty_cod) FROM activity WHERE acty_cod = '" + actyValue + "'";
    sql = sql + " AND acty_cty = '" + type + "'";
    if (lktable.exec(sql))
    {
        lktable.first();
        if (lktable.value(0).toInt() > 0)
            return true;
        else
            return false;
    }
    else
    {
        qWarning() << "Error: " << lktable.lastError().databaseText();
        qWarning() << "Executing: " << sql;
        return false;
    }
}



void grassman::on_cmddetails_clicked()
{
    if (ui->AdvListView4->currentIndex().isValid())
        constructActPopUpMenu(QCursor::pos(),ui->AdvListView4->currentIndex());
}
