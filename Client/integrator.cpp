#include "integrator.h"
#include <cmath>
#include <stdexcept>
#include <thread>
#include <vector>
#include <numeric>

using namespace std;

// Вспомогательная функция
static double f(double x) {
    return 1 / std::log(x);
}

// Однопоточное вычисление методом средних прямоугольников.
double Integrator::integrate(double a, double b, double step)
{
    if (a <= 1.0 && b >= 1.0) {
        throw std::runtime_error("Singularity at x = 1");
    }

    double sum = 0.0;

    for (double x = a; x < b; x += step) {
        double current_step = min(step, b - x);
        double mid = x + current_step / 2.0;
        sum += f(mid) * current_step;
    }

    return sum;
}

// Многопоточное вычисление
double Integrator::integrateParallel(double a, double b, double step)
{
    if (a <= 1.0 && b >= 1.0) {
        throw std::runtime_error("Singularity at x = 1");
    }

    unsigned int count_threads = thread::hardware_concurrency();

    if (count_threads == 0) {
        count_threads = 2;    // Если не смогли вычислить
    }

    vector<thread> threads;
    vector<double> result(count_threads);
    double thread_step = (b - a) / count_threads;

    for (unsigned int i = 0; i < count_threads; ++i) {
        double local_a = a + thread_step * i;
        double local_b = min(local_a + thread_step, b);

        threads.emplace_back([local_a, local_b, step, &result, i] () {
            result[i] = Integrator::integrate(local_a, local_b, step);
        });
    }

    for (auto& t : threads) {
        t.join();             // Присоединяем потоки
    }

    // Возвращаем сумму
    return accumulate(result.begin(), result.end(), 0.0);
}
