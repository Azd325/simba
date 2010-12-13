#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>

#include <QWebView>
#include <QProgressBar>
#include <QDialog>
#include <QTextEdit>
#include <QMessageBox>
#include <QTabWidget>

#include <QLabel>
#include <QLineEdit>
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
    ~MainWindow();

private:
    void createBars();
    void createActions();
    void createSystemTray();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void search();
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

    QGridLayout *qglDialog;
    QDialog *dialog;
    QClipboard *clipboard;
    QLineEdit *qleSearch;
    QSystemTrayIcon *tray;
    QString para,
        url;
};

#endif // MAINWINDOW_H

