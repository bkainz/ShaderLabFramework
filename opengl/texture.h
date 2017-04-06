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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>

#include <QApplication>
#include <QImage>

#include "opengl/openglheaders.h"
#include <QGLWidget>

class Texture
{
public:

    Texture();
    Texture(std::string filePath);
    Texture(int width, int height, int numberOfcomponents);

    ~Texture();

    /**
     * Load an ampty texture with a (width,height,  numberOfComponents)
     * @brief loadEmptyTexture_8UC3
     */
    void loadEmptyTexture_8UC3();

    /**
     * Loads an empty texture with a (width,height,  numberOfComponents) with 32 bits float for each channel.
     * @brief loadEmptyTexture_32FC3
     */
    void loadEmptyTexture_32FC3();

    /**
     * Load textures saved as 8 bits images.
     * @brief load_8UC3
     * @return
     */
    bool load_8UC3();


    /**
     * Set the m_filePath of the texture.
     * @brief setFilePath
     * @param m_filePath
     */
    void setFilePath(std::string filePath);

    /**
     * Get the texture id
     * @brief getTextureId
     * @return
     */
    GLuint getTextureId() const;

    /**
     * Get the aspect ratio.
     * @brief getAspectRatio
     * @return
     */
    float getAspectRatio() const;

    int getWidth() const;
    int getHeight() const;
    bool isTextureLoaded() const;


private:
    GLuint m_textureId;
    std::string m_filePath;

    int m_width;
    int m_height;
    int m_numberOfComponents;

    bool m_isTextureLoaded;

};

#endif // TEXTURE_H
