#ifndef PLUGINNOTFOUND_H
#define PLUGINNOTFOUND_H

#include <QWidget>

namespace Ui {
    class pluginnotfound;
}

class pluginnotfound : public QWidget
{
    Q_OBJECT

public:
    explicit pluginnotfound(QWidget *parent = 0);
    ~pluginnotfound();
    void setInfo(QString file, QString name);


private:
    Ui::pluginnotfound *ui;
};

#endif // PLUGINNOTFOUND_H
