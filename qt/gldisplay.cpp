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

#include "qt/gldisplay.h"
#include "qt/GLSLCodeEditor.h"
#include "qt/GLSLEditorWindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QSize>
#include <cstddef>

using namespace std;

GLDisplay::GLDisplay(QWidget *parent) : QOpenGLWidget(parent),
m_cameraScene(Camera()), m_cameraQuad(Camera()),
m_mousePos(0, 0),
m_lastFPSUpdate(0), m_frameCounter(0), m_FPS(0),
m_wireframe(false), m_backFaceCulling(false), m_renderCoordinateFrame(false)
{
	m_objectFileName = "teapot";
    m_shaderProgram = new QGLShaderProgram(this);
    m_shaderProgramDisplay = new QGLShaderProgram(this);

    m_timer.start(1000.0 / MAX_FPS);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateOpenGL()));
    m_shaderProgramNeedsLink = true;

	connect(&logger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(glMessageLogged(QOpenGLDebugMessage)));
}

GLDisplay::~GLDisplay()
{
    delete m_scene;

    delete m_shaderProgram;
    delete m_shaderProgramDisplay;
    delete m_framebuffer;

    delete m_framebufferFinalResult;
    delete m_shaderEditor;

}

void GLDisplay::glMessageLogged(QOpenGLDebugMessage m)
{
	qDebug() << "gl message logged: " << m;
}

void GLDisplay::initializeGL()
{
	logger.initialize();
    f = QOpenGLContext::currentContext()->functions();
    f->initializeOpenGLFunctions();

	m_framebuffer = new FrameBuffer();
	m_framebufferFinalResult = new FrameBuffer();

    QString OpenGLInfo;
    OpenGLInfo = QString("Widget OpenGl: %1.%2\n").arg(format().majorVersion()).arg(format().minorVersion());

    OpenGLInfo += QString("Context valid : %1\n").arg(context()->isValid() ? "true" : "false");
    OpenGLInfo += QString("Really used OpenGl : %1.%2\n").arg(context()->format().majorVersion()).arg(context()->format().minorVersion());
    OpenGLInfo += QString("OpenGl information : \n \tVENDOR :       %1\n").arg((const char*)glGetString(GL_VENDOR));
    OpenGLInfo += QString("\tRENDERER :    %1\n").arg((const char*)glGetString(GL_RENDERER));
    OpenGLInfo += QString("\tVERSION :      %1\n").arg((const char*)glGetString(GL_VERSION));
    OpenGLInfo += QString("\tGLSL VERSION : %1\n").arg((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    emit updateGLInfo(OpenGLInfo);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0, 0, 0, 0);

    m_shaderEditor = new GLSLEditorWindow(m_shaderProgram, m_shaderProgramDisplay, this);
    connect(m_shaderEditor, SIGNAL(updateLog(QString)), this, SIGNAL(updateLog(QString)));
    connect(m_shaderEditor, SIGNAL(displayLog()), this, SIGNAL(displayLog()));
    connect(m_shaderEditor, SIGNAL(updateUniformTab()), this, SIGNAL(updateUniformTab()));
    connect(m_shaderEditor, SIGNAL(updateShaderProgram()), this, SLOT(linkShaderProgram()));

    m_shaderEditor->loadDefaultShaders();

    m_shaderEditor->resize(QDesktopWidget().availableGeometry(this).size().width() * 0.5,
        QDesktopWidget().availableGeometry(this).size().height() * 0.97);
    m_shaderEditor->move(QDesktopWidget().availableGeometry(this).size().width() * 0.5, 0);

    m_shaderEditor->show();

	this->reinitGL();
}


void GLDisplay::reinitGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0, 0, 0, 0);

	qDebug() << "loading scene";

	m_renderingVAO.destroy();
	//Load VAO
	if (!m_renderingVAO.create())
		cerr << "Could not create VAO" << endl;

	m_renderingVAO.bind();
	
	m_scene = new Scene(m_objectFileName);

	m_shaderProgram->enableAttributeArray("vertex_worldSpace");
	m_shaderProgram->enableAttributeArray("textureCoordinate_input");
	m_shaderProgram->enableAttributeArray("normal_worldSpace");
	m_shaderProgram->setAttributeBuffer("vertex_worldSpace", GL_FLOAT, 0, 3, 0);
	m_shaderProgram->setAttributeBuffer("textureCoordinate_input", GL_FLOAT, m_scene->getObjects()[0].getTextureCoordinatesOffset(), 2, 0);
	m_shaderProgram->setAttributeBuffer("normal_worldSpace", GL_FLOAT, m_scene->getObjects()[0].getNormalsOffset(), 3, 0);

	m_renderingVAO.release();

	this->loadTexturesAndFramebuffers();

	//Starts the FPS time
	m_timeFPS.start();

	/*---- Camera initialisation to render----*/
	//The camera that displays the final square is the moving camera
	//Compute the transformation of the camera
	QVector4D positionQuad = QVector4D(0.0, 0.0, 1.0, 1.0);
	QVector4D upVectorQuad = QVector4D(0.0, 1.0, 0.0, 1.0);
	QVector4D centerQuad = QVector4D(0.0, 0.0, 0.0, 1.0);

	//Render the quad without perspective
	m_cameraQuad = Camera(positionQuad, upVectorQuad, centerQuad, false);

	QVector4D positionScene = QVector4D(0.0, 0.0, INITIAL_CAMERA_Z_POSITION, 1.0);
	QVector4D upVectorScene = QVector4D(0.0, 1.0, 0.0, 1.0);
	QVector4D centerScene = QVector4D(0.0, 0.0, 0.0, 1.0);

    m_R2TVAO.destroy();
	if (!m_R2TVAO.create())
		cerr << "Could not create R2T VAO" << endl;

	m_R2TVAO.bind();

	m_R2Tsquare = Object(string("square"));

	//The square is betweem -0.5 and 0.5
	//Scale it by a factor of 2 so that it covers the entire screen (between -1 and 1)
	m_R2Tsquare.scale(2.0);

	m_shaderProgramDisplay->setAttributeBuffer("vertex_worldSpace", GL_FLOAT, 0, 3, 0);
	m_shaderProgramDisplay->setAttributeBuffer("textureCoordinate_input", GL_FLOAT, m_R2Tsquare.getTextureCoordinatesOffset(), 2, 0);
	m_shaderProgramDisplay->setAttributeBuffer("normal_worldSpace", GL_FLOAT, m_R2Tsquare.getNormalsOffset(), 3, 0);

	m_shaderProgramDisplay->enableAttributeArray("vertex_worldSpace");
	m_shaderProgramDisplay->enableAttributeArray("textureCoordinate_input");
	m_shaderProgramDisplay->enableAttributeArray("normal_worldSpace");

	m_R2TVAO.release();

	m_cameraScene = Camera(positionScene, upVectorScene, centerScene, true, (float)m_framebuffer->getWidth() / (float)m_framebuffer->getHeight(), 45.0);
	emit updateViewMatrix(m_cameraScene.getViewMatrix());
	emit updateProjectionMatrix(m_cameraScene.getProjectionMatrix());
	emit(updateMaterialTab());

	const QList<QOpenGLDebugMessage> messages = logger.loggedMessages();
	for (const QOpenGLDebugMessage &message : messages)
		qDebug() << message;

}

void GLDisplay::resizeGL(int width, int height)
{
    //Avoid division by 0
    if (height == 0)
        height = 1;

    //Recompute projection matrix
    m_cameraQuad.setProjectionMatrix((float)width / (float)height, 30.0); //Reset the projection matrix

    //Map the projection to the GLWidget window
    glViewport(0, 0, width, height);

}

void GLDisplay::paintGL()
{
    //Enable depth test
    glEnable(GL_DEPTH_TEST);

    //Render the scene
    f->glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->getFramebufferID());

    //Clear screen
    //Clear the color and the z buffer
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_framebuffer->getWidth(), m_framebuffer->getHeight());

    this->setOpenGLRenderingState();

    if (m_renderCoordinateFrame)
        this->renderCoordinateFrame();

    glClear(GL_DEPTH_BUFFER_BIT);
    //Render the scene
    this->renderScene();

    //Apply one render to texture pass
    f->glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferFinalResult->getFramebufferID());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0, 0, m_framebufferFinalResult->getWidth(), m_framebufferFinalResult->getHeight());

    this->renderToTexture(m_framebuffer->getColorBufferID(0), false);

    /*------ Display the framebuffer on the screen -----*/
    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0, 0, this->width(), this->height());

    //use the simplified pipeline for better speed
    this->renderToTexture(m_framebufferFinalResult->getColorBufferID(0), true);

    this->drawFPS();

	const QList<QOpenGLDebugMessage> messages = logger.loggedMessages();
	for (const QOpenGLDebugMessage &message : messages)
		qDebug() << message;
}

void GLDisplay::setOpenGLWireframeState(bool activateWireframeMode)
{
    //Wireframe rendering
    if (activateWireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}


void GLDisplay::setOpenGLRenderingState()
{
    //Backface culling rendering
    if (m_backFaceCulling)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}

void GLDisplay::renderCoordinateFrame()
{
	//TODO redo for OpenGL 4 -- this is only possible in compatibility profile.
	//DO NOT USE OLD-STYLE OPENGL here
    // IMPORTANT : During the rotation the coordinate frame might not look orthogonal
    // This is due to the rendering window that is not square
    // Expand the window so that it becomes a square and the coordinate frame will be orthogonal

    //Note : The coordinate frame is rendered using old OpenGL (faster to implement and avoids using shaders for such a simple geometry)
    //The coordinate frame has to be rendered in the camera space without the translation (these are directions)
    //Remove the translation from the viewing matrix
    /**
     * View matrix :
     * Right Vector     RX RY RZ 0
     * Up Vector        UX UY UZ 0
     * Center           CX CY CZ 0
     * Camera position  PX PY PZ 1
     * To only get the rotation of the camera set the camera position (PX, PY, PZ) to 0
     */

    float *matrix = m_cameraScene.getViewMatrix().data();

    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = 0.0;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(matrix);

    glLineWidth(5);

    //Render the coordinate frame
    // ! The rotation must be applied when the line is centered at the origin
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(-1.0, 0.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, -1.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);

    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, -1.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();

    glLineWidth(1);
}

void GLDisplay::renderScene()
{
    //Switch to the regular shader program to render the objects
    m_shaderProgram->bind();

    /*---Camera and matrices---*/

    //Matrices for the Frame buffer object
    //The camera is fixed
    QMatrix4x4 projectionScene, viewMatrixScene;
    viewMatrixScene = m_cameraScene.getViewMatrix();
    projectionScene = m_cameraScene.getProjectionMatrix();

    //Setup the openGL pipeline

    /*---load the scene and draw it ---*/

    //Load the scene
    QVector<Object> objectList = m_scene->getObjects();
    QVector<Light> pointLights = m_scene->getPointLightSources();

    //Repeat that for each object
    QMatrix4x4 modelMatrixObject = QMatrix4x4();
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoordinates;
    QVector<GLuint> indicesArray;
    QVector4D lightPosition = pointLights[0].getLightPosition();
    QMatrix4x4 lightModelMatrix = pointLights[0].getModelMatrix();

    for (int k = 0; k < objectList.size(); k++)
    {
        //Get the data
        modelMatrixObject = objectList[k].getModelMatrix();
        vertices = objectList[k].getMesh().getVertices();
        normals = objectList[k].getMesh().getVertexNormals();
        textureCoordinates = objectList[k].getMesh().getTextureCoordinates();
        indicesArray = objectList[k].getMesh().getIndicesArray();

        //Send uniform data to shaders
        //Do the maximum of matrix multiplication on the CPU for better efficiency
        m_shaderProgram->setUniformValue("mMatrix", modelMatrixObject);
        m_shaderProgram->setUniformValue("mvMatrix", viewMatrixScene*modelMatrixObject);
        m_shaderProgram->setUniformValue("pMatrix", projectionScene);
        m_shaderProgram->setUniformValue("normalMatrix", (viewMatrixScene*modelMatrixObject).normalMatrix()); //Normals are in the camera space
        m_shaderProgram->setUniformValue("lightPosition_camSpace", viewMatrixScene*lightPosition); //Light position in the camera space
        m_shaderProgram->setUniformValue("time", m_timeFPS.elapsed()); //Time

        //sendData
        this->sendObjectDataToShaders(objectList[k]);

        //on some platforms Qt and ANGLE require this workaround
        if (m_wireframe)
        {
            //activate wireframe
            setOpenGLWireframeState(true);
        }

        //Draw the current object
         m_renderingVAO.bind();

         glDrawElements(GL_TRIANGLES, indicesArray.size(), GL_UNSIGNED_INT, 0);

         if (m_wireframe)
         {
             //deactivate wireframe!
             setOpenGLWireframeState(false);
         }

         //Unbind the textures
         glBindTexture(GL_TEXTURE_2D, 0);
    }

    m_renderingVAO.release();
    m_shaderProgram->release();
}


void GLDisplay::linkShaderProgram()
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
        if (displayShaderValid) {
            emit(updateUniformTab());
        }
    }

	reinitGL();
}

void GLDisplay::renderToTexture(const int textureId, bool isSimplifiedPipeline)
{
    //Switch to the display shader
    //Always bind before sending the textures to the shader
    if (!m_shaderProgramDisplay->bind())
        cout << "m_shaderProgramDisplay not bound" << endl;

    //The first texture will always be the rendered texture
    GLint textureRenderedId = f->glGetUniformLocation(m_shaderProgramDisplay->programId(), "textureRendered");

    f->glActiveTexture(GL_TEXTURE0);
    f->glUniform1i(textureRenderedId, 0); // 0 is the texture number

    //Bind the texture so that it can be used by the shader
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, textureId);

    if (!isSimplifiedPipeline)
    {
        //Additional textures
        for (int i = 0; i < m_texturesDisplayProgram.size(); ++i)
        {
            //If the texture has been loaded correctly
            if (m_texturesDisplayProgram[i].isTextureLoaded())
            {
                GLint textureId = f->glGetUniformLocation(m_shaderProgramDisplay->programId(), m_textureNamesDisplayProgram[i].c_str());

                f->glActiveTexture(GL_TEXTURE0 + i + 1);
                f->glUniform1i(textureId, i); // 0 is the texture number

                //Bind the texture so that it can be used by the shader
                f->glActiveTexture(GL_TEXTURE0 + i + 1);
                f->glBindTexture(GL_TEXTURE_2D, m_texturesDisplayProgram[i].getTextureId());
            }
        }
    }


    QMatrix4x4 viewMatrixQuad = m_cameraQuad.getViewMatrix();
    QMatrix4x4 projectionMatrixQuad = m_cameraQuad.getProjectionMatrix();

    m_shaderProgramDisplay->setUniformValue("mMatrix", m_R2Tsquare.getModelMatrix());
    m_shaderProgramDisplay->setUniformValue("mvMatrix", viewMatrixQuad*m_R2Tsquare.getModelMatrix());
    m_shaderProgramDisplay->setUniformValue("mvMatrixScene", m_scene->getObjects()[0].getModelMatrix()*m_cameraScene.getViewMatrix());
    m_shaderProgramDisplay->setUniformValue("pMatrix", projectionMatrixQuad);

    //Draw the current object
    m_R2TVAO.bind();

    glDrawElements(GL_TRIANGLES, m_R2Tsquare.getMesh().getIndicesArray().size(), GL_UNSIGNED_INT, 0);

    m_R2TVAO.release();
    m_shaderProgramDisplay->release();
}


void GLDisplay::loadTexturesAndFramebuffers()
{

    //If the FBO has the width and height of the window then the rendering is aliased (too low resolution)
    //The width is imposed to FRAMEBUFFER_WIDTH and the height is calculated to keep the aspec ratio
    float aspectRatio = this->width() / this->height();
    int widthFBO = FRAMEBUFFER_WIDTH;
    int heightFBO = widthFBO / aspectRatio;

    //Create a framebuffer and load it (empty but creates its ID)
    m_framebuffer = new FrameBuffer(widthFBO, heightFBO);
    m_framebuffer->load_8UC3();

    m_framebufferFinalResult = new FrameBuffer(widthFBO, heightFBO);
    m_framebufferFinalResult->load_8UC3();
}

void GLDisplay::sendObjectDataToShaders(Object &object)
{
    Material material = Material();
    material = object.getMaterial();

    //TODO defaults should come and be set in Uniform Editor widget
    //or define a separate material editor and exclude these here

    m_shaderProgram->setUniformValue("ambient", material.getAmbientColor());
    m_shaderProgram->setUniformValue("diffuse", material.getDiffuseColor());
    m_shaderProgram->setUniformValue("specular", material.getSpecularColor());
    m_shaderProgram->setUniformValue("shininess", material.getShininess());
    m_shaderProgram->setUniformValue("ambientCoefficent", material.getAmbientCoefficient());
    m_shaderProgram->setUniformValue("diffuseCoefficent", material.getDiffuseCoefficient());
    m_shaderProgram->setUniformValue("specularCoefficent", material.getSpecularCoefficient());

    for (int i = 0; i < m_texturesShaderProgram.size(); ++i)
    {
        //If the texture has been loaded correctly
        if (m_texturesShaderProgram[i].isTextureLoaded())
        {
            GLint textureId = f->glGetUniformLocation(m_shaderProgram->programId(), m_textureNamesShaderProgram[i].c_str());

            f->glActiveTexture(GL_TEXTURE0 + i);
            f->glUniform1i(textureId, i); // 0 is the texture number

            //Bind the texture so that it can be used by the shader
            f->glActiveTexture(GL_TEXTURE0 + i);
            f->glBindTexture(GL_TEXTURE_2D, m_texturesShaderProgram[i].getTextureId());
        }
    }
}

void GLDisplay::updateMaterial(int objectID, Material material)
{
    m_scene->updateObjectMaterial(objectID, material);
}

void GLDisplay::drawFPS()
{

    ++m_frameCounter;
    int currentTime = m_timeFPS.elapsed();

    //Every 1 second update the number of FPS
    if (currentTime - m_lastFPSUpdate >= 1000)
    {
        m_FPS = m_frameCounter;
        m_frameCounter = 0;
        m_lastFPSUpdate = currentTime;
    }

    QString textFPS = QString("%1 FPS").arg(m_FPS);

    //Set the color to white for to draw the FPS
   // glColor3f(1.0, 1.0, 1.0);
    renderText(width() - textFPS.size() - 60, 20, textFPS);
}

void GLDisplay::renderText(double x, double y, const QString &str, const QFont & font) {
    // Identify x and y locations to render text within widget
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QPen pen(Qt::white);
    painter.setPen(pen);
    QFont serifFont("Times");
    painter.setFont(serifFont);
    painter.drawText(QPoint(x,y), str);
}

/*--------------------------Mouse events-----------------------------------*/
void GLDisplay::wheelEvent(QWheelEvent* event)
{
    int variation = event->delta();

    //Control the Camera if CTRL NOT pressed
    if (event->orientation() == Qt::Vertical && !(QApplication::keyboardModifiers() == Qt::ControlModifier))
    {
        m_cameraScene.translateAlongViewAxis(-(float)variation / 100.0);
        emit updateViewMatrix(m_cameraScene.getViewMatrix());
        qApp->processEvents();
    }

    //Control the light if CTRL is pressed
    if (event->orientation() == Qt::Vertical && (QApplication::keyboardModifiers() == Qt::ControlModifier))
    {
        m_scene->translateLightSourceZ(0, variation / 1000.0);
    }

    update();
    event->accept();
}

void GLDisplay::mousePressEvent(QMouseEvent *event)
{
    //When the mouse is pressed, save its position
    m_mousePos = QVector2D(event->pos().x(), event->pos().y());
    event->accept();
}

void GLDisplay::mouseMoveEvent(QMouseEvent *event)
{
    // static float rotationX = 0.0;
    // static float rotationY = 0.0;
     //Mouse move camera
     //CTRL+mouse move light

     //When the mouse is moved after it has been pressed,
     //calculate the number of pixels it has moved and rotate the scene
    if (event->buttons() == Qt::LeftButton && !(QApplication::keyboardModifiers() == Qt::ControlModifier))
    {
        //from camera view inverse rotation
        float rotationX = 2.0*(m_mousePos.y() - event->pos().y());
        float rotationY = 2.0*(m_mousePos.x() - event->pos().x());
        m_cameraScene.rotateX(-rotationX);
        m_cameraScene.rotateY(-rotationY);
        emit updateViewMatrix(m_cameraScene.getViewMatrix());
    }
    else if (event->buttons() == Qt::RightButton && !(QApplication::keyboardModifiers() == Qt::ControlModifier))
    {
        float translationX = 100.0*(m_mousePos.x() - event->pos().x()) / this->width();
        float translationY = 100.0*(m_mousePos.y() - event->pos().y()) / this->height();

        m_cameraScene.translateX(-translationX);
        m_cameraScene.translateY(translationY);
        emit updateViewMatrix(m_cameraScene.getViewMatrix());
    }
    else if (event->buttons() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        m_scene->translateLightSourceX(0, (m_mousePos.x() - event->pos().x()) / 100.0);
        m_scene->translateLightSourceY(0, (m_mousePos.y() - event->pos().y()) / 100.0);
    }

    //Update openGL
    update();

    //Update the position of the mouse
    m_mousePos = QVector2D(event->pos().x(), event->pos().y());
    event->accept();
}

void GLDisplay::keyPressEvent(QKeyEvent *event)
{

    //Quick translation of light source
    if (event->key() == Qt::Key_Z)
    {
        m_scene->translateLightSourceZ(0, -5.0);
    }

    if (event->key() == Qt::Key_X)
    {
        m_scene->translateLightSourceZ(0, 5.0);
    }

    //Reset scene and animation
    if (event->key() == Qt::Key_D)
    {
        //Reset the camera
        m_cameraScene.resetCamera();

        emit updateViewMatrix(m_cameraScene.getViewMatrix());

        //Reset the scene
        m_scene->resetScene();

        emit updateModelMatrix(m_scene->getObjects()[0].getModelMatrix());
        qDebug() << "Reset scene" << endl;
    }

    update();//Update openGL
    event->accept();
}

/*--------------------------Slots-----------------------------------*/
void GLDisplay::updateCameraType(QString cameraType)
{
    m_cameraScene.changeCameraType(cameraType);
    emit updateProjectionMatrix(m_cameraScene.getProjectionMatrix());
    update();//Update openGL
}

void GLDisplay::updateCameraFieldOfView(double fieldOfView)
{
    //Changes the field of view if the camera is a perspective camera
    m_cameraScene.setProjectionMatrix((float)m_framebuffer->getWidth() / (float)m_framebuffer->getHeight(), fieldOfView);
    emit updateProjectionMatrix(m_cameraScene.getProjectionMatrix());
    update();//Update openGL
}

void GLDisplay::updateObject(QString object)
{
    if (object == "Square")
    {
		m_objectFileName = "square";
    }
    else if (object == "Cube")
    {
		m_objectFileName = "cube";
    }
    else if (object == "Monkey")
    {
		m_objectFileName = "monkey";
    }
    else if (object == "Teapot")
    {
		m_objectFileName = "teapot";
    }
    else if (object == "Teapot low res")
    {
		m_objectFileName = "teapot-low";
    }

	this->linkShaderProgram();
	//this->reinitGL();

    emit(updateMaterialTab());
    update();//Update openGL
}

void GLDisplay::updateWireframeRendering(bool wireframe)
{
    m_wireframe = wireframe;
    update();//Update openGL
}

void GLDisplay::updateBackfaceCulling(bool backface)
{
    m_backFaceCulling = backface;
    update();//Update openGL
}

void GLDisplay::updateRenderCoordinateFrame(bool renderCoordFrame)
{
    m_renderCoordinateFrame = renderCoordFrame;
    update();
}

void GLDisplay::modelMatrixUpdated(QMatrix4x4 modelMatrix)
{
    m_scene->setModelMatrix(0, modelMatrix);
    update();//Update openGL
}


void GLDisplay::viewMatrixUpdated(QMatrix4x4 viewMatrix)
{
    m_cameraScene.setViewMatrix(viewMatrix);
    update();//Update openGL
}

void GLDisplay::projectionMatrixUpdated(QMatrix4x4 projectionMatrix)
{
    m_cameraScene.setProjectionMatrix(projectionMatrix);
    update();//Update openGL
}

void GLDisplay::takeScreenshot()
{
	QImage screenshot0 = this->grabFramebuffer();

	QDate currentDate = QDate::currentDate();
	QTime currentTime = QTime::currentTime();
	QString username;
#ifdef WIN32
	username = qgetenv("USERNAME");
#else
	username = qgetenv("USER");
#endif
	if (username.isEmpty())
	{
		QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
		username = homePath.first().split(QDir::separator()).last();
	}

	QPainter p;
	if (!p.begin(&screenshot0)) qDebug() << "setting QPainter failed! "; 

	p.setPen(QPen(Qt::white));
	p.setFont(QFont("Times", 8, QFont::Normal));
	p.drawText(screenshot0.rect(), Qt::AlignBottom, username + QString(" ") + 
			currentDate.toString() + QString(" ") + currentTime.toString());
	p.end();

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		QDir::currentPath(),
		tr("Images (*.jpg)"));
	screenshot0.save(fileName);
	QString text = QString("Screenshot saved : %1\n\n").arg(fileName);
	emit updateLog(text);
	emit displayLog();
}

void GLDisplay::resetMatrices()
{
    QVector4D positionScene = QVector4D(0.0, 0.0, INITIAL_CAMERA_Z_POSITION, 1.0);
    QVector4D upVectorScene = QVector4D(0.0, 1.0, 0.0, 1.0);
    QVector4D centerScene = QVector4D(0.0, 0.0, 0.0, 1.0);

    m_cameraScene = Camera(positionScene, upVectorScene, centerScene, true, (float)m_framebuffer->getWidth() / (float)m_framebuffer->getHeight(), 45.0);
    emit updateViewMatrix(m_cameraScene.getViewMatrix());
    emit updateProjectionMatrix(m_cameraScene.getProjectionMatrix());

    QMatrix4x4 identity;
    identity.setToIdentity();
    m_scene->setModelMatrix(0, identity);
    emit updateModelMatrix(identity);
    update();//Update openGL
}

void GLDisplay::setTexture(QString name, bool isAShaderProgramUniform)
{
    //Let the user choose a file
    QString chosenFile = QFileDialog::getOpenFileName(this,
        tr("Choose texture"),
        QDir::currentPath(),
        QString(tr("All Images files (*.jpg *.jpeg *.png *.bmp *.tif);;JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp);;TIFF (*.tif)")));

    if (!chosenFile.isEmpty()) {
        emit updateTexturePath(chosenFile);

        Texture newTexture(chosenFile.toStdString());
        bool loaded = newTexture.load_8UC3();

        if (!loaded)
        {
            QString error = QString("Could not load texture : %1\n\n").arg(chosenFile);
            emit updateLog(error);
            emit displayLog();
        }
        else
        {
            if (isAShaderProgramUniform)
            {
                m_texturesShaderProgram.push_back(newTexture);
                m_textureNamesShaderProgram.push_back(name.toStdString());
            }
            else
            {
                m_texturesDisplayProgram.push_back(newTexture);
                m_textureNamesDisplayProgram.push_back(name.toStdString());
            }

            QString text = QString("Texture correctly loaded : %1\n\n").arg(chosenFile);
            emit updateLog(text);
        }
    }
}

void GLDisplay::updateOpenGL()
{
    m_timer.start(1000.0 / MAX_FPS);
    update();
}
