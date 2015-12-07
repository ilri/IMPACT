#include "general.h"
#include "ui_general.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include "IMPACTGlobal.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>

general::general(QWidget *parent) :
    impgenmaint(parent),
    ui(new Ui::general)
{
    ui->setupUi(this);
    insertMode = false;
    stModel = new systemTypesModel(this);
    stSortModel = new systemTypesSortModel(this);

    connect(ui->cmdsave,SIGNAL(clicked()),this,SLOT(applyChanges()));
    connect(ui->cmdcancel,SIGNAL(clicked()),this,SLOT(cancelChanges()));

    loadLanguages();
    ui->tabWidget->setCurrentIndex(0);

}
general::~general()
{   
    delete ui;
}

void general::setInsertMode(bool mode)
{
    insertMode = mode;
}

bool general::systemExists(QString code)
{
    QSqlQuery table(m_database);
    QString sql;
    sql = "SELECT COUNT(sys_cod) FROM system WHERE sys_cod = '" + code + "'";
    if (table.exec(sql))
    {
        table.first();
        if (table.value(0).toInt() > 0)
            return true;
        else
            return false;
    }
    else
    {
        qWarning() << "Error: " << table.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    return false;
}

QString general::getCurrentDescription()
{
    return ui->txtdesc->text();
}

QString general::getSystemAdded()
{
    return m_systemCode;
}

void general::createNewSystem()
{
    QString newCode;
    newCode = "";

    QMessageBox msgBox;
    QDate date = QDate::fromString(ui->txtdate->text(), "d/M/yyyy");
    if (!date.isValid())
    {
        msgBox.setText(tr("Invalid date."));
        msgBox.exec();
        return;
    }
    if (ui->txtdownerabr->text().isEmpty())
    {
        msgBox.setText(tr("You need to indicate a data owner and its abbreviation"));
        msgBox.exec();
        return;
    }
    if (ui->txtdowner->text().isEmpty())
    {
        msgBox.setText(tr("You need to indicate a data owner and its abbreviation"));
        msgBox.exec();
        return;
    }
    if (!stModel->hasCheckedItems())
    {
        msgBox.setText(tr("You need to select a system type"));
        msgBox.exec();
        return;
    }
    if (ui->txtstate->text().isEmpty())
    {
        msgBox.setText(tr("You need to indicate a state and its abbreviation"));
        msgBox.exec();
        return;
    }
    if (ui->txtstateabr->text().isEmpty())
    {
        msgBox.setText(tr("You need to indicate a state and its abbreviation"));
        msgBox.exec();
        return;
    }
    QString day;
    QString month;
    QString year;
    day = QString::number(date.day());
    month = QString::number(date.month());
    year = QString::number(date.year());

    newCode = ui->cmbcty->getCurrentValue();
    newCode = newCode + year + month + day;
    newCode = newCode + ui->txtdownerabr->text();
    newCode = newCode + ui->txtstateabr->text();
    newCode = newCode + stModel->getCropCode();
    newCode = newCode + stModel->getLivestockCode();

    int count;
    count = 2;
    QString tcode;
    tcode = newCode;
    while (systemExists(tcode))
    {
        tcode = newCode + QString::number(count);
        count++;
    }
    newCode = tcode;
    ui->txtcode->setText(newCode);

    QList<TkeyField> extraFields;
    TkeyField field;
    field.code = "SYST_COD";
    field.value = stModel->getSystemTypeCode();
    extraFields.append(field);

    if (this->insertCurrentData(m_database,extraFields))
    {
        insertMode = false;
        m_systemCode = newCode;
        this->insertKeyField("sys_cod",m_systemCode);
        impgenmaint::loadMaintenance(m_database,0);

        ui->txtdate->setReadOnly(true);
        ui->txtdowner->setReadOnly(true);
        ui->txtdownerabr->setReadOnly(true);
        stModel->setReadOnly(true);
        ui->cmbcty->setEnabled(false);
        ui->txtstate->setReadOnly(true);
        ui->txtstateabr->setReadOnly(true);

        QSqlQuery table(m_database);
        QString sql;
        sql = "UPDATE system SET NUMSEASONS = '2' WHERE sys_cod = '" + m_systemCode + "'";
        table.exec(sql);

        int numseasons;
        sql = "SELECT project_nperiods FROM project";
        table.exec(sql);
        table.first();
        numseasons = table.value(0).toInt();

        for (int pos = 1; pos <= numseasons; pos++)
        {
            sql = "INSERT INTO systemperiods(sys_cod,period_id) VALUES ('" + m_systemCode + "','" + QString::number(pos) + "')";
            table.exec(sql);
        }


    }
    else
    {
        msgBox.setText(tr("An error occurred while inserting the new system. Contact support@qlands.com"));
        msgBox.exec();
    }
}

void general::loadMaintenance(QSqlDatabase db,QTableView *)
{
    m_database = db;

    QSqlQuery table(m_database);
    QString sql;
    sql = "SELECT CNTY_COD,CNTY_NAM";
    sql = sql + " FROM country";
    if (table.exec(sql))
    {
        while (table.next())
        {
            ui->cmbcty->addFixItem(table.value(0).toString(),table.value(1).toString());
        }
    }
    else
    {
        qWarning() << "Error: " << table.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }

    stModel->setDatabase(m_database);
    stModel->loadItems();
    stSortModel->setSourceModel(stModel);
    ui->AdvListView1->setModel(stSortModel);

    sql = "SELECT SYST_COD";
    sql = sql + " FROM system WHERE sys_cod = '" + m_systemCode + "'";
    if (table.exec(sql))
    {
        if (table.first())
        {
            stModel->checkItems(table.value(0).toString());
        }
    }
    else
    {
        qWarning() << "Error: " << table.lastError().databaseText();
        qWarning() << "Executing: " << sql;
    }
    stSortModel->sort(1);


    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->AdvListView1->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    for (int rsize = 0; rsize <= stSortModel->rowCount()-1;rsize++)
        ui->AdvListView1->setRowHeight(rsize,listViewRowSize);

    if (!insertMode)
    {
        impgenmaint::loadMaintenance(db,0);
    }
    else
    {
        ui->txtdate->setReadOnly(false);
        ui->txtdowner->setReadOnly(false);
        ui->txtdownerabr->setReadOnly(false);
        stModel->setReadOnly(false);
        ui->cmbcty->setEnabled(true);
        ui->txtstate->setReadOnly(false);
        ui->txtstateabr->setReadOnly(false);
        ui->cmbleng->selectValue("EN");
    }
}

void general::setSystemCode(QString code)
{
    m_systemCode = code;
    this->insertKeyField("sys_cod",m_systemCode);
}

void general::applyChanges()
{
    if (!insertMode)
        impgenmaint::applyChanges();
    else
        createNewSystem();
}

void general::cancelChanges()
{
    if (!insertMode)
        impgenmaint::cancelChanges();
    else
        on_cmdclose_clicked();
}

void general::on_cmdclose_clicked()
{
    emit closeCalled();
}

void general::loadLanguages()
{
    //Generated by script
    ui->cmbleng->addFixItem("AA","Afar");
    ui->cmbleng->addFixItem("AB","Abkhazian");
    ui->cmbleng->addFixItem("AF","Afrikaans");
    ui->cmbleng->addFixItem("AK","Akan");
    ui->cmbleng->addFixItem("ALS","Alemannic");
    ui->cmbleng->addFixItem("AM","Amharic");
    ui->cmbleng->addFixItem("AN","Aragonese");
    ui->cmbleng->addFixItem("ANG","Anglo-Saxon/Old English");
    ui->cmbleng->addFixItem("AR","Arabic");
    ui->cmbleng->addFixItem("ARC","Aramaic");
    ui->cmbleng->addFixItem("AS","Assamese");
    ui->cmbleng->addFixItem("AST","Asturian");
    ui->cmbleng->addFixItem("AV","Avar");
    ui->cmbleng->addFixItem("AY","Aymara");
    ui->cmbleng->addFixItem("AZ","Azerbaijani");
    ui->cmbleng->addFixItem("BA","Bashkir");
    ui->cmbleng->addFixItem("BAR","Bavarian");
    ui->cmbleng->addFixItem("BAT-SMG","Samogitian");
    ui->cmbleng->addFixItem("BCL","Bikol");
    ui->cmbleng->addFixItem("BE","Belarusian");
    ui->cmbleng->addFixItem("BE-X-OLD","Belarusian (Tara?kievica)");
    ui->cmbleng->addFixItem("BG","Bulgarian");
    ui->cmbleng->addFixItem("BH","Bihari");
    ui->cmbleng->addFixItem("BI","Bislama");
    ui->cmbleng->addFixItem("BM","Bambara");
    ui->cmbleng->addFixItem("BN","Bengali");
    ui->cmbleng->addFixItem("BO","Tibetan");
    ui->cmbleng->addFixItem("BPY","Bishnupriya Manipuri");
    ui->cmbleng->addFixItem("BR","Breton");
    ui->cmbleng->addFixItem("BS","Bosnian");
    ui->cmbleng->addFixItem("BUG","Buginese");
    ui->cmbleng->addFixItem("BXR","Buriat (Russia)");
    ui->cmbleng->addFixItem("CA","Catalan");
    ui->cmbleng->addFixItem("CE","Chechen");
    ui->cmbleng->addFixItem("CEB","Cebuano");
    ui->cmbleng->addFixItem("CH","Chamorro");
    ui->cmbleng->addFixItem("CHO","Choctaw");
    ui->cmbleng->addFixItem("CHR","Cherokee");
    ui->cmbleng->addFixItem("CHY","Cheyenne");
    ui->cmbleng->addFixItem("CO","Corsican");
    ui->cmbleng->addFixItem("CR","Cree");
    ui->cmbleng->addFixItem("CS","Czech");
    ui->cmbleng->addFixItem("CSB","Kashubian");
    ui->cmbleng->addFixItem("CU","Old Church Slavonic/Old Bulgarian");
    ui->cmbleng->addFixItem("CV","Chuvash");
    ui->cmbleng->addFixItem("CY","Welsh");
    ui->cmbleng->addFixItem("DA","Danish");
    ui->cmbleng->addFixItem("DE","German");
    ui->cmbleng->addFixItem("DIQ","Dimli");
    ui->cmbleng->addFixItem("DSB","Lower Sorbian");
    ui->cmbleng->addFixItem("DV","Divehi");
    ui->cmbleng->addFixItem("DZ","Dzongkha");
    ui->cmbleng->addFixItem("EE","Ewe");
    ui->cmbleng->addFixItem("EL","Greek");
    ui->cmbleng->addFixItem("EN","English");
    ui->cmbleng->addFixItem("EO","Esperanto");
    ui->cmbleng->addFixItem("ES","Spanish");
    ui->cmbleng->addFixItem("ET","Estonian");
    ui->cmbleng->addFixItem("EU","Basque");
    ui->cmbleng->addFixItem("EXT","Extremaduran");
    ui->cmbleng->addFixItem("FA","Persian");
    ui->cmbleng->addFixItem("FF","Peul");
    ui->cmbleng->addFixItem("FI","Finnish");
    ui->cmbleng->addFixItem("FIU-VRO","Võro");
    ui->cmbleng->addFixItem("FJ","Fijian");
    ui->cmbleng->addFixItem("FO","Faroese");
    ui->cmbleng->addFixItem("FR","French");
    ui->cmbleng->addFixItem("FRP","Arpitan/Franco-Provençal");
    ui->cmbleng->addFixItem("FUR","Friulian");
    ui->cmbleng->addFixItem("FY","West Frisian");
    ui->cmbleng->addFixItem("GA","Irish");
    ui->cmbleng->addFixItem("GD","Scottish Gaelic");
    ui->cmbleng->addFixItem("GIL","Gilbertese");
    ui->cmbleng->addFixItem("GL","Galician");
    ui->cmbleng->addFixItem("GN","Guarani");
    ui->cmbleng->addFixItem("GOT","Gothic");
    ui->cmbleng->addFixItem("GU","Gujarati");
    ui->cmbleng->addFixItem("GV","Manx");
    ui->cmbleng->addFixItem("HA","Hausa");
    ui->cmbleng->addFixItem("HAW","Hawaiian");
    ui->cmbleng->addFixItem("HE","Hebrew");
    ui->cmbleng->addFixItem("HI","Hindi");
    ui->cmbleng->addFixItem("HO","Hiri Motu");
    ui->cmbleng->addFixItem("HR","Croatian");
    ui->cmbleng->addFixItem("HT","Haitian");
    ui->cmbleng->addFixItem("HU","Hungarian");
    ui->cmbleng->addFixItem("HY","Armenian");
    ui->cmbleng->addFixItem("HZ","Herero");
    ui->cmbleng->addFixItem("IA","Interlingua");
    ui->cmbleng->addFixItem("ID","Indonesian");
    ui->cmbleng->addFixItem("IE","Interlingue");
    ui->cmbleng->addFixItem("IG","Igbo");
    ui->cmbleng->addFixItem("II","Sichuan Yi");
    ui->cmbleng->addFixItem("IK","Inupiak");
    ui->cmbleng->addFixItem("ILO","Ilokano");
    ui->cmbleng->addFixItem("IO","Ido");
    ui->cmbleng->addFixItem("IS","Icelandic");
    ui->cmbleng->addFixItem("IT","Italian");
    ui->cmbleng->addFixItem("IU","Inuktitut");
    ui->cmbleng->addFixItem("JA","Japanese");
    ui->cmbleng->addFixItem("JBO","Lojban");
    ui->cmbleng->addFixItem("JV","Javanese");
    ui->cmbleng->addFixItem("KA","Georgian");
    ui->cmbleng->addFixItem("KG","Kongo");
    ui->cmbleng->addFixItem("KI","Kikuyu");
    ui->cmbleng->addFixItem("KJ","Kuanyama");
    ui->cmbleng->addFixItem("KK","Kazakh");
    ui->cmbleng->addFixItem("KL","Greenlandic");
    ui->cmbleng->addFixItem("KM","Cambodian");
    ui->cmbleng->addFixItem("KN","Kannada");
    ui->cmbleng->addFixItem("KO","Korean");
    ui->cmbleng->addFixItem("KR","Kanuri");
    ui->cmbleng->addFixItem("KS","Kashmiri");
    ui->cmbleng->addFixItem("KSH","Ripuarian");
    ui->cmbleng->addFixItem("KU","Kurdish");
    ui->cmbleng->addFixItem("KV","Komi");
    ui->cmbleng->addFixItem("KW","Cornish");
    ui->cmbleng->addFixItem("KY","Kirghiz");
    ui->cmbleng->addFixItem("LA","Latin");
    ui->cmbleng->addFixItem("LAD","Ladino/Judeo-Spanish");
    ui->cmbleng->addFixItem("LAN","Lango");
    ui->cmbleng->addFixItem("LB","Luxembourgish");
    ui->cmbleng->addFixItem("LG","Ganda");
    ui->cmbleng->addFixItem("LI","Limburgian");
    ui->cmbleng->addFixItem("LIJ","Ligurian");
    ui->cmbleng->addFixItem("LMO","Lombard");
    ui->cmbleng->addFixItem("LN","Lingala");
    ui->cmbleng->addFixItem("LO","Laotian");
    ui->cmbleng->addFixItem("LT","Lithuanian");
    ui->cmbleng->addFixItem("LV","Latvian");
    ui->cmbleng->addFixItem("MAP-BMS","Banyumasan");
    ui->cmbleng->addFixItem("MG","Malagasy");
    ui->cmbleng->addFixItem("MH","Marshallese");
    ui->cmbleng->addFixItem("MI","Maori");
    ui->cmbleng->addFixItem("MK","Macedonian");
    ui->cmbleng->addFixItem("ML","Malayalam");
    ui->cmbleng->addFixItem("MN","Mongolian");
    ui->cmbleng->addFixItem("MO","Moldovan");
    ui->cmbleng->addFixItem("MR","Marathi");
    ui->cmbleng->addFixItem("MS","Malay");
    ui->cmbleng->addFixItem("MT","Maltese");
    ui->cmbleng->addFixItem("MUS","Creek/Muskogee");
    ui->cmbleng->addFixItem("MY","Burmese");
    ui->cmbleng->addFixItem("NA","Nauruan");
    ui->cmbleng->addFixItem("NAH","Nahuatl");
    ui->cmbleng->addFixItem("NAP","Neapolitan");
    ui->cmbleng->addFixItem("ND","North Ndebele");
    ui->cmbleng->addFixItem("NDS","Low German/Low Saxon");
    ui->cmbleng->addFixItem("NDS-NL","Dutch Low Saxon");
    ui->cmbleng->addFixItem("NE","Nepali");
    ui->cmbleng->addFixItem("NEW","Newar");
    ui->cmbleng->addFixItem("NG","Ndonga");
    ui->cmbleng->addFixItem("NL","Dutch");
    ui->cmbleng->addFixItem("NN","Norwegian Nynorsk");
    ui->cmbleng->addFixItem("NO","Norwegian");
    ui->cmbleng->addFixItem("NR","South Ndebele");
    ui->cmbleng->addFixItem("NSO","Northern Sotho");
    ui->cmbleng->addFixItem("NRM","Norman");
    ui->cmbleng->addFixItem("NV","Navajo");
    ui->cmbleng->addFixItem("NY","Chichewa");
    ui->cmbleng->addFixItem("OC","Occitan");
    ui->cmbleng->addFixItem("OJ","Ojibwa");
    ui->cmbleng->addFixItem("OM","Oromo");
    ui->cmbleng->addFixItem("OR","Oriya");
    ui->cmbleng->addFixItem("OS","Ossetian/Ossetic");
    ui->cmbleng->addFixItem("PA","Panjabi/Punjabi");
    ui->cmbleng->addFixItem("PAG","Pangasinan");
    ui->cmbleng->addFixItem("PAM","Kapampangan");
    ui->cmbleng->addFixItem("PAP","Papiamentu");
    ui->cmbleng->addFixItem("PDC","Pennsylvania German");
    ui->cmbleng->addFixItem("PI","Pali");
    ui->cmbleng->addFixItem("PIH","Norfolk");
    ui->cmbleng->addFixItem("PL","Polish");
    ui->cmbleng->addFixItem("PMS","Piedmontese");
    ui->cmbleng->addFixItem("PS","Pashto");
    ui->cmbleng->addFixItem("PT","Portuguese");
    ui->cmbleng->addFixItem("QU","Quechua");
    ui->cmbleng->addFixItem("RM","Raeto Romance");
    ui->cmbleng->addFixItem("RMY","Romani");
    ui->cmbleng->addFixItem("RN","Kirundi");
    ui->cmbleng->addFixItem("RO","Romanian");
    ui->cmbleng->addFixItem("ROA-RUP","Aromanian");
    ui->cmbleng->addFixItem("RU","Russian");
    ui->cmbleng->addFixItem("RW","Rwandi");
    ui->cmbleng->addFixItem("SA","Sanskrit");
    ui->cmbleng->addFixItem("SC","Sardinian");
    ui->cmbleng->addFixItem("SCN","Sicilian");
    ui->cmbleng->addFixItem("SCO","Scots");
    ui->cmbleng->addFixItem("SD","Sindhi");
    ui->cmbleng->addFixItem("SE","Northern Sami");
    ui->cmbleng->addFixItem("SG","Sango");
    ui->cmbleng->addFixItem("SH","Serbo-Croatian");
    ui->cmbleng->addFixItem("SI","Sinhalese");
    ui->cmbleng->addFixItem("SIMPLE","Simple English");
    ui->cmbleng->addFixItem("SK","Slovak");
    ui->cmbleng->addFixItem("SL","Slovenian");
    ui->cmbleng->addFixItem("SM","Samoan");
    ui->cmbleng->addFixItem("SN","Shona");
    ui->cmbleng->addFixItem("SO","Somalia");
    ui->cmbleng->addFixItem("SQ","Albanian");
    ui->cmbleng->addFixItem("SR","Serbian");
    ui->cmbleng->addFixItem("SS","Swati");
    ui->cmbleng->addFixItem("ST","Southern Sotho");
    ui->cmbleng->addFixItem("SU","Sundanese");
    ui->cmbleng->addFixItem("SV","Swedish");
    ui->cmbleng->addFixItem("SW","Swahili");
    ui->cmbleng->addFixItem("TA","Tamil");
    ui->cmbleng->addFixItem("TE","Telugu");
    ui->cmbleng->addFixItem("TET","Tetum");
    ui->cmbleng->addFixItem("TG","Tajik");
    ui->cmbleng->addFixItem("TH","Thai");
    ui->cmbleng->addFixItem("TI","Tigrinya");
    ui->cmbleng->addFixItem("TK","Turkmen");
    ui->cmbleng->addFixItem("TL","Tagalog/Filipino");
    ui->cmbleng->addFixItem("TLH","Klingon");
    ui->cmbleng->addFixItem("TN","Tswana");
    ui->cmbleng->addFixItem("TO","Tonga");
    ui->cmbleng->addFixItem("TPI","Tok Pisin");
    ui->cmbleng->addFixItem("TR","Turkish");
    ui->cmbleng->addFixItem("TS","Tsonga");
    ui->cmbleng->addFixItem("TT","Tatar");
    ui->cmbleng->addFixItem("TUM","Tumbuka");
    ui->cmbleng->addFixItem("TW","Twi");
    ui->cmbleng->addFixItem("TY","Tahitian");
    ui->cmbleng->addFixItem("UDM","Udmurt");
    ui->cmbleng->addFixItem("UG","Uyghur");
    ui->cmbleng->addFixItem("UK","Ukrainian");
    ui->cmbleng->addFixItem("UR","Urdu");
    ui->cmbleng->addFixItem("UZ","Uzbek");
    ui->cmbleng->addFixItem("VE","Venda");
    ui->cmbleng->addFixItem("VI","Vietnamese");
    ui->cmbleng->addFixItem("VEC","Venetian");
    ui->cmbleng->addFixItem("VLS","West Flemish");
    ui->cmbleng->addFixItem("VO","Volapük");
    ui->cmbleng->addFixItem("WA","Walloon");
    ui->cmbleng->addFixItem("WAR","Waray-Waray/Samar-Leyte Visayan");
    ui->cmbleng->addFixItem("WO","Wolof");
    ui->cmbleng->addFixItem("XAL","Kalmyk");
    ui->cmbleng->addFixItem("XH","Xhosa");
    ui->cmbleng->addFixItem("YI","Yiddish");
    ui->cmbleng->addFixItem("YO","Yoruba");
    ui->cmbleng->addFixItem("ZA","Zhuang");
    ui->cmbleng->addFixItem("ZH","Chinese");
    ui->cmbleng->addFixItem("ZH-MIN-NAN","Minnan");
    ui->cmbleng->addFixItem("ZH-YUE","Cantonese");
    ui->cmbleng->addFixItem("ZU","Zulu");
}

