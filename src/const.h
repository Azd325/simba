#ifndef CONST_H
#define CONST_H

#include <QString>
#include <QDir>

// Simba Infos
const QString APP_NAME = "Simba";
const QString APP_VERSION = "0.95";
const QString APP_URL = "azd325.github.com/simba/";

// Simba MainWindow
const QString URL = "http://pda.leo.org/";
const QString EN = "?lp=ende&search=";
const QString ES = "?lp=esde&search=";
const QString FR = "?lp=frde&search=";
const QString IT = "?lp=itde&search=";
const QString CN = "?lp=chde&search=";
const QString RU = "?lp=rude&search=";

// Database Constants
#ifdef Q_OS_LINUX
const QString DB_PATH = QDir::home ().path () + "/.config/" + APP_NAME + "/" + APP_NAME + ".db";
#else
const QString DB_PATH = APP_NAME + ".db";
#endif // Q_OS_LINUX

const QString DB_NOT_OPEN = "Failed to connect. Cannot open database.";

#endif // CONST_H
