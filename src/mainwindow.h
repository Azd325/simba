/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QProgressBar>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QPrintPreviewDialog>
#include <QtGui/QPrinter>
#include <QtGui/QTabWidget>
#include <QtGui/QClipboard>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QCloseEvent>

#include <QtWebKit/QWebView>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QSettings>
#include <QtCore/QResource>
#include <QtCore/QDebug>

#include "iconloader.h"
#include "database.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QStringList, QWidget *parent = 0);
    ~MainWindow();

private:
    void createBars();
    void createActions();
    void createSystemTray();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void search();
    void clearSearch();
    void about();
    void aboutLicense();
    void aboutCredits();
    void startedLoad();
    void progressLoad(int);
    void finishedLoad(bool);
    void clipboardChange();
    void lineSearch();
    void trayActivate(QSystemTrayIcon::ActivationReason);
    QString loadINI();
    void printpreview();
    void loadSettings();
    void writeSettings();
    void languageActionTriggered( QAction* );
    void zoomActionTriggered( QAction* );
    void navigationActionTriggered( QAction* );

private:
    QMenuBar *qmbMain;
    QStatusBar *qsbMain;
    QToolBar *qtbMain;
    QProgressBar *qpbMain;
    QMenu *qmFile,
        *qmEdit,
        *qmSubEdit,
        *qmView,
        *qmHelp,
        *qmTray;
    QActionGroup *qagLanguages,
        *qagZoom,
        *qagNavigation;
    QAction *qaExit,
        *qaReload,
        *qaBack,
        *qaHome,
        *qaForward,
        *qaStop,
        *qaAbout,
        *qaAboutQt,
        *qaShow,
        *qaSearch,
	*qaClearSearch,
        *qaPrintDialog,
        *qaZoomIn,
        *qaZoomOut,
        *qaZoomNormal,
        *qaEnglish,
        *qaFrench,
        *qaSpanish,
        *qaItalian,
        *qaChinese,
        *qaRussian;
    QWebView *view;
    QToolButton *qtbDeleteSearch;

    QGridLayout *qglDialog;
    QClipboard *clipboard;
    QLineEdit *qleSearch;
    QSystemTrayIcon *tray;
    QString para,
        url;
};

#endif // MAINWINDOW_H

