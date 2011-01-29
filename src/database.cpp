/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include "database.h"
#include "const.h"

bool Database::openDB () {
    // Find QSLite driver

    QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));

    QSqlQuery query;

#ifdef Q_OS_LINUX
    QString path( QDir::home ().path () + "/.config/" + APP_NAME + "/" + APP_NAME + ".db" );
    if( !QFile::exists ( path )) {
	db.setDatabaseName( path );
    }
#else
    if( !QFile::exists ( APP_NAME + ".db" ))
	db.setDabaseName( APP_NAME + ".db");
#endif

    if( !db.open ()) {
        qWarning()<< DB_NOT_OPEN ;
	return false;
    }

    query.exec( "CREATE TABLE searchWords (id INTEGER PRIMARY KEY, seachWord VARCHAR(20), numberOfUsed INTEGER)" );

    db.close();
    db.removeDatabase( "QSQLITE" );
    return true;
}

bool Database::deleteDB () {
#ifdef Q_OS_LINUX
    return QFile::remove ( QDir::home ().path () + "/.config/" + APP_NAME + ".db" + "/" + APP_NAME + ".db" + ".db" );
#else
    return QFile::remove ( APP_NAME + ".db" + ".db" );
#endif
}

bool Database::setSearchWord( QString word = "" ) {
    QString path( QDir::home ().path () + "/.config/" + APP_NAME + "/" + APP_NAME + ".db" );

    QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
    db.setDatabaseName( path );
    if( !db.open ()) {
	qWarning()<< DB_NOT_OPEN ;
	return false;
    }

    QSqlQuery query;
	query.prepare( "INSERT INTO searchWords ( seachWord, numberOfUsed ) VALUES ( :w, :n )" );
	query.bindValue( ":w", word, QSql::InOut );
	query.bindValue( ":n", 1, QSql::InOut );
	query.exec();

    db.close();
    db.removeDatabase( "QSQLITE" );
    return true;
}
