#include <QCoreApplication>
#include <thread>
#include "server.h"
#include "ConsoleReader.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    Server server(1234);
    ConsoleReader reader;

    QObject::connect(&reader, &ConsoleReader::taskReady, [&server]() {
        qDebug() << "Enter detected! Starting task...";
        server.startTask(0, 100, 0.1);
    });

    std::thread inputThread([&reader]() {
        reader.run();
    });

    inputThread.detach();

    return app.exec();
}
