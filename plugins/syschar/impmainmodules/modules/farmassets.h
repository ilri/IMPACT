#ifndef farmassets_H
#define farmassets_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class farmassets;
}

class farmassets : public QWidget
{
    Q_OBJECT

public:
    explicit farmassets(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~farmassets();
    void loadForm();

private:
    Ui::farmassets *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    fieldInColModel *m_colModel;

    void setParentStatus(bool status);
    int numseasons;
    FlickCharm charm;
    void loadAndroidEffects();


private slots:



    void on_BitBtn10_clicked();
    void on_BitBtn9_clicked();
    void on_BitBtn15_clicked();
    void on_BitBtn14_clicked();
    void dataChanged();
    void loadCalculator (const QModelIndex &index);


};

#endif // farmassets_H
