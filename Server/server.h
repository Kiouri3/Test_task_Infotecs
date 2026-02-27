#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>

/**
 * @brief Структура для представления подключенного вычислительного узла (клиента).
 * * Хранит сокет, характеристики оборудования и промежуточные результаты вычислений.
 */
struct WorkerClient {
    QTcpSocket* socket;       ///< Сокет для обмена данными с клиентом.
    uint32_t cores = 0;       ///< Количество ядер CPU у клиента
    double partialResult = 0; ///< Результат, полученный от этого клиента
    bool isReady = false;     ///< Флаг готовности результата

    /**
     * @brief Конструктор структуры.
     * @param socket Указатель на открытый TCP-сокет клиента.
     */
    WorkerClient(QTcpSocket* socket) : socket(socket) {}
};

/**
 * @class Server
 * @brief Класс сервера для управления распределенными вычислениями.
 * * Сервер принимает подключения от клиентов, распределяет между ними интервалы
 * интегрирования пропорционально количеству их ядер и собирает результаты.
 */
class Server : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Инициализирует сервер на указанном порту.
     * @param port Номер порта для прослушивания (например, 1234).
     * @param parent Указатель на родительский объект Qt.
     */
    explicit Server(uint16_t port, QObject *parent = nullptr);

    /**
     * @brief Запускает процесс распределения задачи по клиентам.
     * * @param a Начало интервала интегрирования.
     * @param b Конец интервала интегрирования.
     * @param step Шаг интегрирования.
     */
    void startTask(double a, double b, double step);

private slots:
    /**
     * @brief Обработчик нового входящего TCP-подключения.
     */
    void onNewConnection();

    /**
     * @brief Слот для чтения данных из сокетов клиентов.
     * * Обрабатывает как первое сообщение (количество ядер), так и результаты вычислений.
     */
    void onReadyRead();

    /**
     * @brief Очищает ресурсы при отключении клиента.
     */
    void onClientDisconnected();

private:
    QTcpServer* _server;                 ///< Объект TCP-сервера Qt.

    std::vector<WorkerClient*> _clients; ///< Список структур данных подключенных клиентов.

    double _totalA, _totalB, _totalStep; ///< Хранилище параметров текущей задачи.
    int _responsesReceived = 0;          ///< Счетчик полученных ответов от клиентов.
};

#endif // SERVER_H
