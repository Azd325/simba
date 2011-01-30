/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#ifndef DATABASE_H
#define DATABASE_H

#include <QtGui/QApplication>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql>

#include <QDebug>

class Database
{
    public:
        static bool openDB();
        static bool deleteDB();
	static bool setSearchWord( QString );
	static bool getSearchWord();
};

#endif // DATABASE_H
