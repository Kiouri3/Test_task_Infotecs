#include "client.h"
#include "integrator.h"
#include <thread>

Client::Client(QObject *parent) {
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(m_socket, &QAbstractSocket::errorOccurred, this, &Client::onError);
}

void Client::tryConnect() {
    // Пытаемся соединиться только если сокет полностью свободен
    if (m_socket->state() == QAbstractSocket::UnconnectedState) {
        qDebug() << "Client: Attempting to connect to server...";
        m_socket->connectToHost("127.0.0.1", 1234);
    }
}

void Client::onConnected() {
    qDebug() << "Client: Connected to server.";

    uint32_t cores = std::thread::hardware_concurrency();

    // Отправляем серверу количество ядер клиента
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);

    out << cores;
    m_socket->write(data);
}

void Client::onReadyRead() {
    QDataStream in(m_socket);

    // Получаем от сервера задание
    double a, b, step;
    in >> a >> b >> step;
    qDebug() << "Client: Received task" << a << "," << b <<  "step:" << step;

    double result = Integrator::integrateParallel(a, b, step);

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);

    out << result;
    m_socket->write(data);
    qDebug() << "Client: Result sent to server:" << result;
}

void Client::onError(QAbstractSocket::SocketError socketError) {
    qWarning() << "Client Error:" << m_socket->errorString();
}
