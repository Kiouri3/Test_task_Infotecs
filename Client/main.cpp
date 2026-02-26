#include <QCoreApplication>
#include <QTimer>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Client client;
    qInfo() << "Client started. Automatic reconnection every 5 seconds...";

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, &client, &Client::tryConnect);
    timer->start(5000);

    return a.exec();
}
