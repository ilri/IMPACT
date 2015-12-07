#include "impcombobox.h"
#include "impcomboboxplugin.h"

#include <QtCore/QtPlugin>

IMPComboBoxPlugin::IMPComboBoxPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void IMPComboBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool IMPComboBoxPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *IMPComboBoxPlugin::createWidget(QWidget *parent)
{
    return new IMPComboBox(parent);
}

QString IMPComboBoxPlugin::name() const
{
    return QLatin1String("IMPComboBox");
}

QString IMPComboBoxPlugin::group() const
{
    return QLatin1String("IMPACT Widgets");
}

QIcon IMPComboBoxPlugin::icon() const
{
    return QIcon(QLatin1String(":/impact.png"));
}

QString IMPComboBoxPlugin::toolTip() const
{
    return QLatin1String("IMPACT ComboBox");
}

QString IMPComboBoxPlugin::whatsThis() const
{
    return QLatin1String("A QComboBox with tableName and fieldName");
}

bool IMPComboBoxPlugin::isContainer() const
{
    return false;
}

QString IMPComboBoxPlugin::domXml() const
{
    return QLatin1String("<widget class=\"IMPComboBox\" name=\"iMPComboBox\">\n</widget>\n");
}

QString IMPComboBoxPlugin::includeFile() const
{
    return QLatin1String("impcombobox.h");
}

