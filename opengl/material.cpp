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

#include "opengl/material.h"

using namespace std;

Material::Material() : m_ambientColor(QColor()), m_diffuseColor(QColor()), m_specularColor(QColor()), m_ambientCoefficient(0), m_diffuseCoefficient(0), m_specularCoefficient(0), m_shininess(0)
{

}

Material::Material(QColor ambient, QColor diffuseColor, QColor specularColor, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient, float shininess) :
    m_ambientColor(QColor(ambient)), m_diffuseColor(QColor(diffuseColor)), m_specularColor(QColor(specularColor)),
    m_ambientCoefficient(ambientCoefficient), m_diffuseCoefficient(diffuseCoefficient), m_specularCoefficient(specularCoefficient), m_shininess(shininess)
{

}

QColor Material::getAmbientColor()
{
    return m_ambientColor;
}

QColor Material::getDiffuseColor()
{
    return m_diffuseColor;
}

QColor Material::getSpecularColor()
{
    return m_specularColor;
}

float Material::getAmbientCoefficient()
{
    return m_ambientCoefficient;
}

float Material::getDiffuseCoefficient()
{
    return m_diffuseCoefficient;
}

float Material::getSpecularCoefficient()
{
    return m_specularCoefficient;
}

float Material::getShininess()
{
    return m_shininess;
}


void Material::setAmbientColor(QColor color)
{
    m_ambientColor = color;
}

void Material::setDiffuseColor(QColor color)
{
    m_diffuseColor = color;
}

void Material::setSpecularColor(QColor color)
{
    m_specularColor = color;
}

void Material::setAmbientCoefficient(float val)
{
    m_ambientCoefficient = val;
}

void Material::setDiffuseCoefficient(float val)
{
    m_diffuseCoefficient = val;
}

void Material::setSpecularCoefficient(float val)
{
    m_specularCoefficient = val;
}

void Material::setShininess(float val)
{
    m_shininess = val;
}

