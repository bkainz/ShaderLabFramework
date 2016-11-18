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

#ifndef MESH_H
#define MESH_H

#include "opengl/openglheaders.h"

#include <QVector>

#include <QVector2D>
#include <QVector3D>
#include <QDir>

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

class Mesh
{
public:
    Mesh();
    Mesh(const std::string &fileName);
    ~Mesh();

    /**
     * Reads off file.
     * @brief offReader
     */
    void offReader();

    /**
     * Reads obj file.
     * @brief objReader
     * @param fileName
     */
    void objReader();


    /**
     * Sets the UV texture coordinates.
     * @brief setTextureCoordinates
     * @param textureCoordinates
     */
    void setTextureCoordinates();

    /**
     * Sets the UV texture coordinates of the mesh to the ones given as a parameter.
     * @brief setTextureCoordinates
     * @param textureCoordinates
     */
    void setTextureCoordinates(QVector<QVector2D> &textureCoordinates);

    /**
     * Centers the mesh so that its center of mass is at the origin of the world coordinate system.
     * @brief centerMesh
     */
    void centerMesh();

    /**
     * Reads an obj file, rotate the object and normal by 90 degrees around the x axis and saves it.
     * @brief obj_rotate90X
     */
    void obj_rotate90X();

    QVector<QVector3D> getVertices() const;
    QVector<QVector3D> getIndices() const;
    QVector<GLuint> getIndicesArray() const;
    QVector<QVector3D> getVertexNormals() const;
    QVector<QVector2D> getTextureCoordinates() const;

private:
    /**
     * Parse a string in the form a/b/c/d/... where a,b,c,d are integers
     * Store the values of a,b,c,d in values[]. This is necessary for the obj reader.
     * @brief parseString
     * @param input
     * @param delimiter
     * @param values
     * @param numberOfValues
     */
    void parseString(std::string input, char delimiter, int values[], int numberOfValues);

    std::string m_fileName;
    QVector<QVector3D> m_vertices;

    /**
     * Contains the list of indices for each triangle. QVector3D contains the 3 indices for a given triangle
     * @brief m_indices
     */
    QVector<QVector3D> m_indices;

    /**
     * Contains the list of indices for all the triangles. Each consecutive triplet of integers correspond to the three indices of the vertices that make a triangle.
     * This is the QVector that openGL needs for rendering (in glDrawElements)
     * @brief m_indices
     */
    QVector<GLuint> m_indicesArray;

    QVector<QVector3D> m_triangleNormals;
    QVector<QVector3D> m_vertexNormals;
    QVector<QVector2D> m_textureCoordinates;
};

#endif // MESH_H
