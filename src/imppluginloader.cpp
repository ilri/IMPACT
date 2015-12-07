#include "imppluginloader.h"

impPlugin::impPlugin(QString name)
{
    pluginName = name;
}

impPluginInstance::impPluginInstance(QObject *instanceRef, QString library)
{
    instance = instanceRef;
    libraryName = library;
    if (instanceRef)
    {
        impInterface *interface = qobject_cast<impInterface *>(instanceRef);
        if (interface)
        {
            instanceName = tr("impInterface");
            QStringList lstPlugins = interface->pluginNames();
            int pos;
            for (pos =0; pos <= lstPlugins.count()-1;pos++)
            {
                plugins.append(new impPlugin(lstPlugins[pos]));
            }
        }
    }
}

impPluginInstance::~impPluginInstance()
{
    // Any destructor for the plugin? Shoud be fine as the instance comes from the
    // the pluging loader and unloaded in termination
}
