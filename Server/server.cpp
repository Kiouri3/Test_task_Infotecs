#include "server.h"
#include <QTimer>
#include <QDebug>

Server::Server(uint16_t port, QObject *parent)
    : QObject{parent}
{
    _server = new QTcpServer(this);

    connect(_server, &QTcpServer::newConnection, this, &Server::onNewConnection);

    if (!_server->listen(QHostAddress::Any, port)) {
        qWarning() << "Server could not start!";
    } else {
        qDebug() << "Server started on port" << port;
    }
}

void Server::startTask(double a, double b, double step) {
    if (_clients.empty()) {
        qWarning() << "Server: No clients connected to perform task!";
        return;
    }

    // Считаем ядра во всей сети
    uint32_t totalCores = 0;
    for (auto& client : _clients) {
        totalCores += client->cores;
    }

    double currentA = a;
    double fullRange = b - a;
    for (auto& client : _clients) {
        double weight = static_cast<double>(client->cores) / totalCores;
        double rangeForClient = fullRange * weight;
        double currentB = currentA + rangeForClient;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_10);
        out << currentA << currentB << step;

        client->socket->write(block);
        client->isReady = false;

        currentA = currentB;
    }
}

void Server::onNewConnection() {
    QTcpSocket* clientSocket = _server->nextPendingConnection();

    WorkerClient *client = new WorkerClient(clientSocket);
    _clients.push_back(client);

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

    qDebug() << "New client connected:" << clientSocket->peerAddress().toString();
}

void Server::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_10);
    qDebug() << "onReadyRead";

    // Находим, какой клиент прислал данные
    for (auto client : _clients) {
        if (client->socket == socket) {
            if (client->cores == 0) {
                // Первое сообщение от клиента — количество его ядер
                in >> client->cores;
                qInfo() << "Server: Client reported" << client->cores << "cores.";
            } else {
                // Последующие сообщения — результаты вычислений
                double localResult = 0;
                in >> localResult;
                client->partialResult = localResult;
                client->isReady = true;
                _responsesReceived++;
                qInfo() << "Server: Received partial result from client.";

                // Если все ответили, суммируем
                if (_responsesReceived == _clients.size()) {
                    double total = 0;
                    for (auto c : _clients) total += c->partialResult;
                    qInfo() << "FINAL RESULT:" << total;
                }
            }
            break;
        }
    }
}

void Server::onClientDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    for (int i = 0; i < _clients.size(); ++i) {
        if (_clients[i]->socket == socket) {
            qInfo() << "Server: Client disconnected.";
            delete _clients[i];
            break;
        }
    }
}
