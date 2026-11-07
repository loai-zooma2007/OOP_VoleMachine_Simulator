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

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []()
                     { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.loadFromModule("VoleMachine_ui", "Simulator");

    return app.exec();
}
