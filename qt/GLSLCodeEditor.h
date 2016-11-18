/****************************************************************************
* This is the Computer Graphics Shader Lab.
*
* Copyright (c) 2016 Bernhard Kainz, Antoine S Toisoul
* (b.kainz@imperial.ac.uk, antoine.toisoul13@imperial.ac.uk)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
****************************************************************************/

#ifndef __SOURCEEDITOR_HPP_INCLUDED__
#define __SOURCEEDITOR_HPP_INCLUDED__

//=============================================================================
//	GLSLCodeEditor - text editor for GLSL source code.
//=============================================================================

#include "opengl/openglheaders.h"
#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QGLShader>
#include "codeeditor.h"

//TODO move to config
//=============================================================================
//	misc
//=============================================================================

#define CONFIG_SHADER_DIRECTORY		"shaders/"	///< Where to look for shader source files
#define CONFIG_MODEL_DIRECTORY		"models/"	///< Where to look for .OBJ model files
#define CONFIG_TEXTURE_DIRECTORY	"textures/"	///> Where to look for texture iamge files
#define CONFIG_EDITOR_FONT_NAME		"Consolas"	///< Editor font
#define	CONFIG_TAB_SIZE				4			///< One tab quals that many spaces
#define CONFIG_REFRESH_INTERVAL		10			///< 100 fps, periodic screen refesh in ms
#define CONFIG_MAX_USED_TMUS		4			///< number of texture mapping units accessable by CTextureWidget

/** Commet this out to disable geometry shader support */
#define CONFIG_ENABLE_GEOMETRY_SHADER

/** Font size for the editor.
*  10 is hard to read on linux.
*/
#ifdef WIN32
#define CONFIG_EDITOR_FONT_SIZE		10
#else // linux, mac
#define CONFIG_EDITOR_FONT_SIZE		12
#endif

#define MAX_SHADER_TYPES 5

#define CONFIG_STRING_APPLICATION_TITLE "Imperial CO317 Computer Graphics"

//=============================================================================
//	useful macros
//=============================================================================

/** Deletes an object, if not NULL.
* @warning Do not use this for arrays!
*/
#define SAFE_DELETE(p)			do{  if( (p) != NULL ) { delete    (p); (p)=NULL; } } while(0)

/** Deletes an array of objects, if not NULL.
* @warning Only use this for arrays!
*/
#define SAFE_DELETE_ARRAY(p)	do{  if( (p) != NULL ) { delete [] (p); (p)=NULL; } } while(0)

//////////////////////////////////////////////////

/** This is a GLSL source code editing widget.
* It is simply an improved QTextEdit with some additional functionality
* like open/save/new file operations and syntax highlighting.
* This widget knows about the shader type it is editing. This allows automatic
* selection of filename filters in dialogs.
* The shader type must be set during object construction.
*/
class GLSLCodeEditor : public CodeEditor
{
    Q_OBJECT
public:
    /** Constructs a GLSLCodeEditor object.
    * A shader type must be specified that controls some aspects of the editor,
    * like filters in open/save dialogs.
    * @param shaderType Shader type from IShader.
    */
    GLSLCodeEditor(QGLShader::ShaderType shaderType);

    /** Load the initial source code for the assigned shader type.
    * The source code loaded is based on the shaderType argument passed to the constructor.
    */
    void loadInitialShaderSource(void);

    /** Returns the shader type assigned to this source editor.
    */
    QGLShader::ShaderType shaderType(void) const;

    /** Clears the content to an empty string.
    * Pops up a 'save changes' dialog if necessary.
    */
    void newFile(void);

    /** Asks the user to open a file.
    * Pops up a 'save changes' dialog if necessary.
    */
    void open(void);

    /** Saves the currently opened file.
    * If no filename is stored, the uses is asked for a file name.
    */
    bool save(void);

    /** Asks the user for a file name and saves the content in that file.
    */
    bool saveAs(void);

    /** Asks the user to save changes, if necessary.
    * @return True if the data is saved and the calling operation can be continued.
    *         False if the calling operation should be aborded.
    */
    bool maybeSave(void);

    /** Returns the filename of the currently opened file.
    * An empty string indicates that no file name is assigned to the current content.
    */
    QString fileName(void) const;

    /** Tells the widget to directly load a named file.
    * @param fileName Name of the file to load.
    */
    void loadFile(const QString & fileName);

signals:
    /** Emitted every time the owner of this widget needs to update.
    * For example, this happens when the documents modified flag changed.
    */
    void updateMainWindow(void);

    /** Indicates that the used opened an existing or created a new shader.
    * It is emitted every time a document was successfully loaded from disk.
    */
    void shaderChangedCompletely(void);

    private slots:
    void documentContentsChanged(void);

private:
    bool saveFile(const QString & fileName);
    void setFileName(const QString & fileName);
    QString fileTypeFilter(void);

    // auto sizing
    QSize minimumSizeHint(void) const { return QSize(200, 100); }
    QSize        sizeHint(void) const { return QSize(800, 600); }


    // creates the syntax highlighter for GLSL
    // and assigns it to this GLSLCodeEditor.
    void createSyntaxHighlighter(void);
    QSyntaxHighlighter* m_highlighter;

    QGLShader::ShaderType m_shaderType;
    QString	m_fileName; // empty string == untitled document
};

#endif	// __SOURCEEDITOR_HPP_INCLUDED__

