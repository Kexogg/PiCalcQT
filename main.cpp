#include <QApplication>
#include "PiCalcQT.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PiCalcQT calc;
    calc.show();
    return app.exec();
}
