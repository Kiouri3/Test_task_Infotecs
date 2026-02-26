#ifndef CONSOLEREADER_H
#define CONSOLEREADER_H

#include <QObject>
#include <iostream>
#include <string>

class ConsoleReader : public QObject {
    Q_OBJECT
public:

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
    void run() {
        double a, b, step;

        while (true) {
            std::cout << "\n--- Configuration of New Task ---" << "\n";

            a = readDouble("Enter lower limit (a):");
            b = readDouble("Enter upper limit (b):", a + 0.000001);
            step = readDouble("Enter integration step:");

            std::cout << "\nReady! Settings: [" << a << " to " << b << "], step: " << step << "\n";
            break;
        }

        while (true) {
            std::cout << "Press ENTER to send task to clients...\n";

            std::string dummy;
            std::getline(std::cin, dummy);

            emit taskReady(a, b, step);
        }
    }

signals:
    void taskReady(double a, double b, double step);
};

#endif
