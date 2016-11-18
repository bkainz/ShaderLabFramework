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

#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <iostream>
#include <cmath>

class Light
{
public:
    Light();
    Light(QVector4D lightPosition, QVector3D lightColor, float lightIntensity);
    void setPosition(float x, float y, float z);
    void translateX(float translationX);
    void translateY(float translationY);
    void translateZ(float translationZ);

    void computeSpectrumRGB();

    QVector4D getLightPosition() const;
    QMatrix4x4 getModelMatrix() const;


private:
    QVector4D m_lightPosition;
    QVector3D m_lightColor;
    QMatrix4x4 m_modelMatrix;
    float m_lightIntensity;
};

#endif // LIGHT_H
