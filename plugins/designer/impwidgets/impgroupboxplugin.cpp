#include "impgroupbox.h"
#include "impgroupboxplugin.h"

#include <QtCore/QtPlugin>

IMPGroupBoxPlugin::IMPGroupBoxPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void IMPGroupBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool IMPGroupBoxPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *IMPGroupBoxPlugin::createWidget(QWidget *parent)
{
    return new IMPGroupBox(parent);
}

QString IMPGroupBoxPlugin::name() const
{
    return QLatin1String("IMPGroupBox");
}

QString IMPGroupBoxPlugin::group() const
{
    return QLatin1String("IMPACT Widgets");
}

QIcon IMPGroupBoxPlugin::icon() const
{
    return QIcon(QLatin1String(":/impact.png"));
}

QString IMPGroupBoxPlugin::toolTip() const
{
    return QLatin1String("IMPACT GroupBox");
}

QString IMPGroupBoxPlugin::whatsThis() const
{
    return QLatin1String("A QGroupBox with tablename and fieldname");
}

bool IMPGroupBoxPlugin::isContainer() const
{
    return true;
}

QString IMPGroupBoxPlugin::domXml() const
{
    return QLatin1String("<widget class=\"IMPGroupBox\" name=\"iMPGroupBox\">\n</widget>\n");
}

QString IMPGroupBoxPlugin::includeFile() const
{
    return QLatin1String("impgroupbox.h");
}

