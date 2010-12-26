/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include <QtGui/QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include "mainwindow.h"
#include "iconloader.h"

int main(int argc, char *argv[])
{
#ifdef Q_OS_UNIX
    if (geteuid() == 0) {
        qDebug ()<< QObject::tr("Simba is not supposed to be run as root");
        exit(0);
    }
#endif
    Q_INIT_RESOURCE(data);

    QApplication a(argc, argv);

    // check tray exist
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system."));
    }

    // install qt translator
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

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

