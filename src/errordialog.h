#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>

namespace Ui {
    class errorDialog;
}

class errorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit errorDialog(QWidget *parent = 0);
    ~errorDialog();
    void setErrorMessage(QString message);
    void setErrorMessage(QStringList messages);
    void setLastSQLCommand(QString command);

private:
    Ui::errorDialog *ui;
    QString errorMsg;
    QString lastSQLCommand;

private slots:
    void closeForm();
};

#endif // ERRORDIALOG_H
