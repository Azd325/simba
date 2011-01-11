#ifndef DATABASE_H
#define DATABASE_H

#include <QtGui/QApplication>
#include <QtSql/QSqlDatabase>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtSql/QSqlQuery>

class Database
{
    public:
        static bool openDB();
        static bool deleteDB();
        static QSqlDatabase db;
};

#endif // DATABASE_H
