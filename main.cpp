#include <QtGui/QApplication>
#include "Op3nD.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Op3nD foo;
    foo.show();
    return app.exec();
}
