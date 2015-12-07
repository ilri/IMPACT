#include "ohtlabour.h"
#include "ui_ohtlabour.h"
#include "delegates/fixcombodelegate.h"
#include "delegates/imagecheckdelegate.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qpixmap.h>
#include "submodules/labourallocation.h"
#include "modulesubscreen.h"
#include "moduleglobal.h"
#include <QMenu>

ohtlabour::ohtlabour(QWidget *parent,QSqlDatabase db, QString systemID) :
    QWidget(parent),
    ui(new Ui::ohtlabour)
{
    dataBase = db;
    //currentSystem = "KE20061129ILREMBMZCTB";
    currentSystem = systemID;
    ui->setupUi(this);
    loadForm();
    createActions();
    constructCustomDelegator();
    if (modules_use_Android == true)
        loadAndroidEffects();
}

ohtlabour::~ohtlabour()
{
    //qDebug() << "Before destroy ohtlabour UI";
    delete ui;
    //qDebug() << "After destroy ohtlabour UI";
}

void ohtlabour::loadAndroidEffects()
{
    charm.activateOn(ui->AdvListView4);
}

void ohtlabour::loadForm()
{

    header1 = "";
    QList<TmoduleFieldDef> keys;
    TmoduleFieldDef key;
    key.code = "SYS_COD";
    key.value = currentSystem;
    keys.append(key);

    //Creates and loads activity model
    m_activityModel = new managementModel(this);
    m_actSortModel = new managementSortModel(this);
    connect(m_activityModel,SIGNAL(modeldDataChanged()),this,SLOT(activityChanged()));

    m_activityModel->setDatabase(dataBase);
    //m_activityModel->setImpactModule(this);

    QList<TmoduleFieldDef> cpmankeys;
    key.code = "SYS_COD";
    cpmankeys.append(key);

    m_activityModel->addMasterTable("otherman");
    m_activityModel->setKeysFields("otherman",cpmankeys);
    m_activityModel->setLookUpLinkedField("otherman","ACTY_COD");
    m_activityModel->setPeriodTable("otherman","othermantim");
    key.code = "ACTY_COD";
    cpmankeys.append(key);
    m_activityModel->setPeriodKeyFields("otherman",cpmankeys);
    m_activityModel->setPeriodField("otherman","NMONTH");
    m_activityModel->setPeriodStatusField("otherman","CHECKED");
    m_activityModel->setPeriodDefaultValue("otherman","N");



    m_activityModel->setlkUpTable("otherman","activity");
    m_activityModel->setlkUpCodeFields("otherman","ACTY_COD");
    m_activityModel->setlkUpDescFields("otherman","ACTY_DES");
    m_activityModel->setlkUpWhereClause("otherman"," ACTY_TYP = 'O'");
    m_activityModel->setlkUpColDescription("Activities");
    m_activityModel->setlkUpUnChkColor("otherman",QColor(230,255,255));
    m_activityModel->setlkUpChkColor("otherman",QColor(199,255,255));

    m_actSortModel->setSourceModel(m_activityModel);
    m_activityModel->loadItems();
    ui->AdvListView4->setModel(m_actSortModel);
    m_activityModel->chkitems(keys);
    m_actSortModel->sort(1);
    //m_activityModel->distributeColums();

    for (int rsize = 0; rsize <= m_actSortModel->rowCount()-1;rsize++)
        ui->AdvListView4->setRowHeight(rsize,listViewRowSize);

}

void ohtlabour::constructCustomDelegator()
{


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

}

void ohtlabour::activityChanged()
{
    ui->BitBtn1->setEnabled(true);
    ui->BitBtn5->setEnabled(true);
}

void ohtlabour::on_BitBtn5_clicked()
{
    if (m_activityModel->applyData())
    {
        ui->BitBtn1->setEnabled(false);
        ui->BitBtn5->setEnabled(false);
        m_actSortModel->sort(1);
        ui->AdvListView4->scrollToTop();
    }
}

void ohtlabour::on_BitBtn1_clicked()
{
    m_activityModel->cancelData();
    m_actSortModel->sort(1);
    ui->BitBtn1->setEnabled(false);
    ui->BitBtn5->setEnabled(false);
}

void ohtlabour::constructActPopUpMenu(QPoint mousepos, QModelIndex index)
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
            mlabourTable = "othermanlabour";
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
            mlabourTable = "otherlabour";
            menu.addAction(showLabourAct);            
            menu.exec(mousepos);
        }
    }
}

void ohtlabour::createActions()
{
    showLabourAct = new QAction(tr("Labour allocation"), this);
    connect(showLabourAct,SIGNAL(triggered()),this,SLOT(loadLabour()));
}

void ohtlabour::loadLabour()
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




void ohtlabour::on_cmddetails_clicked()
{
    if (ui->AdvListView4->currentIndex().isValid())
        constructActPopUpMenu(QCursor::pos(),ui->AdvListView4->currentIndex());
}
