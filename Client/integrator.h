#ifndef INTEGRATOR_H
#define INTEGRATOR_H

/**
 * @class Integrator
 * @brief Статический класс для проведения численного интегрирования.
 * * Содержит методы для вычисления интеграла функции 1/ln(x) методом средних прямоугольников.
 */
class Integrator
{
public:
    /**
     * @brief Выполняет последовательное (однопоточное) интегрирование.
     * @param a Нижняя граница интервала.
     * @param b Верхняя граница интервала.
     * @param step Шаг интегрирования (по умолчанию 0.001).
     * @return double Результат вычисления площади под кривой.
     * @throw std::runtime_error Если интервал [a, b] включает точку разрыва x = 1.
     */
    static double integrate(double a, double b, double step = 0.001);

    /**
     * @brief Выполняет параллельное (многопоточное) интегрирование.
     * * Разделяет интервал на сегменты и вычисляет их в разных потоках CPU для ускорения.
     * @param a Нижняя граница интервала.
     * @param b Верхняя граница интервала.
     * @param step Шаг интегрирования.
     * @return double Суммарный результат всех потоков.
     */
    static double integrateParallel(double a, double b, double step = 0.001);
};

#endif // INTEGRATOR_H
