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

#ifndef __GLSLEditorWindow_HPP_INCLUDED__
#define __GLSLEditorWindow_HPP_INCLUDED__

#include "opengl/openglheaders.h"
#include "ui_GLSLEditorWindow.h"
#include <QVector>
#include <QMatrix4x4>
#include <QGLShaderProgram>
#include <QtXml>

class GLSLCodeEditor;

class GLSLEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    GLSLEditorWindow(QGLShaderProgram* sProgram, QGLShaderProgram* dsProgram, QWidget *parent);
    ~GLSLEditorWindow();

    //TODO documentation
    void loadDefaultShaders();
    void linkShader();
    QGLShaderProgram* getShaderProgram() { return m_shaderProgram; };
    QGLShaderProgram* getShaderProgramDisplay() { return m_shaderProgramDisplay; };

    //	private slots:

signals:
    /**
    * Appends text to the log tab
    * @brief updateLog
    */
    void updateLog(QString);

    /**
    * Displays the log tab.
    * @brief displayLog
    */
    void displayLog();

    /**
    * Update the uniform tab in mainwindow.
    * @brief updateUniformWiget
    */
    void updateUniformTab();

    /**
    * Link the shaderprogram in mainwindow.
    * @brief updateUniformWiget
    */
    void updateShaderProgram();

    public slots:
    void compileAndLink();
    bool savePipelineAction();
    bool savePipelineAsAction();
    bool loadPipeline();
    void loadFromFileAction();
    bool saveToFileAction();
    void exitApplicationAction();
    void documentWasModified();
    bool saveAs();
    void about();

protected:
    void setupTabs();
    bool maybeSave();
    void loadFile(QString &fileName);
    bool save();
    bool saveFile(const QString &fileName);
    bool savePipeline(QString &fileName);
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    void writeSettings();
    void readSettings();

    Ui::GLSLEditorWindow* ui;
    QGLShaderProgram* m_shaderProgram;
    QGLShaderProgram* m_shaderProgramDisplay;
    QString pipelineFileName;
};

#endif
