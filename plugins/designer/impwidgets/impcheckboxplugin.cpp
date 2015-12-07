#include "impcheckbox.h"
#include "impcheckboxplugin.h"

#include <QtCore/QtPlugin>

IMPCheckBoxPlugin::IMPCheckBoxPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void IMPCheckBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool IMPCheckBoxPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *IMPCheckBoxPlugin::createWidget(QWidget *parent)
{
    return new IMPCheckBox(parent);
}

QString IMPCheckBoxPlugin::name() const
{
    return QLatin1String("IMPCheckBox");
}

QString IMPCheckBoxPlugin::group() const
{
    return QLatin1String("IMPACT Widgets");
}

QIcon IMPCheckBoxPlugin::icon() const
{
    return QIcon(QLatin1String(":/impact.png"));
}

QString IMPCheckBoxPlugin::toolTip() const
{
    return QLatin1String("IMPACT CheckBox");
}

QString IMPCheckBoxPlugin::whatsThis() const
{
    return QLatin1String("A QCheckBox with tablename and fieldname");
}

bool IMPCheckBoxPlugin::isContainer() const
{
    return false;
}

QString IMPCheckBoxPlugin::domXml() const
{
    return QLatin1String("<widget class=\"IMPCheckBox\" name=\"iMPCheckBox\">\n</widget>\n");
}

QString IMPCheckBoxPlugin::includeFile() const
{
    return QLatin1String("impcheckbox.h");
}

