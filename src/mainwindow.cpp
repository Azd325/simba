/* -*- codingr utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include "mainwindow.h"

MainWindow::MainWindow( QStringList list, QWidget *parent )
    : QMainWindow( parent ) {
    loadSettings ();

    setWindowTitle( tr( "%1 -- For better use of leo.org" ).arg( APP_NAME ));

    createBars();
    createSystemTray();

    // Remove the path item of the argv-list
    list.removeFirst();
    para = list.join(" ");

    view = new QWebView;
        view->load( QString( "%1%2%3" ).arg( URL ).arg( loadINI ()).arg( para ));
    setCentralWidget( view );

    connect( view, SIGNAL( loadStarted ()), this, SLOT( show ()));
    connect( view, SIGNAL( loadProgress ( int )), this, SLOT( progressLoad ( int )));
    connect( view, SIGNAL( loadFinished ( bool )), this, SLOT( finishedLoad ( bool )));

    clipboard = QApplication::clipboard ();
        connect(clipboard,SIGNAL( changed( QClipboard::Mode )), this, SLOT( clipboardChange ()));
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
        qpbMain->setMaximumSize( 150, 15 );
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

    qtbDeleteSearch = new QToolButton(this);
        qtbDeleteSearch->setDefaultAction( qaClearSearch );
        qtbDeleteSearch->setToolTip( "Clear" );
        qtbDeleteSearch->setFocusPolicy( Qt::NoFocus );

    qleSearch = new QLineEdit;
        connect( qleSearch, SIGNAL( returnPressed ()), this, SLOT( lineSearch ()));
        connect( qleSearch, SIGNAL( returnPressed ()), this, SLOT( setSearchWord ()));

#if ( QT_VERSION >= 0x040700 )
        qleSearch->setPlaceholderText( tr( "Search" ));
#endif

    QSqlTableModel qstm;
        qstm.setTable("searchWords"); // table name
        qstm.removeColumn(0); // remove the id column
        qstm.removeColumn(2); // remove the numberofused column
        qstm.select();

    QCompleter::CompletionMode mode = QCompleter::InlineCompletion; // a new completer mode
    QCompleter *qcSearchWordHelp = new QCompleter(&qstm);
	qcSearchWordHelp->setCompletionMode(mode); // set the mode 
    qleSearch->setCompleter(qcSearchWordHelp);

    qtbMain = new QToolBar( "Toolbar" );
	qtbMain->setFloatable( false );
	qtbMain->setMovable( false );
        qtbMain->addAction( qaHome );
        qtbMain->addSeparator();
        qtbMain->addActions( qagNavigation->actions ());
	qtbMain->addSeparator();
	qtbMain->addWidget( qtbDeleteSearch );
        qtbMain->addWidget( qleSearch );
        addToolBar( qtbMain );
}

void MainWindow::createActions() {
    qaHome = new QAction( IconLoader::Load( "go-home" ), tr( "Home" ), this );
        qaHome->setShortcut( Qt::ControlModifier + Qt::Key_H );
        qaHome->setStatusTip( tr( "Click to go home" ));

    qaExit = new QAction( IconLoader::Load( "system-shutdown" ), tr( "&Exit" ), this);
        qaExit->setShortcut( QKeySequence::Quit );
        qaExit->setStatusTip( tr( "Exit the application" ));
        connect( qaExit, SIGNAL( triggered()), qApp, SLOT( quit()));

    qaBack = new QAction( IconLoader::Load( "go-previous" ), tr( "Back" ), this);
        qaBack->setShortcut( Qt::ControlModifier + Qt::Key_Left );
        qaBack->setStatusTip(  tr( "Click to go back" ));

    qaForward = new QAction( IconLoader::Load( "go-next" ), tr( "Forward" ), this );
        qaForward->setShortcut( Qt::ControlModifier + Qt::Key_Right );
        qaForward->setStatusTip( tr( "Click to go forward" ));

    qaStop = new QAction( IconLoader::Load( "process-stop" ), tr( "Stop" ), this);
        qaStop->setShortcut( Qt::Key_Escape );
        qaStop->setStatusTip( tr( "Click to go stop" ));

    qaReload = new QAction( IconLoader::Load( "view-refresh" ), tr( "Reload" ), this );
        qaReload->setShortcut( QKeySequence::Refresh );
        qaReload->setStatusTip( tr( "Click to go reload" ));

     qaAbout = new QAction( IconLoader::Load( "help-about" ), tr( "&About" ), this );
        qaAbout->setStatusTip( tr( "About the application" ));
        connect( qaAbout, SIGNAL( triggered()), this, SLOT( about()));

    qaAboutQt = new QAction( tr( "About &Qt" ), this);
        qaAboutQt->setStatusTip( tr( "About the Qt version" ));
        connect( qaAboutQt, SIGNAL( triggered()), qApp, SLOT( aboutQt()));

    qagNavigation = new QActionGroup ( this );
        qagNavigation->addAction ( qaHome );
        qagNavigation->addAction ( qaBack );
        qagNavigation->addAction ( qaReload );
        qagNavigation->addAction ( qaForward );
        qagNavigation->addAction ( qaStop );
        connect ( qagNavigation, SIGNAL( triggered( QAction* )), this, SLOT( navigationActionTriggered( QAction* )));

    qaSearch = new QAction ( IconLoader::Load ( "edit-find" ), tr ( "Search" ), this );
        qaSearch->setShortcut ( QKeySequence::Find );
        qaSearch->setStatusTip ( tr( "Search words" ));
        connect( qaSearch, SIGNAL( triggered()), this, SLOT( search()));

    qaClearSearch = new QAction ( IconLoader::Load ( "edit-find" ), "", this );
        qaClearSearch->setStatusTip( tr ( "Clear search words" ));
        connect( qaClearSearch, SIGNAL( triggered()), this, SLOT( clearSearch ()));

    qaPrintDialog = new QAction( IconLoader::Load ( "document-print-preview" ), tr ( "Print" ), this );
        qaPrintDialog->setShortcut( QKeySequence::Print );
        qaPrintDialog->setStatusTip( tr ( "Print Preview" ));
        connect( qaPrintDialog, SIGNAL( triggered ()), this, SLOT( printpreview ()));

    qaZoomIn = new QAction( IconLoader::Load ( "zoom-in" ), tr ( "Zoom &In" ), this );
        qaZoomIn->setShortcut( QKeySequence::ZoomIn );
        qaZoomIn->setStatusTip( tr ( "Zoom in of the page" ));

    qaZoomOut = new QAction( IconLoader::Load( "zoom-out" ), tr ( "Zoom &Out" ), this );
        qaZoomOut->setShortcut( QKeySequence::ZoomOut );
        qaZoomOut->setStatusTip( tr( "Zoom out of the page" ));

   qaZoomNormal = new QAction( IconLoader::Load( "zoom-original" ), tr ( "Zoom &Normal" ), this );
        qaZoomNormal->setShortcut( QKeySequence( "CTRL+0" ));
        qaZoomNormal->setStatusTip( tr ( "Zoom normal of the page" ));

    qagZoom = new QActionGroup( this );
        qagZoom->addAction ( qaZoomIn );
        qagZoom->addAction ( qaZoomNormal );
        qagZoom->addAction ( qaZoomOut );
        connect ( qagZoom, SIGNAL( triggered( QAction* )), this, SLOT( zoomActionTriggered( QAction* )));

    qaEnglish = new QAction( QIcon( ":/flags/en.svg" ), tr( "English" ), this );
        qaEnglish->setCheckable( true );
    qaSpanish = new QAction( QIcon( ":/flags/es.svg" ), tr( "Spanish" ), this );
        qaSpanish->setCheckable( true );
    qaFrench = new QAction( QIcon( ":/flags/fr.svg" ), tr( "French" ), this );
        qaFrench->setCheckable( true );
    qaItalian = new QAction( QIcon( ":/flags/it.svg" ), tr( "Italian" ), this );
        qaItalian->setCheckable( true );
    qaChinese = new QAction( QIcon( ":/flags/cn.svg" ), tr( "Chinese" ), this );
        qaChinese->setCheckable( true );
    qaRussian = new QAction( QIcon( ":/flags/ru.svg" ), tr( "Russian" ), this );
        qaRussian->setCheckable( true );

    qagLanguages = new QActionGroup( this );
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
    settings.beginGroup ( APP_NAME );
    if( action == qaEnglish )
        settings.setValue ( "Language", EN );
    else if( action == qaSpanish )
        settings.setValue ( "Language", ES );
    else if( action == qaFrench )
        settings.setValue ( "Language", FR );
    else if( action == qaItalian )
        settings.setValue ( "Language", IT );
    else if( action == qaChinese )
        settings.setValue ( "Language", CN );
    else if( action == qaRussian )
        settings.setValue ( "Language", RU );
    settings.endGroup ();
}

void MainWindow::zoomActionTriggered( QAction* action ) {
    if( action == qaZoomIn )
        view->setZoomFactor( view->zoomFactor() + 0.1 );
    else if( action == qaZoomNormal )
        view->setZoomFactor( 1 );
    else if( action == qaZoomOut )
        view->setZoomFactor( view->zoomFactor() - 0.1 );
}

void MainWindow::navigationActionTriggered ( QAction *action ) {
    if( action == qaHome )
        view->load( QString("%1%2" ).arg( URL ).arg( loadINI ()));
    else if( action == qaBack )
        view->back ();
    else if( action == qaForward )
        view->forward ();
    else if( action == qaReload )
        view->reload ();
    else if( action == qaStop )
        view->stop ();
}

void MainWindow::search () {
    qleSearch->setFocus ();
}

void MainWindow::clearSearch () {
    qleSearch->clear();
}

void MainWindow::setSearchWord () {
    if(!qleSearch->text ().isEmpty ())
        db.setSearchWord ( qleSearch->text ());
}

void MainWindow::createSystemTray() {
    tray = new QSystemTrayIcon( QIcon( ":/tray.png" ));
    tray->setContextMenu( qmTray = new QMenu );
        qmTray->addAction( qaShow = new QAction( tr( "Show" ), this ));
            connect( qaShow, SIGNAL( triggered() ), this, SLOT( show()));
        qmTray->addSeparator();
        qmTray->addAction( qaExit );
    connect( tray, SIGNAL( activated( QSystemTrayIcon::ActivationReason )), this,
            SLOT( trayActivate( QSystemTrayIcon::ActivationReason )));
    tray->show();
}

void MainWindow::trayActivate ( QSystemTrayIcon::ActivationReason reason ) {
    switch ( reason ) {
        case QSystemTrayIcon::DoubleClick:
            if ( isHidden ())
                show ();
            else
                hide ();
            break;
        default:
            break;
     }

}

void MainWindow::closeEvent( QCloseEvent *event ) {
    if ( isVisible() ) {
        hide();
        tray->showMessage( tr( "INFO "), tr( "The application is minimize to the tray"), QSystemTrayIcon::Information, 10000 );
        event->ignore();
    }
    else
	event->accept();
}

void MainWindow::lineSearch() {
    view->load( QString("%1%2%3" ).arg( URL ).arg( loadINI ()).arg( qleSearch->text()));
}

void MainWindow::clipboardChange() {
    view->load( QString("%1%2%3" ).arg( URL ).arg( loadINI ()).arg( qleSearch->text()));
}

QString MainWindow::loadINI() {
    QSettings settings;
    return settings.value ( APP_NAME + "/Language", EN ).toString ();
}

void MainWindow::printpreview() {
    QPrintPreviewDialog dialog( this );
    connect( &dialog, SIGNAL( paintRequested( QPrinter* )), view, SLOT( print( QPrinter* )));
    dialog.showMaximized();
    if( dialog.exec() == QDialog::Accepted )
        qsbMain->showMessage ( tr( "Print was successful!" ));
    else
        qsbMain->showMessage ( tr( "Print was not successful!" ));
}

/* about functions*/

void MainWindow::about() {
    QDialog *dialog = new QDialog( this );

    QPushButton *qpbClose = new QPushButton( IconLoader::Load( "window-close"), tr( "&Close" ), dialog );
        connect( qpbClose, SIGNAL( clicked()), dialog, SLOT( deleteLater()));
    QPushButton *qpbCredits = new QPushButton( IconLoader::Load( "help-about"), tr( "C&redits" ), dialog );
        connect( qpbCredits, SIGNAL( clicked()), this, SLOT( aboutCredits()));
    QPushButton *qpbLicense = new QPushButton( tr( "&License" ), dialog );
        connect( qpbLicense, SIGNAL( clicked()), this, SLOT( aboutLicense()));

    QLabel *qlAbout = new QLabel( dialog );
        qlAbout->setText( tr("<center><h1>%1 %2</h1><h3>For better use of leo.org</h3>Copyright \251 2010 %3</center>")
                        .arg( APP_NAME ).arg( APP_VERSION ).arg( APP_NAME ));


    qglDialog = new QGridLayout( dialog );
        qglDialog->addWidget( qlAbout, 0, 0, 1, 3, Qt::AlignCenter );
        qglDialog->addWidget( qpbCredits, 1, 0, Qt::AlignCenter );
        qglDialog->addWidget( qpbLicense, 1, 1, Qt::AlignCenter );
        qglDialog->addWidget( qpbClose, 1, 2, Qt::AlignCenter );

        dialog->setWindowTitle( tr( "About %1").arg( APP_NAME ));
        dialog->setLayout( qglDialog );
        dialog->setFixedSize( 312, 156 );
        dialog->exec();
}

void MainWindow::aboutLicense() {
    QDialog *dialog = new QDialog( this );

    QFile file( ":/GPL" );
    if(!file.open( QIODevice::ReadOnly | QIODevice::Text ))
        qCritical( "GPL LicenseFile not found" );

    QTextStream out ( &file );
    out.setFieldAlignment ( QTextStream::AlignCenter );

    QTextEdit *qteLicense = new QTextEdit ( dialog );
        qteLicense->setText ( out.readAll ());
        qteLicense->setReadOnly ( 1 );
    QPushButton *qpbClose = new QPushButton ( IconLoader::Load( "window-close" ), tr( "&Close" ), dialog );
        connect( qpbClose, SIGNAL( clicked()), dialog, SLOT( deleteLater()));

    qglDialog = new QGridLayout( dialog );
        qglDialog->addWidget( qteLicense, 0, 0 );
        qglDialog->addWidget( qpbClose, 1, 0, Qt::AlignRight );

        dialog->setLayout( qglDialog);
        dialog->setWindowTitle( "GNU General Public License" );
        dialog->setFixedSize( 550, 400 );
        dialog->exec();
}

void MainWindow::aboutCredits() {
    QDialog *dialog = new QDialog( this );

    QTextEdit *qteCreditsWritten = new QTextEdit( dialog );
        qteCreditsWritten->setReadOnly( 1 );
        qteCreditsWritten->setLineWrapMode( QTextEdit::NoWrap );
        qteCreditsWritten->setText( "Tim Kleinschmidt <tim.kleinschmidt@googlemail.com>" );

    QTextEdit *qteCreditsArtwork = new QTextEdit( dialog );
        qteCreditsArtwork->setReadOnly( 1 );
        qteCreditsArtwork->setLineWrapMode( QTextEdit::NoWrap );
        qteCreditsArtwork->setText( "Elementary Theme\n" );

    QTabWidget *qtwCredits = new QTabWidget( dialog );
        qtwCredits->addTab( qteCreditsWritten, tr( "Written by" ));
        qtwCredits->addTab( qteCreditsArtwork, tr( "Artwork by" ));
        qtwCredits->setElideMode( Qt::ElideRight );

    QPushButton *qpbClose = new QPushButton( IconLoader::Load( "window-close"), tr( "&Close" ), dialog );
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

void MainWindow::progressLoad( int i ) {
    qpbMain->setValue( i );
}

void MainWindow::finishedLoad( bool value ) {
    qpbMain->hide();
    if( !value ) {
        QResource res( ":/noLeo.html" );
        QByteArray html(( const char* )res.data(), res.size());
        view->setHtml( html );
    }
}

