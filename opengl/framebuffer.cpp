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

#include "opengl/framebuffer.h"

using namespace std;

FrameBuffer::FrameBuffer() : m_framebufferId(0), m_width(0), m_height(0),
m_colourBuffers(vector<Texture>()), m_depthBufferId(0)
{
    f = QOpenGLContext::currentContext()->functions();
}

FrameBuffer::FrameBuffer(int width, int height) : m_framebufferId(0), m_width(width), m_height(height),
m_colourBuffers(vector<Texture>()), m_depthBufferId(0)
{
    f = QOpenGLContext::currentContext()->functions();
}

FrameBuffer::~FrameBuffer()
{
    f->glDeleteFramebuffers(1, &m_framebufferId);
    f->glDeleteRenderbuffers(1, &m_depthBufferId);
    m_colourBuffers.clear();

}


void FrameBuffer::createRenderBuffer(GLuint &id, GLenum format)
{
    //Delete any previous buffer
    if (f->glIsRenderbuffer(id) == GL_TRUE)
    {
        //1 is the number of id to delete
        f->glDeleteRenderbuffers(1, &id);
    }

    //Generate a new id for the renderbuffer of the Framebuffer
    f->glGenRenderbuffers(1, &id);

    //Start working with the renderbuffer : bing
    f->glBindRenderbuffer(GL_RENDERBUFFER, id);

    //Create renderbuffer with same width and height
    f->glRenderbufferStorage(GL_RENDERBUFFER, format, m_width, m_height);

    //Stop working with it
    f->glBindRenderbuffer(GL_RENDERBUFFER, 0);

}

bool FrameBuffer::load_8UC3()
{

    if (f->glIsFramebuffer(m_framebufferId) == GL_TRUE)
    {
        f->glDeleteFramebuffers(1, &m_framebufferId);
        m_colourBuffers.clear(); //Empty colour buffers
    }

    //Generate ID
    f->glGenFramebuffers(1, &m_framebufferId);

    //Create renderbuffer with same width and height
    f->glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);

    //Colorbuffer
    Texture colorBuffer = Texture(m_width, m_height, 3);
    colorBuffer.loadEmptyTexture_8UC3();

    m_colourBuffers.push_back(colorBuffer);

    //Renderbuffer
    this->createRenderBuffer(m_depthBufferId, GL_DEPTH24_STENCIL8);

    //Attach colour buffer and render buffer to framebuffer
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourBuffers[0].getTextureId(), 0);
    f->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferId);

    if (f->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Error in framebuffer creation. Clear memory." << endl;

        //Clear the memory associated with the framebuffer, renderbuffer and color buffer
        f->glDeleteFramebuffers(1, &m_framebufferId);
        f->glDeleteRenderbuffers(1, &m_depthBufferId);
        m_colourBuffers.clear();

        return false;
    }

    //Stop working with it
    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;

}

bool FrameBuffer::load_32FC3()
{

    if (f->glIsFramebuffer(m_framebufferId) == GL_TRUE)
    {
        f->glDeleteFramebuffers(1, &m_framebufferId);
        m_colourBuffers.clear(); //Empty colour buffers
    }

    //Generate ID
    f->glGenFramebuffers(1, &m_framebufferId);

    //Create renderbuffer with same width and height
    f->glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);

    //Colorbuffer
    Texture colorBuffer = Texture(m_width, m_height, 3);


    colorBuffer.loadEmptyTexture_32FC3();


    m_colourBuffers.push_back(colorBuffer);

    //Renderbuffer
    this->createRenderBuffer(m_depthBufferId, GL_DEPTH24_STENCIL8);

    //Attach colour buffer and render buffer to framebuffer
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourBuffers[0].getTextureId(), 0);

    f->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferId);

    if (f->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Error in framebuffer creation. Clear memory." << endl;

        //Clear the memory associated with the framebuffer, renderbuffer and color buffer
        f->glDeleteFramebuffers(1, &m_framebufferId);
        f->glDeleteRenderbuffers(1, &m_depthBufferId);
        m_colourBuffers.clear();

        return false;
    }

    //Stop working with it
    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;

}

GLuint FrameBuffer::getFramebufferID() const
{
    return m_framebufferId;
}

GLuint FrameBuffer::getColorBufferID(unsigned int index) const
{
    return m_colourBuffers[index].getTextureId();
}

int FrameBuffer::getWidth() const
{
    return m_width;
}

int FrameBuffer::getHeight() const
{
    return m_height;
}
