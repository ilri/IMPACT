#include "impradiobutton.h"
#include "impradiobuttonplugin.h"

#include <QtCore/QtPlugin>

IMPRadioButtonPlugin::IMPRadioButtonPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void IMPRadioButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool IMPRadioButtonPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *IMPRadioButtonPlugin::createWidget(QWidget *parent)
{
    return new IMPRadioButton(parent);
}

QString IMPRadioButtonPlugin::name() const
{
    return QLatin1String("IMPRadioButton");
}

QString IMPRadioButtonPlugin::group() const
{
    return QLatin1String("IMPACT Widgets");
}

QIcon IMPRadioButtonPlugin::icon() const
{
    return QIcon(QLatin1String(":/impact.png"));
}

QString IMPRadioButtonPlugin::toolTip() const
{
    return QLatin1String("IMPACT Radiobutton");
}

QString IMPRadioButtonPlugin::whatsThis() const
{
    return QLatin1String("A QRadiobutton with tableName, fieldName and fieldValue");
}

bool IMPRadioButtonPlugin::isContainer() const
{
    return false;
}

QString IMPRadioButtonPlugin::domXml() const
{
    return QLatin1String("<widget class=\"IMPRadioButton\" name=\"iMPRadioButton\">\n</widget>\n");
}

QString IMPRadioButtonPlugin::includeFile() const
{
    return QLatin1String("impradiobutton.h");
}

