#include <QCoreApplication>
#include <QDebug>
#include "integrator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << Integrator::integrate(5, 8, 5);

    return 0;
}
