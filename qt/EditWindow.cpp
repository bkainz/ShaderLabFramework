//=============================================================================
/** @file		EditWindow.cpp
 *
 * Implements the text editor window classes.
 *
    Shader Maker - a cross-platform GLSL editor.
    Copyright (C) 2007-2008 Markus Kramer
    For details, see main.cpp or COPYING.

=============================================================================*/

#include "GLSLCodeEditor.h"
#include "EditWindow.h"

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QAction>
#include <QMenuBar>
#include <QMouseEvent>
#include <QGridLayout>

//=============================================================================
//	GLSLBaseEditWindow implementation
//=============================================================================

// construction
GLSLBaseEditWindow::GLSLBaseEditWindow( void )
{
    // buttons
    m_btnLink = new QPushButton( "Compile and Link (F5)" );
    connect( m_btnLink, SIGNAL( clicked() ), this, SLOT( link() ) );

    m_lineNumber = new QLabel(this);
    m_lineNumber->setAlignment(Qt::AlignRight);

    // check boxes.
    m_chkAttach = new QCheckBox( "Attach to program" );

    // make sure no dummy text appears
    setWindowTitle( CONFIG_STRING_APPLICATION_TITLE );
}

GLSLBaseEditWindow::~GLSLBaseEditWindow( void )
{
}


/*
========================
newFile
========================
*/
void GLSLBaseEditWindow::newFile( void )
{
    GLSLCodeEditor* doc = activeDocument();
    if( doc != NULL )
        doc->newFile();
}


/*
========================
open
========================
*/
void GLSLBaseEditWindow::open( void )
{
    GLSLCodeEditor* doc = activeDocument();
    if( doc != NULL )
        doc->open();
}


/*
========================
save
========================
*/
bool GLSLBaseEditWindow::save( void )
{
    GLSLCodeEditor* doc = activeDocument();
    if( doc != NULL )
        return doc->save();

    return false;
}


/*
========================
saveAs
========================
*/
bool GLSLBaseEditWindow::saveAs( void )
{
    GLSLCodeEditor* doc = activeDocument();
    if( doc != NULL )
        return doc->saveAs();

    return false;
}


/*
========================
about
========================
*/
void GLSLBaseEditWindow::about( void )
{
    QMessageBox::about( this,
                        CONFIG_STRING_APPLICATION_TITLE,
                        "TODO " );
}


/*
========================
aboutQt
========================
*/
void GLSLBaseEditWindow::aboutQt( void )
{
    QMessageBox::aboutQt( this, CONFIG_STRING_APPLICATION_TITLE );
}


/*
========================
undo/redo/copy/cut/paste
========================
*/
void GLSLBaseEditWindow::undo( void )
{
    QTextEdit* doc = activeDocument();
    if( doc != NULL ) { doc->undo(); }
}
void GLSLBaseEditWindow::redo( void )
{
    QTextEdit* doc = activeDocument();
    if( doc != NULL ) { doc->redo(); }
}
void GLSLBaseEditWindow::copy( void )
{
    QTextEdit* doc = activeDocument();
    if( doc != NULL ) { doc->copy(); }
}
void GLSLBaseEditWindow::cut( void )
{
    QTextEdit* doc = activeDocument();
    if( doc != NULL ) { doc->cut(); }
}
void GLSLBaseEditWindow::paste( void )
{
    QTextEdit* doc = activeDocument();
    if( doc != NULL ) { doc->paste(); }
}


/*
========================
link
========================
*/
void GLSLBaseEditWindow::link( void )
{
    // compile and link
    emit linkProgram();
}


/*
========================
contentsChanged
========================
*/
void GLSLBaseEditWindow::contentsChanged( void )
{
    updateWindowTitle();
}


/*
========================
shaderFileChanged
========================
*/
void GLSLBaseEditWindow::shaderFileChanged( void )
{
    // there is a new shader. deactivate the old one.
    emit deactivateProgram();
}


/*
========================
updateWindowTitle
========================
*/
void GLSLBaseEditWindow::updateWindowTitle( void )
{
    GLSLCodeEditor* doc = activeDocument();
    if( doc != NULL )
    {
        // get the base file name
		//TODO
//        QString title = extractFileNameFromPath( doc->fileName() );
		QString title;
        if( title.length() == 0 ) // source not assigned to a file
            title = QString( "" );

        // add 'modified' flag
        if( doc->document()->isModified() ) {
            title += tr( "*" );
        }

        // lookup the shader type name
		//TODO
      //  QString shType = QString( " (%1) " ).
        //    arg( IShader::getShaderTypeName( doc->shaderType() ) );

        // add app title
       // title += shType; // show shader type in the window title
        title += QString( " - " + QString(CONFIG_STRING_APPLICATION_TITLE) );

        setWindowTitle( title );
    }
    else // no active document
    {
        setWindowTitle( CONFIG_STRING_APPLICATION_TITLE );
    }
}



/*
========================
createActions
========================
*/
//TODO
#if 0
void GLSLBaseEditWindow::createActions( /*IShader*/ )
{
    //
    // files
    //
    m_actNew = new QAction( QIcon(":/images/new.png"), tr( "&New" ), this );
    m_actNew->setShortcut( tr( "Ctrl+N" ) );
    connect( m_actNew, SIGNAL(triggered() ), this, SLOT(newFile()) );

    m_actOpen = new QAction( QIcon(":/images/open.png"), tr( "&Open..." ), this );
    m_actOpen->setShortcut( tr( "Ctrl+O" ) );
    connect( m_actOpen, SIGNAL(triggered()), this, SLOT(open()) );

    m_actSave = new QAction( QIcon(":/images/save.png"), tr( "&Save" ), this );
    m_actSave->setShortcut( tr( "Ctrl+S" ) );
    connect( m_actSave, SIGNAL(triggered()), this, SLOT(save()) );

    m_actSaveAs = new QAction( tr( "Save &As" ), this );
    connect( m_actSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()) );

    m_actQuit = new QAction( QIcon(":/images/exit.png"), tr( "E&xit" ), this );
    m_actQuit->setShortcut( tr( "Ctrl+Q" ) );
    connect( m_actQuit, SIGNAL(triggered()), this, SLOT(close()) );

    //
    // edit
    //
    m_actUndo = new QAction( QIcon(":/images/undo.png"), tr( "&Undo" ), this );
    m_actUndo->setShortcut( tr( "Ctrl+Z" ) );
    connect( m_actUndo, SIGNAL(triggered()), this, SLOT(undo()) );

    m_actRedo = new QAction( QIcon(":/images/redo.png"), tr( "&Redo" ), this );
    m_actRedo->setShortcut( tr( "Ctrl+Shift+Z" ) );
    connect( m_actRedo, SIGNAL(triggered()), this, SLOT(redo()) );

    m_actCopy = new QAction( QIcon(":/images/copy.png"), tr( "&Copy" ), this );
    m_actCopy->setShortcut( tr( "Ctrl+C" ) );
    connect( m_actCopy, SIGNAL(triggered()), this, SLOT(copy()) );

    m_actCut = new QAction( QIcon(":/images/cut.png"), tr( "Cu&t" ), this );
    m_actCut->setShortcut( tr( "Ctrl+X" ) );
    connect( m_actCut, SIGNAL(triggered()), this, SLOT(cut()) );

    m_actPaste = new QAction( QIcon(":/images/paste.png"), tr( "&Paste" ), this );
    m_actPaste->setShortcut( tr( "Ctrl+V" ) );
    connect( m_actPaste, SIGNAL(triggered()), this, SLOT(paste()) );

    //
    // shaders
    //
    m_actLink = new QAction( tr( "&Link" ), this );
    m_actLink->setShortcut( tr( "F5" ) );
    connect( m_actLink, SIGNAL(triggered()), this, SLOT(link()) );

    //
    // help
    //
    m_actAbout = new QAction( tr( "&About" ), this );
    connect( m_actAbout, SIGNAL(triggered()), this, SLOT(about()) );

    m_actAboutQt = new QAction( QIcon(":/images/qt.png"), tr( "About &Qt" ), this );
    connect( m_actAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()) );
}


/*
========================
createMenus
========================
*/
void GLSLBaseEditWindow::createMenus( /*IShader**/)
{
    // files 
    m_menuFile = menuBar()->addMenu( tr( "&File" ) );
    m_menuFile->addAction( m_actNew );
    m_menuFile->addAction( m_actOpen );
    m_menuFile->addAction( m_actSave );
    m_menuFile->addAction( m_actSaveAs );
    m_menuFile->addSeparator();
    m_menuFile->addAction( m_actQuit );

    // edit
    m_menuEdit = menuBar()->addMenu( "&Edit" );
    m_menuEdit->addAction( m_actUndo );
    m_menuEdit->addAction( m_actRedo );
    m_menuEdit->addSeparator();
    m_menuEdit->addAction( m_actCopy );
    m_menuEdit->addAction( m_actCut  );
    m_menuEdit->addAction( m_actPaste );

    // shader
    m_menuShader = menuBar()->addMenu( tr( "&Shader" ) );
    m_menuShader->addAction( m_actLink );

    // view
    m_menuView = menuBar()->addMenu( tr( "&View" ) );

    // help
    m_menuHelp = menuBar()->addMenu( tr( "&?" ) );
    m_menuHelp->addAction( m_actAbout );
    m_menuHelp->addAction( m_actAboutQt );
}
#endif

/*
========================
closeEvent
========================
*/
void GLSLBaseEditWindow::closeEvent( QCloseEvent* event )
{
    // do NOT close!
    event->ignore();

    // forward to the owner.
    emit aboutToQuit();
}


//=============================================================================
//	GLSLSdiEditWindow implementation
//=============================================================================

// construction
GLSLSdiEditWindow::GLSLSdiEditWindow( /*IShader* shader,*/ int shaderType )
{
    m_document = new GLSLCodeEditor( shaderType );

    // don't attach geometry shaders by default
    // -> confuses the novice
	//TODO
   // m_attachToShader = ( shaderType == IShader::TYPE_GEOMETRY ) ? false : true;
	m_attachToShader = false;

    m_chkAttach->setCheckState( m_attachToShader ? Qt::Checked : Qt::Unchecked );

    // setup signals
    connect( m_document, SIGNAL(updateMainWindow()), this, SLOT(contentsChanged()) );
    connect( m_document, SIGNAL(shaderChangedCompletely()), this, SLOT(shaderFileChanged()) );
    connect( m_document, SIGNAL(cursorPositionChanged()), this, SLOT(positionChanged()) );
    connect( m_chkAttach, SIGNAL(stateChanged(int)), this, SLOT(checkAttachToShader(int)) );

	//TODO
  //  createActions( shader );
   // createMenus  ( shader );

    // setup layout
    QWidget* central = new QWidget;
    QGridLayout* layout = new QGridLayout;
    layout->addWidget( m_document,  0, 0, 1, 3 );
    layout->addWidget( m_chkAttach, 1, 0, 1, 1 );
    layout->addWidget( m_btnLink,   1, 1, 1, 1 );
    layout->addWidget( m_lineNumber,1, 2, 1, 1 );
    central->setLayout( layout );

    setCentralWidget( central );
}


/*
========================
positionChanged
========================
*/
void GLSLSdiEditWindow::positionChanged( void ) {
    QTextCursor cursor = m_document->textCursor();
    int ln = cursor.blockNumber() + 1;
    int col = cursor.columnNumber() + 1;
    m_lineNumber->setText( "Ln: " + QString::number(ln) + " | Col: " + QString::number(col) );
}


/*
========================
maybeSave
========================
*/
bool GLSLSdiEditWindow::maybeSave( void )
{
    return m_document->maybeSave();
}


/*
========================
loadInitialShaderSource
========================
*/
void GLSLSdiEditWindow::loadInitialShaderSource( void )
{
    m_document->loadInitialShaderSource();
    updateWindowTitle();
}


/*
========================
uploadShaderSource
========================
*/
//TODO
/*
void GLSLSdiEditWindow::uploadShaderSource(IShader* shader, QGLShaderProgram* qglShader)
{

        if( m_attachToShader )
        {
            if(m_document->shaderType() == IShader::TYPE_VERTEX_FBO)
            {
                qglShader->addShaderFromSourceCode(QGLShader::Vertex, m_document->document()->toPlainText());
            }
            else if(m_document->shaderType() == IShader::TYPE_FRAGMENT_FBO)
            {
                qglShader->addShaderFromSourceCode(QGLShader::Fragment, m_document->document()->toPlainText());
            }
            else
            {
                shader->setShaderSource( m_document->shaderType(), m_document->toPlainText() );
            }

        }
        else // disabled by the user
        {
            if(m_document->shaderType() == IShader::TYPE_VERTEX_FBO)
            {
                qglShader->addShaderFromSourceCode(QGLShader::Vertex, QString( "" ));
            }
            else if(m_document->shaderType() == IShader::TYPE_FRAGMENT_FBO)
            {
                qglShader->addShaderFromSourceCode(QGLShader::Fragment,QString( "" ));
            }
            else
            {
                shader->setShaderSource( m_document->shaderType(), m_document->toPlainText() );
            }
        }
}
*/

/*
========================
loadSourceFile
========================
*/
void GLSLSdiEditWindow::loadSourceFile( const QString & fileName )
{
    // if there is text in the string
    if( !fileName.isEmpty() )
    {
        m_document->loadFile( fileName );
    }
    else
    {
        m_document->loadInitialShaderSource();
    }

    updateWindowTitle();
}


/*
========================
fileName
========================
*/
QString GLSLSdiEditWindow::fileName( void ) const
{
    return m_document->fileName();
}


/*
========================
createActions
========================
*/
//TODO
#if 0
void GLSLSdiEditWindow::createActions( IShader* shader )
{
    GLSLBaseEditWindow::createActions( shader );

    m_actToMDI = new QAction( tr( "Switch to MDI view" ), this );
    connect( m_actToMDI, SIGNAL(triggered()), this, SLOT(requestMdiModeSlot()) );
}


/*
========================
createMenus
========================
*/
void GLSLSdiEditWindow::createMenus( IShader* shader )
{
    GLSLBaseEditWindow::createMenus( shader );

    //
    // view
    //
    m_menuView->addAction( m_actToMDI );
}

#endif
/*
========================
checkAttachToShader
========================
*/
void GLSLSdiEditWindow::checkAttachToShader( int checkState )
{
    m_attachToShader = ( checkState != Qt::Unchecked );
}


/*
========================
requestMdiModeSlot
========================
*/
void GLSLSdiEditWindow::requestMdiModeSlot( void )
{
    emit requestMdiMode();
}


//=============================================================================
//	GLSLMdiEditWindow implementation
//=============================================================================

// construction
//TODO
GLSLMdiEditWindow::GLSLMdiEditWindow( /*IShader* shader*/ )
{
    m_editors = new GLSLCodeEditor*	[ MAX_SHADER_TYPES ];
    m_actFocusEditor = new QAction*	[ MAX_SHADER_TYPES ];
    m_attachToShader = new bool		[ MAX_SHADER_TYPES ];
    m_signalMapper = new QSignalMapper( this );
  //  createActions( shader );
  //  createMenus  ( shader );
  //  createTabs   ( shader );

    // setup layout
    QWidget* central = new QWidget;
    QGridLayout* layout = new QGridLayout;
    layout->addWidget( m_tabs,      0, 0, 1, 3 );
    layout->addWidget( m_chkAttach, 1, 0, 1, 1 );
    layout->addWidget( m_btnLink,   1, 1, 1, 1 );
    layout->addWidget( m_lineNumber,1, 2, 1, 1 );
    central->setLayout( layout );
    setCentralWidget( central );

    connect( m_chkAttach, SIGNAL(stateChanged(int)), this, SLOT(checkAttachToShader(int)) );

    // trigger state update
    activeEditorChanged( m_tabs->currentIndex() );
}

GLSLMdiEditWindow::~GLSLMdiEditWindow( void )
{
    SAFE_DELETE_ARRAY( m_editors );
    SAFE_DELETE_ARRAY( m_actFocusEditor );
    SAFE_DELETE_ARRAY( m_attachToShader );
}


/*
========================
maybeSave

 returns true, if the caller should continue.
========================
*/
bool GLSLMdiEditWindow::maybeSave( void )
{
    // loop through all sources
    for( int i = 0 ; i < MAX_SHADER_TYPES; i++ )
    {
        if( m_editors[ i ] != NULL &&
            !m_editors[ i ]->maybeSave() )
        {
            return false;
        }
    }

    return true;
}


/*
========================
loadInitialShaderSource
========================
*/
void GLSLMdiEditWindow::loadInitialShaderSource( void )
{
    for( int i = 0 ; i < MAX_SHADER_TYPES; i++ )
    {
        if( m_editors[ i ] != NULL )
        {
            m_editors[ i ]->loadInitialShaderSource();
        }
    }

    updateWindowTitle();
}


/*
========================
loadSourceFile
========================
*/
void GLSLMdiEditWindow::loadSourceFile( int shaderType, const QString & fileName )
{
    if( shaderType >= 0 && shaderType < MAX_SHADER_TYPES &&
        m_editors[ shaderType ] != NULL )
    {
        // if there is text in the string
        if( !fileName.isEmpty() )
        {
            m_editors[ shaderType ]->loadFile( fileName );
        }
        else
        {
            m_editors[ shaderType ]->loadInitialShaderSource();
        }
    }

    updateWindowTitle();
}


/*
========================
fileName
========================
*/
QString GLSLMdiEditWindow::fileName( int shaderType )
{
    if( shaderType >= 0 && shaderType < MAX_SHADER_TYPES &&
        m_editors[ shaderType ] != NULL )
    {
        return m_editors[ shaderType ]->fileName();
    }

    return QString( "" );
}


/*
========================
uploadShaderSource
========================
*/
//TODO
#if 0
void GLSLMdiEditWindow::uploadShaderSource(IShader* shader, QGLShaderProgram* qglShader)
{
    // read out all editors
    for( int i = 0 ; i < IShader::MAX_SHADER_TYPES ; i++ )
    {
        if(i == IShader::TYPE_VERTEX || i == IShader::TYPE_GEOMETRY || i == IShader::TYPE_FRAGMENT)
        {
            // not available
            if( m_editors[ i ] == NULL )
                continue;

            if( m_attachToShader[ i ] )
            {
                shader->setShaderSource( i, m_editors[ i ]->document()->toPlainText() );
            }
            else // user diabled this shader type
            {
                shader->setShaderSource( i, QString( "" ) );
            }
        }
        else
        {
            if( m_attachToShader[ i ] )
            {
                if(m_editors[i]->shaderType() == IShader::TYPE_VERTEX_FBO)
                {
                    qglShader->addShaderFromSourceCode(QGLShader::Vertex, m_editors[i]->document()->toPlainText());
                }
                else if(m_editors[i]->shaderType() == IShader::TYPE_FRAGMENT_FBO)
                {
                    qglShader->addShaderFromSourceCode(QGLShader::Fragment, m_editors[i]->document()->toPlainText());
                }

            }
            else // user diabled this shader type
            {
                if(m_editors[i]->shaderType() == IShader::TYPE_VERTEX_FBO)
                {
                    qglShader->addShaderFromSourceCode(QGLShader::Vertex, QString( "" ));
                }
                else if(m_editors[i]->shaderType() == IShader::TYPE_FRAGMENT_FBO)
                {
                    qglShader->addShaderFromSourceCode(QGLShader::Fragment,QString( "" ));
                }
            }
        }

    }
}

/*
========================
createActions
========================
*/
void GLSLMdiEditWindow::createActions( IShader* shader )
{
    GLSLBaseEditWindow::createActions( shader );

    //
    // view
    //
    m_actToSDI = new QAction( tr( "Switch to SDI view" ), this );
    connect( m_actToSDI, SIGNAL(triggered()), this, SLOT(requestSdiModeSlot()) );

    //
    // shaders
    //
    m_actNextShader = new QAction( tr( "Next Shader" ), this );
    m_actNextShader->setShortcut( tr( "Alt+Right" ) );
    connect( m_actNextShader, SIGNAL(triggered()), this, SLOT(nextShaderTab()) );

    m_actPrevShader = new QAction( tr( "Previous Shader" ), this );
    m_actPrevShader->setShortcut( tr( "Alt+Left" ) );
    connect( m_actPrevShader, SIGNAL(triggered()), this, SLOT(prevShaderTab()) );

    //
    // changing the active tab
    //
    for( int i = 0, j = 0 ; i < IShader::MAX_SHADER_TYPES ; i++ )
    {
        m_actFocusEditor[ i ] = NULL;
        if( shader->isShaderTypeAvailable( i ) )
        {
            QString name = IShader::getShaderTypeName(i);
            QString shortcut = tr( "F%1" ).arg( i+1 );

            m_actFocusEditor[ i ] = new QAction( name, this );
            m_actFocusEditor[ i ]->setShortcut( shortcut );

            connect( m_actFocusEditor[ i ], SIGNAL(triggered()),
                     m_signalMapper, SLOT(map()) );
            m_signalMapper->setMapping( m_actFocusEditor[ i ], j );
            j++;
        }
    }
}


/*
========================
createMenus
========================
*/
void GLSLMdiEditWindow::createMenus( IShader* shader )
{
    GLSLBaseEditWindow::createMenus( shader );

    //
    // view
    //
    for( int i = 0 ; i < IShader::MAX_SHADER_TYPES ; i++ )
    {
        if( m_actFocusEditor[ i ] != NULL )
            m_menuView->addAction( m_actFocusEditor[ i ] );
    }
    m_menuView->addSeparator();
    m_menuView->addAction( m_actNextShader );
    m_menuView->addAction( m_actPrevShader );

    m_menuView->addSeparator();
    m_menuView->addAction( m_actToSDI );
}


/*
========================
createTabs
========================
*/
void GLSLMdiEditWindow::createTabs( IShader* shader )
{
    m_tabs = new QTabWidget();

    // add tabs
    for( int i = 0 ; i < IShader::MAX_SHADER_TYPES ; i++ )
    {
        m_editors[ i ] = NULL;

        m_attachToShader[ i ] = false;

        // if available
        if( shader->isShaderTypeAvailable( i ) )
        {
            // don't attach geometry shaders be default
            // -> confuses the novice
            if( i != IShader::TYPE_GEOMETRY ) {
                m_attachToShader[ i ] = true;
            }

            m_editors[ i ] = new GLSLCodeEditor( i );
            m_tabs->addTab( m_editors[ i ], IShader::getShaderTypeName( i ) );
            connect( m_editors[i], SIGNAL(updateMainWindow()), this, SLOT(contentsChanged()) );
            connect( m_editors[i], SIGNAL(shaderChangedCompletely()), this, SLOT(shaderFileChanged()) );
            connect( m_editors[i], SIGNAL(cursorPositionChanged()), this, SLOT(positionChanged()) );
        }
    }

    connect( m_tabs, SIGNAL(currentChanged(int)), this, SLOT(activeEditorChanged(int)) );
    connect( m_tabs, SIGNAL(currentChanged(int)), this, SLOT(positionChanged()) );
    connect( m_signalMapper, SIGNAL(mapped(int)), m_tabs, SLOT(setCurrentIndex(int)) );
}

#endif

/*
========================
positionChanged
========================
*/
void GLSLMdiEditWindow::positionChanged( void ) {
    QTextCursor cursor = m_editors[tabToShader(m_tabs->currentIndex())]->textCursor();
    int ln = cursor.blockNumber() + 1;
    int col = cursor.columnNumber() + 1;
    m_lineNumber->setText( "Ln: " + QString::number(ln) + " | Col: " + QString::number(col) );
}


/*
========================
activeDocument
========================
*/
GLSLCodeEditor* GLSLMdiEditWindow::activeDocument( void )
{
    QWidget* widget = m_tabs->widget( m_tabs->currentIndex() );

    // do not type cast. does not hurt, since there are only 3 elements...
    for( int i = 0 ; i < MAX_SHADER_TYPES ; i++ )
    {
        if( m_editors[ i ] != NULL && 
            m_editors[ i ] == widget )
        {
            return m_editors[ i ];
        }
    }

    return NULL;
}


/*
========================
tabToShader
========================
*/
int GLSLMdiEditWindow::tabToShader( int tabIndex )
{
    QWidget* widget = m_tabs->widget( tabIndex );

    // look up the widget... it only take 3 loops...
    for( int i = 0 ; i < MAX_SHADER_TYPES ; i++ )
    {
        if( m_editors[ i ] != NULL && 
            m_editors[ i ] == widget )
        {
            return i;
        }
    }

    // not found...
    return -1;
}


/*
========================
nextShaderTab
========================
*/
void GLSLMdiEditWindow::nextShaderTab( void )
{
    if( m_tabs->currentIndex() < m_tabs->count()-1 )
        m_tabs->setCurrentIndex( m_tabs->currentIndex() + 1 );
}


/*
========================
prevShaderTab
========================
*/
void GLSLMdiEditWindow::prevShaderTab( void )
{
    if( m_tabs->currentIndex() > 0 )
        m_tabs->setCurrentIndex( m_tabs->currentIndex() - 1 );
}


/*
========================
activeEditorChanged

 idx: tab-index
========================
*/
void GLSLMdiEditWindow::activeEditorChanged( int idx )
{
    updateWindowTitle();

    idx = tabToShader( idx );

    // look up attach flag
    bool attach = false;
    if( idx >= 0 && idx < MAX_SHADER_TYPES )
        attach = m_attachToShader[ idx ];

    // set new state
    m_chkAttach->setCheckState( attach ? Qt::Checked : Qt::Unchecked );
}


/*
========================
checkAttachToShader
========================
*/
void GLSLMdiEditWindow::checkAttachToShader( int checkState )
{
    // store check state for the current shader
    int idx = tabToShader( m_tabs->currentIndex() );
    if( idx >= 0 && idx < MAX_SHADER_TYPES )
        m_attachToShader[ idx ] = ( checkState != Qt::Unchecked );
}


/*
========================
requestSdiModeSlot
========================
*/
void GLSLMdiEditWindow::requestSdiModeSlot( void )
{
    emit requestSdiMode();
}



