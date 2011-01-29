/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>

#include "const.h"
#include "mainwindow.h"
#include "iconloader.h"
#include "database.h"

int main(int argc, char *argv[])
{
    // Settings stuff
    QCoreApplication::setApplicationName ( APP_NAME );
    QCoreApplication::setApplicationVersion ( APP_VERSION );
    QCoreApplication::setOrganizationDomain ( APP_URL );
    QCoreApplication::setOrganizationName ( APP_NAME);

    // verify the user if not run as root
#ifdef Q_OS_UNIX
    if ( geteuid () == 0 ) {
        qWarning ()<< APP_NAME + QObject::tr ( " is not supposed to be run as root" );
        exit(0);
    }
#endif
    // initiliaze the resource datas
    Q_INIT_RESOURCE( data );

    // only one session of simba
    QApplication a( argc, argv );

    // check tray exist
    if ( !QSystemTrayIcon::isSystemTrayAvailable ())
        qWarning ()<< QObject::tr ( "I couldn't detect any system tray on this system." );

    // install qt translator
    QTranslator qtTranslator;
    qtTranslator.load( "qt_" + QLocale::system ().name (), QLibraryInfo::location ( QLibraryInfo::TranslationsPath ));
    a.installTranslator( &qtTranslator );

    // verify the exist of the app database, when not create a database
    Database::openDB();

    // Icons
    IconLoader::Init ();

    // delivery the cli argument
    MainWindow w( QCoreApplication::arguments ());
    w.show ();
    return a.exec ();
}

