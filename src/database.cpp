/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include "database.h"

bool Database::openDB () {
// bracket are for the correct way to work with databases
  {
    // Find QSLite driver
    QSqlDatabase db( QSqlDatabase::addDatabase( "QSQLITE", "db" ));

    if( QFile::exists ( DB_PATH ))
	return true;
    else
	db.setDatabaseName( DB_PATH );

    if( db.open ()) {
	QSqlQuery query( db );
	query.exec( "CREATE TABLE searchWords (id INTEGER PRIMARY KEY, "
	                                      "searchWord VARCHAR(20) NOT NULL,"
	                                      "numberOfUsed INTEGER NOT NULL DEFAULT 1)" );
	query.exec("CREATE UNIQUE INDEX word_idx ON searchWords( searchWord )" );

	if ( !query.isActive ())
	    qWarning()<< QObject::tr( "Database Error: " ) + query.lastError ().text ();
    }
    else {
        qDebug() << db.lastError();
        qFatal( DB_NOT_OPEN );
        return false;
    }
    db.close ();
  }

    QSqlDatabase::removeDatabase ( "db" );
    return true;
}

bool Database::deleteDB () {
    return QFile::remove ( DB_PATH );
}

bool Database::setSearchWord( QString word = "" ) {
// bracket are for the correct way to work with databases
  {
    QSqlDatabase db( QSqlDatabase::addDatabase( "QSQLITE", "db" ));
    db.setDatabaseName ( DB_PATH );

    if( db.open ()) {
	QSqlQuery query( db );
            query.prepare ( "INSERT INTO searchWords ( searchWord ) VALUES ( :w )" );
            query.bindValue ( ":w", word, QSql::InOut );
            query.exec ();
	    if ( !query.isActive ()){ qWarning()<< QObject::tr( "Database Error: " ) + query.lastError ().text ();}
    }
    else {
        qDebug() << db.lastError();
        qFatal( DB_NOT_OPEN );
	return false;
    }

    db.close ();
  }
    QSqlDatabase::removeDatabase ( "db" );
    return true;
}

