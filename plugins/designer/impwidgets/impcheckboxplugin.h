#ifndef IMPCHECKBOXPLUGIN_H
#define IMPCHECKBOXPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class IMPCheckBoxPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    //Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "impwidgets.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    IMPCheckBoxPlugin(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

private:
    bool m_initialized;
};

#endif