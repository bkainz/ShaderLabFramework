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

#include "opengl/texture.h"
#include <QDir>

using namespace std;

Texture::Texture() : m_textureId(0), m_filePath(""), m_width(0), m_height(0), m_numberOfComponents(0), m_isTextureLoaded(false)
{

}

Texture::Texture(string filePath) : m_textureId(0), m_filePath(filePath), m_width(0), m_height(0), m_numberOfComponents(0), m_isTextureLoaded(false)
{

}

Texture::Texture(int width, int height, int numberOfcomponents) : m_textureId(0), m_filePath(string("")), m_width(width), m_height(height), m_numberOfComponents(numberOfcomponents), m_isTextureLoaded(false)
{

}

Texture::~Texture()
{

}

void Texture::loadEmptyTexture_8UC3()
{
    //remove an eventual previous picture from the memory
    if (glIsTexture(m_textureId) == GL_TRUE)
    {
        glDeleteTextures(1, &m_textureId);
    }

    //Generate a new texture ID
    glGenTextures(1, &m_textureId);

    //Bind the texture and start working on it
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    //Allocate memory for a width*height texture but without data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);

    //Smooth close textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //Do not smooth textures that are far away (performance optimisation)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_isTextureLoaded = true;
}

void Texture::loadEmptyTexture_32FC3()
{
    //remove an eventual previous picture from the memory
    if (glIsTexture(m_textureId) == GL_TRUE)
    {
        glDeleteTextures(1, &m_textureId);
    }

    //Generate a new texture ID
    glGenTextures(1, &m_textureId);

    //Bind the texture and start working on it
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    //Allocate memory for a width*height texture but without data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);

    //Smooth close textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //Do not smooth textures that are far away (performance optimisation)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_isTextureLoaded = true;
}

//Untested
/**
 * Loads the texture from the given file name (file name sets by the constructor).
 * Texture is converted to 8bits 3 channels.
 * @brief Texture::load_8UC3
 * @return
 */
bool Texture::load_8UC3()
{
    //Load the texture in BGR format
    QString imagePath = QString("%1").arg(m_filePath.c_str());

    QImage texture = QImage(imagePath);

    //If the texture cannot be loaded
    if (texture.isNull())
    {
        cout << "Could not load the texture : " << QDir::currentPath().toStdString() + m_filePath << endl;
        return false;
    }
    else
    {
        //The texture has to be inverted as the coordinate system for the (u,v) coordinates and the QImage are different
        QImage mirroredTexture = texture.mirrored();

        m_width = mirroredTexture.width();
        m_height = mirroredTexture.height();

        //Convert image to pixels [in 0;255]
        mirroredTexture.convertToFormat(QImage::Format_RGB888);

        //Generate the texture id
        glGenTextures(1, &m_textureId);

        //Bind a texture 2D to the texture
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        //Send the data to the memory
        //The first GL_RGB says that the data used will be in the RGB format
        // !!!!!!!!!!!!!!!!!!!!!! GL_RGB clamps the texture between 0 and 1 range.
        // To use floats above 1 : use GL_RGB32F
        //The second one says that texture.data is stored in BGR format (OpenCV)
        QImage textureOpenGL = QGLWidget::convertToGLFormat(mirroredTexture);
        m_numberOfComponents = 4;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureOpenGL.bits());

        //Smooth close textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //Do not smooth textures that are far away (performance optimisation)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Repeat the texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //Unbind
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //Texture correctly loaded
    m_isTextureLoaded = true;
    return true;
}

void Texture::setFilePath(string filePath)
{
    this->m_filePath = filePath;
}

GLuint Texture::getTextureId() const
{
    return m_textureId;
}

float Texture::getAspectRatio() const
{
    return (float)m_width / (float)m_height;
}

int Texture::getWidth() const
{
    return m_width;
}

int Texture::getHeight() const
{
    return m_height;
}

bool Texture::isTextureLoaded() const
{
    return m_isTextureLoaded;
}
