#include "plasmoidplugin.h"
#include "systemCalls.h"

#include <QDebug>
#include <QtQml>

void PlasmoidPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("com.github.archupdate"));

    qmlRegisterType<systemCalls>(uri, 1, 0, "SystemCalls");
}
