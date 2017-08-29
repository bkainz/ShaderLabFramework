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

#include "opengl/object.h"
#include <QDir>

using namespace std;

Object::Object() : m_objectName(), m_mesh(Mesh()), m_material(Material()),
m_modelMatrix(QMatrix4x4()), m_rotationX(0), m_rotationY(0), m_rotationZ(0)
{

}

Object::Object(string objectName) : m_objectName(objectName), m_mesh(Mesh()), m_material(Material()),
m_modelMatrix(QMatrix4x4()), m_rotationX(0), m_rotationY(0), m_rotationZ(0)
{

    string objectPath = loadPath(objectName);
    m_mesh = Mesh(objectPath);
    this->loadMesh();

    m_modelMatrix = QMatrix4x4();
    m_modelMatrix.setToIdentity();

    m_material = Material(QColor(128, 0, 0), QColor(255, 0, 0), QColor(255, 255, 255), 1.0, 1.0, 1.0, 10.0);

    if (m_QtVBO.create()) qDebug() << "Success creating vertex position buffer";
    m_QtVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

    if (m_QtVBO.bind()) qDebug() << "Success biding vertex position buffer";

    size_t VBOSize =   3 * m_mesh.getVertices().size() * sizeof(float)
            + m_mesh.getIndicesArray().size() * sizeof(GLuint)
            + 2 * m_mesh.getTextureCoordinates().size() * sizeof(float)
            + 3 * m_mesh.getVertexNormals().size() * sizeof(float);


    m_vertexOffset = 0;
    m_indicesOffset = 3 * m_mesh.getVertices().size() * sizeof(float);
    m_texturesCoordsOffset = m_indicesOffset + m_mesh.getIndicesArray().size() * sizeof(GLuint);
    m_normalsOffset = m_texturesCoordsOffset + 2 * m_mesh.getTextureCoordinates().size() * sizeof(float);

    m_QtVBO.allocate(VBOSize);

    //  send the vertice data to the vbo using allocate
    m_QtVBO.write(m_vertexOffset, m_mesh.getVertices().constData(), 3 * m_mesh.getVertices().size() * sizeof(float));

    //Send the indices data
    m_QtVBO.write(m_indicesOffset, m_mesh.getIndicesArray().constData(), m_mesh.getIndicesArray().size() * sizeof(GLuint));

    //Send the texture coordinates data
    m_QtVBO.write(m_texturesCoordsOffset, m_mesh.getTextureCoordinates().constData(), 2 * m_mesh.getTextureCoordinates().size() * sizeof(float));

    //Send the normals data
    m_QtVBO.write(m_normalsOffset, m_mesh.getVertexNormals().constData(), 3 * m_mesh.getVertexNormals().size() * sizeof(float));

    qDebug() << "buffer size " << m_QtVBO.size();
}

Object::~Object()
{

}

void Object::resetModelMatrix()
{
    m_modelMatrix.setToIdentity();
    m_rotationX = 0.0;
    m_rotationY = 0.0;
    m_rotationZ = 0.0;
}

void Object::setAspectRatio(float aspectRatio)
{
    m_modelMatrix.scale(1.0, 1.0 / aspectRatio);
}

void Object::scale(float scalingFactor)
{
    m_modelMatrix.scale(scalingFactor, scalingFactor);
}

void Object::rotateX(int angleX)
{
    angleX %= 360;

    m_rotationX += angleX;
    m_rotationX %= 360;

    m_modelMatrix.rotate(angleX, QVector3D(1.0, 0.0, 0.0));
}

void Object::rotateY(int angleY)
{
    angleY %= 360;

    m_rotationY += angleY;
    m_rotationY %= 360;

    m_modelMatrix.rotate(angleY, QVector3D(0.0, 1.0, 0.0));
}

void Object::rotateZ(int angleZ)
{
    angleZ %= 360;

    m_rotationZ += angleZ;
    m_rotationZ %= 360;

    m_modelMatrix.rotate(angleZ, QVector3D(0.0, 0.0, 1.0));
}

string Object::loadPath(string &objectName)
{
    string objectPath;

    if (objectName == "square")
    {
#ifdef _WIN32
        objectPath = string(QDir::currentPath().toStdString() + "\\off\\square.off");
#endif

#ifdef __gnu_linux__
        objectPath = QDir::currentPath().toStdString() + string("/off/square.off");
#endif

#if defined(__APPLE__) && defined(__MACH__)
        objectPath = QDir::currentPath().toStdString() + string("/../../../off/square.off");
#endif
    }
    else if (objectName == "monkey")
    {
#ifdef _WIN32
        objectPath = string(QDir::currentPath().toStdString() + "\\off\\monkey.off");
#endif

#ifdef __gnu_linux__
        objectPath = QDir::currentPath().toStdString() + string("/off/monkey.off");
#endif

#if defined(__APPLE__) && defined(__MACH__)
        objectPath = QDir::currentPath().toStdString() + string("/../../../off/monkey.off");
#endif
    }
    else if (objectName == "cube")
    {
#ifdef _WIN32
        objectPath = string(QDir::currentPath().toStdString() + "\\off\\cube.off");
#endif

#ifdef __gnu_linux__
        objectPath = QDir::currentPath().toStdString() + string("/off/cube.off");
#endif

#if defined(__APPLE__) && defined(__MACH__)
        objectPath = QDir::currentPath().toStdString() + string("/../../../off/cube.off");
#endif
    }
    else if (objectName == "teapot")
    {
#ifdef _WIN32
        objectPath = string(QDir::currentPath().toStdString() + "\\obj\\teapotR90.obj");
#endif

#ifdef __gnu_linux__
        objectPath = QDir::currentPath().toStdString() + string("/obj/teapotR90.obj");
#endif

#if defined(__APPLE__) && defined(__MACH__)
        objectPath = QDir::currentPath().toStdString() + string("/../../../obj/teapotR90.obj");
#endif
    }
    else if (objectName == "teapot-low")
    {
#ifdef _WIN32
        objectPath = string(QDir::currentPath().toStdString() + "\\obj\\teapot-lowR90.obj");
#endif

#ifdef __gnu_linux__
        objectPath = QDir::currentPath().toStdString() + string("/obj/teapot-lowR90.obj");
#endif

#if defined(__APPLE__) && defined(__MACH__)
        objectPath = QDir::currentPath().toStdString() + string("/../../../obj/teapot-lowR90.obj");
#endif
    }
    else
    {
        cerr << objectName << ".off does not exist";
    }

    return objectPath;
}

void Object::loadMesh()
{
    if (m_objectName == "teapot" || m_objectName == "teapot-low")
    {
        m_mesh.objReader();
    }
    else
    {
        m_mesh.offReader();
        m_mesh.setTextureCoordinates();
    }

    m_mesh.centerMesh();
}

void Object::setModelMatrix(QMatrix4x4 modelMatrix)
{
    m_modelMatrix = QMatrix4x4(modelMatrix);
}

Material Object::getMaterial() const
{
    return m_material;
}

void Object::setMaterial(Material material)
{
    m_material = material;
}

Mesh Object::getMesh() const
{
    return m_mesh;
}


QOpenGLBuffer Object::getQtVBO() const
{
    return m_QtVBO;
}

int Object::getVertexOffset() const
{
    return m_vertexOffset;
}

int Object::getIndicesOffset() const
{
    return m_indicesOffset;
}

int Object::getTextureCoordinatesOffset() const
{
    return m_texturesCoordsOffset;
}

int Object::getNormalsOffset() const
{
    return m_normalsOffset;
}

QMatrix4x4 Object::getModelMatrix() const
{
    return m_modelMatrix;
}

int Object::getRotationX() const
{
    return m_rotationX;
}

int Object::getRotationY() const
{
    return m_rotationY;
}

int Object::getRotationZ() const
{
    return m_rotationZ;
}
