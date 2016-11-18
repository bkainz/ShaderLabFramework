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

#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

#include <string>
#include <sstream>
#include <iostream>

class Camera
{
public:
    Camera();
    Camera(QVector4D position, QVector4D upVector, QVector4D center, bool perspectiveCamera = true, float aspectRatio = 16.0 / 9.0, float fieldOfView = 30.0);

    /**
     * Sets the view matrix so that the camera is at position, looks at the center with a upVector.
     * @brief setViewMatrix
     * @param position
     * @param upVector
     * @param center
     */
    void setViewMatrix(QVector4D position, QVector4D upVector, QVector4D center);


    /**
     * Sets the view matrix to a given value.
     * @brief setViewMatrix
     * @param viewMatrix
     */
    void setViewMatrix(QMatrix4x4 &viewMatrix);

    /**
     * Sets the projection matrix given an aspect ratio and a field of view (if perspective camera, orthographic camera otherwise).
     * @brief setProjectionMatrix
     * @param aspectRatio
     * @param fieldOfView
     */
    void setProjectionMatrix(float aspectRatio, float fieldOfView);

    /**
     * Sets the projection matrix to a given value.
     * @brief setProjectionMatrix
     * @param aspectRatio
     * @param fieldOfView
     */
    void setProjectionMatrix(QMatrix4x4 &viewMatrix);

    /**
     * Changes the camera type between perspective and orthographic.
     * @brief changeCameraType
     * @param cameraType
     */
    void changeCameraType(QString cameraType);

    /**
     * Rotates the camera along the X axis
     * @brief rotateX
     * @param xRotation
     */
    void rotateX(float xRotation);

    /**
     * Rotates the camera along the Y axis
     * @brief rotateY
     * @param yRotation
     */
    void rotateY(float yRotation);

    /**
     * Rotates the camera along the X and Y axis
     * @brief rotateXY
     * @param xRotation
     * @param yRotation
     */
    void rotateXY(float xRotation, float yRotation);

    /**
    * Translation along the axis cameraPosition-ViewCenter
    * @brief translateAlongViewAxis
    * @param zTranslation
    */
    void translateAlongViewAxis(float zTranslation);

    /**
     * Translate the camera along the X axis.
     * @brief translateX
     * @param translation
     */
    void translateX(float translation);

    /**
     * Translate the camera along the Y axis.
     * @brief translateY
     * @param translation
     */
    void translateY(float translation);

    /**
    * Translate the camera along the Z axis.
    * @brief translateZ
    * @param translation
    */
    void translateZ(float translation);

    /**
     * Resets the camera position.
     * @brief resetCamera
     */
    void resetCamera();

    QMatrix4x4 getViewMatrix();
    QMatrix4x4 getProjectionMatrix();

    /**
     * Returns true if the camera is a perspective camera.
     * @brief isPerspective
     * @return
     */
    bool isPerspective();

private:
    float m_fieldOfView;

    bool m_perspectiveCamera;
    float m_aspectRatio;

    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;

};

#endif // CAMERA_H
