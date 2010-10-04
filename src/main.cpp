#include <QtGui/QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(data);

    QApplication a(argc, argv);
    MainWindow w(QCoreApplication::arguments());
    w.show();
    return a.exec();
}
