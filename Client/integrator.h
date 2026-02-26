#ifndef INTEGRATOR_H
#define INTEGRATOR_H

class Integrator
{
public:
    static double integrate(double a, double b, double step = 0.001);

    static double integrateParallel(double a, double b, double step = 0.001);
};

#endif // INTEGRATOR_H
