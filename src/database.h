#ifndef DATABASE_H
#define DATABASE_H

#include <QtGui/QApplication>

#include <QSqlDatabase>
#include <QFile>
#include <QDir>
#include <QSqlQuery>

class Database
{
    public:
        static bool openDB();
//        static bool deleteDB();
};

#endif // DATABASE_H
