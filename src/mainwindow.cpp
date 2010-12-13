#include "mainwindow.h"

MainWindow::MainWindow( QStringList list, QWidget *parent )
    : QMainWindow( parent ) {
    loadSettings ();

    setWindowTitle( QCoreApplication::applicationName() + tr( " -- For better use of leo.org" ));

    createBars();
    createSystemTray();

    url = "http://pda.leo.org/";

    // Remove the path item of the argv-list
    list.removeFirst();
    para = list.join(" ");

    view = new QWebView;
        view->load( QUrl( url + loadINI() + para ));
        view->setContextMenuPolicy( Qt::ActionsContextMenu );
        view->addAction( qaAbout );
        connect( view, SIGNAL( loadStarted()), this, SLOT( startedLoad()));
        connect( view, SIGNAL( loadProgress( int )), this, SLOT( progressLoad( int )));
        connect( view, SIGNAL( loadFinished( bool )), this, SLOT( finishedLoad( bool )));
    setCentralWidget( view );

    clipboard = QApplication::clipboard();
        connect(clipboard,SIGNAL( selectionChanged()), this, SLOT( clipboardChange()));
}

MainWindow::~MainWindow() {
    writeSettings ();
}

void MainWindow::loadSettings() {
    QSettings settings;

    settings.beginGroup ( "MainWindow" );
    resize(settings.value ( "Size", QSize( 400, 600 )).toSize ());
    move(settings.value ( "Pos", QPoint( 100, 100 )).toPoint ());
    settings.endGroup ();
}

void MainWindow::writeSettings() {
    QSettings settings;

    settings.beginGroup ( "MainWindow" );
    settings.setValue ( "Size", size());
    settings.setValue ( "Pos", pos ());
    settings.endGroup ();
}

void MainWindow::createBars() {
    createActions();

    qpbMain = new QProgressBar;
        qpbMain->setMaximumSize( 150, 25 );
        qpbMain->setRange( 0, 99 );
        qpbMain->setTextVisible( 0 );

    setStatusBar(qsbMain = new QStatusBar);
        qsbMain->showMessage( tr( "Ready" ));
        qsbMain->addPermanentWidget( qpbMain );

    setMenuBar( qmbMain = new QMenuBar );
        qmbMain->addMenu( qmFile = new QMenu( tr( "&File" )));
            qmFile->addSeparator();
            qmFile->addAction( qaPrintDialog );
            qmFile->addSeparator();
            qmFile->addAction( qaExit );
        qmbMain->addMenu( qmEdit = new QMenu( tr( "&Edit" )));
            qmEdit->addActions( qagNavigation->actions ());
            qmEdit->addAction( qaSearch );
            qmEdit->addMenu (qmSubEdit = new QMenu( tr( "Languages" )));
                qmSubEdit->addActions (qagLanguages->actions ());
        qmbMain->addMenu( qmView = new QMenu( tr( "&View" )));
            qmView->addActions ( qagZoom->actions ());
        qmbMain->addMenu( qmHelp = new QMenu( tr( "&Help" )));
            qmHelp->addAction( qaAboutQt );
            qmHelp->addSeparator();
            qmHelp->addAction( qaAbout );

    qleSearch = new QLineEdit;
        connect(qleSearch, SIGNAL( returnPressed()), this, SLOT( lineSearch()));
#if ( QT_VERSION >= 0x040700 )
        qleSearch->setPlaceholderText( "Search" );
#endif

    qtbMain = new QToolBar( "Toolbar" );
        qtbMain->addAction( qaHome );
        qtbMain->addSeparator();
        qtbMain->addActions( qagNavigation->actions ());
        qtbMain->addWidget( qleSearch );
        addToolBar( qtbMain );
}

void MainWindow::createActions() {
    qaHome = new QAction( QIcon( ":/home.png" ), tr( "Home" ), this );
        qaHome->setShortcut( Qt::ControlModifier + Qt::Key_H );
        qaHome->setStatusTip( tr( "Click to go home" ));

    qaExit = new QAction( QIcon( ":/exit.png" ), tr( "&Exit" ), this);
        qaExit->setShortcut( QKeySequence::Quit );
        qaExit->setStatusTip( tr( "Exit the application" ));
        connect( qaExit, SIGNAL( triggered()), qApp, SLOT( quit()));

    qaBack = new QAction( QIcon( ":/back.png" ), tr( "Back" ), this);
        qaBack->setShortcut( Qt::ControlModifier + Qt::Key_Left );
        qaBack->setStatusTip(  tr( "Click to go back" ));

    qaForward = new QAction( QIcon( ":/forward.png" ), tr( "Forward" ), this );
        qaForward->setShortcut( Qt::ControlModifier + Qt::Key_Right );
        qaForward->setStatusTip( tr( "Click to go forward" ));

    qaStop = new QAction( QIcon( ":/stop.png" ), tr( "Stop" ), this);
        qaStop->setShortcut( Qt::Key_Escape );
        qaStop->setStatusTip( tr( "Click to go stop" ));

    qaReload = new QAction( QIcon( ":/reload.png" ), tr( "Reload" ), this );
        qaReload->setShortcut( QKeySequence::Refresh );
        qaReload->setStatusTip( tr( "Click to go reload" ));

     qaAbout = new QAction( QIcon( ":/about.png" ), tr( "&About" ), this );
        qaAbout->setStatusTip( tr( "About the application" ));
        connect( qaAbout, SIGNAL( triggered()), this, SLOT( about()));

    qaAboutQt = new QAction( tr( "About &Qt" ), this);
        qaAboutQt->setStatusTip( tr( "About the Qt version" ));
        connect( qaAboutQt, SIGNAL( triggered()), qApp, SLOT( aboutQt()));

    qagNavigation = new QActionGroup( this );
        qagNavigation->addAction (qaHome);
        qagNavigation->addAction ( qaBack );
        qagNavigation->addAction ( qaReload );
        qagNavigation->addAction ( qaForward );
        qagNavigation->addAction ( qaStop );
        connect (qagNavigation, SIGNAL(triggered(QAction*)), this, SLOT(navigationActionTriggered(QAction*)));

    qaSearch = new QAction( QIcon( ":/search.png" ), tr( "Search" ), this );
        qaSearch->setShortcut( QKeySequence::Find );
        qaSearch->setStatusTip( tr( "Search words" ));
        connect( qaSearch, SIGNAL( triggered()), this, SLOT(search()));

    qaPrintDialog = new QAction( QIcon( ":/print.png" ), tr( "Print" ), this );
        qaPrintDialog->setShortcut( QKeySequence::Print );
        qaPrintDialog->setStatusTip( tr( "Print Preview" ));
        connect( qaPrintDialog, SIGNAL( triggered()), this, SLOT( printpreview()));

    qaZoomIn = new QAction( QIcon( ":/zoom-in.png" ), tr( "Zoom &In" ), this );
        qaZoomIn->setShortcut( QKeySequence::ZoomIn );
        qaZoomIn->setStatusTip( tr( "Zoom in of the page" ));

    qaZoomOut = new QAction( QIcon( ":/zoom-out.png" ), tr( "Zoom &Out" ), this );
        qaZoomOut->setShortcut( QKeySequence::ZoomOut );
        qaZoomOut->setStatusTip( tr( "Zoom out of the page" ));

   qaZoomNormal = new QAction( QIcon( ":/zoom-normal.png" ), tr( "Zoom &Normal" ), this );
        qaZoomNormal->setShortcut( QKeySequence( "CTRL+0" ));
        qaZoomNormal->setStatusTip( tr( "Zoom normal of the page" ));

    qagZoom = new QActionGroup(this);
        qagZoom->addAction (qaZoomIn);
        qagZoom->addAction (qaZoomNormal);
        qagZoom->addAction (qaZoomOut);
        connect (qagZoom,SIGNAL(triggered(QAction*)),this,SLOT( zoomActionTriggered( QAction* )));

    qaEnglish = new QAction( QIcon( ":/gb.png" ), tr( "English" ), this );
        qaEnglish->setCheckable(true);
    qaSpanish = new QAction( QIcon( ":/es.png" ), tr( "Spanish" ), this );
        qaSpanish->setCheckable(true);
    qaFrench = new QAction( QIcon( ":/fr.png" ), tr( "French" ), this );
        qaFrench->setCheckable(true);
    qaItalian = new QAction( QIcon( ":/it.png" ), tr( "Italian" ), this );
            qaItalian->setCheckable(true);
    qaChinese = new QAction( QIcon( ":/cn.png" ), tr( "Chinese" ), this );
        qaChinese->setCheckable(true);
    qaRussian = new QAction( QIcon( ":/ru.png" ), tr( "Russian" ), this );
            qaRussian->setCheckable(true);

    qagLanguages = new QActionGroup(this);
        qagLanguages->addAction ( qaEnglish );
        qagLanguages->addAction ( qaSpanish );
        qagLanguages->addAction ( qaFrench );
        qagLanguages->addAction ( qaItalian );
        qagLanguages->addAction ( qaChinese );
        qagLanguages->addAction ( qaRussian );
        connect( qagLanguages, SIGNAL( triggered( QAction* )), this, SLOT( languageActionTriggered( QAction* )));

}

void MainWindow::languageActionTriggered( QAction *action ) {
    QSettings settings;
    if( action == qaEnglish ){
        settings.setValue ( "Simba/Language", "?lp=ende&search=" );
    }
    else if( action == qaSpanish ) {
        settings.setValue ( "Simba/Language", "?lp=esde&search=" );
    }
    else if( action == qaFrench ) {
        settings.setValue ( "Simba/Language", "?lp=frde&search=" );
    }
    else if( action == qaItalian ) {
        settings.setValue ( "Simba/Language", "?lp=itde&search=" );
    }
    else if( action == qaChinese ) {
        settings.setValue ( "Simba/Language", "?lp=chde&search=" );
    }
    else if( action == qaRussian ) {
        settings.setValue ( "Simba/Language", "?lp=rude&search=" );
    }
}
void MainWindow::zoomActionTriggered( QAction* action ) {
    if( action == qaZoomIn ) {
        view->setZoomFactor( view->zoomFactor() + 0.1 );
    }
    else if( action == qaZoomNormal ) {
        view->setZoomFactor( 1 );
    }
    else if( action == qaZoomOut ) {
        view->setZoomFactor( view->zoomFactor() - 0.1 );
    }
}

void MainWindow::navigationActionTriggered ( QAction *action ) {
    if( action == qaHome ) {
        view->load( QUrl( url + loadINI()));
    }
    else if( action == qaBack ) {
        view->back ();
    }
    else if( action == qaForward ) {
        view->forward ();
    }
    else if( action == qaReload ) {
        view->reload ();
    }
    else if( action == qaStop ) {
        view->stop ();
    }
}

void MainWindow::search() {
    qleSearch->setFocus();
}

void MainWindow::createSystemTray() {
    tray = new QSystemTrayIcon( QIcon( ":/tray.png" ));
    tray->setContextMenu( qmTray = new QMenu );
        qmTray->addAction( qaShow = new QAction( tr( "Show" ), this ));
            connect( qaShow, SIGNAL( triggered() ), this, SLOT( show() ));
        qmTray->addSeparator();
        qmTray->addAction( qaExit );
    connect( tray, SIGNAL( activated( QSystemTrayIcon::ActivationReason )), this,
            SLOT( trayActivate( QSystemTrayIcon::ActivationReason )));
    tray->show();
}

void MainWindow::trayActivate( QSystemTrayIcon::ActivationReason reason ) {
    if( reason == QSystemTrayIcon::DoubleClick ){
        if( !isHidden() ){
            hide();
            tray->showMessage( "INFO","The application is minimize to the tray", QSystemTrayIcon::Information, 10000 );
        }
        else{
            show();
        }
    }

}

void MainWindow::closeEvent( QCloseEvent *event ) {
    if ( !this->isHidden() ) {
        this->hide();
        tray->showMessage( "INFO","The application is minimize to the tray", QSystemTrayIcon::Information, 10000 );
        event->ignore();
    }
}

void MainWindow::lineSearch() {
    view->load( QUrl( url + loadINI() + qleSearch->text()));
}

void MainWindow::clipboardChange() {
#ifdef Q_WS_X11
    view->load( QUrl( url + loadINI() + clipboard->text( QClipboard::Selection )));
#elif Q_OS_WIN32
    view->load( QUrl( url + loadINI() + clipboard->text( QClipboard::Clipboard )));
#elif Q_OS_MAC
    view->load( QUrl( url + loadINI() + clipboard->text( QClipboard::FindBuffer )));
#else
    view->load( QUrl( url + loadINI() + clipboard->text()));
#endif
}

QString MainWindow::loadINI() {
    QSettings settings;
    return settings.value ( "Simba/Language", "?lp=ende&search=" ).toString ();
}

void MainWindow::printpreview() {
    QPrintPreviewDialog dialog;
    connect( &dialog, SIGNAL( paintRequested( QPrinter* )), view, SLOT( print( QPrinter* )));
    dialog.showMaximized();
    if( dialog.exec() == QDialog::Accepted ) {
        QMessageBox msg;
            msg.setIcon( QMessageBox::Information );
            msg.setWindowTitle( "Information" );
            msg.setText( "Print was successful!" );
            msg.exec();
    }
}

/* about functions*/

void MainWindow::about() {
    dialog = new QDialog( this );

    QPushButton *qpbClose = new QPushButton( QIcon( ":/cancel.png"), tr( "&Close" ));
        qpbClose->setFixedSize( 93, 34 );
        connect( qpbClose, SIGNAL( clicked()), dialog, SLOT( deleteLater()));
    QPushButton *qpbCredits = new QPushButton( QIcon( ":/about.png"), tr( "C&redits" ));
        qpbCredits->setFixedSize( 93, 34 );
        connect( qpbCredits, SIGNAL( clicked()), this, SLOT( aboutCredits()));
    QPushButton *qpbLicense = new QPushButton( tr( "&License" ));
        qpbLicense->setFixedSize( 93, 34 );
        connect( qpbLicense, SIGNAL( clicked()), this, SLOT( aboutLicense()));

    qglDialog = new QGridLayout(dialog);
        qglDialog->addWidget( new QLabel( "<center><h1>" +QCoreApplication::applicationName ()
                                         + " " + QCoreApplication::applicationVersion () + "</h1><h3>"
                                         + tr("For better use of leo.org") + "</h3>Copyright \251 2010 "
                                         + QCoreApplication::organizationName () + "</center>"), 0, 0, 1, 3);
        qglDialog->addWidget( qpbCredits, 1, 0, Qt::AlignCenter );
        qglDialog->addWidget( qpbLicense, 1, 1, Qt::AlignCenter );
        qglDialog->addWidget( qpbClose, 1, 2, Qt::AlignCenter );

        dialog->setWindowTitle( tr( "About " ) + QCoreApplication::applicationName ());
        dialog->setLayout( qglDialog );
        dialog->setFixedSize( 312, 156 );
        dialog->exec();
}

void MainWindow::aboutLicense() {
    dialog = new QDialog( this );

    QFile file( ":/GPL" );
    if(!file.open( QIODevice::ReadOnly | QIODevice::Text )) {
        qCritical( "GPL LicenseFile not found" );
    }
    QTextStream out( &file );
    out.setFieldAlignment( QTextStream::AlignCenter );

    QTextEdit *qteLicense = new QTextEdit;
        qteLicense->setText( out.readAll());
        qteLicense->setReadOnly( 1 );
    QPushButton *qpbClose = new QPushButton( QIcon( ":/cancel.png" ), tr( "&Close" ));
        connect(qpbClose, SIGNAL( clicked()), dialog, SLOT( deleteLater()));

    qglDialog = new QGridLayout( dialog );
        qglDialog->addWidget( qteLicense, 0, 0 );
        qglDialog->addWidget( qpbClose, 1, 0, Qt::AlignRight );

        dialog->setLayout( qglDialog);
        dialog->setWindowTitle( tr( "GNU General Public License" ));
        dialog->setFixedSize( 550, 400 );
        dialog->exec();
}

void MainWindow::aboutCredits() {
    dialog = new QDialog( this );

    QTextEdit *qteCreditsWritten = new QTextEdit;
        qteCreditsWritten->setReadOnly( 1 );
        qteCreditsWritten->setLineWrapMode( QTextEdit::NoWrap );
        qteCreditsWritten->setText( "Tim Kleinschmidt <tim.kleinschmidt@googlemail.com>" );

    QTextEdit *qteCreditsArtwork = new QTextEdit;
        qteCreditsArtwork->setReadOnly( 1 );
        qteCreditsArtwork->setLineWrapMode( QTextEdit::NoWrap );
        qteCreditsArtwork->setText( "Elementary Theme\n" );

    QTabWidget *qtwCredits = new QTabWidget;
        qtwCredits->addTab( qteCreditsWritten, tr( "Written by" ));
        qtwCredits->addTab( qteCreditsArtwork, tr( "Artwork by" ));
        qtwCredits->setElideMode( Qt::ElideRight );

    QPushButton *qpbClose = new QPushButton( QIcon( ":/cancel.png"), tr( "&Close" ));
        connect( qpbClose, SIGNAL( clicked()), dialog, SLOT( close()));

    qglDialog = new QGridLayout( dialog );
    qglDialog->addWidget( qtwCredits, 0, 0,Qt::AlignCenter );
        qglDialog->addWidget( qpbClose, 1, 0, Qt::AlignRight );

        dialog->setWindowTitle( tr( "Credits" ));
        dialog->setLayout( qglDialog );
        dialog->setFixedSize( 250, 200 );
        dialog->exec();
}

/* progressbar functions */

void MainWindow::startedLoad() {
    qpbMain->show();
}

void MainWindow::progressLoad( int i ) {
    qpbMain->setValue( i );
}

void MainWindow::finishedLoad( bool value ) {
    if( value == false ) {
        qpbMain->hide();
        QResource res( ":/noLeo.html" );
        QByteArray html(( const char* )res.data(), res.size());
        view->setHtml( html );
    }else {
        qpbMain->hide();
    }
}
