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

#include "opengl/camera.h"

using namespace std;

Camera::Camera() :
    m_fieldOfView(0.0),
    m_perspectiveCamera(false), m_aspectRatio(1.0), m_viewMatrix(QMatrix4x4()), m_projectionMatrix(QMatrix4x4())
{
    m_viewMatrix.setToIdentity();
    m_projectionMatrix.setToIdentity();
}

Camera::Camera(QVector4D position, QVector4D upVector, QVector4D center,
    bool perspectiveCamera, float aspectRatio, float fieldOfView) :
    m_fieldOfView(fieldOfView),
    m_perspectiveCamera(perspectiveCamera), m_aspectRatio(aspectRatio), m_viewMatrix(QMatrix4x4()), m_projectionMatrix(QMatrix4x4())
{
    //Initialise matrices
    m_viewMatrix.setToIdentity();
    m_projectionMatrix.setToIdentity();

    m_viewMatrix.lookAt(position.toVector3D(), center.toVector3D(), upVector.toVector3D());

    if (m_perspectiveCamera)//If it is a perspective camera
        m_projectionMatrix.perspective(fieldOfView, aspectRatio, (float)0.001, (float)10000.0);
    else
        m_projectionMatrix.ortho(-1.0, 1.0, -1.0, 1.0, 0.001, 10000.0);
}

void Camera::setViewMatrix(QVector4D position, QVector4D upVector, QVector4D center)
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(position.toVector3D(), center.toVector3D(), upVector.toVector3D());
}

void Camera::setViewMatrix(QMatrix4x4 &viewMatrix)
{
    m_viewMatrix = viewMatrix;
}


void Camera::setProjectionMatrix(float aspectRatio, float fieldOfView)
{
    m_fieldOfView = fieldOfView;
    m_aspectRatio = aspectRatio;

    //Perspective function multiplies by a matrix : need to reinitialize the matrix to identity first
    m_projectionMatrix.setToIdentity();

    if (m_perspectiveCamera)//If it is a perspective camera
        m_projectionMatrix.perspective(fieldOfView, aspectRatio, (float)0.001, (float)10000.0);
    else
        m_projectionMatrix.ortho(-1.0, 1.0, -1.0, 1.0, 0.001, 10000.0);

}

void Camera::setProjectionMatrix(QMatrix4x4 &projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
}

void Camera::changeCameraType(QString cameraType)
{
    //Resets the projection matrix
    m_projectionMatrix.setToIdentity();

    if (cameraType == "Perspective")
    {
        m_projectionMatrix.perspective(m_fieldOfView, m_aspectRatio, (float)0.001, (float)10000.0);
        m_perspectiveCamera = true;
    }
    else
    {
        m_projectionMatrix.ortho(-1.0, 1.0, -1.0, 1.0, 0.001, 10000.0);
        m_perspectiveCamera = false;
    }
}

void Camera::rotateX(float xRotation)
{
    m_viewMatrix.rotate(xRotation, 1.0, 0.0, 0.0);
}

void Camera::rotateY(float yRotation)
{
    m_viewMatrix.rotate(yRotation, 0.0, 1.0, 0.0);
}

void Camera::rotateXY(float xRotation, float yRotation)
{
    this->rotateX(xRotation);
    this->rotateY(yRotation);
}

void Camera::translateAlongViewAxis(float translation)
{
    m_viewMatrix(2, 3) += translation;
}


void Camera::translateX(float translation)
{
    //Translate in camera space
    m_viewMatrix(0, 3) += translation;
}

void Camera::translateY(float translation)
{
    //Translate in camera space
    m_viewMatrix(1, 3) += translation;
}

void Camera::translateZ(float translation)
{
    m_viewMatrix.translate(0.0, 0.0, translation);
}

void Camera::resetCamera()
{
    m_viewMatrix.setToIdentity();
}

QMatrix4x4 Camera::getViewMatrix()
{
    return m_viewMatrix;
}

QMatrix4x4 Camera::getProjectionMatrix()
{
    return m_projectionMatrix;
}

bool Camera::isPerspective()
{
    return m_perspectiveCamera;
}
