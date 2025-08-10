#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "PedalController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    PedalController pedalController;

    engine.rootContext()->setContextProperty("pedalController", &pedalController);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GuitarPedal", "Main");

    return app.exec();
}
