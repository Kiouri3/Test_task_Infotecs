#ifndef CONSOLEREADER_H
#define CONSOLEREADER_H

#include <QObject>
#include <iostream>
#include <string>
#include <QThread>

/**
 * @class ConsoleReader
 * @brief Класс для интерактивного взаимодействия с пользователем через консоль.
 * * Работает в отдельном потоке, обеспечивая ввод параметров задачи (интервалы, шаг)
 * без блокировки основного сетевого потока сервера.
 */
class ConsoleReader : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Считывает число типа double с проверкой корректности ввода.
     * * Метод зацикливается до тех пор, пока пользователь не введет валидное число,
     * не меньшее заданного порога.
     * * @param label Текст приглашения ко вводу (подсказка).
     * @param minVal Минимально допустимое значение.
     * @return double Введенное и проверенное число.
     */
    double readDouble(const std::string& label, double minVal = -std::numeric_limits<double>::max()) {
        std::string input;
        double value;
        while (true) {
            std::cout << label << " " << std::flush;
            if (!std::getline(std::cin, input)) {
                std::cin.clear();
                continue;
            }

            if (input.empty()) continue;

            try {
                size_t pos;
                value = std::stod(input, &pos);

                // Проверка: всё ли в строке было числом?
                if (pos != input.length()) {
                    throw std::invalid_argument("Extra chars");
                }

                // Проверка: не слишком ли маленькое число?
                if (value < minVal) {
                    std::cout << "Error: Value must be at least " << minVal << "\n";
                    continue;
                }
                return value;
            } catch (...) {
                std::cout << "Invalid input! Please enter a valid number." << "\n";
            }
        }
    }

public slots:
    /**
     * @brief Основной цикл чтения данных.
     * * Последовательно запрашивает параметры 'a', 'b' и 'step', проверяет их
     * на наличие разрыва функции (x=1) и генерирует сигнал taskReady.
     */
    void run() {
        while (true) {
            std::cout << "\n--- Configuration of New Task ---" << "\n";
            double a = readDouble("Enter lower limit (a):");
            double b = readDouble("Enter upper limit (b):", 0.00000001);

            if (a <= 1.0 && b >= 1.0) {
                std::cout << "Error.The interval contains x = 1\n"
                          << "Please choose an interval that does not include 1.0\n";
                continue;
            }

            double step = readDouble("Enter integration step:");

            std::cout << "\nReady! Settings: [" << a << " to " << b << "], step: " << step << "\n";
            std::cout << "Press ENTER to send task to clients...\n";

            std::string dummy;
            std::getline(std::cin, dummy);
            emit taskReady(a, b, step);

            /** Пауза для предотвращения смешивания вывода сервера и нового меню конфигурации. */
            QThread::msleep(500);
        }
    }

signals:
    /**
     * @brief Сигнал о готовности новой задачи для распределения.
     * @param a Нижняя граница интегрирования.
     * @param b Верхняя граница интегрирования.
     * @param step Шаг вычислений.
     */
    void taskReady(double a, double b, double step);
};

#endif
