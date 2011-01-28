/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include "database.h"

void Database::openDB () {
    // Find QSLite driver

    QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));

    QSqlQuery query;
    query.prepare( "create table lineEditComplete (id int primary key, seachWord varchar(20), numberOfUsed int)" );

#ifdef Q_OS_LINUX
    QString path( QDir::home ().path () + "/.config/" + QCoreApplication::applicationName () + "/" + QCoreApplication::applicationName () + ".db" );
    if( !QFile::exists ( path ))
        query.exec();
    db.setDatabaseName( path );
#else
    if( !QFile::exists ( QCoreApplication::applicationName () + ".db" ))
        query.exec();
    db.setDatabaseName( QCoreApplication::applicationName () + ".db" );
#endif

    db.close();
    db.removeDatabase("QSQLITE");
}

bool Database::deleteDB () {
#ifdef Q_OS_LINUX
    return QFile::remove ( QDir::home ().path () + "/.config/" + QCoreApplication::applicationName () + "/" + QCoreApplication::applicationName () + ".db" );
#else
    return QFile::remove ( QCoreApplication::applicationName () + ".db" );
#endif
}

bool Database::setSearchWord() {
    QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));

    QSqlQuery query;
    query.prepare( "create table lineEditComplete (id int primary key, seachWord varchar(20), numberOfUsed int)" );

    db.close();
    db.removeDatabase("QSQLITE");
    return true;
}
