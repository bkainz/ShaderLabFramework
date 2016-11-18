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

#include "GLSLCodeEditor.h"

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

//=============================================================================
//	GLSLCodeEditor implementation
//=============================================================================

// construction
GLSLCodeEditor::GLSLCodeEditor(QGLShader::ShaderType shaderType)
{
    // member initialization
    m_shaderType = shaderType;
    m_fileName = QString("");
    m_highlighter = NULL;

    // setup font
    QFont f(font());
    f.setFamily(CONFIG_EDITOR_FONT_NAME);
    f.setPointSize(CONFIG_EDITOR_FONT_SIZE);
    f.setFixedPitch(true);
    setFont(f);

    // turn off dynamic line wrap
    setLineWrapMode(NoWrap);

    // set tab sizes...
    char text[CONFIG_TAB_SIZE + 1];
    memset(text, ' ', CONFIG_TAB_SIZE);
    text[CONFIG_TAB_SIZE] = '\0';
    setTabStopWidth(fontMetrics().width(text));

    // syntax highlighting
    createSyntaxHighlighter();

    // notify parent
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentContentsChanged()));
}


/*
========================
loadInitialShaderSource
========================
*/
void GLSLCodeEditor::loadInitialShaderSource(void)
{
    // initial shader source is not assigned to a file.
    setFileName("");
    setPlainText("");

    //TODO
    // set initial fragment shaders
    //
    /*switch (m_shaderType)
    {
    case IShader::TYPE_VERTEX:
      setPlainText(INITIAL_VERTEX_SHADER_SOURCE);
      break;

    case IShader::TYPE_GEOMETRY:
      setPlainText(INITIAL_GEOMETRY_SHADER_SOURCE);
      break;

    case IShader::TYPE_FRAGMENT:
      setPlainText(INITIAL_FRAGMENT_SHADER_SOURCE);
      break;
    case IShader::TYPE_VERTEX_FBO:
      setPlainText(INITIAL_RENDER_TO_TEXTURE_VERTEX_SHADER_SOURCE);
      break;

    case IShader::TYPE_FRAGMENT_FBO:
      setPlainText(INITIAL_RENDER_TO_TEXTURE_FRAGMENT_SHADER_SOURCE);
      break;
    }*/

    // clear the modified flag
    document()->setModified(false);
}


/*
========================
documentContentsChanged

this is only used to reduce the amount connections between objects.
========================
*/
void GLSLCodeEditor::documentContentsChanged(void)
{
    // notify parent
    emit updateMainWindow();
}


/*
========================
newFile
========================
*/
void GLSLCodeEditor::newFile(void)
{
    if (!maybeSave())
        return;

    // set new state
    loadInitialShaderSource();
    setFocus(Qt::OtherFocusReason);

    emit updateMainWindow();
    emit shaderChangedCompletely();
}


/*
========================
open
========================
*/
void GLSLCodeEditor::open(void)
{
    if (!maybeSave())
        return;

    // setup initial directory
    QString initialDir = CONFIG_SHADER_DIRECTORY;
    if (m_fileName.length() > 0)
        initialDir = m_fileName;

    //
    // ask for the file to open
    //
    QString fileName = QFileDialog::getOpenFileName(
        parentWidget(), QString(),
        initialDir,
        fileTypeFilter());
    setFocus(Qt::OtherFocusReason);
    if (!fileName.isEmpty())
    {
        loadFile(fileName);
    }
}


/*
========================
save
========================
*/
bool GLSLCodeEditor::save(void)
{
    if (m_fileName.isEmpty())
        return saveAs();

    return saveFile(m_fileName);
}


/*
========================
saveAs
========================
*/
bool GLSLCodeEditor::saveAs(void)
{
    // setup initial directory
    QString initialDir = CONFIG_SHADER_DIRECTORY;
    if (m_fileName.length() > 0)
        initialDir = m_fileName;

    // bring up save dialog
    QString fileName = QFileDialog::getSaveFileName(
        parentWidget(), QString(),
        initialDir,
        fileTypeFilter());
    setFocus(Qt::OtherFocusReason);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}


/*
========================
loadFile
========================
*/
void GLSLCodeEditor::loadFile(const QString & fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(parentWidget(),
            CONFIG_STRING_APPLICATION_TITLE,
            tr("Cannot read file %1:\n%2.")
            .arg(fileName)
            .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString text = in.readAll();

    // set file name first, so the contentsChanged() signal will be up to date
    setFileName(fileName);
    setPlainText(text);

    QApplication::restoreOverrideCursor();

    emit updateMainWindow();
    emit shaderChangedCompletely();
}


/*
========================
saveFile
========================
*/
bool GLSLCodeEditor::saveFile(const QString & fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(parentWidget(),
            CONFIG_STRING_APPLICATION_TITLE,
            tr("Cannot write file %1:\n%2.")
            .arg(fileName)
            .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    QApplication::restoreOverrideCursor();

    setFileName(fileName);
    document()->setModified(false);
    emit updateMainWindow();

    return true;
}


/*
========================
maybeSave

returns true, if the interrupted operation should continue
========================
*/
bool GLSLCodeEditor::maybeSave(void)
{
    // no need to save it.
    if (!document()->isModified())
        return true;

    // query good file name
    QString name = fileName();
    if (name.isEmpty())
        name = QString("untitled");

    // setup text and title
    QString title("Save Changes");
    QString text = QString("The %1 '%2' has been modified.\nSave changes?").
        arg("TODO").//arg(IShader::getShaderTypeName(m_shaderType)).
        arg(name);

    // display the message box
    int ret = QMessageBox::question(parentWidget(), title, text,
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Cancel);

    switch (ret)
    {
    case QMessageBox::Save:
        return save();
        break;

    case QMessageBox::Discard:
        return true;
        break;
    }

    // default: cancel
    return false;
}


/*
========================
setFileName
========================
*/
void GLSLCodeEditor::setFileName(const QString & fileName)
{
    m_fileName = fileName;
}


/*
========================
fileName
========================
*/
QString GLSLCodeEditor::fileName(void) const
{
    return m_fileName;
}


/*
========================
shaderType
========================
*/
QGLShader::ShaderType GLSLCodeEditor::shaderType(void) const
{
    return m_shaderType;
}


/*
========================
fileTypeFilter
========================
*/
QString GLSLCodeEditor::fileTypeFilter(void)
{
    QString specific;
    QString all("All Files (*)");

    //TODO
    /*switch (m_shaderType)
    {
    case IShader::TYPE_VERTEX:   specific = "Vertex Shaders (*.vert);;"; break;
    case IShader::TYPE_GEOMETRY: specific = "Geometry Shaders (*.geom);;"; break;
    case IShader::TYPE_FRAGMENT: specific = "Fragment Shaders (*.frag);;"; break;
    }*/

    return (specific + all);
}


