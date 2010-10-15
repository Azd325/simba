#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMenu>
#include <QAction>

#include <QWebView>
#include <QProgressBar>
#include <QDialog>
#include <QTextEdit>
#include <QMessageBox>
#include <QTabWidget>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>

#include <QClipboard>
#include <QApplication>
#include <QSystemTrayIcon>

#include <QSettings>

#include <QCloseEvent>

#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QResource>

#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QStringList, QWidget *parent = 0);

private:
    void createBars();
    void createActions();
    void createSystemTray();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void goBack();
    void goForward();
    void goHome();
    void reload();
    void stop();
    void search();
    void preferences();
    void about();
    void aboutLicense();
    void aboutCredits();
    void startedLoad();
    void progressLoad(int);
    void finishedLoad(bool);
    void changeLanguage(int);
    void clipboardChange();
    void lineSearch();
    void trayActivate(QSystemTrayIcon::ActivationReason);
    void writeINI(QString);
    QString loadINI();
    void printpreview();
    void zoomIn();
    void zoomOut();
    void zoomNormal();

private:
    QMenuBar *qmbMain;
    QStatusBar *qsbMain;
    QToolBar *qtbMain;
    QProgressBar *qpbMain;
    QMenu *qmFile,
        *qmEdit,
        *qmView,
        *qmHelp,
        *qmTray;
    QAction *qaExit,
        *qaPreferences,
        *qaReload,
        *qaBack,
        *qaHome,
        *qaForward,
        *qaStop,
        *qaAbout,
        *qaAboutQt,
        *qaShow,
        *qaSearch,
        *qaPrintDialog,
        *qaZoomIn,
        *qaZoomOut,
        *qaZoomNormal;
    QWebView *view;

    QComboBox *qcbLanguage;
    QGridLayout *qglDialog;
    QDialog *dialog;
    QClipboard *clipboard;
    QLineEdit *qleSearch;
    QSystemTrayIcon *tray;
    QString para,
        url;
};

#endif // MAINWINDOW_H
