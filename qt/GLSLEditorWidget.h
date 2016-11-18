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

#ifndef __GLSLEditorWidget_HPP_INCLUDED__
#define __GLSLEditorWidget_HPP_INCLUDED__

#include "ui_GLSLEditorWidget.h"
#include <QMatrix4x4>
#include <string>

class QGLShader;
class GLSLCodeEditor;

class GLSLEditorWidget : public QWidget
{
    Q_OBJECT

public:
    GLSLEditorWidget(QGLShader* shader, QWidget *parent);
    ~GLSLEditorWidget();

    /**
     * Removes the OpenGLES defines added by Qt.
     * @brief removeQtDefines
     * @param sourceCode
     * @return
     */
    QString removeQtDefines(QString sourceCode);
    void setLinkToProgram(bool val);
    bool getLinkToProgram();
    QGLShader* getShader();
    QString getCurFileName();
    void setShaderCode(QString& text);
    QString getShaderCode();
    GLSLCodeEditor* getCodeEditor();

signals:
    /**
    * triggers linking
    * @brief updateLog
    */
    void compileAndLink();
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

    public slots:
    void setCurrentFile(const QString &fileName);
    void updateShaderSource();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::GLSLEditorWidget* ui;
    QGLShader* m_shader;
    GLSLCodeEditor* sEditor;
    QString currentFileName;
};

#endif
