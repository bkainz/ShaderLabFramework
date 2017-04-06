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

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "opengl/openglheaders.h"
#include "opengl/texture.h"

#include <iostream>
#include <string>
#include <vector>

#include <QApplication>

class FrameBuffer
{
public:
    FrameBuffer();
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void createRenderBuffer(GLuint &id, GLenum format);

    /**
     * Load a framebuffer with a color buffer of 8 bits.
     * @brief load_8UC3
     * @return
     */
    bool load_8UC3();

    /**
     * Load a framebuffer with a color buffer of 32 bits floats.
     * @brief load_32FC3
     * @return
     */
    bool load_32FC3();

    GLuint getFramebufferID() const;
    GLuint getColorBufferID(unsigned int index) const;

    int getWidth() const;
    int getHeight() const;

signals:

    public slots :

private:
    GLuint m_framebufferId;
    int m_width;
    int m_height;

    //A framebuffer contains a color buffer, a depth buffer and a stencil buffer
    std::vector<Texture> m_colourBuffers;
    GLuint m_depthBufferId;

    QOpenGLFunctions *f;
};

#endif // FRAMEBUFFER_H
