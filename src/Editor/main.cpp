#include <GL/glew.h>
#include <QDebug>
#include <QtWidgets/QApplication>
#include "Op3nD.h"
#include <States/StateManager.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setOrganizationName("Op3nD");
    app.setOrganizationDomain("Op3nD.com");
    app.setApplicationName("Op3nD");
    Op3nD op3nd;
    op3nd.show();
    return app.exec();
}
