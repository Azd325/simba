#include <QtGui/QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(data);

    QApplication a(argc, argv);

    // QSettings stuff
    QCoreApplication::setApplicationName ("Simba");
    QCoreApplication::setApplicationVersion ("0.89");
    QCoreApplication::setOrganizationDomain ("github.com/azd325/simba");
    QCoreApplication::setOrganizationName ("Tim Kleinschmidt");

    // delivery the cli argument
    MainWindow w(QCoreApplication::arguments());
    w.show();
    return a.exec();
}
