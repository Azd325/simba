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

#ifdef Q_OS_LINUX
    QString path( QDir::home ().path () + "/.config/" + APP_NAME + "/" + APP_NAME + ".db" );
    if( !QFile::exists ( path )) {
	db.setDatabaseName( path );
	db.open();
	query.prepare( "create table lineEditComplete (id int primary key, seachWord varchar(20), numberOfUsed int)" );
        query.exec();
    }
#else
    if( !QFile::exists ( APP_NAME + ".db" )) {
	db.setDatabaseName( path );
	db.open();
	query.prepare( "create table lineEditComplete (id int primary key, seachWord varchar(20), numberOfUsed int)" );
        query.exec();
    }
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

bool Database::setSearchWord( QString word ) {
    QString path( QDir::home ().path () + "/.config/" + APP_NAME + "/" + APP_NAME + ".db" );
    QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
    db.setDatabaseName( path );
    db.open();


    QSqlQuery query;
    query.prepare( "create table lineEditComplete (id int primary key, seachWord varchar(20), numberOfUsed int)" );
    query.exec();

    db.close();
    db.removeDatabase("QSQLITE");
    return true;
}
