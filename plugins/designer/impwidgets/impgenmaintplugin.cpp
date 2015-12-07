#include "impgenmaint.h"
#include "impgenmaintplugin.h"

#include <QtCore/QtPlugin>

IMPGenMaintPlugin::IMPGenMaintPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void IMPGenMaintPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool IMPGenMaintPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *IMPGenMaintPlugin::createWidget(QWidget *parent)
{
    return new impgenmaint(parent);
}

QString IMPGenMaintPlugin::name() const
{
    return QLatin1String("impgenmaint");
}

QString IMPGenMaintPlugin::group() const
{
    return QLatin1String("IMPACT Widgets");
}

QIcon IMPGenMaintPlugin::icon() const
{
    return QIcon(QLatin1String(":/impact.png"));
}

QString IMPGenMaintPlugin::toolTip() const
{
    return QLatin1String("IMPACT Generic Maintenance Widget");
}

QString IMPGenMaintPlugin::whatsThis() const
{
    return QLatin1String("The Generic Maintenance");
}

bool IMPGenMaintPlugin::isContainer() const
{
    return true;
}

QString IMPGenMaintPlugin::domXml() const
{
    return QLatin1String("<widget class=\"impgenmaint\" name=\"IMPGenMaint\">\n</widget>\n");
}

QString IMPGenMaintPlugin::includeFile() const
{
    return QLatin1String("impgenmaint.h");
}

