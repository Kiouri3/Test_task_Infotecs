#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

/**
 * @class Client
 * @brief Класс сетевого клиента для выполнения вычислительных задач.
 * * Подключается к серверу, передает информацию о доступных ядрах процессора,
 * принимает параметры интегрирования и возвращает результат вычислений.
 */
class Client : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор клиента.
     * @param parent Родительский объект Qt.
     */
    explicit Client(QObject *parent = nullptr);
    /**
     * @brief Пытается установить соединение с сервером.
     * * Проверяет текущее состояние сокета и инициирует подключение к localhost:1234.
     */
    void tryConnect();

private slots:
    /** @brief Обработчик успешного установления соединения. */
    void onConnected();

    /** @brief Слот для чтения входящих задач от сервера и отправки результатов. */
    void onReadyRead();

    /** * @brief Обработчик ошибок сокета.
     * @param socketError Код ошибки сокета.
     */
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *m_socket;   ///< Объект сокета для работы с TCP-сетью.
};

#endif // CLIENT_H
