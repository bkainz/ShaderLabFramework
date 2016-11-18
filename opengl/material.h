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

#ifndef MATERIAL_H
#define MATERIAL_H

#include <QColor>
#include <iostream>
#include <QVector>

class Material
{
public:
    Material();
    Material(QColor ambientColor, QColor diffuseColor, QColor specularColor, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient, float shininess);

    QColor getAmbientColor();
    QColor getDiffuseColor();
    QColor getSpecularColor();
    float getAmbientCoefficient();
    float getDiffuseCoefficient();
    float getSpecularCoefficient();
    float getShininess();

    void setAmbientColor(QColor color);
    void setDiffuseColor(QColor color);
    void setSpecularColor(QColor color);
    void setAmbientCoefficient(float val);
    void setDiffuseCoefficient(float val);
    void setSpecularCoefficient(float val);
    void setShininess(float val);

private:
    QColor m_ambientColor;
    QColor m_diffuseColor;
    QColor m_specularColor;
    float m_ambientCoefficient;
    float m_diffuseCoefficient;
    float m_specularCoefficient;
    float m_shininess;

};

#endif // MATERIAL_H
