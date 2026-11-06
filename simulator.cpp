#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Console.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Console console;
    engine.rootContext()->setContextProperty("vm", &console);

    const QUrl url(u"qrc:/qt/qml/appVoleMachine_ui/Simulator.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl)
                     {
    if (!obj && url == objUrl)
        QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
