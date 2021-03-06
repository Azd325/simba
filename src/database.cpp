/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include "database.h"

Database::Database() {
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
        db.setDatabaseName( DB_PATH ); 

    if( !db.open() )
        qWarning() << db.lastError () << "\n" << DB_NOT_OPEN;

    openDB();
}

void Database::openDB () {
// bracket are for the correct way to work with databases. its fix problems
  {
    // create table with  index
    QSqlQuery query;
        query.prepare ( "CREATE TABLE IF NOT EXISTS searchWords (id INTEGER PRIMARY KEY, "
                       "searchWord VARCHAR(20) NOT NULL, numberOfUsed INTEGER NOT NULL DEFAULT 1)");
        if ( !query.exec ())
            qWarning()<< QObject::tr( "Database Error: " ) << query.lastError ();

        query.prepare ( "CREATE UNIQUE INDEX word_idx ON searchWords( searchWord )" );
        if ( !query.exec())
            qWarning()<< QObject::tr( "Database Error: " ) << query.lastError ();
  }
}

bool Database::deleteDB () {
    return QFile::remove ( DB_PATH );
}

void Database::setSearchWord( QString word ) {
 {
    // insert the searchword in the db
    QSqlQuery query;
        query.prepare ( "INSERT OR IGNORE INTO searchWords ( searchWord ) VALUES ( :w )" );
        query.bindValue ( ":w", word );
        if ( !query.exec ())
            qWarning()<< QObject::tr( "Database Error: " ) << query.lastError ();
  }
}

