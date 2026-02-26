#include <QCoreApplication>
#include <QThread>
#include "server.h"
#include "ConsoleReader.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    Server server(1234);

    // Создаем объекты в куче (heap)
    QThread* thread = new QThread();
    ConsoleReader* reader = new ConsoleReader();

    reader->moveToThread(thread);

    // Соединяем сигнал со слотом.
    // Обрати внимание: 4-й аргумент Qt::QueuedConnection критически важен!
    QObject::connect(reader, &ConsoleReader::taskReady, &server, &Server::startTask, Qt::QueuedConnection);

    // Запуск логики чтения при старте потока
    QObject::connect(thread, &QThread::started, reader, &ConsoleReader::run);

    // Чистим память после закрытия
    QObject::connect(&app, &QCoreApplication::aboutToQuit, thread, &QThread::quit);
    QObject::connect(thread, &QThread::finished, reader, &QObject::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();

    return app.exec();
}
