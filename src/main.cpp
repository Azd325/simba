/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include <QtGui/QApplication>
#include "mainwindow.h"
#include "iconloader.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(data);

    QApplication a(argc, argv);

    // QSettings stuff
    QCoreApplication::setApplicationName ("Simba");
    QCoreApplication::setApplicationVersion ("0.94");
    QCoreApplication::setOrganizationDomain ("azd325.github.com/simba/");
    QCoreApplication::setOrganizationName (QCoreApplication::applicationName ());

    // Icons
    IconLoader::Init();

    // delivery the cli argument
    MainWindow w(QCoreApplication::arguments());
    w.show();
    return a.exec();
}

