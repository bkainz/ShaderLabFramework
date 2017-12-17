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

#ifndef OBJECT_H
#define OBJECT_H

#include "opengl/mesh.h"
#include "opengl/material.h"
#include "opengl/texture.h"

#include <QApplication>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QObject>

#include <string>
#include <sstream>

class Object
{
public:
    Object();

    /**
     * Loads an object just from its name.
     * @brief Object::Object
     * @param objectName
     */
    Object(std::string objectName);

    ~Object();

    void resetModelMatrix();


    /**
     * Set a given aspect ratio.
     * @brief setAspectRatio
     * @param aspectRatio
     */
    void setAspectRatio(float aspectRatio);

    /**
     * Scales the object in every dimension.
     * @brief scale
     * @param scalingFactor
     */
    void scale(float scalingFactor);

    /**
   * Function that returns the path of the .off file corresponding to the object.
   * Also sets the texture coordinates
   * @brief loadPath
   * @param objectName
   * @return
   */
    std::string loadPath(std::string &objectName);


    /**
     * Loads the mesh.
     * @brief loadMesh
     */
    void loadMesh();

    void setModelMatrix(QMatrix4x4 modelMatrix);

    void rotateX(int angleX);
    void rotateY(int angleY);
    void rotateZ(int angleZ);

    Mesh getMesh() const;
    Material getMaterial() const;
    QOpenGLBuffer getQtVBO() const;
    QOpenGLBuffer getIndexBuffer() const;

    void setMaterial(Material material);

    int getVertexOffset() const;
    int getIndicesOffset() const;
    int getTextureCoordinatesOffset() const;
    int getNormalsOffset() const;

    QMatrix4x4 getModelMatrix() const;
    int getRotationX() const;
    int getRotationY() const;
    int getRotationZ() const;

    std::string getObjectName() const;

private:
    std::string m_objectName;
    Mesh m_mesh;
    Material m_material;
    QOpenGLBuffer m_QtVBO;
    QOpenGLBuffer m_QtIndexBuffer;

    int m_vertexOffset;
    int m_texturesCoordsOffset;
    int m_normalsOffset;

    QMatrix4x4 m_modelMatrix;
    int m_rotationX;
    int m_rotationY;
    int m_rotationZ;

};

#endif // OBJECT_H
