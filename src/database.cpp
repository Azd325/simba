#include "database.h"

QSqlDatabase Database::db;

bool Database::openDB () {
    // Find QSLite driver

    db = QSqlDatabase::database ("connection", false);
    db.open ();
    QSqlQuery query;
    query.prepare( "create table lineEditComplete (id int primary key, seachWord varchar(20), numberOfUsed int)" );

#ifdef Q_OS_LINUX
    QString path( QDir::home ().path () + "/.config/" + QCoreApplication::applicationName () + "/" + QCoreApplication::applicationName () + ".db" );
    if( !QFile::exists ( path ))
        query.exec();
    // NOTE: We have to store database file into user home folder in Linux
    db.setDatabaseName( path );
#else
    if( !QFile::exists ( QCoreApplication::applicationName () + ".db" ))
        query.exec();
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName( QCoreApplication::applicationName () + ".db" );
#endif
    if ( !db.open ()) {
        qWarning ( "Unable to establish a database connection." );
        return false;
    }
    return true;
}

bool Database::deleteDB () {
#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    return QFile::remove ( QDir::home ().path () + "/.config/" + QCoreApplication::applicationName () + "/" + QCoreApplication::applicationName () + ".db" );
#else
    // Remove created database binary file
    return QFile::remove ( QCoreApplication::applicationName () + ".db" );
#endif
}
