#include <QCoreApplication>
#include <QDebug>
#include <QElapsedTimer>
#include "integrator.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server(1234);

    return a.exec();
}
