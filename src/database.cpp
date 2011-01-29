/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include "database.h"
#include "const.h"

void Database::openDB () {
    // Find QSLite driver

    QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));

    QSqlQuery query;
    query.prepare( "create table lineEditComplete (id int primary key, seachWord varchar(20), numberOfUsed int)" );

#ifdef Q_OS_LINUX
    QString path( QDir::home ().path () + "/.config/" + APP_NAME + "/" + APP_NAME + ".db" );
    if( !QFile::exists ( path ))
        query.exec();
    db.setDatabaseName( path );
#else
    if( !QFile::exists ( APP_NAME + ".db" ))
        query.exec();
    db.setDatabaseName( APP_NAME + ".db" );
#endif

    db.close();
    db.removeDatabase("QSQLITE");
}

bool Database::deleteDB () {
#ifdef Q_OS_LINUX
    return QFile::remove ( QDir::home ().path () + "/.config/" + APP_NAME + ".db" + "/" + APP_NAME + ".db" + ".db" );
#else
    return QFile::remove ( APP_NAME + ".db" + ".db" );
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
