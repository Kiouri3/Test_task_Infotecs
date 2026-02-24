#include "server.h"
#include "integrator.h"

Server::Server(uint16_t port, QObject *parent)
    : QObject{parent}
{
    _server = new QTcpServer(this);

    connect(_server, &QTcpServer::newConnection, this, &Server::onNewConnection);

    if (!_server->listen(QHostAddress::Any, port)) {
        qCritical() << "Server could not start!";
    } else {
        qDebug() << "Server started on port" << port;
    }
}

void Server::onNewConnection() {
    QTcpSocket* clientSocket = _server->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::connected, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

    qDebug() << "New client connected:" << clientSocket->peerAddress().toString();
}

void Server::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());

    if (!clientSocket) {
        return;
    }

    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_6_10);

    // Проверяем, хватает ли данных (3 числа: double, double, double)
    if (clientSocket->bytesAvailable() < (sizeof(double) * 3)) {
        return;
    }

    double a, b, step;
    in >> a >> b >> step;

    qDebug() << "Received task: a=" << a << "b=" << b << "step=" << step;

    try {
        double result = Integrator::integrateParallel(a, b, step);

        // Отправляем обратно
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        out << result;

        clientSocket->write(block);
    } catch (const std::exception &e) {
        qWarning() << "Error during calculation:" << e.what();
        clientSocket->disconnectFromHost();
    }
}

void Server::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Client disconnected";
    clientSocket->deleteLater();
}
