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

#ifndef __uniformEditorWidget_HPP_INCLUDED__
#define __uniformEditorWidget_HPP_INCLUDED__

#include "opengl/openglheaders.h"
#include "texturewidget.h"
#include "ui_uniformEditorWidget.h"
#include <QVector>
#include <QMatrix4x4>
#include <QGLShaderProgram>

class UniformEditorWidget : public QWidget
{
    Q_OBJECT

public:
    UniformEditorWidget(QGLShaderProgram* sProgram, QGLShaderProgram* dsProgram,
        QOpenGLContext* glContext, QWidget *parent, QOpenGLWidget* glWidget);
    ~UniformEditorWidget();

    //enum uniformType { BOOL, INT, UINT, FLOAT, DOUBLE, VEC2, VEC3, VEC4, MAT3, MAT4, SAMPLER};
    struct mUniform
    {
        QString type;
        QString name;
    };

    void updateShaderPrograms(QGLShaderProgram* sProgram, QGLShaderProgram* dsProgram, QOpenGLContext* glContext);

    //TODO documentation

    // it would have been nice to read the uniforms directly from the program
    // but this seems to be broken in Qt 5
    // need to parse source code manually
    // Qt has some restirctions here, thus this is just a simple Uniform editor

      /**
       * Creates the widgets that are necessary in the uniform editor depending on the type of uniform.
       * index must be a value relative to the uniform list and NOT the QComboBox of the uniform editor.
       * The uniforms are also bound to the correct program shader : the render program shader or the display program (Render to Texture).
       * Sets the connection between a uniform widget and the updateUniform SLOT.
       * @brief setUniformEditorWidgets
       * @param uniformList
       * @param index
       * @param isAShaderProgramUniform
       */
    void setUniformEditorWidgets(QList<mUniform> &uniformList, int index, bool isAShaderProgramUniform);

    public slots:
    void uniformComboBoxActivated(int index);

    //This could probably be simplified to take into account the two shader programs
  //TODO any way to templetize this (GLtype is used for overloads)?
  //Qt does not allow templates for slots
    void updateUniform(double value);
    void updateUniform(int value);
    void updateUniformVector2D(QVector4D value);
    void updateUniformVector3D(QVector4D value);
    void updateUniformVector4D(QVector4D value);
    void updateUniformMatrix3x3(QMatrix4x4 value);
    void updateUniformMatrix4x4(QMatrix4x4 value);

    void updateUniformDisplay(double value);
    void updateUniformDisplay(int value);
    void updateUniformDisplayVector2D(QVector4D value);
    void updateUniformDisplayVector3D(QVector4D value);
    void updateUniformDisplayVector4D(QVector4D value);
    void updateUniformDisplayMatrix3x3(QMatrix4x4 value);
    void updateUniformDisplayMatrix4x4(QMatrix4x4 value);

    /**
     * Update texture path for a sampler2D.
     * @brief updateTexturePath
     * @param filePath
     */
    void updateTexturePath(QString filePath);

signals:
    void updateGL();

    /**
     * Sends a signal to open a QFileDialog to look for the texture.
     * The texture name is the first parameter and the second parameter
     * is true if the texture has to be bound to the shader program or false if it has to be bound to the Render To Texture display program.
     * @brief textureBrowse
     */
    void textureBrowse(QString, bool);

    void updateShaderProgram();
    void updateUniformsSignal();

private:
    void updateEditorWidget();
    QList<mUniform> parseUniformsFromSource(QString sourceCode);

    Ui::UniformEditorWidget* ui;
    QOpenGLWidget* m_glWidget;
    QGLShaderProgram* m_shaderProgram;
    QGLShaderProgram* m_shaderProgramDisplay;
    QList<mUniform> m_shaderProgramUserUniforms; //List of uniforms in the shader program
    QList<mUniform> m_displayShaderUserUniforms; //List of uniforms in the display shader program
    QOpenGLContext* m_glContext;
};

#endif
