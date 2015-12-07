#include "impedit.h"
#include <QMouseEvent>
#include "widgets/calculator/calcdialog.h"
#include "widgets/calendar/calendardialog.h"
#include <QDebug>

IMPEdit::IMPEdit(QWidget *parent) :
    QLineEdit(parent)
{
    m_dateFormat = "ddMMyyyy";
    connect(this,SIGNAL(textEdited(QString)),this,SLOT(valueHasChanged(QString)));
}

QString IMPEdit::tableName()
{
    return m_tableName;
}

void IMPEdit::setTableName(QString name)
{
    m_tableName = name;
}

QString IMPEdit::fieldName()
{
    return m_fieldName;
}

QString IMPEdit::dateFormat()
{
    return m_dateFormat;
}

void IMPEdit::setFieldName(QString name)
{
    m_fieldName = name;
}

void IMPEdit::valueHasChanged(QString text)
{
    emit valueChanged(m_tableName,m_fieldName,text);
}

void IMPEdit::setDataBase(QSqlDatabase db)
{
    dataBase = db;
}

void IMPEdit::mousePressEvent (QMouseEvent * event)
{
    if (IMPWidgets_use_Android)
    {
        if (!this->isReadOnly())
        {
            if ((m_fieldType == Numeric) || (m_fieldType == Decimal))
            {
                event->accept();
                calcDialog calc;
                calc.setDatabase(dataBase);
                calc.setCurrentDisplay(this->text().toDouble());
                calc.exec();
                this->setText(calc.getCurrentDisplay());
                valueHasChanged(this->text());
            }
            if (m_fieldType == Date)
            {
                event->accept();
                calendarDialog calendar;
                QDate date;
                date = date.fromString(this->text(),m_dateFormat);
                if (date.isValid())
                    calendar.setCurrentDate(date);
                else
                    qDebug() << "Invalid date. Check format";
                calendar.exec();
                this->setText(calendar.getCurrentDate().toString(m_dateFormat));
                valueHasChanged(this->text());
            }
        }
    }
    else
        QLineEdit::mousePressEvent(event);
}
