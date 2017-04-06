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

#include "qt/uniformEditorWidget.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpacerItem>
#include "Vector4Widget.h"
#include "Matrix4x4Widget.h"
#include <QOpenGLContext>
#include <QGLFunctions>

UniformEditorWidget::UniformEditorWidget(QGLShaderProgram* sProgram, QGLShaderProgram* dsProgram,
    QOpenGLContext* glContext, QWidget *parent, QOpenGLWidget* glWidget) : QWidget(parent), m_glWidget(glWidget), ui(new Ui::UniformEditorWidget)
{
    ui->setupUi(this);
    m_glContext = glContext;
    updateShaderPrograms(sProgram, dsProgram, glContext);
    QObject::connect(ui->m_uniformComboBox, SIGNAL(activated(int)),
        this, SLOT(uniformComboBoxActivated(int)));

    if (m_shaderProgramUserUniforms.length() > 0)
    {
        uniformComboBoxActivated(0); // init to first one.
    }

    if (m_displayShaderUserUniforms.length() > 0)
    {
        uniformComboBoxActivated(0); // init to first one.
    }
}

UniformEditorWidget::~UniformEditorWidget()
{
    delete ui;
}

void UniformEditorWidget::updateShaderPrograms(QGLShaderProgram* sProgram, QGLShaderProgram* dsProgram, QOpenGLContext* glContext)
{
    //qDebug() << "updateShaderPrograms";
    m_shaderProgram = sProgram;
    m_shaderProgramDisplay = dsProgram;
    m_glContext = glContext;
    updateEditorWidget();
}

void UniformEditorWidget::updateEditorWidget()
{
    //qDebug() << "updateEditorWidget() " << m_shaderProgram->shaders().length();
      //Clear the vectors
    m_shaderProgramUserUniforms.clear();
    m_displayShaderUserUniforms.clear();

    if (m_shaderProgram->isLinked())
    {
        for (int i = 0; i < m_shaderProgram->shaders().length(); i++) //if linked? e.g. geometry shader
        {
            m_shaderProgramUserUniforms.append(parseUniformsFromSource(m_shaderProgram->shaders().at(i)->sourceCode()));
        }
    }

    if (m_shaderProgramDisplay->isLinked())
    {
        for (int i = 0; i < m_shaderProgramDisplay->shaders().length(); i++) //if linked? e.g. geometry shader
        {
            m_displayShaderUserUniforms.append(parseUniformsFromSource(m_shaderProgramDisplay->shaders().at(i)->sourceCode()));
        }
    }

    //Add the items in the combo box
    ui->m_uniformComboBox->clear();
    for (int i = 0; i < m_shaderProgramUserUniforms.size(); i++)
    {
        ui->m_uniformComboBox->addItem(m_shaderProgramUserUniforms.at(i).name);
    }

    for (int i = 0; i < m_displayShaderUserUniforms.size(); i++)
    {
        ui->m_uniformComboBox->addItem(m_displayShaderUserUniforms.at(i).name);
    }

    if (m_shaderProgramUserUniforms.length() > 0)
    {
        uniformComboBoxActivated(0); // init to fist one.
    }

    if (m_displayShaderUserUniforms.length() > 0)
    {
        uniformComboBoxActivated(0); // init to fist one.
    }

    //TODO
    //this is not stable due to some obscure Qt 5 reasons!
    //works in Qt 4 though
    //we need to parse manually
    /*
    m_glContext->makeCurrent();
    QGLFunctions fncs(m_glContext);

    if (m_shaderProgram->isLinked())
    {
      //TODO get list of uniforms
      //m_shaderProgram->programId();
      int total = -1;
      fncs.glGetProgramiv(m_shaderProgram->programId(), GL_ACTIVE_UNIFORMS, &total);
      qDebug() << total;
      for (int i = 0; i < total; ++i)
      {
        GLsizei nameLen = -1;
        int num = -1;
        GLenum type = GL_ZERO;
        char name[100];
        fncs->glGetActiveUniform(m_shaderProgram->programId(), i, sizeof(name) - 1,
          &nameLen, &num, &type, name);

        name[nameLen] = 0;

        qDebug() << name;//glsl_var_type(shaderParamType(type), name, num);
      }
    }
  */

}

void UniformEditorWidget::uniformComboBoxActivated(int index)
{
    QString currentText = ui->m_uniformComboBox->currentText();

    bool isAShaderProgramUniform = false;

    //Find if it is a shader program uniform or a display program uniform
    for (int i = 0; i < m_shaderProgramUserUniforms.size(); ++i)
    {
        //index==i to take into account that the same name can be used in both the shaderProgram and the display program
        if (m_shaderProgramUserUniforms[i].name == currentText && index == i)
        {
            isAShaderProgramUniform = true;
            break;
        }
    }

    //qDebug("activated(%d), currentIndex() = %d, uniform %s", index, currentIndex, m_allUserUniforms.at(index).name.toStdString().c_str());

    if (ui->m_UniformEditFrame->layout() != NULL)
    {
        QLayoutItem* item;
        while ((item = ui->m_UniformEditFrame->layout()->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
    }

    //Set the uniform widgets corresponding ot the uniforms in the list.
    //Relative index assumes that the display program uniforms come at the end of the QComboBox
    int relativeIndex = 0;
    if (isAShaderProgramUniform)
    {
        relativeIndex = index;
        this->setUniformEditorWidgets(m_shaderProgramUserUniforms, relativeIndex, isAShaderProgramUniform);
    }
    else
    {
        relativeIndex = index - m_shaderProgramUserUniforms.size();
        this->setUniformEditorWidgets(m_displayShaderUserUniforms, relativeIndex, isAShaderProgramUniform);
    }
}

void UniformEditorWidget::setUniformEditorWidgets(QList<mUniform> &uniformList, int index, bool isAShaderProgramUniform)
{
    if (uniformList.length() > 0)
    {
        QLabel* textLabel = new QLabel(ui->m_UniformEditFrame);
        textLabel->setText(uniformList.at(index).name);
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(textLabel, 0, 0);
    }

//    QGLFunctions fncs(m_glContext);

    //TODO nicify this
    if (uniformList.at(index).type == QString("bool"))
    {
        QCheckBox* cbox = new QCheckBox(ui->m_UniformEditFrame);
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(cbox, 0, 1);

        if (isAShaderProgramUniform)
        {
            connect(cbox, SIGNAL(stateChanged(int)), this, SLOT(updateUniform(int)));
        }
        else
        {
            connect(cbox, SIGNAL(stateChanged(int)), this, SLOT(updateUniformDisplay(int)));
        }
    }

    if (uniformList.at(index).type == QString("int"))
    {
        QSpinBox* sBox = new QSpinBox(ui->m_UniformEditFrame);
        sBox->setRange(INT_MIN, INT_MAX);
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(sBox, 0, 1);

        if (isAShaderProgramUniform)
        {
            connect(sBox, SIGNAL(valueChanged(int)), this, SLOT(updateUniform(int)));
        }
        else
        {
            connect(sBox, SIGNAL(valueChanged(int)), this, SLOT(updateUniformDisplay(int)));
        }
    }

    if (uniformList.at(index).type == QString("uint"))
    {
        QSpinBox* sBox = new QSpinBox(ui->m_UniformEditFrame);
        sBox->setRange(0, INT_MAX); //to comply with Qt signals no UINT_MAX
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(sBox, 0, 1);

        if (isAShaderProgramUniform)
        {
            connect(sBox, SIGNAL(valueChanged(int)), this, SLOT(updateUniform(int)));
        }
        else
        {
            connect(sBox, SIGNAL(valueChanged(int)), this, SLOT(updateUniformDisplay(int)));
        }
    }

    if (uniformList.at(index).type == QString("float") || uniformList.at(index).type == QString("double"))
    {
        QDoubleSpinBox* sBox = new QDoubleSpinBox(ui->m_UniformEditFrame);
        sBox->setSingleStep(0.1);
        sBox->setDecimals(4);
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(sBox, 0, 1);

        if (isAShaderProgramUniform)
        {
            connect(sBox, SIGNAL(valueChanged(double)), this, SLOT(updateUniform(double)));
        }
        else
        {
            connect(sBox, SIGNAL(valueChanged(double)), this, SLOT(updateUniformDisplay(double)));
        }
    }

    //TODO vecs and matrices
    if (uniformList.at(index).type == QString("vec2"))
    {
        Vector4Widget* v2Box = new Vector4Widget(ui->m_UniformEditFrame);
        v2Box->set2D();
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(v2Box, 1, 0);

        if (isAShaderProgramUniform)
        {
            connect(v2Box, SIGNAL(vectorChanged(QVector4D)), this, SLOT(updateUniformVector2D(QVector4D)));
        }
        else
        {
            connect(v2Box, SIGNAL(vectorChanged(QVector4D)), this, SLOT(updateUniformDisplayVector2D(QVector4D)));
        }
    }

    if (uniformList.at(index).type == QString("vec3"))
    {
        Vector4Widget* v3Box = new Vector4Widget(ui->m_UniformEditFrame);
        v3Box->set3D();
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(v3Box, 1, 0);

        if (isAShaderProgramUniform)
        {
            connect(v3Box, SIGNAL(vectorChanged(QVector4D)), this, SLOT(updateUniformVector3D(QVector4D)));
        }
        else
        {
            connect(v3Box, SIGNAL(vectorChanged(QVector4D)), this, SLOT(updateUniformDisplayVector3D(QVector4D)));
        }
    }

    if (uniformList.at(index).type == QString("vec4"))
    {
        Vector4Widget* v4Box = new Vector4Widget(ui->m_UniformEditFrame);
        v4Box->set4D();
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(v4Box, 1, 0);

        //TODO -- with separate material editor we don't need to sync. Only user uniforms are processed here
#if 0
        if (m_shaderProgram->isLinked())
        {
            QVector4D temp;
            fncs.glGetUniformfv(m_shaderProgram->programId(),
                m_shaderProgram->uniformLocation(uniformList.at(index).name.toStdString().c_str()), &temp[0]);
            qDebug() << uniformList.at(index).name << ": " << temp;
            v4Box->updateSpinBoxes(temp);
        }

#endif

        if (isAShaderProgramUniform)
        {
            connect(v4Box, SIGNAL(vectorChanged(QVector4D)), this, SLOT(updateUniformVector4D(QVector4D)));
        }
        else
        {
            connect(v4Box, SIGNAL(vectorChanged(QVector4D)), this, SLOT(updateUniformDisplayVector4D(QVector4D)));
        }
    }

    if (uniformList.at(index).type == QString("mat3"))
    {
        Matrix4x4Widget* m3Box = new Matrix4x4Widget(ui->m_UniformEditFrame);
        m3Box->set3x3();
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(m3Box, 1, 0);

        if (isAShaderProgramUniform)
        {
            connect(m3Box, SIGNAL(matrixChanged(QMatrix4x4)), this, SLOT(updateUniformMatrix3x3(QMatrix4x4)));
        }
        else
        {
            connect(m3Box, SIGNAL(matrixChanged(QMatrix4x4)), this, SLOT(updateUniformDisplayMatrix3x3(QMatrix4x4)));
        }
    }

    if (uniformList.at(index).type == QString("mat4"))
    {
        Matrix4x4Widget* m4Box = new Matrix4x4Widget(ui->m_UniformEditFrame);
        m4Box->set4x4();
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(m4Box, 1, 0);

        if (isAShaderProgramUniform)
        {
            connect(m4Box, SIGNAL(matrixChanged(QMatrix4x4)), this, SLOT(updateUniformMatrix4x4(QMatrix4x4)));
        }
        else
        {
            connect(m4Box, SIGNAL(matrixChanged(QMatrix4x4)), this, SLOT(updateUniformDisplayMatrix4x4(QMatrix4x4)));
        }
    }

    if (uniformList.at(index).type == QString("sampler2D"))
    {
        TextureWidget* textureWidget = new TextureWidget(ui->m_UniformEditFrame, uniformList.at(index).name,
            isAShaderProgramUniform);

        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addWidget(textureWidget, 1, 0);
        connect(textureWidget, SIGNAL(sendTextureName(QString, bool)), this, SIGNAL(textureBrowse(QString, bool)));

    }

    if (uniformList.length() > 0)
    {
        QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->addItem(spacer, 1, 1);
    }
}

//TODO need to to this somewhere else, idally before the render pass 
//TODO any way to templetize this (GLtype is used for overloads)?
//Qt does not allow templates for slots
void UniformEditorWidget::updateUniform(double value)
{
    //qDebug() << "updateUniformFloatDouble(double value) " <<
    //	m_allUserUniforms.at(ui->m_uniformComboBox->currentIndex()).name.toStdString().c_str() << " " << value;

    m_glWidget->makeCurrent();
    m_shaderProgram->bind();
    m_shaderProgram->setUniformValue(m_shaderProgramUserUniforms.at(ui->m_uniformComboBox->currentIndex()).name.toStdString().c_str(),
        (GLfloat)value);
    m_shaderProgram->release();

    emit(updateGL());
}

void UniformEditorWidget::updateUniform(int value)
{
    m_glWidget->makeCurrent();
    m_shaderProgram->bind();
    m_shaderProgram->setUniformValue(m_shaderProgramUserUniforms.at(ui->m_uniformComboBox->currentIndex()).name.toStdString().c_str(), (GLint)value);
    m_shaderProgram->release();
    emit(updateGL());
}


void UniformEditorWidget::updateUniformVector2D(QVector4D value)
{
    m_glWidget->makeCurrent();
    m_shaderProgram->bind();
    m_shaderProgram->setUniformValue(m_shaderProgramUserUniforms.at(ui->m_uniformComboBox->currentIndex()).name.toStdString().c_str(), value.toVector2D());
    m_shaderProgram->release();
    emit(updateGL());
}

void UniformEditorWidget::updateUniformVector3D(QVector4D value)
{
    m_glWidget->makeCurrent();
    m_shaderProgram->bind();
    m_shaderProgram->setUniformValue(m_shaderProgramUserUniforms.at(ui->m_uniformComboBox->currentIndex()).name.toStdString().c_str(), value.toVector3D());
    m_shaderProgram->release();
    emit(updateGL());
}

void UniformEditorWidget::updateUniformVector4D(QVector4D value)
{
    //qDebug() << "updateUniformVector4D " << value;
    m_glWidget->makeCurrent();
    m_shaderProgram->bind();
    m_shaderProgram->setUniformValue(m_shaderProgramUserUniforms.at(ui->m_uniformComboBox->currentIndex()).name.toStdString().c_str(), value);
    m_shaderProgram->release();
    emit(updateGL());
}


void UniformEditorWidget::updateUniformMatrix3x3(QMatrix4x4 value)
{
    QMatrix3x3 mat;
    mat.setToIdentity();
    for (int col = 0; col < 3; col++)
    {
        for (int row = 0; row < 3; row++)
        {
            mat(row, col) = value(row, col);
        }
    }

    m_glWidget->makeCurrent();
    m_shaderProgram->bind();
    m_shaderProgram->setUniformValue(m_shaderProgramUserUniforms.at(ui->m_uniformComboBox->currentIndex()).name.toStdString().c_str(), value);
    m_shaderProgram->release();
    emit(updateGL());
}


void UniformEditorWidget::updateUniformMatrix4x4(QMatrix4x4 value)
{
    m_glWidget->makeCurrent();
    m_shaderProgram->bind();
    m_shaderProgram->setUniformValue(m_shaderProgramUserUniforms.at(ui->m_uniformComboBox->currentIndex()).name.toStdString().c_str(), value);
    m_shaderProgram->release();
    emit(updateGL());

}

void UniformEditorWidget::updateUniformDisplay(double value)
{
    //qDebug() << "updateUniformFloatDouble(double value) " <<
    //	m_allUserUniforms.at(ui->m_uniformComboBox->currentIndex()).name.toStdString().c_str() << " " << value;

    //Relative index assumes that the display program uniforms come at the end of the QComboBox
    int relativeIndex = ui->m_uniformComboBox->currentIndex() - m_shaderProgramUserUniforms.size();

    m_glWidget->makeCurrent();
    m_shaderProgramDisplay->bind();
    m_shaderProgramDisplay->setUniformValue(m_displayShaderUserUniforms.at(relativeIndex).name.toStdString().c_str(), (GLfloat)value);
    m_shaderProgramDisplay->release();

    emit(updateGL());
}

void UniformEditorWidget::updateUniformDisplay(int value)
{
    //Relative index assumes that the display program uniforms come at the end of the QComboBox
    int relativeIndex = ui->m_uniformComboBox->currentIndex() - m_shaderProgramUserUniforms.size();

    m_glWidget->makeCurrent();
    m_shaderProgramDisplay->bind();
    m_shaderProgramDisplay->setUniformValue(m_displayShaderUserUniforms.at(relativeIndex).name.toStdString().c_str(), (GLint)value);
    m_shaderProgramDisplay->release();
    emit(updateGL());
}


void UniformEditorWidget::updateUniformDisplayVector2D(QVector4D value)
{
    //Relative index assumes that the display program uniforms come at the end of the QComboBox
    int relativeIndex = ui->m_uniformComboBox->currentIndex() - m_shaderProgramUserUniforms.size();

    m_glWidget->makeCurrent();
    m_shaderProgramDisplay->bind();
    m_shaderProgramDisplay->setUniformValue(m_displayShaderUserUniforms.at(relativeIndex).name.toStdString().c_str(), value.toVector2D());
    m_shaderProgramDisplay->release();
    emit(updateGL());
}

void UniformEditorWidget::updateUniformDisplayVector3D(QVector4D value)
{
    //Relative index assumes that the display program uniforms come at the end of the QComboBox
    int relativeIndex = ui->m_uniformComboBox->currentIndex() - m_shaderProgramUserUniforms.size();

    m_glWidget->makeCurrent();
    m_shaderProgramDisplay->bind();
    m_shaderProgramDisplay->setUniformValue(m_displayShaderUserUniforms.at(relativeIndex).name.toStdString().c_str(), value.toVector3D());
    m_shaderProgramDisplay->release();
    emit(updateGL());
}

void UniformEditorWidget::updateUniformDisplayVector4D(QVector4D value)
{
    //Relative index assumes that the display program uniforms come at the end of the QComboBox
    int relativeIndex = ui->m_uniformComboBox->currentIndex() - m_shaderProgramUserUniforms.size();

    m_glWidget->makeCurrent();
    m_shaderProgramDisplay->bind();
    m_shaderProgramDisplay->setUniformValue(m_displayShaderUserUniforms.at(relativeIndex).name.toStdString().c_str(), value);
    m_shaderProgramDisplay->release();
    emit(updateGL());
}


void UniformEditorWidget::updateUniformDisplayMatrix3x3(QMatrix4x4 value)
{
    QMatrix3x3 mat;
    mat.setToIdentity();
    for (int col = 0; col < 3; col++)
    {
        for (int row = 0; row < 3; row++)
        {
            mat(row, col) = value(row, col);
        }
    }

    //Relative index assumes that the display program uniforms come at the end of the QComboBox
    int relativeIndex = ui->m_uniformComboBox->currentIndex() - m_shaderProgramUserUniforms.size();

    m_glWidget->makeCurrent();
    m_shaderProgramDisplay->bind();
    m_shaderProgramDisplay->setUniformValue(m_displayShaderUserUniforms.at(relativeIndex).name.toStdString().c_str(), value);
    m_shaderProgramDisplay->release();
    emit(updateGL());
}


void UniformEditorWidget::updateUniformDisplayMatrix4x4(QMatrix4x4 value)
{
    //Relative index assumes that the display program uniforms come at the end of the QComboBox
    int relativeIndex = ui->m_uniformComboBox->currentIndex() - m_shaderProgramUserUniforms.size();

    m_glWidget->makeCurrent();
    m_shaderProgramDisplay->bind();
    m_shaderProgramDisplay->setUniformValue(m_displayShaderUserUniforms.at(relativeIndex).name.toStdString().c_str(), value);
    m_shaderProgramDisplay->release();
    emit(updateGL());

}

QList<UniformEditorWidget::mUniform> UniformEditorWidget::parseUniformsFromSource(QString sourceCode)
{
    QList<mUniform> uniforms;

    //TODO
    //this is a workaround for unstable direkt GL access in Qt 5
    //we get the uniform names directly from the source code

    //remove all comments from code
      //This line does not work for :  uniform matrix4x4 a //My comment
      //Every uniform after that will be removed!
      //QString cleanCode =  sourceCode.remove(QRegExp("/\\*.*\\*/")).remove(QRegExp("/\\/.*\\\n"));

    //qDebug() << sourceCode;
    //extract uniforms
    QStringList  list = sourceCode.split(QRegExp("\n|\r|;")).filter("uniform");

    //handle uniforms
    for (int i = 0; i < list.length(); i++)
    {
        QStringList lineList = list.at(i).split(" ");
        mUniform uniform;
        uniform.type = lineList.at(1);
        uniform.name = lineList.at(2);
        //sort out the fixed uniforms and only leave user-variables
        if (uniform.name == QString("mvMatrix") || uniform.name == QString("pMatrix") ||
            uniform.name == QString("normalMatrix") || uniform.name == QString("lightPosition_camSpace") ||
            uniform.name == QString("diffuseColor") || uniform.name == QString("specularColor") ||
            uniform.name == QString("textureRendered") || uniform.name == QString("ambient") ||
            uniform.name == QString("diffuse") || uniform.name == QString("specular") ||
            uniform.name == QString("shininess") || 
            uniform.name == QString("ambientCoefficent") || uniform.name == QString("diffuseCoefficent") ||
            uniform.name == QString("specularCoefficent") ||  uniform.name == QString("time"))
        {
            continue;
        }

        uniforms.push_back(uniform);

        //qDebug() << lineList.at(1);
        //qDebug() << lineList.at(2);
    }

    //qDebug() << "\n";

    return uniforms;
}


void UniformEditorWidget::updateTexturePath(QString filePath)
{
    TextureWidget* textureWidget = static_cast<TextureWidget*>(static_cast<QGridLayout*>(ui->m_UniformEditFrame->layout())->itemAtPosition(1, 0)->widget());
    textureWidget->updatePath(filePath);

    emit(updateGL());

}
