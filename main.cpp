#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "servercommunication.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Creating server object and fetching data from server
    ServerCommucation* server_object = new ServerCommucation;
    server_object->fetch();
    server_object->fetchSpecies();

    // Passing object to qml files
    engine.rootContext()->setContextProperty("server_object", server_object);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
