#include <QCoreApplication>
#include <QDebug>
#include <QElapsedTimer>
#include "integrator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    double start = 5.0;
    double end = 1000000.0;
    double step = 1;

    QElapsedTimer timer;
    timer.start();
    double res2 = Integrator::integrateParallel(start, end, step);
    qDebug() << "Result:" << res2 << "Time:" << timer.elapsed() << "ms";

    return 0;
}
