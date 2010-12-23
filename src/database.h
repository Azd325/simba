#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QDir>

class Database : public QObject
{
public:
    public:
        bool openDB();
        bool deleteDB();
        QSqlError lastError();
 
    private:
        QSqlDatabase db;
};

#endif // DATABASE_H
