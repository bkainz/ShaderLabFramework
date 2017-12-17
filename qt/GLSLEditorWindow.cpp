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

#include "GLSLEditorWindow.h"
#include "qt/GLSLCodeEditor.h"
#include "qt/GLSLEditorWidget.h"
#include <QDir>
#include <QFileDialog>
#include <QWidget>
#include <QMessageBox>
#include <QSettings>
#include <QApplication>
#include <QDesktopWidget>
#include <QShortcut>

GLSLEditorWindow::GLSLEditorWindow(QGLShaderProgram* sProgram, QGLShaderProgram* dsProgram, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::GLSLEditorWindow)
{
    ui->setupUi(this);
    m_shaderProgram = sProgram;
    m_shaderProgramDisplay = dsProgram;
    pipelineFileName = QString();

    readSettings();

    setupTabs();

    connect(ui->actionSave_pipeline, SIGNAL(triggered()), this, SLOT(savePipelineAction()));
    connect(ui->actionSave_pipeline_As, SIGNAL(triggered()), this, SLOT(savePipelineAsAction()));
    connect(ui->actionLoad_pipeline, SIGNAL(triggered()), this, SLOT(loadPipeline()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadFromFileAction()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveToFileAction()));
    connect(ui->actionSave_As_, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionExit_Ctrl_X, SIGNAL(triggered()), this, SLOT(exitApplicationAction()));

    new QShortcut(QKeySequence(Qt::Key_F3), this, SLOT(savePipelineAction()));

    //connect(textEdit->document(), &QTextDocument::contentsChanged,
    //	this, &MainWindow::documentWasModified);
}

GLSLEditorWindow::~GLSLEditorWindow()
{
    delete ui;
}

void GLSLEditorWindow::setupTabs()
{
    QList<QGLShader *> shaders = m_shaderProgram->shaders();

    //setup tabs
    for (int i = 0; i < shaders.length(); i++)
    {
        GLSLEditorWidget* sEditor = new GLSLEditorWidget(shaders.at(i), this);
        if (shaders.at(i)->shaderType() == QGLShader::Vertex)
        {
            ui->EditorTabWidget->addTab(sEditor, "Vertex Shader");
            sEditor->setLinkToProgram(true);
            sEditor->setObjectName("VertexShader");
        }
        else if (shaders.at(i)->shaderType() == QGLShader::Geometry)
        {
            ui->EditorTabWidget->addTab(sEditor, "Geometry Shader");
            sEditor->setLinkToProgram(false);
            sEditor->setObjectName("GeometryShader");
        }
        else if (shaders.at(i)->shaderType() == QGLShader::Fragment)
        {
            ui->EditorTabWidget->addTab(sEditor, "Fragment Shader");
            sEditor->setLinkToProgram(true);
            sEditor->setObjectName("FragmentShader");
        }
        connect(sEditor, SIGNAL(compileAndLink()), this, SLOT(compileAndLink()));
        connect(sEditor, SIGNAL(updateLog(QString)), this, SIGNAL(updateLog(QString)));
        connect(sEditor, SIGNAL(displayLog()), this, SIGNAL(displayLog()));


    }

    QList<QGLShader *> displayShaders = m_shaderProgramDisplay->shaders();
    //setup additional tabs
    for (int i = 0; i < displayShaders.length(); i++)
    {
        GLSLEditorWidget* dsEditor = new GLSLEditorWidget(displayShaders.at(i), this);
        if (displayShaders.at(i)->shaderType() == QGLShader::Vertex)
        {
            ui->EditorTabWidget->addTab(dsEditor, "R2T Vertex Shader");
            dsEditor->setLinkToProgram(true);
            dsEditor->setObjectName("R2TVertexShader");
        }
        else if (displayShaders.at(i)->shaderType() == QGLShader::Fragment)
        {
            ui->EditorTabWidget->addTab(dsEditor, "R2T Fragment Shader");
            dsEditor->setLinkToProgram(true);
            dsEditor->setObjectName("R2TFragmentShader");
        }
        connect(dsEditor, SIGNAL(compileAndLink()), this, SLOT(compileAndLink()));
        connect(dsEditor, SIGNAL(updateLog(QString)), this, SIGNAL(updateLog(QString)));
        connect(dsEditor, SIGNAL(displayLog()), this, SIGNAL(displayLog()));
    }


}

void GLSLEditorWindow::loadDefaultShaders()
{
    for (int i = ui->EditorTabWidget->count(); i > -1; --i)
    {
        ui->EditorTabWidget->removeTab(i);
    }

    m_shaderProgram->removeAllShaders();
    m_shaderProgramDisplay->removeAllShaders();

    //TODO -- default shaders inline
    QString stdVert("#version 410 \n\n\
in vec3 vertex_worldSpace;\n\
in vec3 normal_worldSpace;\n\
in vec2 textureCoordinate_input;\n\
\n\
uniform mat4 mvMatrix;\n\
uniform mat4 pMatrix;\n\
uniform mat3 normalMatrix; //mv matrix without translation\n\
\n\
uniform vec4 lightPosition_camSpace; //light Position in camera space\n\
\n\
uniform vec4 ambient;\n\
uniform vec4 diffuse;\n\
uniform vec4 specular;\n\
uniform float shininess;\n\
uniform float ambientCoefficent;\n\
uniform float diffuseCoefficent;\n\
uniform float specularCoefficent;\n\
\n\
out data\n\
{\n\
  vec4 position_camSpace;\n\
  vec3 normal_camSpace;\n\
  vec2 textureCoordinate;\n\
  vec4 color;\n\
}vertexInOut;\n\
\n\
//Vertex shader compute the vectors per vertex\n\
void main(void)\n\
{\n\
  //Put the vertex in the correct coordinate system by applying the model view matrix\n\
  vec4 vertex_camSpace = mvMatrix*vec4(vertex_worldSpace,1.0f); \n\
  vertexInOut.position_camSpace = vertex_camSpace;\n\
  \n\
  //Apply the model-view transformation to the normal (only rotation, no translation)\n\
  //Normals put in the camera space\n\
  vertexInOut.normal_camSpace = normalize(normalMatrix*normal_worldSpace);\n\
  \n\
  //we need to make sure that the normals and texture coordinates\n\
  //aren't optimised away, \n\
  //so we have to use them somehow.\n\
  //Uniforms and array objects that are nor used for \n\
  //the final output(!) are  removed during \n\
  //glsl compilation regardless if you assign them. \n\
  vec4 workaround = \n\
		vec4((vertexInOut.normal_camSpace.x + textureCoordinate_input.x)*0.0001, 0, 0, 1);\n\
  \n\
  //forwarding pure red as RGBA color\n\
  //Try to use the normals as RGB color or the texture coordiantes!\n\
  vertexInOut.color = vec4(1.0, 0.0, 0.0, 1.0);\n\
  \n\
  //a negligible contribution from normals and texcoords is added \n\
  //to ensure these array objects are not optimsed away \n\
  vertexInOut.color += workaround;\n\
  \n\
  //Texture coordinate\n\
  vertexInOut.textureCoordinate = textureCoordinate_input;\n\
  \n\
  gl_Position = pMatrix * vertex_camSpace;\n\
}");
    QString stdGeom("#version 410 \n\n\
layout(triangles) in;\n\
layout(triangle_strip, max_vertices = 3) out;\n\
\n\
uniform mat4 mvMatrix;\n\
uniform mat4 pMatrix;\n\
uniform mat3 normalMatrix; //mv matrix without translation\n\
\n\
uniform vec4 lightPosition_camSpace; //light Position in camera space\n\
\n\
uniform int time;\n\
\n\
in data\n\
{\n\
  vec4 position_camSpace;\n\
  vec3 normal_camSpace;\n\
  vec2 textureCoordinate;\n\
  vec4 color;\n\
}vertexIn[3];\n\
\n\
out fragmentData\n\
{\n\
  vec4 position_camSpace;\n\
  vec3 normal_camSpace;\n\
  vec2 textureCoordinate;\n\
  vec4 color;\n\
} frag;\n\
\n\
void main() {\n\
  for (int i = 0; i < 3; i++) { // You used triangles, so it's always 3\n\
    gl_Position = gl_in[i].gl_Position;\n\
    frag.position_camSpace = vertexIn[i].position_camSpace;\n\
    frag.normal_camSpace = vertexIn[i].normal_camSpace;\n\
    frag.textureCoordinate = vertexIn[i].textureCoordinate;\n\
    frag.color = vertexIn[i].color;\n\
	EmitVertex();\n\
  }\n\
  EndPrimitive();\n\
 }");
    QString stdFrag("#version 410\n\n\
uniform vec4 ambient;\n\
uniform vec4 diffuse;\n\
uniform vec4 specular;\n\
uniform float shininess;\n\
uniform float ambientCoefficent;\n\
uniform float diffuseCoefficent;\n\
uniform float specularCoefficent;\n\
\n\
uniform vec4 lightPosition_camSpace; //light Position in camera space\n\
\n\
in fragmentData\n\
{\n\
  vec4 position_camSpace;\n\
  vec3 normal_camSpace;\n\
  vec2 textureCoordinate;\n\
  vec4 color;\n\
} frag;\n\
\n\
out vec4 fragColor; \n\
\n\
//Fragment shader computes the final color\n\
void main(void)\n\
{\n\
  fragColor =  frag.color;\n\
}");

    m_shaderProgram->addShaderFromSourceCode(QGLShader::Vertex, stdVert);
    m_shaderProgram->addShaderFromSourceCode(QGLShader::Geometry, stdGeom);
    m_shaderProgram->addShaderFromSourceCode(QGLShader::Fragment, stdFrag);

    QString texStdVert("#version 410\n\n\
layout(location = 0) in vec4 vertex_worldSpace;\n\
uniform mat4 mvMatrix;\n\
uniform mat4 pMatrix;\n\
\n\
//in vec4 vertex_worldSpace;\n\
in vec2 textureCoordinate_input;\n\
\n\
out vec2 varyingTextureCoordinate;\n\
\n\
//Vertex shader compute the vectors per vertex\n\
void main(void)\n\
{\n\
  //Put the vertex in the correct coordinate system by applying the model view matrix\n\
  vec4 vertex_camSpace = mvMatrix*vertex_worldSpace;\n\
\n\
  varyingTextureCoordinate = textureCoordinate_input;\n\
  gl_Position = pMatrix * vertex_camSpace;\n\
}");
    QString texStdFrag("#version 410\n\n\
uniform sampler2D textureRendered;\n\
\n\
in vec2 varyingTextureCoordinate;\n\
\n\
out vec4 fragColor;\n\
\n\
void main(void)\n\
{\n\
  //Render the texture on a quad\n\
  fragColor = texture(textureRendered, varyingTextureCoordinate.st);\n\
}");

    m_shaderProgramDisplay->addShaderFromSourceCode(QGLShader::Vertex, texStdVert);
    m_shaderProgramDisplay->addShaderFromSourceCode(QGLShader::Fragment, texStdFrag);

    setupTabs();
    linkShader();
}

void GLSLEditorWindow::linkShader()
{
    bool displayShaderValid = false;
    if (!m_shaderProgramDisplay->link())
    {
        QString error = m_shaderProgramDisplay->log();
        emit updateLog(error);
        emit displayLog();
    }
    else
    {
        //TODO shader names from file menu actual file names. Include geom shader
        //QString text = QString("Compilation of vertex shader : %1.vsh successfull.\n\n").arg("Your Shader");
        //text += QString("Compilation of fragment shader : %1.fsh successfull.\n\n").arg("Your Shader");
        //text += QString("________________________________________________\n\n");
        QString text = QString("Linking of shader program successful.");
        emit updateLog(text);
        emit displayLog();
        displayShaderValid = true;
    }

    if (!m_shaderProgram->link())
    {
        QString error = m_shaderProgram->log();
        emit updateLog(error);
        emit displayLog();
    }
    else
    {
        //QString text = QString("Compilation of render to texture vertex shader : %1.vsh successfull.\n\n").arg("Display Shader");
        //text += QString("Compilation of render to texture fragment shader : %1.fsh successfull.\n\n").arg("Display Shader");
        //text += QString("________________________________________________\n\n");
        QString text = QString("Linking of display shader program successful.\n");
        text += QString("________________________________________________\n");
        emit updateLog(text);
        emit displayLog();
        if (displayShaderValid) {
            emit(updateUniformTab());
        }
    }
}

void GLSLEditorWindow::compileAndLink()
{
    for (int i = 0; i < ui->EditorTabWidget->count(); i++)
    {
        GLSLEditorWidget* sEdit = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->widget(i));
        //qDebug() << sEdit->objectName();
        if (sEdit->getLinkToProgram())
        {
            if (sEdit->objectName() == QString("VertexShader") ||
                sEdit->objectName() == QString("GeometryShader") ||
                sEdit->objectName() == QString("FragmentShader"))
            {
                m_shaderProgram->addShader(sEdit->getShader());
                //qDebug() << "m_shaderProgram";
            }
            if (sEdit->objectName() == QString("R2TVertexShader") ||
                sEdit->objectName() == QString("R2TFragmentShader"))
            {
                m_shaderProgramDisplay->addShader(sEdit->getShader());
                //qDebug() << "m_shaderProgramDisplay";
            }
        }
    }

    //TODO needs to be done in context widget??
    linkShader();
    emit(updateShaderProgram());
    //static_cast<QGLWidget*>(this->parent())->update();
}

/////////////////////////////////////////////////////////////
//File IO

void GLSLEditorWindow::documentWasModified()
{
    GLSLEditorWidget* sEdit = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->currentWidget());
    setWindowModified(sEdit->getCodeEditor()->document()->isModified());
}

void GLSLEditorWindow::loadFile(QString &fileName)
{
    GLSLEditorWidget* sEdit = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->currentWidget());

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
            tr("Cannot read file %1:\n%2.")
            .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QString shaderCode = in.readAll();
    sEdit->setShaderCode(shaderCode);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    sEdit->setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool GLSLEditorWindow::savePipelineAsAction()
{
    QStringList filters;
    filters << "ShaderLab pipeline definitions (*.xml)";

    QFileDialog dialog(this);
    dialog.setNameFilters(filters);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return savePipeline(dialog.selectedFiles().first());
}

bool GLSLEditorWindow::savePipelineAction()
{
    if (pipelineFileName.isEmpty()) {
        return savePipelineAsAction();
    }
    else {
        return savePipeline(pipelineFileName);
    }

}

bool GLSLEditorWindow::savePipeline(QString& fileName)
{
    GLSLEditorWidget* vertexEditor = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->widget(0));
    GLSLEditorWidget* geomEditor = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->widget(1));
    GLSLEditorWidget* fragEditor = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->widget(2));
    GLSLEditorWidget* R2TVertEditor = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->widget(3));
    GLSLEditorWidget* T2TFragEditor = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->widget(4));

    // Make sure the fileName has the correct extension
    if (!fileName.endsWith(QString(".xml"))) {
        fileName.append(QString(".xml"));
    }

    QString text = QString("Saving pipeline to %1").arg(fileName);
    emit updateLog(text);
    emit displayLog();

    if (fileName.isEmpty())
    {
        pipelineFileName = QFileDialog::getSaveFileName(this,
            tr("Save Pipeline"), "",
            tr("XML file (*.xml);"));
    }
    else
    {
        pipelineFileName = fileName;
    }

    if (!pipelineFileName.isEmpty())
    {
        QFile file(pipelineFileName);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            //Could not write the file
            QMessageBox::warning(this, tr("Application"),
                tr("Cannot write file %1:\n%2.")
                .arg(QDir::toNativeSeparators(pipelineFileName),
                    file.errorString()));
            return false;
        }



        QTextStream out(&file);
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        //Stores the pipeline in an xml file.
        //Save the text in between <![CDATA[\n as the text my contain special characters.
        out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
        out << "<pipeline>\n";
        out << "<vertex>\n";
        out << "<![CDATA[";
        out << vertexEditor->getShaderCode();
        out << "\n]]>";
        out << "</vertex>\n";

        out << "<geom>\n";
        out << "<![CDATA[";
        out << geomEditor->getShaderCode();
        out << "\n]]>";
        out << "</geom>\n";

        out << "<frag>\n";
        out << "<![CDATA[";
        out << fragEditor->getShaderCode();
        out << "\n]]>";
        out << "</frag>\n";

        out << "<R2TVert>\n";
        out << "<![CDATA[";
        out << R2TVertEditor->getShaderCode();
        out << "\n]]>";
        out << "</R2TVert>\n";

        out << "<R2TFrag>\n";
        out << "<![CDATA[";
        out << T2TFragEditor->getShaderCode();
        out << "\n]]>";
        out << "</R2TFrag>\n";
        out << "</pipeline>\n";

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif

        statusBar()->showMessage(tr("File saved"), 2000);
    }
    return true;
}

bool GLSLEditorWindow::loadPipeline()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Pipeline"), "",
        tr("XML file (*.xml);; All Files (*)"));

    if (!fileName.isEmpty())
    {

        QDomDocument dom = QDomDocument("Pipeline");
        QFile xmlDocument(fileName);

        //Open the XML document
        if (!xmlDocument.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"),
                xmlDocument.errorString());
            return false;
        }

        //Associate the xml document to DOM
        if (!dom.setContent(&xmlDocument))
        {
            //If it did not work
            xmlDocument.close();
            statusBar()->showMessage(tr("Could not load the XML document"), 2000);
            return false;
        }


        QDomElement domElement = dom.documentElement();
        QDomNode node = domElement.firstChild();

        //Read the child one by one
        int i = 0;
        while (!node.isNull())
        {
            //Current node
            QDomElement element = node.toElement();

            GLSLEditorWidget* sEdit = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->widget(i));
            QString shaderCode = element.text();
            sEdit->setShaderCode(shaderCode);
            sEdit->updateShaderSource();
            node = node.nextSibling();
            ++i;
        }

        //Close the document
        xmlDocument.close();
    }
    pipelineFileName = fileName;
    return true;
}

void GLSLEditorWindow::loadFromFileAction()
{
    //TODO
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool GLSLEditorWindow::saveFile(const QString &fileName)
{
    GLSLEditorWidget* sEdit = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->currentWidget());

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
            tr("Cannot write file %1:\n%2.")
            .arg(QDir::toNativeSeparators(fileName),
                file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << sEdit->getShaderCode();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    sEdit->setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

bool GLSLEditorWindow::save()
{
    GLSLEditorWidget* sEdit = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->currentWidget());

    if (sEdit->getCurFileName().isEmpty()) {
        return saveAs();
    }
    else {
        return saveFile(sEdit->getCurFileName());
    }
}

bool GLSLEditorWindow::saveAs()
{
    QStringList filters;
    GLSLEditorWidget* sEdit = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->currentWidget());
    if (sEdit->getShader()->shaderType() == QGLShader::Vertex)
    {
        filters << "Vertex shader (*.vert)";
    }
    else if (sEdit->getShader()->shaderType() == QGLShader::Geometry)
    {
        filters << "Geometry shader (*.geom)";
    }
    else if (sEdit->getShader()->shaderType() == QGLShader::Fragment)
    {
        filters << "Fragment shader (*.frag)";
    }

    filters << "Text files (*.txt)"
        << "Any files (*)";

    QFileDialog dialog(this);
    dialog.setNameFilters(filters);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

bool GLSLEditorWindow::saveToFileAction()
{
    GLSLEditorWidget* sEdit = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->currentWidget());

    if (sEdit->getCurFileName().isEmpty()) {
        return saveAs();
    }
    else {
        return saveFile(sEdit->getCurFileName());
    }
}


void GLSLEditorWindow::exitApplicationAction()
{
    this->close();
}

bool GLSLEditorWindow::maybeSave()
{
    GLSLEditorWidget* sEdit = static_cast<GLSLEditorWidget*>(ui->EditorTabWidget->currentWidget());
    if (!sEdit->getCodeEditor()->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
            tr("The shader code has been modified.\n"
                "Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;

}

void GLSLEditorWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        qApp->closeAllWindows();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void GLSLEditorWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

void GLSLEditorWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = qApp->desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
            (availableGeometry.height() - height()) / 2);
    }
    else {
        restoreGeometry(geometry);
    }
}

void GLSLEditorWindow::about()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("About");
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText("This is the <b>Computer Graphics Shader Lab. </b> \
		<p>\
		This tool was inspired by 'Shader Maker', a Computer Graphics teaching tool developed by the team of Prof.Dr.Gabriel Zachmann, \
		University of Bremen(<a href = 'http://cgvr.cs.uni-bremen.de/teaching/shader_maker/'> \
		http://cgvr.cs.uni-bremen.de/teaching/shader_maker/</a>). The Copyright holders of 'Shader Maker' granted \
		permission to use their code. The GLSL shader editor syntax highlighter classes are based on the implementation provided by 'Shader Maker'. \
	<p> \
		Copyright(c) 2016 Bernhard Kainz(b.kainz@imperial.ac.uk), Antoine S Toisoul(antoine.toisoul13@imperial.ac.uk) \
		<p> \
		Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation \
		files(the ''Software''), to deal in the Software without restriction, including without limitation the rights to use, copy, \
		modify, merge, publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the \
		Software is furnished to do so, subject to the following conditions : \
	<p> \
		The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. \
		<p> \
		THE SOFTWARE IS PROVIDED ''AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO \
		THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE \
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, \
		TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
    msgBox.exec();
}
