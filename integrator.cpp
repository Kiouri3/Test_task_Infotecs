#include "integrator.h"
#include <cmath>
#include <QtMinMax>
#include <stdexcept>

static double f(double x) {
    return 1 / std::log(x);
}

double Integrator::integrate(double a, double b, double step)
{
    if (a <= 1.0 && b >= 1.0) {
        throw std::runtime_error("Interval crosses singularity at x = 1");
    }

    double sum = 0.0;

    for (double x = a; x < b; x += step) {
        double current_step = std::min(step, b - x);
        double mid = x + current_step / 2.0;
        sum += f(mid) * current_step;
    }

    return sum;
}
