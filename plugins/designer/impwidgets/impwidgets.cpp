#include "impeditplugin.h"
#include "impcheckboxplugin.h"
#include "impgroupboxplugin.h"
#include "impradiobuttonplugin.h"
#include "impcomboboxplugin.h"
#include "impgenmaintplugin.h"
#include "impwidgets.h"

IMPWidgets::IMPWidgets(QObject *parent)
        : QObject(parent)
{
    m_widgets.append(new IMPEditPlugin(this));
    m_widgets.append(new IMPCheckBoxPlugin(this));
    m_widgets.append(new IMPGroupBoxPlugin(this));
    m_widgets.append(new IMPRadioButtonPlugin(this));
    m_widgets.append(new IMPComboBoxPlugin(this));
    m_widgets.append(new IMPGenMaintPlugin(this));

}

QList<QDesignerCustomWidgetInterface*> IMPWidgets::customWidgets() const
{
    return m_widgets;
}

//Q_EXPORT_PLUGIN2(impwidgetsplugin, IMPWidgets)
