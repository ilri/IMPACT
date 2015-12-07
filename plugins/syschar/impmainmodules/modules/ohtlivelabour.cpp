#include "ohtlivelabour.h"
#include "ui_ohtlivelabour.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qpixmap.h>
#include "submodules/labourallocation.h"
#include "modulesubscreen.h"
#include "moduleglobal.h"
#include <QMenu>

ohtlivelabour::ohtlivelabour(QWidget *parent,QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::ohtlivelabour)
{
    dataBase = db;
    //currentSystem = "KE20061129ILREMBMZCTB";
    currentSystem = systemID;
    ui->setupUi(this);
    loadForm();
    createActions();
    currentLiveCod = "NULL";
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

void ohtlivelabour::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView1);
    charmact.activateOn(ui->AdvListView4);
}

ohtlivelabour::~ohtlivelabour()
{
    //qDebug() << "Before destroy ohtlivelabour UI";
    delete ui;
    //qDebug() << "After destroy ohtlivelabour UI";
}

void ohtlivelabour::loadForm()
{
    connect(ui->AdvListView1,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(cropMenuRequested(QPoint)));

    ui->AdvListView1->setContextMenuPolicy(Qt::CustomContextMenu);

    //Creates and load the parent model
    m_parentModel = new fieldInColModel(this);
    m_parentModel->setReadOnly(true);
    m_parentModel->setDatabase(dataBase);
    //m_parentModel->setImpactModule(this);
    m_parentModel->setTableName("lvstosys");


    m_parentModel->insertField("SYS_COD","Plot ID","000","000",true,false,false);
    m_parentModel->insertField("LIVE_ICOD","Livestock","000","000",true,false,true,true);
    m_parentModel->insertField("PURPOSE","Purpose","000","000",false,false,true);
    m_parentModel->insertField("BREDD","Breed","000","000",false,false,true);

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
    key.code = "LIVE_ICOD";
    cpmankeys.append(key);


    m_activityModel->addMasterTable("liveman");
    m_activityModel->setKeysFields("liveman",cpmankeys);
    m_activityModel->setLookUpLinkedField("liveman","ACTY_COD");
    m_activityModel->setPeriodTable("liveman","livemantim");
    key.code = "ACTY_COD";
    cpmankeys.append(key);
    m_activityModel->setPeriodKeyFields("liveman",cpmankeys);
    m_activityModel->setPeriodField("liveman","NMONTH");
    m_activityModel->setPeriodStatusField("liveman","CHECKED");
    m_activityModel->setPeriodDefaultValue("liveman","N");



    m_activityModel->setlkUpTable("liveman","activity");
    m_activityModel->setlkUpCodeFields("liveman","ACTY_COD");
    m_activityModel->setlkUpDescFields("liveman","ACTY_DES");
    m_activityModel->setlkUpWhereClause("liveman"," ACTY_TYP = 'L'");
    m_activityModel->setlkUpColDescription("Activities");
    m_activityModel->setlkUpUnChkColor("liveman",QColor(230,255,255));
    m_activityModel->setlkUpChkColor("liveman",QColor(199,255,255));

    m_actSortModel->setSourceModel(m_activityModel);
    m_activityModel->loadItems();
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

void ohtlivelabour::constructCustomDelegator()
{

    fixComboDelegate *pdelegate = new fixComboDelegate(this);
    pdelegate->setReadOnly(true);
    QSqlQuery livestock(dataBase);
    QString sql;
    QString desc;
    sql = "SELECT lvstosys.live_icod,livestoc.LIVE_DES FROM lvstosys,livestoc";
    sql = sql + " WHERE lvstosys.LIVE_COD = livestoc.live_cod AND lvstosys.sys_cod = '" + currentSystem + "'";
    livestock.exec(sql);
    while(livestock.next())
    {
        desc = livestock.value(1).toString();
        pdelegate->insertItem(livestock.value(0).toString(),desc);
    }

    ui->AdvListView1->setItemDelegateForColumn(0,pdelegate);


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
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

}

void ohtlivelabour::constructHeader1(QList<TmoduleFieldDef> keys)
{
    header1 = "";
    QString livestock = "";

    QSqlQuery query(dataBase);
    QString sql;
    int pos;
    for (pos = 0; pos <= keys.count()-1;pos++)
    {
        if (keys[pos].code.toUpper() == "LIVE_ICOD")
            livestock = keys[pos].value.toString();

    }
    sql = "SELECT lvstosys.purpose,lvstosys.bredd,livestoc.LIVE_DES";
    sql = sql + " FROM lvstosys,livestoc";
    sql = sql + " WHERE lvstosys.LIVE_COD = livestoc.LIVE_COD";
    sql = sql + " AND lvstosys.LIVE_ICOD = '" + livestock + "'";
    if (query.exec(sql))
    {
        if (query.first())
        {
            header1 = query.value(2).toString() + " - Purpose: " + query.value(0).toString();
            header1 = header1 + " - Breed: " + query.value(1).toString();
        }
    }
}

void ohtlivelabour::parentViewActivated(const QModelIndex &index)
{
    QList<TmoduleFieldDef> keys;
    keys = m_parentModel->getRowKeyValues(index.row());
    constructHeader1(keys);
    if (keys.count() > 0)
    {
        m_activityModel->chkitems(keys);
        m_actSortModel->sort(1);
        applyAcyData = false;


        for (int rsize = 0; rsize <= m_actSortModel->rowCount()-1;rsize++)
            ui->AdvListView4->setRowHeight(rsize,listViewRowSize);

    }
}

void ohtlivelabour::activityChanged()
{
    ui->AdvListView1->setEnabled(false);
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn5->setEnabled(true);
    applyAcyData = true;
}

void ohtlivelabour::on_BitBtn5_clicked()
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

void ohtlivelabour::on_BitBtn1_clicked()
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

void ohtlivelabour::constructActPopUpMenu(QPoint mousepos, QModelIndex index)
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
            mlabourTable = "livemanlabour";
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

            subModuleKeyFields = m_activityModel->getPeriodKeyFields(idx);
            flabourTable = m_activityModel->getPeriodTable(idx.row());
            mlabourTable = "livelabour";
            menu.addAction(showLabourAct);            
            menu.exec(mousepos);
        }
    }
}

void ohtlivelabour::cropMenuRequested(QPoint mousepos)
{
    QModelIndex pos;
    pos = ui->AdvListView1->currentIndex();
    if (pos.isValid())
    {
        QMenu menu(this);
        subModuleKeyFields = m_parentModel->getRowKeyValues(pos.row());
        flabourTable = "lvstosys";
        mlabourTable = "lvstosyslabour";
        menu.addAction(showLabourAct);
        menu.exec(this->ui->AdvListView1->mapToGlobal(mousepos));
    }
}

void ohtlivelabour::actMenuRequested(QPoint mousepos)
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
                subModuleKeyFields.clear();
                subModuleKeyFields.append(m_activityModel->getItemKeyFields(pos.row()));
                flabourTable = "liveman";
                mlabourTable = "livemanlabour";
                menu.addAction(showLabourAct);
                menu.exec(ui->AdvListView4->mapToGlobal(mousepos));
            }
        }
    }
}

void ohtlivelabour::createActions()
{
    showLabourAct = new QAction(tr("Labour allocation"), this);
    connect(showLabourAct,SIGNAL(triggered()),this,SLOT(loadLabour()));
}

void ohtlivelabour::loadLabour()
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




void ohtlivelabour::on_cmddetails_clicked()
{
    if (ui->AdvListView4->currentIndex().isValid())
        constructActPopUpMenu(QCursor::pos(),ui->AdvListView4->currentIndex());
}
