/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include "database.h"

bool Database::openDB () {
    // Find QSLite driver
{
    QSqlDatabase db( QSqlDatabase::addDatabase( "QSQLITE", "db" ));

#ifdef Q_OS_LINUX
    QString path( QDir::home ().path () + "/.config/" + APP_NAME + "/" + APP_NAME + ".db" );
    if( QFile::exists ( path ))
	return true;
    else
	db.setDatabaseName( path );
#else
    if( QFile::exists ( APP_NAME + ".db" ))
	return true;
    else
	db.setDabaseName( APP_NAME + ".db");
#endif

    if( db.open ()) {
	QSqlQuery query( db );
	query.exec( "CREATE TABLE searchWords (id INTEGER PRIMARY KEY, seachWord VARCHAR(20), numberOfUsed INTEGER)" );
	if ( !query.isActive ()){ qWarning()<< QObject::tr( "Database Error: " ) + query.lastError ().text ();}
    }
    else {
        qWarning()<< DB_NOT_OPEN ;
	return false;
    }

    db.close ();
}
    QSqlDatabase::removeDatabase ( "db" );
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

{
    QSqlDatabase db( QSqlDatabase::addDatabase( "QSQLITE", "db" ));
    db.setDatabaseName ( path );
    if( !db.open ()) {
	QSqlQuery query( db );
            query.prepare ( "INSERT INTO searchWords ( seachWord, numberOfUsed ) VALUES ( :w, :n )" );
            query.bindValue ( ":w", word, QSql::InOut );
            query.bindValue ( ":n", 1, QSql::InOut );
            query.exec ();
	    if ( !query.isActive ()){ qWarning()<< QObject::tr( "Database Error: " ) + query.lastError ().text ();}
    }
    else {
	qWarning()<< DB_NOT_OPEN ;
	return false;
    }

    db.close ();
}
    QSqlDatabase::removeDatabase ( "db" );
    return true;
}
