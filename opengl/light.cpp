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

#include "opengl/light.h"

using namespace std;

Light::Light() :
    m_lightPosition(QVector4D()), m_lightColor(QVector3D()), m_modelMatrix(QMatrix4x4()), m_lightIntensity()
{

}

Light::Light(QVector4D lightPosition, QVector3D lightColor, float lightIntensity) :
    m_lightPosition(QVector4D(lightPosition)), m_lightColor(QVector3D(lightColor)), m_modelMatrix(QMatrix4x4()),
    m_lightIntensity(lightIntensity)
{

    m_modelMatrix.setToIdentity();

}

void Light::setPosition(float x, float y, float z)
{
    m_lightPosition = QVector4D(x, y, z, 1.0);
    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(x, y, z);
}

void Light::translateX(float translationX)
{
    m_modelMatrix.translate(translationX, 0.0, 0.0);

    float currentPosition = m_lightPosition.x();
    m_lightPosition.setX(currentPosition + translationX);
}

void Light::translateY(float translationY)
{

    m_modelMatrix.translate(0.0, translationY, 0.0);
    float currentPosition = m_lightPosition.y();
    m_lightPosition.setY(currentPosition + translationY);
}

void Light::translateZ(float translationZ)
{
    m_modelMatrix.translate(0.0, 0.0, translationZ);
    float currentPosition = m_lightPosition.z();
    m_lightPosition.setZ(currentPosition + translationZ);
}

QVector4D Light::getLightPosition() const
{
    return m_lightPosition;
}

QMatrix4x4 Light::getModelMatrix() const
{
    return m_modelMatrix;
}

