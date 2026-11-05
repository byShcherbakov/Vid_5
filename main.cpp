#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QApplication::setApplicationName(QStringLiteral("WatermelonScene"));
    QApplication::setApplicationVersion(QStringLiteral("1.0"));
    QApplication::setOrganizationName(QStringLiteral("StudentLabs"));
    MainWindow window;
    window.show();
    return application.exec();
}
