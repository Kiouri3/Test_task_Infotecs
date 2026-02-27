#include <QCoreApplication>
#include <QTimer>
#include "client.h"

/**
 * @brief Главная функция клиента.
 * * Запускает событийный цикл Qt и настраивает QTimer для автоматических
 * попыток переподключения к серверу каждые 5 секунд.
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Client client;
    qInfo() << "Client started. Automatic reconnection every 5 seconds...";

    /** * @brief Таймер для периодического вызова Client::tryConnect.
     * Позволяет клиенту автоматически найти сервер, если тот был запущен позже.
     */
    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, &client, &Client::tryConnect);
    timer->start(5000);

    return a.exec();
}
