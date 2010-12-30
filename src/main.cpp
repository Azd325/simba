/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QLibraryInfo>

#include "mainwindow.h"
#include "iconloader.h"
#include "database.h"

int main(int argc, char *argv[])
{
    // Settings stuff
    QCoreApplication::setApplicationName ( "Simba" );
    QCoreApplication::setApplicationVersion ( "0.95" );
    QCoreApplication::setOrganizationDomain ( "azd325.github.com/simba/" );
    QCoreApplication::setOrganizationName ( QCoreApplication::applicationName ());

    if( !Database::openDB ())
        exit (0);

#ifdef Q_OS_UNIX
    if ( geteuid () == 0 ) {
        qWarning ()<< QCoreApplication::applicationName () + QObject::tr ( " is not supposed to be run as root" );
        exit(0);
    }
#endif
    Q_INIT_RESOURCE( data );

    QApplication a( argc, argv );

    // check tray exist
    if ( !QSystemTrayIcon::isSystemTrayAvailable ())
        qWarning ()<< QObject::tr ( "I couldn't detect any system tray on this system." );

    // install qt translator
    QTranslator qtTranslator;
    qtTranslator.load( "qt_" + QLocale::system ().name (), QLibraryInfo::location ( QLibraryInfo::TranslationsPath ));
    a.installTranslator( &qtTranslator );

    // Icons
    IconLoader::Init ();

    // delivery the cli argument
    MainWindow w( QCoreApplication::arguments ());
    w.show ();
    return a.exec ();
}

