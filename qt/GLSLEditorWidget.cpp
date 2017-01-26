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
#include "qt/GLSLEditorWidget.h"
#include "qt/Matrix4x4Widget.h"
#include "qt/GLSLCodeEditor.h"
#include <QGLShader>

GLSLEditorWidget::GLSLEditorWidget(QGLShader* shader, QWidget *parent) : QWidget(parent), ui(new Ui::GLSLEditorWidget)
{
    m_shader = shader;
    ui->setupUi(this);

    sEditor = new GLSLCodeEditor(shader->shaderType());

    sEditor->setPlainText(removeQtDefines(shader->sourceCode()));

    ui->mainVLayout->insertWidget(0, sEditor);
    connect(ui->compileAndLinkButton, SIGNAL(clicked()), this, SLOT(updateShaderSource()));
}

GLSLEditorWidget::~GLSLEditorWidget()
{
    delete ui;
}

QString GLSLEditorWidget::removeQtDefines(QString sourceCode)
{
    QString defines[3] = { QString("#define lowp") , QString("#define mediump") , QString("#define highp") };

    for (int i = 0; i < 3; ++i)
    {
        int pos = sourceCode.indexOf(defines[i]);

        if (pos != -1)
        {
            sourceCode.remove(pos, defines[i].length());
            
            pos = sourceCode.indexOf('\n', pos);

            // If the removal of QtDefine leaves empty lines
            // remove the \n
            if (pos != -1)
            {
                sourceCode.remove(pos, 1);
            }
        }

    }

    return sourceCode;
}

void GLSLEditorWidget::setLinkToProgram(bool val)
{
    ui->linkToProgramCheckBox->setChecked(val);
}

bool GLSLEditorWidget::getLinkToProgram()
{
    return ui->linkToProgramCheckBox->isChecked();
}

QGLShader* GLSLEditorWidget::getShader()
{
    return m_shader;
};

void GLSLEditorWidget::updateShaderSource()
{
    if (!m_shader->compileSourceCode(sEditor->toPlainText()))
    {
        QString error = m_shader->log();
        emit updateLog(error);
        emit displayLog();
    }
    else
    {
        QString text = QString("Compilation of %1 successfull.").arg(this->objectName());
        emit updateLog(text);
        emit displayLog();
        if (ui->linkToProgramCheckBox->isChecked())
        {
            emit(compileAndLink());
        }
        else
        {
            QString text_ = QString("________________________________________________\n");
            emit updateLog(text_);
            emit displayLog();
        }
    }
}

void GLSLEditorWidget::setShaderCode(QString &text)
{
    sEditor->setPlainText(removeQtDefines(text));
}

QString GLSLEditorWidget::getShaderCode()
{
    return sEditor->toPlainText();
}

void GLSLEditorWidget::setCurrentFile(const QString &fileName)
{
    currentFileName = fileName;
    sEditor->document()->setModified(false);
    setWindowModified(false);

    QString shownName = currentFileName;
    if (currentFileName.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString GLSLEditorWidget::getCurFileName()
{
    return currentFileName;
}

GLSLCodeEditor* GLSLEditorWidget::getCodeEditor()
{
    return sEditor;
}

void GLSLEditorWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F5)
    {
        updateShaderSource();
    }
}
