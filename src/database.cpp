#include "database.h"
#include <QDebug>

bool Database::openDB () {
    // Find QSLite driver
    QSqlDatabase db = QSqlDatabase::addDatabase ( "QSQLITE" );

#ifdef Q_OS_LINUX
    QString path( QDir::home ().path () + "/.config/" + QCoreApplication::applicationName () + "/" + QCoreApplication::applicationName () + ".db" );
    if( QFile::exists ( path ))
        return true;
    // NOTE: We have to store database file into user home folder in Linux
    db.setDatabaseName( path );
#else
    if( QFile::exists ( QCoreApplication::applicationName () + ".db" ))
        return true;
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName( QCoreApplication::applicationName () + ".db" );
#endif
    if ( !db.open ()) {
        qWarning ( "Unable to establish a database connection.\n" );
        return false;
    }

    QSqlQuery query;
    query.exec( "create table lineEditComplete (id int primary key, seachWord varchar(20), numberOfUsed int)" );

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
