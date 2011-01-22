/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

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
