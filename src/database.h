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
        static void openDB();
        static bool deleteDB();
};

#endif // DATABASE_H
