#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>

struct WorkerClient {
    QTcpSocket* socket;
    uint32_t cores = 0;       // Количество ядер CPU у клиента
    double partialResult = 0; // Результат, полученный от этого клиента
    bool isReady = false;     // Флаг готовности результата

    WorkerClient(QTcpSocket* socket) : socket(socket) {}
};

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(uint16_t port, QObject *parent = nullptr);

    void startTask(double a, double b, double step);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    QTcpServer* _server;

    std::vector<WorkerClient*> _clients;

    double _totalA, _totalB, _totalStep;
    int _responsesReceived = 0;
};

#endif // SERVER_H
