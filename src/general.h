#ifndef GENERAL_H
#define GENERAL_H
#include <impgenmaint.h>
#include "genmodels.h"
//#include "flickcharm.h"

namespace Ui {
    class general;
}

class general : public impgenmaint
{
    Q_OBJECT

public:
    explicit general(QWidget *parent = 0);
    ~general();
    QTableView *list;
    void loadMaintenance(QSqlDatabase db,QTableView *tableView);
    void setSystemCode(QString code);
    void setInsertMode(bool mode);
    QString getSystemAdded();
    QString getCurrentDescription();
protected:
    void applyChanges();
    void cancelChanges();
private:
    Ui::general *ui;
    QSqlDatabase m_database;
    QString m_systemCode;
    bool insertMode;
    void createNewSystem();
    systemTypesModel *stModel;
    systemTypesSortModel *stSortModel;
    void loadLanguages();
    bool systemExists(QString code);
    //FlickCharm charm;

private slots:

    void on_cmdclose_clicked();

};

#endif // GENERAL_H
