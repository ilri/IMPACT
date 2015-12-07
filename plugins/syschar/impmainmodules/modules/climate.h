#ifndef CLIMATE_H
#define CLIMATE_H

#include <QWidget>
#include "moduleclasses.h"
#include "flickcharm.h"

namespace Ui {
    class climate;
}

class climate : public QWidget
{
    Q_OBJECT

public:
    explicit climate(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase(), QString systemID = "NONE");
    ~climate();


private:
    Ui::climate *ui;
    QSqlDatabase dataBase;
    QString currentSystem;
    periodTableModel *m_periodModel;
    void loadForm();
    int numseasons;
    int newNumseasons;
    void constructCustomDelegator();
    FlickCharm charm;
    void loadAndroidEffects();
public slots:
    void loadCalculator (const QModelIndex &index);
private slots:
    void on_BitBtn45_clicked();
    void on_BitBtn43_clicked();
    void on_RadioButton1_clicked();
    void on_RadioButton2_clicked();
};

#endif // CLIMATE_H
