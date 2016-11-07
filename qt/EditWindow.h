//=============================================================================
/** @file		EditWindow.hpp
 *
 * Defines the editor main window classes.
 *
    Shader Maker - a cross-platform GLSL editor.
    Copyright (C) 2007-2008 Markus Kramer
    For details, see main.cpp or COPYING.

=============================================================================*/

#ifndef __EDITWINDOW_HPP_INCLUDED__
#define __EDITWINDOW_HPP_INCLUDED__

#include "opengl/openglheaders.h"
#include <QCheckBox>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QTabWidget>
#include <QGLShaderProgram>

// forward declarations
//class IShader;
class GLSLCodeEditor;


//=============================================================================
//	GLSLBaseEditWindow
//=============================================================================

/** Editor main window base class.
 * The different editor main window classes are derived from this class.
 * This calss is designed to derive editor windows that allow SDI and MDI
 * source code editing.
 */
class GLSLBaseEditWindow : public QMainWindow
{
    Q_OBJECT

public:

    /** Constructor.
     * @internal
     *  The constructor only creates and connects the m_btnLink and
     *  m_chkAttach components.
     */
    GLSLBaseEditWindow( void );
    virtual ~GLSLBaseEditWindow( void ); ///< Destructor.

    /** Asks the user to save all modified shaders.
     * @return	True, if the calling operation can safely continue (no data is lost).
     *			False, if the calling operation should be canceled.
     */
    virtual bool maybeSave( void ) = 0;

    /** Loads the initial shader sources into the editor.
     */
    virtual void loadInitialShaderSource( void ) = 0;

    /** Uploads the source code to an IShader object and a QGLShaderProgram object (render to texture).
     * @param shader IShader object where the source code should be stored.
     */
	//TODO
    //virtual void uploadShaderSource(IShader* shader, QGLShaderProgram* qglShader) = 0;


signals:

    /** The user requested to compile and link the current shader.
     */
    void linkProgram( void );

    /** The current shader should be deactivated.
     * This happens, when the user opens an existing or creates a new shader.
     */
    void deactivateProgram( void );

    /** Tells the application that the user wants to quit the application.
     * @warning
     *  The reciever should call maybeSave() to avoid loss of data!
     */
    void aboutToQuit( void );

    /** Requests to switch to SDI mode.
     */
    void requestSdiMode( void );

    /** Requests to switch to MDI mode.
     */
    void requestMdiMode( void );

private slots:

    // operation
    void link( void );
    void shaderFileChanged( void );
    void contentsChanged( void );

    // files
    void newFile( void );
    void open( void );
    bool save( void );
    bool saveAs( void );

    // edit
    void undo ( void );
    void redo ( void );
    void copy ( void );
    void cut  ( void );
    void paste( void );

    // about
    void about( void );
    void aboutQt( void );

protected:

    /** Returns the active document in the window.
     * This method can be used to create MDI emulation functionality.
     * @return The active source code editor.
     */
    virtual GLSLCodeEditor* activeDocument( void ) = 0;

    /** Creates the QAction objects used in this class.
     * Must be called explicitly in the constructor of the derived class.
     * @param shader IShader object to read state from.
     */
	//TODO
    //virtual void createActions( IShader* shader );

    /** Creates the QMenu objects used in the class.
     * Must be called explicitly in the constructor of the derived class.
     * @pre Assumes that createActions() has already been called.
     * @param shader IShader object to read state from.
     */
	//TODO
    //virtual void createMenus( IShader* shader );

    /** Updates the window title.
     * It ready the active document's file name and constructs a title string.
     */
    void updateWindowTitle( void );

    // buttons
    QPushButton*	m_btnLink; ///< The 'Compile and Link' button.

    // check boxes
    QCheckBox*		m_chkAttach; ///< The 'Attach to Shader' check box.

    // labels
    QLabel* m_lineNumber;

    // menus
    QMenu*			m_menuFile;   ///< The 'File' menu.
    QMenu*			m_menuEdit;   ///< The 'Edit' menu.
    QMenu*			m_menuShader; ///< The 'Shader' menu.
    QMenu*			m_menuView;   ///< The 'View' menu.
    QMenu*			m_menuHelp;   ///< The '?' menu.

private:
    // QWidget
    void closeEvent( QCloseEvent* event );

    // actions, mainly those available in the menus.
    QAction* m_actQuit;
    QAction* m_actNew;
    QAction* m_actOpen;
    QAction* m_actSave;
    QAction* m_actSaveAs;
    QAction* m_actUndo;
    QAction* m_actRedo;
    QAction* m_actCopy;
    QAction* m_actCut;
    QAction* m_actPaste;
    QAction* m_actLink; // when the 'compile and link' button is pushed.
    QAction* m_actAbout;
    QAction* m_actAboutQt;
};


//=============================================================================
//	GLSLSdiEditWindow
//=============================================================================

/** SDI editor window class.
 * An instance of GLSLSdiEditWindow class contains only one text editor.
 * You need a separate window for each shader type!
 */
class GLSLSdiEditWindow : public GLSLBaseEditWindow
{
    Q_OBJECT
public:

    /** Constructor.
     * @param shader IShader object to qurey state from.
     *				 Assumes the object is already initialized.
     * @param shaderType What shader type should be supported by this window.
     */
    GLSLSdiEditWindow( /*IShader* shader,*/ int shaderType );

    // GLSLBaseEditWindow methods
    bool maybeSave( void ) override;
    void loadInitialShaderSource( void ) override;
	//TODO
    //void uploadShaderSource(IShader* shader, QGLShaderProgram* qglShader) override;

    /** Loads a source code file for the shader type this edit window represents.
     * @param fileName Source code file name. If this is an emoty string, the default shader
     *					source code will be loaded.
     */
    void loadSourceFile( const QString & fileName );

    /** Returns the file name if the currently opened file.
     * If the current shader is not assigned to a file ( still 'untitled' ),
     * this method returns an emtpty string.
     */
    QString fileName( void ) const;

private slots:

    // simple forwarding
    void requestMdiModeSlot( void );
    void positionChanged( void );
    void checkAttachToShader( int checkState );

private:

    // construction helpers
	//TODO
    //void createActions( IShader* shader ) override;
    //void createMenus  ( IShader* shader ) override;

    GLSLCodeEditor* activeDocument( void ) override { return m_document; }

    // single document for the single edit window.
    GLSLCodeEditor* m_document;
    bool m_attachToShader;

    QAction* m_actToMDI; ///< switch to MDI view.
};


//=============================================================================
//	GLSLMdiEditWindow
//=============================================================================

/** MDI editor main window class.
 * Contains the editors for all shader types in a single window.
 * Yout need only a single instance to deal with all shader types.
 */
class GLSLMdiEditWindow : public GLSLBaseEditWindow
{
    Q_OBJECT

public:

    /** Constructor.
     * @param shader IShader object to qurey state from.
     *				 Assumes the object is already initialized.
     */
    GLSLMdiEditWindow( /*IShader* shader*/ );
    virtual ~GLSLMdiEditWindow( void ); /// Destructor.

    // GLSLBaseEditWindow methods
    bool maybeSave( void ) override;
    void loadInitialShaderSource( void ) override;
	//TODO
    //void uploadShaderSource(IShader* shader, QGLShaderProgram* qglShader) override;

    /** Loads a source code file for a given shader type.
     * @param shaderType Specifies the editor, which shuld load the source code.
     * @param fileName Source code file name. If this is an emoty string, the default shader
     *					source code will be loaded.
     */
    void loadSourceFile( int shaderType, const QString & fileName );

    /** Returns the file name for a shader type.
     * It returns form the file name from the GLSLCodeEditor widget
     * responsible for the given shader type.
     * @param shaderType Shader type where to request the name.
     */
    QString fileName( int shaderType );

private slots:

    void activeEditorChanged( int tabIndex );
    void nextShaderTab( void );
    void prevShaderTab( void );
    void positionChanged( void );
    void requestSdiModeSlot( void ); // signal forwarding
    void checkAttachToShader( int checkState );

private:

    // MDI emulation
    virtual GLSLCodeEditor* activeDocument( void ) override;

    // construction helpers
	//TODO
   // void createActions( IShader* shader ) override;
    //void createMenus( IShader* shader ) override;
   // void createTabs( IShader* shader );

    // converts from tab indices to shader type.
    // returns -1 on error.
    int tabToShader( int tabIndex );

    QTabWidget*		m_tabs;
    QSignalMapper*	m_signalMapper; // used for tab selection shortcuts

    // all documents
    GLSLCodeEditor**	m_editors; // [ IShader::MAX_SHADER_TYPES ]
    bool*			m_attachToShader; // [ IShader::MAX_SHADER_TYPES ]

    // actions
    QAction*	m_actNextShader;
    QAction*	m_actPrevShader;
    QAction*	m_actToSDI; ///< switch to SDI view.
    QAction**	m_actFocusEditor; // [ IShader::MAX_SHADER_TYPES ]
};



#endif	// __EDITWINDOW_HPP_INCLUDED__
