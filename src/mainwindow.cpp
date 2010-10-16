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
        connect( view, SIGNAL( loadStarted()), this, SLOT( startedLoad()));
        connect( view, SIGNAL( loadProgress( int )), this, SLOT( progressLoad( int )));
        connect( view, SIGNAL( loadFinished( bool )), this, SLOT( finishedLoad( bool )));
    setCentralWidget( view );
    view->setContextMenuPolicy( Qt::ActionsContextMenu );

    clipboard = QApplication::clipboard();
        connect(clipboard,SIGNAL( selectionChanged()), this, SLOT( clipboardChange()));
                view->addAction( qaAbout );
                view->addAction( qaPreferences );
}

MainWindow::~MainWindow() {
    writeSettings ();
}

void MainWindow::loadSettings() {
    QSettings settings;
    if( settings.contains ( "MainWindow/Size" )) {
        resize(settings.value ( "MainWindow/Size", sizeHint ()).toSize ());
    } else {
        setGeometry (50,50,600,400);
    }
}

void MainWindow::writeSettings() {
    QSettings settings;
    //settings.beginGroup ("MainWindow");
    settings.setValue ( "MainWindow/Size", size());
    //settings.endGroup ();
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
            qmFile->addAction( qaPreferences );
            qmFile->addSeparator();
            qmFile->addAction( qaExit );
        qmbMain->addMenu( qmEdit = new QMenu( tr( "&Edit" )));
            qmEdit->addAction( qaHome );
            qmEdit->addSeparator();
            qmEdit->addAction( qaBack );
            qmEdit->addAction( qaForward );
            qmEdit->addSeparator();
            qmEdit->addAction( qaReload );
            qmEdit->addAction( qaStop );
            qmEdit->addSeparator();
            qmEdit->addAction( qaSearch );
        qmbMain->addMenu( qmView = new QMenu( tr( "&View" )));
            qmView->addAction( qaZoomIn );
            qmView->addAction( qaZoomOut );
            qmView->addAction( qaZoomNormal );
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
        qtbMain->addAction( qaBack );
        qtbMain->addAction( qaReload );
        qtbMain->addAction( qaForward );
        qtbMain->addWidget( qleSearch );
        qtbMain->addAction( qaStop );
        addToolBar( qtbMain );
}

void MainWindow::createActions() {
    qaHome = new QAction( QIcon( ":/home.png" ), tr( "Home" ), this );
        qaHome->setShortcut( Qt::ControlModifier + Qt::Key_H );
        qaHome->setStatusTip( tr( "Click to go home" ));
        connect( qaHome, SIGNAL( triggered()), this, SLOT( goHome()));

    qaExit = new QAction( QIcon( ":/exit.png" ), tr( "&Exit" ), this);
        qaExit->setShortcut( QKeySequence::Quit );
        qaExit->setStatusTip( tr( "Exit the application" ));
        connect( qaExit, SIGNAL( triggered()), qApp, SLOT( quit()));

    qaPreferences = new QAction( QIcon( ":/preferences.png" ), tr( "Preferences" ), this );
        qaPreferences->setShortcut( QKeySequence::Preferences );
        qaPreferences->setStatusTip( tr( "Change preferences of the application" ));
        connect( qaPreferences, SIGNAL( triggered()),this, SLOT( preferences()));

    qaBack = new QAction( QIcon( ":/back.png" ), tr( "Back" ), this);
        qaBack->setShortcut( Qt::ControlModifier + Qt::Key_Left );
        qaBack->setStatusTip(  tr( "Click to go back" ));
        connect( qaBack, SIGNAL( triggered()), this, SLOT( goBack()));

    qaForward = new QAction( QIcon( ":/forward.png" ), tr( "Forward" ), this );
        qaForward->setShortcut( Qt::ControlModifier + Qt::Key_Right );
        qaForward->setStatusTip( tr( "Click to go forward" ));
        connect( qaForward, SIGNAL( triggered()),this, SLOT( goForward()));

    qaStop = new QAction( QIcon( ":/stop.png" ), tr( "Stop" ), this);
        qaStop->setShortcut( Qt::Key_Escape );
        qaStop->setStatusTip( tr( "Click to go stop" ));
        connect( qaStop, SIGNAL( triggered()), this, SLOT( stop()));

    qaReload = new QAction( QIcon( ":/reload.png" ), tr( "Reload" ), this );
        qaReload->setShortcut( QKeySequence::Refresh );
        qaReload->setStatusTip( tr( "Click to go reload" ));
        connect( qaReload, SIGNAL( triggered()), this, SLOT( reload()));

     qaAbout = new QAction( QIcon( ":/about.png" ), tr( "&About" ), this );
        qaAbout->setStatusTip( tr( "About the application" ));
        connect( qaAbout, SIGNAL( triggered()), this, SLOT( about()));

    qaAboutQt = new QAction( tr( "About &Qt" ), this);
        qaAboutQt->setStatusTip( tr( "About the Qt version" ));
        connect( qaAboutQt, SIGNAL( triggered()), qApp, SLOT( aboutQt()));

    qaSearch = new QAction( QIcon( ":/search.png" ), tr( "Search" ), this );
        qaSearch->setShortcut( QKeySequence::Find );
        qaSearch->setStatusTip( tr( "Search words" ));
        connect( qaSearch, SIGNAL( triggered()), this, SLOT( search()));

    qaPrintDialog = new QAction( QIcon( ":/print.png" ), tr( "Print" ), this );
        qaPrintDialog->setShortcut( QKeySequence::Print );
        qaPrintDialog->setStatusTip( tr( "Print Preview" ));
        connect( qaPrintDialog, SIGNAL( triggered()), this, SLOT( printpreview()));

    qaZoomIn = new QAction( QIcon( ":/zoom-in.png" ), tr( "Zoom &In" ), this );
        qaZoomIn->setShortcut( QKeySequence::ZoomIn );
        qaZoomIn->setStatusTip( tr( "Zoom in of the page" ));
        connect( qaZoomIn, SIGNAL( triggered()), this, SLOT( zoomIn()));

    qaZoomOut = new QAction( QIcon( ":/zoom-out.png" ), tr( "Zoom &Out" ), this );
        qaZoomOut->setShortcut( QKeySequence::ZoomOut );
        qaZoomOut->setStatusTip( tr( "Zoom out of the page" ));
        connect( qaZoomOut, SIGNAL( triggered()), this, SLOT( zoomOut()));

   qaZoomNormal = new QAction( QIcon( ":/zoom-normal.png" ), tr( "Zoom &Normal" ), this );
        qaZoomNormal->setShortcut( QKeySequence( "CTRL+0" ));
        qaZoomNormal->setStatusTip( tr( "Zoom normal of the page" ));
        connect( qaZoomNormal, SIGNAL( triggered()), this, SLOT( zoomNormal()));
}

void MainWindow::createSystemTray() {
    tray = new QSystemTrayIcon( QIcon( ":/tray.png" ));
    tray->setContextMenu( qmTray = new QMenu );
        qmTray->addAction( qaShow = new QAction( tr( "Show" ), this ));
            connect( qaShow, SIGNAL( triggered() ), this, SLOT( show() ));
        qmTray->addSeparator();
        qmTray->addAction( qaPreferences );
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
    view->load( QUrl( url + loadINI() + clipboard->text());
#endif
}

void MainWindow::changeLanguage( int value ) {
    // the language part of the url
    switch( value ){
    case 0:
        writeINI( "?lp=ende&search=" );
        break;
    case 1:
        writeINI( "?lp=frde&search=" );
        break;
    case 2:
        writeINI( "?lp=esde&search=" );
        break;
    case 3:
        writeINI( "?lp=itde&search=" );
        break;
    case 4:
        writeINI( "?lp=chde&search=" );
        break;
    case 5:
        writeINI( "?lp=rude&search=" );
        break;
    }
}

void MainWindow::writeINI( QString language ) {
    QFile file( "simba.ini" );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text )){
        writeINI( url + "ende?lp=ende&search=" );
    }

    QTextStream out( &file );
    out << language<< "\n";
}

QString MainWindow::loadINI() {
    QFile file( "simba.ini" );
        if ( !file.open( QIODevice::ReadOnly | QIODevice::Text )){
            writeINI( "ende?lp=ende&search=" );
            return "ende?lp=ende&search=" ;
        }

    QTextStream in( &file );

    return in.readLine();
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

void MainWindow::preferences() {
    dialog = new QDialog;
        dialog->setFixedSize( 225, 100 );

    qcbLanguage = new QComboBox;
        qcbLanguage->addItem( QIcon( ":/gb.png" ), tr( "English" ));
        qcbLanguage->addItem( QIcon( ":/fr.png" ), tr( "French" ));
        qcbLanguage->addItem( QIcon( ":/es.png" ), tr( "Spanish" ));
        qcbLanguage->addItem( QIcon( ":/it.png" ), tr( "Italian" ));
        qcbLanguage->addItem( QIcon( ":/cn.png" ), tr( "Chinese" ));
        qcbLanguage->addItem( QIcon( ":/ru.png" ), tr( "Russian" ));
        if (dialog->Accepted) {
            connect( qcbLanguage, SIGNAL( currentIndexChanged( int )), this, SLOT( changeLanguage( int )));
            goHome();
        }

        QPushButton *qpbOk = new QPushButton( QIcon( ":/ok.png" ), tr( "&OK" ));
            connect( qpbOk, SIGNAL( clicked()), dialog, SLOT( accept()));

        QPushButton *qpbCancel = new QPushButton( QIcon( ":/cancel.png" ), tr( "&Cancel" ));
            connect( qpbCancel, SIGNAL( clicked()), dialog, SLOT( reject()));

    qglDialog = new QGridLayout;
    qglDialog->addWidget( new QLabel( tr( "Language:" )), 0, 0 );
        qglDialog->addWidget( qcbLanguage, 0, 1 );
        qglDialog->addWidget( qpbOk, 1, 0 );
        qglDialog->addWidget( qpbCancel, 1, 1 );

    dialog->setLayout( qglDialog );


    dialog->exec();
}

/* about functions*/

void MainWindow::about() {
    dialog = new QDialog;

    QPushButton *qpbClose = new QPushButton( QIcon( ":/cancel.png"), tr( "&Close" ));
        qpbClose->setFixedSize( 93, 34 );
        connect( qpbClose, SIGNAL( clicked()), dialog, SLOT( close()));
    QPushButton *qpbCredits = new QPushButton( QIcon( ":/about.png"), tr( "C&redits" ));
        qpbCredits->setFixedSize( 93, 34 );
        connect( qpbCredits, SIGNAL( clicked()), this, SLOT( aboutCredits()));
    QPushButton *qpbLicense = new QPushButton( tr( "&License" ));
        qpbLicense->setFixedSize( 93, 34 );
        connect( qpbLicense, SIGNAL( clicked()), this, SLOT( aboutLicense()));

    qglDialog = new QGridLayout;
        qglDialog->addWidget( new QLabel( "<center><h1>" +QCoreApplication::applicationName () + " "
                                         + QCoreApplication::applicationVersion () + "</h1><h3>" +
                                         tr("For better use of leo.org") + "</h3>Copyright \251 2010 "+
                                         QCoreApplication::organizationName () + "</center>"), 0, 0, 1, 3);
        qglDialog->addWidget( qpbCredits, 1, 0, Qt::AlignCenter );
        qglDialog->addWidget( qpbLicense, 1, 1, Qt::AlignCenter );
        qglDialog->addWidget( qpbClose, 1, 2, Qt::AlignCenter );

        dialog->setWindowTitle( tr( "About " ) + QCoreApplication::applicationName ());
        dialog->setLayout( qglDialog );
        dialog->setFixedSize( 312, 156 );
        dialog->exec();
}

void MainWindow::aboutLicense() {
    dialog = new QDialog;

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
        connect(qpbClose, SIGNAL( clicked()), dialog, SLOT( close()));

    qglDialog = new QGridLayout;
        qglDialog->addWidget( qteLicense, 0, 0 );
        qglDialog->addWidget( qpbClose, 1, 0, Qt::AlignRight );

        dialog->setLayout( qglDialog);
        dialog->setWindowTitle( tr( "GNU General Public License" ));
        dialog->setFixedSize( 550, 400 );
        dialog->exec();
}

void MainWindow::aboutCredits() {
    dialog = new QDialog;

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

    qglDialog = new QGridLayout;
    qglDialog->addWidget( qtwCredits, 0, 0,Qt::AlignCenter );
        qglDialog->addWidget( qpbClose, 1, 0, Qt::AlignRight );

        dialog->setWindowTitle( tr( "Credits" ));
        dialog->setLayout( qglDialog );
        dialog->setFixedSize( 250, 200 );
        dialog->exec();
}

/* toolbar actions functions */

void MainWindow::search() {
    qleSearch->setFocus();
}

void MainWindow::goBack() {
    view->back();
}

void MainWindow::goForward() {
    view->forward();
}

void MainWindow::goHome() {
    view->load( QUrl( url + loadINI()));
}

void MainWindow::reload() {
    view->reload();
}

void MainWindow::stop() {
    view->stop();
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

/* zoom funcctions */

void MainWindow::zoomIn() {
    view->setZoomFactor( view->zoomFactor() + 0.1 );
}

void MainWindow::zoomOut() {
    view->setZoomFactor( view->zoomFactor() - 0.1 );
}

void MainWindow::zoomNormal() {
    view->setZoomFactor( 1 );
}
