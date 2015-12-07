#include "impedit.h"
#include "impeditplugin.h"

#include <QtCore/QtPlugin>

IMPEditPlugin::IMPEditPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void IMPEditPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool IMPEditPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *IMPEditPlugin::createWidget(QWidget *parent)
{
    return new IMPEdit(parent);
}

QString IMPEditPlugin::name() const
{
    return QLatin1String("IMPEdit");
}

QString IMPEditPlugin::group() const
{
    return QLatin1String("IMPACT Widgets");
}

QIcon IMPEditPlugin::icon() const
{
    return QIcon(QLatin1String(":/impact.png"));
}

QString IMPEditPlugin::toolTip() const
{
    return QLatin1String("IMPACT Edit");
}

QString IMPEditPlugin::whatsThis() const
{
    return QLatin1String("A QLineEdit with tablename and fieldname");
}

bool IMPEditPlugin::isContainer() const
{
    return false;
}

QString IMPEditPlugin::domXml() const
{
    return QLatin1String("<widget class=\"IMPEdit\" name=\"iMPEdit\">\n</widget>\n");
}

QString IMPEditPlugin::includeFile() const
{
    return QLatin1String("impedit.h");
}

