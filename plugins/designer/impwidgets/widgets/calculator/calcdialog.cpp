#include "calcdialog.h"
#include "ui_calcdialog.h"
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QDebug>
#include <QSqlRecord>
#include <QHeaderView>

calcDialog::calcDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::calcDialog)
{
    ui->setupUi(this); //QDialog
    calWidget = new Calculator(this);
    ui->calclayout->addWidget(calWidget);
    connect(calWidget,SIGNAL(closeCalled()),this,SLOT(close()));
    kgsfactor = 1;
    ltsfactor = 1;
    //closeUnitsDB = false;
    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowTitle(tr("Calculator"));
}

calcDialog::~calcDialog()
{
    delete ui;
    //if (closeUnitsDB)
    //{
    //    m_database.close();
   // }
}

void calcDialog::setDatabase(QSqlDatabase db)
{
    m_database = db;
    readSettings();
}


QString calcDialog::getCurrentDisplay()
{
    return calWidget->getCurrentDisplay();
}

void calcDialog::setCurrentDisplay(double value)
{
    calWidget->setCurrentDisplay(value);
}

void calcDialog::on_cmdkgs_clicked()
{    
    if (ui->radioButton->isChecked() == false)
    {
        float currValue;
        currValue = getCurrentDisplay().toFloat();
        currValue = currValue * kgsfactor;
        setCurrentDisplay(currValue);
    }
}

void calcDialog::on_cmdlts_clicked()
{
    if (ui->radioButton->isChecked() == false)
    {
        float currValue;
        currValue = getCurrentDisplay().toFloat();
        currValue = currValue * ltsfactor;
        setCurrentDisplay(currValue);
    }
}

void calcDialog::readSettings()
{
    //settings.setValue("currentDB","FALSE");
    QSettings settings("ILRI", "IMPACT");

    //QString currDB = settings.value("currentDB","").toString();

    QString indUnitCode = settings.value("indigenousUnitCode","NONE").toString();
    if (indUnitCode != "NONE")
    {
        loadUnit(indUnitCode);
        //loadUnit(currDB,indUnitCode);
    }
    else
    {
        kgsfactor = 1;
        ltsfactor = 1;
        currIndUnit = "NONE";
        currIndUnitDesc = "None";
        settings.setValue("useIndigenousUnit",false);
        settings.setValue("indigenousUnitCode","NONE");
    }

    bool useIndUnit = settings.value("useIndigenousUnit",false).toBool();
    if (!useIndUnit)
    {
        settings.setValue("useIndigenousUnit",false);
        ui->radioButton->setChecked(true);
        ui->radioButton_2->setText(currIndUnitDesc);
    }
    else
    {
        ui->radioButton_2->setText(currIndUnitDesc);
        if (currIndUnit != "NONE")
        {
            ui->radioButton_2->setChecked(true);
        }
        else
        {
            settings.setValue("useIndigenousUnit",false);
            ui->radioButton->setChecked(true);
        }
    }
    loadUnits();
    //loadUnits(currDB);
}

void calcDialog::loadUnits()
{
    //closeUnitsDB = false;
    /*if (!m_database.isValid())
    {
        m_database = QSqlDatabase::addDatabase("QSQLITE","indUnitsCon");
        m_database.setDatabaseName(currDB); //Set the databasename according to the textbox in the UI
        if (!m_database.open()) //Try to opens the database
        {
            return;
        }
        closeUnitsDB = true;
    }*/

    m_colModel = new indUnitsModel(this);
    m_colModel->setDatabase(m_database);
    m_colModel->setTableName("indigenousunits");

    m_colModel->insertField("unit_cod","Unit ID","000","000",true,true,false);
    m_colModel->insertField("unit_des","Indigenous unit","","",false,false,true);
    m_colModel->insertField("kgs","Kgs factor","0","0",false,false,true);
    m_colModel->insertField("lts","Lts factor","0","0",false,false,true);

    connect(m_colModel,SIGNAL(modeldDataChanged()),this,SLOT(valueChanged()));

    m_colModel->loadData();

    ui->tableView->setModel(m_colModel);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

}

void calcDialog::loadUnit(QString code)
{
    //closeUnitsDB = false;
    /*if (!m_database.isValid())
    {
        m_database = QSqlDatabase::addDatabase("QSQLITE","indUnitsCon");
        m_database.setDatabaseName(currDB); //Set the databasename according to the textbox in the UI
        if (!m_database.open()) //Try to opens the database
        {
            kgsfactor = 1;
            ltsfactor = 1;
            currIndUnit = "NONE";
            currIndUnitDesc = "None";
            return;
        }
        closeUnitsDB = true;
    }*/
    QSqlQuery lktable(m_database);
    QString sql;
    sql = "SELECT unit_cod,unit_des,kgs,lts FROM indigenousunits WHERE ";
    sql = sql + " unit_cod = '" + code + "'";
    if (lktable.exec(sql))
    {
        if (lktable.first())
        {
            kgsfactor = lktable.value(2).toFloat();
            ltsfactor = lktable.value(3).toFloat();
            currIndUnitDesc = lktable.value(1).toString();
            currIndUnit = code;
        }
        else
        {
            kgsfactor = 1;
            ltsfactor = 1;
            currIndUnit = "NONE";
            currIndUnitDesc = "None";
        }
    }
    else
    {
        kgsfactor = 1;
        ltsfactor = 1;
        currIndUnit = "NONE";
        currIndUnitDesc = "None";
    }
}

void calcDialog::on_cmdunitdef_clicked()
{
    QSettings settings("ILRI", "IMPACT");
    if (ui->radioButton->isChecked())
    {
        settings.setValue("useIndigenousUnit",false);
    }
    else
    {
        if (currIndUnit != "NONE")
        {
            settings.setValue("useIndigenousUnit",true);
            settings.setValue("indigenousUnitCode",currIndUnit);
        }
        else
        {
            settings.setValue("useIndigenousUnit",false);
        }
    }
}

void calcDialog::on_cmdnew_clicked()
{
    m_colModel->insertNewItem();
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void calcDialog::on_cmddelete_clicked()
{
    if (ui->tableView->currentIndex().isValid())
    {
        m_colModel->deleteItem(ui->tableView->currentIndex().row());
    }
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void calcDialog::on_cmdapply_clicked()
{
    if (m_colModel->applyChanges())
    {
        ui->cmdapply->setEnabled(false);
        ui->cmdcancel->setEnabled(false);
    }
}

void calcDialog::on_cmdcancel_clicked()
{
    m_colModel->cancelChanges();
    ui->cmdapply->setEnabled(false);
    ui->cmdcancel->setEnabled(false);
}

void calcDialog::valueChanged()
{
    ui->cmdapply->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void calcDialog::on_cmdchange_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void calcDialog::on_cmdback_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void calcDialog::on_cmdselect_clicked()
{
    if (ui->tableView->currentIndex().isValid())
    {
        currIndUnit = m_colModel->getDataFromField(ui->tableView->currentIndex().row(),"unit_cod").toString();
        currIndUnitDesc = m_colModel->getDataFromField(ui->tableView->currentIndex().row(),"unit_des").toString();
        kgsfactor = m_colModel->getDataFromField(ui->tableView->currentIndex().row(),"kgs").toFloat();
        ltsfactor = m_colModel->getDataFromField(ui->tableView->currentIndex().row(),"lts").toFloat();
        ui->radioButton_2->setText(currIndUnitDesc);
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void calcDialog::on_cmddefault_clicked()
{
    if (ui->tableView->currentIndex().isValid())
    {
        on_cmdselect_clicked();
        QSettings settings("ILRI", "IMPACT");
        settings.setValue("indigenousUnitCode",currIndUnit);
    }
}

//***********************Indigenous units model********************************

indUnitsModel::indUnitsModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    nShowCols = -1;
    m_tableName = "";
    isReadOnly = false;
    maxKeySize = 3;
}

indUnitsModel::~indUnitsModel()
{
    //qDebug() << "Destroy: indUnitsModel";
}

void indUnitsModel::setMaxKeySize(int size)
{
    maxKeySize = size;
}

void indUnitsModel::resetModelStructure()
{
    this->beginResetModel();
    nShowCols = -1;
    //Clear the whole structure
    m_fields.clear();
    m_items.clear();
    m_keyValues.clear();
    this->endResetModel();
}

int indUnitsModel::getFieldByIndex(int idx) const
{
    int pos;
    int res;
    res = -1;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].colIndex == idx)
        {
            res = pos;
            break;
        }
    }
    return res;
}

int indUnitsModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int indUnitsModel::columnCount(const QModelIndex &) const
{
    return nShowCols+1;
}

Qt::ItemFlags indUnitsModel::flags(const QModelIndex &index) const
{
    if (isReadOnly)
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    int idx;
    idx = getFieldByIndex(index.column());
    if (idx >= 0)
    {
        if (m_items[index.row()].fields[idx].editable)
        {
            if (m_items[index.row()].currentStatus != "D")
            {

                return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled;

            }
            else
                return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        else
        {
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
    }
    return Qt::NoItemFlags;
}

QVariant indUnitsModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        int idx;
        idx = getFieldByIndex(index.column());
        if (idx >= 0)
        {
            return m_items[index.row()].fields[idx].value;
        }
    }
    if (role == Qt::DecorationRole)
    {
        if (index.column() == 0)
        {
            if (m_items[index.row()].currentStatus == "N")
                return QPixmap(":/images/new3.png");
            if (m_items[index.row()].currentStatus == "D")
                return QPixmap(":/images/delete3.png");
            if (m_items[index.row()].currentStatus == "M")
                return QPixmap(":/images/modify2.png");
            return QVariant();
        }
        else
        {
            return QVariant();
        }
    }
    if (role == Qt::BackgroundRole)
    {
        QColor color;
        QBrush Background;
        int idx;
        idx = getFieldByIndex(index.column());
        if (idx >= 0)
            color = m_items[index.row()].fields[idx].Color;
        Background.setColor(color);
        return Background;
    }
    return QVariant();
}

bool indUnitsModel::setData(const QModelIndex &index,const QVariant &value,int role)
{

    if ((role == Qt::EditRole) && (!isReadOnly))
    {
        if (m_items[index.row()].error == false)
        {
            int idx;
            idx = getFieldByIndex(index.column());
            if (idx >= 0)
            {
                m_items[index.row()].fields[idx].value = value;
                if ((m_items[index.row()].currentStatus != "N") && (m_items[index.row()].currentStatus != "D"))
                    m_items[index.row()].currentStatus = "M";
                QModelIndex begin;
                QModelIndex end;
                begin = this->index(index.row(),0);
                end = this->index(index.row(),columnCount()-1);
                emit dataChanged(begin,end);
                emit modeldDataChanged();
                emit modeldDataChanged(index.row(), index.column());
                return true;
            }
        }
    }
    return false;
}

QVariant indUnitsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            int idx;
            idx = getFieldByIndex(section);
            if (idx >= 0)
                return m_fields[idx].description;
        }
    }
    return QVariant();
}

int indUnitsModel::getFieldIndex(QSqlQuery query,QString fieldName)
{
    return query.record().indexOf(fieldName); //Return the index of fieldName;
}

void indUnitsModel::setDatabase(QSqlDatabase database)
{
    m_database = database;
}

void indUnitsModel::setTableName(QString tablename)
{
    m_tableName = tablename;
}

void indUnitsModel::insertField(QString code,
                 QString description,
                 QVariant value,
                 QVariant defvalue,
                 bool key,
                 bool autogen,
                 bool show,
                 bool editable,
                 QColor Color)
{
    TunitFieldDef field;
    field.code = code;
    field.newRecords = 0;
    field.description = description;
    field.value = value;
    field.defValue = defvalue;
    field.key = key;
    field.autogen = autogen;
    field.show = show;
    if (show)
    {
        nShowCols++;
        field.colIndex = nShowCols;
    }
    else
    {
        field.colIndex = -2;
    }
    field.editable = editable;
    field.Color = Color;
    m_fields.append(field);
}

void indUnitsModel::insertNewItem()
{

    TunitItemDef item;
    item.previousStatus = "";
    item.currentStatus = "N";
    item.error = false;
    item.fields.append(m_fields);
    int pos;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        item.fields[pos].value = item.fields[pos].defValue;
    }
    int pos2;
    //Generate an auto gen value for the keys that require it
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if ((m_fields[pos].key == true) && (m_fields[pos].autogen == true))
        {
            item.fields[pos].value = QVariant(genAutoGenKey(pos));
        }
    }
    //Set the value for keys fields to those passed to the model in the load function
    for (pos = 0; pos <= m_keyValues.count()-1;pos++)
    {
        for (pos2 = 0; pos2 <= m_fields.count()-1;pos2++)
        {
            if (item.fields[pos2].code.toLower() == m_keyValues[pos].code.toLower())
            {
                item.fields[pos2].value =  m_keyValues[pos].value;
                break;
            }
        }
    }
    m_items.append(item);

    QModelIndex end;
    end = this->index(m_items.count(),nShowCols);
    this->beginInsertRows(end,m_items.count(),m_items.count());


    this->endInsertRows();
    /*if (m_tableView)
    {
        m_tableView->resizeColumnsToContents();
        m_tableView->update(this->index(m_items.count()-1,0));
        m_tableView->setCurrentIndex(this->index(m_items.count()-1,0));
        m_tableView->scrollTo(this->index(m_items.count()-1,0));
    }*/
    emit modeldDataChanged();
    //this->distributeColums();
}

QVariant indUnitsModel::getDataFromField(int row, QString field)
{
    for (int pos = 0; pos <= m_fields.count()-1; pos++)
    {
        if (m_items[row].fields[pos].code.toLower() == field.toLower())
        {
            return m_items[row].fields[pos].value;
        }
    }
    return QVariant();
}

QList<TunitFieldDef> indUnitsModel::getRowKeyValues(int row)
{
    QList<TunitFieldDef> res;
    for (int pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].key == true)
        {
            res.append(m_items[row].fields[pos]);
        }
    }
    return res;
}

QString indUnitsModel::getRowStatus(int row)
{
    return m_items[row].currentStatus;
}

void indUnitsModel::deleteItem(int row)
{
    m_items[row].currentStatus = "D";
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,nShowCols);
    emit dataChanged(begin,end);
    emit modeldDataChanged();
}

void indUnitsModel::loadData()
{
    this->beginResetModel();
    m_items.clear();
    m_keyValues.clear();

    if (m_fields.count() > 0)
    {
        int pos;
        QSqlQuery table(m_database);
        QString sql;

        sql = "SELECT ";
        for (pos = 0; pos <= m_fields.count()-1;pos++)
        {
            sql = sql + m_fields[pos].code + ",";
        }
        sql = sql.left(sql.length()-1);
        sql = sql + " FROM " + m_tableName;
        if (!table.exec(sql))
        {
            qWarning() << "Error: " << table.lastError().databaseText();
            qWarning() << "Executing: " << sql;
        }


        while (table.next())
        {
            TunitItemDef item;
            item.currentStatus = "C";
            item.previousStatus = "C";
            item.error = false;
            for (pos = 0; pos <= m_fields.count()-1;pos++)
            {
                TunitFieldDef field;
                field = m_fields[pos];
                field.value = table.value(getFieldIndex(table,field.code));
                field.previousValue = field.value;
                item.fields.append(field);
            }
            m_items.append(item);
        }
    }
    else
        qWarning() << "Error: Set the fields first!!!";
    this->endResetModel();
}

void indUnitsModel::removeDeleted()
{
    int pos;
    pos = 0;
    bool reload;
    reload = false;
    while (pos <= m_items.count()-1)
    {
        if (m_items[pos].currentStatus == "CLR")
        {
            m_items.removeAt(pos);
            pos = 0;
            reload = true;
        }
        else
        {
            pos++;
        }
    }
    if (reload)
    {
        for (pos = 0; pos <= m_fields.count()-1;pos++)
        {
            m_fields[pos].newRecords = 0;
        }
        this->beginResetModel();
        this->endResetModel();
        //this->distributeColums();
    }
}

bool indUnitsModel::applyChanges()
{
    int pos;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus != m_items[pos].previousStatus)
        {
            if (m_items[pos].currentStatus == "N")
            {
                insertData(pos);
            }
            if (m_items[pos].currentStatus == "D")
            {
                deleteData(pos);
            }
            if (m_items[pos].currentStatus == "M")
            {
                updateData(pos);
            }
        }
    }


    removeDeleted();
    return true;

}

void indUnitsModel::cancelChanges()
{
    int pos;
    int pos2;
    for (pos = 0; pos <= m_items.count()-1;pos++)
    {
        if (m_items[pos].currentStatus != m_items[pos].previousStatus)
        {
            if (m_items[pos].currentStatus != "N")
            {
                for (pos2 = 0; pos2 <= m_fields.count()-1;pos2++)
                {
                    m_items[pos].fields[pos2].value = m_items[pos].fields[pos2].previousValue;
                    m_items[pos].currentStatus = m_items[pos].previousStatus;
                    m_items[pos].error = false;
                    updateRow(pos,m_items[pos].currentStatus);
                }
            }
            else
            {
                m_items[pos].currentStatus = "CLR";
            }
        }
    }
    removeDeleted();
}

QString indUnitsModel::genAutoGenKey(int fieldIndex)
{
    int pos;
    if (m_fields[fieldIndex].newRecords == 0)
    {
        QSqlQuery temp(m_database);
        QString sql;
        sql = "SELECT max(cast(" + m_fields[fieldIndex].code + " as integer)) FROM " + m_tableName;
        if (m_keyValues.count() > 0)
        {
            sql = sql + " WHERE ";
            for (pos = 0; pos <= m_keyValues.count() - 1; pos++)
            {
                sql = sql + m_keyValues[pos].code;
                sql = sql + " = '" + m_keyValues[pos].value.toString() + "' AND ";
            }
            sql = sql.left(sql.length()-5);
        }
        temp.exec(sql);
        temp.first();
        m_fields[fieldIndex].newRecords = temp.value(0).toInt() + 1;
    }
    else
    {
        m_fields[fieldIndex].newRecords++;
    }
    QString newCode;
    newCode = QString::number(m_fields[fieldIndex].newRecords);
    int tl;
    tl = newCode.length();
    for (pos = 1; pos <= maxKeySize - tl; pos++)
    {
        newCode = "0" + newCode;
    }
    return newCode;
}

void indUnitsModel::insertData(int row)
{
    int pos;
    QSqlQuery tquery(m_database);
    QString sql;



    sql = "INSERT INTO " + m_tableName + "(" ;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        sql = sql + m_fields[pos].code + ",";
    }
    sql = sql.left(sql.length()-1) + ")";
    sql = sql + " VALUES (";
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        sql = sql + "'" + m_items[row].fields[pos].value.toString() + "',";
    }
    sql = sql.left(sql.length()-1) + ")";
    if (!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    else
    {
        updateRow(row,"C");
    }
}

void indUnitsModel::updateData(int row)
{
    QSqlQuery tquery(m_database);
    QString sql;
    int pos;
    sql = "UPDATE " + m_tableName + " SET ";
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].key == false)
        {
            sql = sql + m_fields[pos].code + " = ";
            sql = sql + "'" + m_items[row].fields[pos].value.toString() + "',";
        }
    }
    sql = sql.left(sql.length()-1);
    sql = sql + " WHERE ";
    bool keys;
    keys = false;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].key == true)
        {
            keys = true;
            sql = sql + m_fields[pos].code + " = ";
            sql = sql + "'" + m_items[row].fields[pos].value.toString() + "' AND ";
        }
    }
    if (keys)
        sql = sql.left(sql.length()-5);
    else
        sql = sql.left(sql.length()-7);

    if (!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    else
    {
        updateRow(row,"C");
    }
}

void indUnitsModel::deleteData(int row)
{
    QSqlQuery tquery(m_database);
    QString sql;
    int pos;
    sql = "DELETE FROM  " + m_tableName;
    sql = sql + " WHERE ";
    bool keys;
    keys = false;
    for (pos = 0; pos <= m_fields.count()-1;pos++)
    {
        if (m_fields[pos].key == true)
        {
            keys = true;
            sql = sql + m_fields[pos].code + " = ";
            sql = sql + "'" + m_items[row].fields[pos].value.toString() + "' AND ";
        }
    }
    if (keys)
        sql = sql.left(sql.length()-5);
    else
        sql = sql.left(sql.length()-7);


    if (!tquery.exec(sql))
    {
        qWarning() << "Error: " << tquery.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    else
    {
        m_items[row].currentStatus = "CLR";
    }
}

void indUnitsModel::updateRow(int row,QString status)
{
    m_items[row].currentStatus = status;
    m_items[row].previousStatus = status;
    QModelIndex begin;
    begin = this->index(row,0);
    QModelIndex end;
    end = this->index(row,nShowCols);
    emit dataChanged(begin,end);
}





