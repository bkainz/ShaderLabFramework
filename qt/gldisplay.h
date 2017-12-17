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

#ifndef GLDISPLAY_H
#define GLDISPLAY_H 

#define FRAMEBUFFER_WIDTH 1280
#define MAX_FPS 60.0
#define INITIAL_CAMERA_Z_POSITION 40.0

#include "opengl/material.h"
#include "opengl/object.h"
#include "opengl/light.h"
#include "opengl/scene.h"
#include "opengl/framebuffer.h"
#include "opengl/camera.h"

#include "opengl/openglheaders.h"

#include <QApplication>
#include <QGLWidget>
#include <QMatrix>
#include <QGLShaderProgram>
#include <QVector>
#include <QVector2D>
#include <QSize>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include <QStandardPaths>

#include <QWheelEvent>
#include <QKeyEvent>

#include <string>
#include <sstream>
#include <iostream>
#include <QFileDialog>

class GLSLEditorWindow;

using namespace std;

class GLDisplay : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLDisplay(QWidget *parent);
    virtual ~GLDisplay();

    /**
     * Sets the opengl parameters (e.g wireframe rendering ...)
     * @brief setOpenGLRenderingState
     */
    void setOpenGLRenderingState();
    void setOpenGLWireframeState(bool activateWireframeMode);

    /**
     * Render the coordinate frame
     * @brief renderCoordinateFrame
     */
    void renderCoordinateFrame();

    /**
     * Renders the scene to a FBO.
     * @brief renderScene
     */
    void renderScene();

    /**
     * Renders the textureID on a quad.
     * @brief renderToTexture
     * @param textureID
     */
    void renderToTexture(const int textureID, bool simplifiedPipeline);

    /**
     * Function to load textures and the framebuffers.
     * @brief loadTexturesAndFramebuffers
     */
    void loadTexturesAndFramebuffers();

    /**
     * Sends object properties to shaders.
     * @brief sendObjectDataToShaders
     * @param object
     */
    void sendObjectDataToShaders(Object &object);

    /**
     * Counts and draw the FPS on the screen.
     * @brief drawFPS
     */
    void drawFPS();

    //test
    QGLShaderProgram* getShaderProgram() { return m_shaderProgram; };
    QGLShaderProgram* getShaderDisplayProgram() { return m_shaderProgramDisplay; };
    Scene* getScene() { return m_scene; };

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
     * Update the GLInfo tab.
     * @brief updateGLInfo
     */
    void updateGLInfo(QString);

    void updateModelMatrix(QMatrix4x4);
    void updateViewMatrix(QMatrix4x4);
    void updateProjectionMatrix(QMatrix4x4);
    void updateTexturePath(QString);

    void updateUniformTab();
    void updateMaterialTab();


    public slots:
    void updateCameraType(QString cameraType);
    void updateCameraFieldOfView(double fieldOfView);
    void updateObject(QString object);
    void updateWireframeRendering(bool wireframe);
    void updateBackfaceCulling(bool backface);
    void updateRenderCoordinateFrame(bool renderCoordFrame);
    void modelMatrixUpdated(QMatrix4x4 modelMatrix);
    void viewMatrixUpdated(QMatrix4x4 viewMatrix);
    void projectionMatrixUpdated(QMatrix4x4 projectionMatrix);
    void takeScreenshot();
    void resetMatrices();
    void updateMaterial(int objectID, Material material);
    void linkShaderProgram();

    /**
     * Slot to select a texture file and use it in a shader.
     * @brief setTexture
     * @param name
     */
    void setTexture(QString name, bool isAShaderProgramUniform);

    /**
     * Update the openGL window when the timer reaches the end and restarts the timer.
     * @brief updateOpenGL
     */
    void updateOpenGL();

	void glMessageLogged(QOpenGLDebugMessage m);

protected:
    void initializeGL();
	void reinitGL();
	void resizeGL(int width, int height);
    void paintGL();
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);


private:
    void renderText(double x, double y, const QString &str, const QFont & font = QFont());
   
    //Framebuffer for highres rendering
    FrameBuffer* m_framebuffer;
    FrameBuffer* m_framebufferFinalResult;

    //Camera
    Camera m_cameraScene;
    Camera m_cameraQuad;

    //Mouse events
    QVector2D m_mousePos;

    //Frame per second
    QTime m_timeFPS;
    int m_lastFPSUpdate;
    int m_frameCounter;
    int m_FPS;
    QTimer m_timer;

    //Shaders
    QGLShaderProgram* m_shaderProgram;
    QGLShaderProgram* m_shaderProgramDisplay;

    //Scene
    Scene* m_scene;

    //Textures
    QVector<Texture> m_texturesShaderProgram;
    QVector<std::string> m_textureNamesShaderProgram;
    QVector<Texture> m_texturesDisplayProgram;
    QVector<std::string> m_textureNamesDisplayProgram;

    //Rendering
    bool m_wireframe;
    bool m_backFaceCulling;
    bool m_renderCoordinateFrame;

    //Editor
    GLSLEditorWindow* m_shaderEditor;

    QOpenGLFunctions *f;
    bool m_shaderProgramNeedsLink;

    Object m_R2Tsquare;
    QOpenGLVertexArrayObject m_renderingVAO;
    QOpenGLVertexArrayObject m_R2TVAO;
	QOpenGLDebugLogger logger;
	string m_objectFileName;
};

#endif // GLDISPLAY_H
