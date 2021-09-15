#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QLoggingCategory>

#include "systemCalls.h"

int main(int argc, char *argv[])
{
#ifndef NDEBUG
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    systemCalls *systemcalls = new systemCalls();
    QQmlContext* ctx = engine.rootContext();
    ctx->setContextProperty(QStringLiteral("systemCalls"), systemcalls);
    engine.load(QUrl(QStringLiteral("../package/contents/ui/main.qml")));
    return app.exec();
}
