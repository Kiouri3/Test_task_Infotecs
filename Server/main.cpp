#include <QCoreApplication>
#include <QThread>
#include "server.h"
#include "ConsoleReader.h"

/**
 * @brief Точка входа в приложение.
 * * Выполняет инициализацию сервера, создает отдельный поток для чтения консоли
 * и настраивает межпотоковое взаимодействие через сигналы и слоты.
 */
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    /** Инициализация сервера на порту 1234. */
    Server server(1234);

    /** Создание потока для обработки пользовательского ввода. */
    QThread* thread = new QThread();
    ConsoleReader* reader = new ConsoleReader();
    reader->moveToThread(thread);

    /** * Связываем ридер и сервер.
     * Используется Qt::QueuedConnection для безопасной передачи данных между разными потоками.
     */
    QObject::connect(reader, &ConsoleReader::taskReady, &server, &Server::startTask, Qt::QueuedConnection);
    /** Запуск метода run() при старте потока. */
    QObject::connect(thread, &QThread::started, reader, &ConsoleReader::run);

    /** Настройка корректного завершения потока при закрытии приложения. */
    QObject::connect(&app, &QCoreApplication::aboutToQuit, thread, &QThread::quit);
    QObject::connect(thread, &QThread::finished, reader, &QObject::deleteLater);
    QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();

    return app.exec();
}
