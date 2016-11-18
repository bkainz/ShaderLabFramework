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

#include "opengl/mesh.h"

using namespace std;

Mesh::Mesh() : m_fileName(string()), m_vertices(QVector<QVector3D>()), m_indices(QVector<QVector3D>()),
m_indicesArray(QVector<GLuint>()), m_triangleNormals(QVector<QVector3D>()),
m_vertexNormals(QVector<QVector3D>()), m_textureCoordinates(QVector<QVector2D>())
{

}


Mesh::Mesh(const string& fileName) : m_fileName(fileName), m_vertices(QVector<QVector3D>()), m_indices(QVector<QVector3D>()),
m_indicesArray(QVector<GLuint>()), m_triangleNormals(QVector<QVector3D>()),
m_vertexNormals(QVector<QVector3D>()), m_textureCoordinates(QVector<QVector2D>())
{

}

Mesh::~Mesh()
{

}

void Mesh::offReader()
{
    ifstream file(m_fileName.c_str(), ios::in);

    if (!file)
    {
        cerr << "Could not open the file : " << m_fileName << endl;
        exit(-1);
    }

    string fileType;
    string numberOfVertices, numberOfTriangles, numberOfEdges;
    string data;
    string xVertex, yVertex, zVertex;
    string vertex1, vertex2, vertex3;
    int index1 = 0, index2 = 0, index3 = 0;
    string numberOfIndices;

    file >> fileType;
    //Add and exception


    file >> numberOfVertices >> numberOfTriangles >> numberOfEdges;
    m_vertices.resize(atoi(numberOfVertices.c_str()));
    m_indices.resize(atoi(numberOfTriangles.c_str()));

    //Blank line
    getline(file, data);

    //Vertices
    for (int i = 0; i < atoi(numberOfVertices.c_str()); i++)
    {
        file >> xVertex >> yVertex >> zVertex;
        m_vertices[i] = QVector3D(atof(xVertex.c_str()), atof(yVertex.c_str()), atof(zVertex.c_str()));
    }

    //Indices and normals for each triangle
    QVector3D crossProduct;
    for (int i = 0; i < atoi(numberOfTriangles.c_str()); i++)
    {
        file >> numberOfIndices >> vertex1 >> vertex2 >> vertex3;
        index1 = atoi(vertex1.c_str());
        index2 = atoi(vertex2.c_str());
        index3 = atoi(vertex3.c_str());

        //Indices for each triangle
        m_indices[i] = QVector3D(index1, index2, index3);

        //List of indices for OpenGL rendering
        m_indicesArray.push_back(index1);
        m_indicesArray.push_back(index2);
        m_indicesArray.push_back(index3);

        //Compute the normal NORMALIZED
        /*   v3
         * v1__v2
         * normal = (v2-v1)^(v3-v1)
         */
        crossProduct = QVector3D::crossProduct(m_vertices[index1] - m_vertices[index2], m_vertices[index1] - m_vertices[index3]);
        crossProduct.normalize();
        m_triangleNormals.push_back(crossProduct);
    }

    m_vertexNormals.resize(m_vertices.size());
    QVector3D vector1;
    QVector3D vector2;

    //Compute the normals for each vertex
    for (int k = 0; k < m_vertices.size(); k++)
    {
        m_vertexNormals[k] = QVector3D(0.0, 0.0, 0.0);
        for (int i = 0; i < m_indices.size(); i++)
        {
            //If the vertex k belongs to triangle i
            if (m_indices[i].x() == k)
            {
                /*   z
                 * kx__y
                 * angle = acos(kxy.kxz)
                 */
                vector1 = m_vertices[m_indices[i].y()] - m_vertices[k];
                vector2 = m_vertices[m_indices[i].z()] - m_vertices[k];
                vector1.normalize();
                vector2.normalize();
                m_vertexNormals[k] += acos(QVector3D::dotProduct(vector1, vector2))*m_triangleNormals[i];
            }
            else if (m_indices[i].y() == k)
            {
                /*   z
                 * x__ky
                 * angle = acos(kxy.kxz)
                 */
                vector1 = m_vertices[m_indices[i].z()] - m_vertices[k];
                vector2 = m_vertices[m_indices[i].x()] - m_vertices[k];
                vector1.normalize();
                vector2.normalize();
                m_vertexNormals[k] += acos(QVector3D::dotProduct(vector1, vector2))*m_triangleNormals[i];
            }
            else if (m_indices[i].z() == k)
            {
                /*   kz
                 * x__y
                 * angle = acos(kxy.kxz)
                 */
                vector1 = m_vertices[m_indices[i].x()] - m_vertices[k];
                vector2 = m_vertices[m_indices[i].y()] - m_vertices[k];
                vector1.normalize();
                vector2.normalize();
                m_vertexNormals[k] += acos(QVector3D::dotProduct(vector1, vector2))*m_triangleNormals[i];
            }
        }
        m_vertexNormals[k].normalize();
    }

    file.close();
}

void Mesh::objReader()
{
    //Assumes vertices first, then normals, then texture coordinates
    //# comments
    //Empty line
    //v x y z =  vertex
    //# number vertices
    //Empty line
    //vn x y z =  normal
    //# number normals
    //Empty line
    //vt x y 0.0 = texture coordinate
    //# number texture coordinates
    //Empty line
    //g Teapot
    //f v1/vn1/vt1 v2/vn2/vt2 v3/vn3/vt3 v4/vn4/vt4 Quad face indices //BOTH quads and triangles !!
    //f v1/vn1/vt1 v2/vn2/vt2 v3/vn3/vt3 Triangle face indices
    //# number faces
    //Empty line
    // !!! Indices in f v1/vn1/vt1 start at 1 and not 0!

    ifstream file(m_fileName.c_str(), ios::in);

    if (!file)
    {
        cerr << "Could not open the file : " << m_fileName << endl;
        exit(-1);
    }
    else
    {
        string comments;
        string dataType;

        //Skip the comments
        getline(file, comments);
        while ((comments.size() > 0) && (comments[0] == '#'))
        {
            getline(file, comments);
        }

        //Empty line is in the last string comments
        //Now starts the vertices
        string xVertex, yVertex, zVertex;
        do
        {
            file >> dataType;
            if (dataType == "v")
            {
                file >> xVertex >> yVertex >> zVertex;
                m_vertices.push_back(QVector3D(atof(xVertex.c_str()), atof(yVertex.c_str()), atof(zVertex.c_str())));
            }
        } while (dataType[0] != '#' && dataType.size() > 0);;

        //Resize the texture coordinates vector
        m_textureCoordinates.resize(m_vertices.size());

        //Skip the comment line and blank line
        getline(file, comments);
        getline(file, comments);

        //Now starts the normals
        string xNormal, yNormal, zNormal;
        int normalIndex = 0;
        do
        {

            file >> dataType;
            if (dataType == "vn")
            {
                file >> xNormal >> yNormal >> zNormal;
                m_vertexNormals.push_back(QVector3D(atof(xNormal.c_str()), atof(yNormal.c_str()), atof(zNormal.c_str())));
                m_vertexNormals[normalIndex].normalize(); //The normal might not be normalized!
            }
            ++normalIndex;
        } while (dataType[0] != '#' && dataType.size() > 0);

        //Skip the comment line and blank line
        getline(file, comments);
        getline(file, comments);

        //Now starts the texture coordinates
        string xTexture, yTexture, zTexture;
        QVector<QVector2D> textureCoordinates;
        do
        {
            file >> dataType;
            if (dataType == "vt")
            {
                file >> xTexture >> yTexture >> zTexture;
                //zTexture is always 0.0
                float u = atof(xTexture.c_str());
                float v = atof(yTexture.c_str());
                //U,V can be above 1.0 : use GL_REPEAT for the texture
                textureCoordinates.push_back(QVector2D(u, v));
            }
        } while (dataType[0] != '#' && dataType.size() > 0);

        //Skip the comment line the blank line and the g teapot line
        getline(file, comments);
        getline(file, comments);
        getline(file, comments);

        //Now starts the faces
        //These are stored as a quad and as triangles
        // 1---4 == Two triangles : 123 and 134
        // 2---3
        string faceLine, vertexInfo[4];

        do
        {

            //Initialise
            for (unsigned int k = 0; k < 4; ++k)
            {
                vertexInfo[k] = string("");
            }

            //read the line
            getline(file, faceLine);

            //Parse the line to store separately the information of the primitive
            //The primitive can be a Quad (4 vertices) or a triangle (3 vertices)
            if (faceLine.size() > 0 && faceLine[0] == 'f')
            {
                int index = 0;
                //Parse the string
                //Start from the first integer at position 2
                for (unsigned int k = 2; k < faceLine.size(); ++k)
                {
                    if (faceLine[k] == ' ')
                    {
                        ++index;
                    }
                    else
                    {
                        //The line might end with a ' ' producing a buffer overflow
                        //Check index to avoid that
                        if (index < 4)
                        {
                            vertexInfo[index] += faceLine[k];
                        }
                    }
                }

                //If the last vertex has a size>1 then it is a quad otherwise it is a triangle
                //The line might end with a ' ' hence size>1
                if (vertexInfo[3].size() > 1)
                {
                    //[0] = vertex number ; [1] = texture coordinate number ; [2] = normal number at vertex
                    int faceValues1[3], faceValues2[3], faceValues3[3], faceValues4[3];
                    vertexInfo[0] += "/"; //Add a final delimiter
                    parseString(vertexInfo[0], '/', faceValues1, 3);

                    vertexInfo[1] += "/"; //Add a final delimiter
                    parseString(vertexInfo[1], '/', faceValues2, 3);

                    vertexInfo[2] += "/"; //Add a final delimiter
                    parseString(vertexInfo[2], '/', faceValues3, 3);

                    vertexInfo[3] += "/"; //Add a final delimiter
                    parseString(vertexInfo[3], '/', faceValues4, 3);

                    //Divide the quad into two triangles
                    //Push the indices of the two triangles
                    m_indices.push_back(QVector3D(faceValues1[0] - 1, faceValues2[0] - 1, faceValues3[0] - 1));
                    m_indices.push_back(QVector3D(faceValues1[0] - 1, faceValues3[0] - 1, faceValues4[0] - 1));
                    m_indicesArray.push_back(faceValues1[0] - 1); m_indicesArray.push_back(faceValues2[0] - 1); m_indicesArray.push_back(faceValues3[0] - 1);
                    m_indicesArray.push_back(faceValues1[0] - 1); m_indicesArray.push_back(faceValues3[0] - 1); m_indicesArray.push_back(faceValues4[0] - 1);

                    //Associate the texture coordinates
                    m_textureCoordinates[faceValues1[0] - 1] = textureCoordinates[faceValues1[1] - 1];
                    m_textureCoordinates[faceValues2[0] - 1] = (textureCoordinates[faceValues2[1] - 1]);
                    m_textureCoordinates[faceValues3[0] - 1] = (textureCoordinates[faceValues3[1] - 1]);
                    m_textureCoordinates[faceValues4[0] - 1] = (textureCoordinates[faceValues4[1] - 1]);
                }
                else
                {
                    //[0] = vertex number ; [1] = texture coordinate number ; [2] = normal number at vertex
                    int faceValues1[3], faceValues2[3], faceValues3[3];
                    vertexInfo[0] += "/"; //Add a final delimiter
                    parseString(vertexInfo[0], '/', faceValues1, 3);

                    vertexInfo[1] += "/"; //Add a final delimiter
                    parseString(vertexInfo[1], '/', faceValues2, 3);

                    vertexInfo[2] += "/"; //Add a final delimiter
                    parseString(vertexInfo[2], '/', faceValues3, 3);

                    //Divide the quad into two triangles
                    //Push the indices of the two triangles
                    m_indices.push_back(QVector3D(faceValues1[0] - 1, faceValues2[0] - 1, faceValues3[0] - 1));
                    m_indicesArray.push_back(faceValues1[0] - 1); m_indicesArray.push_back(faceValues2[0] - 1); m_indicesArray.push_back(faceValues3[0] - 1);


                    //Associate the texture coordinates
                    m_textureCoordinates[faceValues1[0] - 1] = textureCoordinates[faceValues1[1] - 1];
                    m_textureCoordinates[faceValues2[0] - 1] = (textureCoordinates[faceValues2[1] - 1]);
                    m_textureCoordinates[faceValues3[0] - 1] = (textureCoordinates[faceValues3[1] - 1]);
                }
            }
        } while (faceLine[0] != '#' && faceLine.size() > 0);
    }
}

void Mesh::parseString(string input, char delimiter, int values[], int numberOfValues)
{
    int k = 0;

    string temp("");
    for (unsigned int i = 0; i < input.size(); ++i)
    {
        if (input[i] == delimiter)
        {
            if (k < numberOfValues)
            {
                values[k] = atoi(temp.c_str());
                ++k;
                temp = "";
            }
        }
        else
        {
            temp += input[i];
        }

    }
}

void Mesh::setTextureCoordinates()
{
    //TODO Only contains the UV coordinates for a square.
    //Should be read from the file

    /*The square is defined as follows :
     * v1------v0
     * |        |
     * |        |
     * v2------v3
     * First triangle v2-v3-v0
     * Second triangle v0-v1-v2
     */

     //Texture coordinates
     //The vertices are stored in the order v0, v1, v2, v3
      //Then the triangles are formed using the indices : First triangle v2-v3-v0 Second triangle v0-v1-v2
      //Hence only 4 texture coordinates are required : one for each vertex (and not 6).
    m_textureCoordinates.push_back(QVector2D(1.0, 1.0));
    m_textureCoordinates.push_back(QVector2D(0.0, 1.0));
    m_textureCoordinates.push_back(QVector2D(0.0, 0.0));
    m_textureCoordinates.push_back(QVector2D(1.0, 0.0));
}

void Mesh::centerMesh()
{
    //Calculate the center of mass and subtract it
    QVector3D centerOfMass;

    for (int i = 0; i < m_vertices.size(); ++i)
    {
        centerOfMass += m_vertices[i];
    }

    centerOfMass /= (float)m_vertices.size();

    for (int i = 0; i < m_vertices.size(); ++i)
    {
        m_vertices[i] -= centerOfMass;
    }

}

void Mesh::obj_rotate90X()
{
    //Assumes vertices first, then normals, then texture coordinates
    //# comments
    //Empty line
    //v x y z =  vertex
    //# number vertices
    //Empty line
    //vn x y z =  normal
    //# number normals
    //Empty line
    //vt x y 0.0 = texture coordinate
    //# number texture coordinates
    //Empty line
    //g Teapot
    //f v1/vn1/vt1 v2/vn2/vt2 v3/vn3/vt3 v4/vn4/vt4 Quad face indices //BOTH quads and triangles !!
    //f v1/vn1/vt1 v2/vn2/vt2 v3/vn3/vt3 Triangle face indices
    //# number faces
    //Empty line
    // !!! Indices in f v1/vn1/vt1 start at 1 and not 0!

    ifstream file(m_fileName.c_str(), ios::in);
    ofstream rotatedObject(QDir::currentPath().toStdString() + "\\obj\\teapot-lowR90.obj", ios::out | ios::trunc);

    if (!file || !rotatedObject)
    {
        cerr << "Could not open the file : " << m_fileName << endl;
        exit(-1);
    }
    else
    {
        string comments;
        string dataType;

        //Copy the comments
        getline(file, comments);
        while ((comments.size() > 0) && (comments[0] == '#'))
        {
            rotatedObject << comments << endl;
            getline(file, comments);
        }

        //Empty line is in the last string comments
        //Copy it

        rotatedObject << comments << endl;
        //Now starts the vertices
        string xVertex, yVertex, zVertex;
        do
        {
            file >> dataType;
            if (dataType == "v")
            {
                file >> xVertex >> yVertex >> zVertex;
                //Copy each rotated vertex by -90 degrees
                float yValueVertex = atof(yVertex.c_str());
                if (yValueVertex > 0)
                    rotatedObject << dataType << "  " << xVertex << " " << zVertex << " -" << yVertex << '\n';
                else if (yValueVertex == 0.0)
                    rotatedObject << dataType << "  " << xVertex << " " << zVertex << " " << yVertex << '\n';
                else
                    rotatedObject << dataType << "  " << xVertex << " " << zVertex << " " << yVertex.substr(1, yVertex.size() - 1) << '\n';
            }
        } while (dataType[0] != '#' && dataType.size() > 0);;

        //Add the #
        rotatedObject << dataType[0];
        //Skip the comment line and blank line
        getline(file, comments);
        rotatedObject << comments << endl;
        getline(file, comments);
        rotatedObject << comments << endl;

        //Now starts the normals
        string xNormal, yNormal, zNormal;
        int normalIndex = 0;

        do
        {

            file >> dataType;
            if (dataType == "vn")
            {
                file >> xNormal >> yNormal >> zNormal;
                //Copy each rotated normal by -90 degrees
                float yValueNormal = atof(yNormal.c_str());

                if (yValueNormal > 0)
                    rotatedObject << dataType << " " << xNormal << " " << zNormal << " -" << yValueNormal << endl;
                else if (yValueNormal == 0.0)
                    rotatedObject << dataType << " " << xNormal << " " << zNormal << " " << yValueNormal << endl;
                else
                    rotatedObject << dataType << " " << xNormal << " " << zNormal << " " << yNormal.substr(1, yNormal.size() - 1) << endl;
            }
            ++normalIndex;
        } while (dataType[0] != '#' && dataType.size() > 0);

        //Add the #
        rotatedObject << dataType[0];

        //Skip the comment line and blank line
        getline(file, comments);
        rotatedObject << comments << endl;
        getline(file, comments);
        rotatedObject << comments << endl;

        //Now starts the texture coordinates
        string xTexture, yTexture, zTexture;
        QVector<QVector2D> textureCoordinates;
        do
        {
            file >> dataType;
            if (dataType == "vt")
            {
                file >> xTexture >> yTexture >> zTexture;
                rotatedObject << dataType << " " << xTexture << " " << yTexture << " " << zTexture << endl;
                //zTexture is always 0.0
                float u = atof(xTexture.c_str());
                float v = atof(yTexture.c_str());
                //U,V can be above 1.0 : use GL_REPEAT for the texture
                textureCoordinates.push_back(QVector2D(u, v));
            }
        } while (dataType[0] != '#' && dataType.size() > 0);


        //Add the #
        rotatedObject << dataType[0];
        //Skip the comment line the blank line and the g teapot line
        getline(file, comments);
        rotatedObject << comments << endl;
        getline(file, comments);
        rotatedObject << comments << endl;
        getline(file, comments);
        rotatedObject << comments << endl;

        string faceLine;
        do
        {
            //read the line and copy it
            getline(file, faceLine);
            rotatedObject << faceLine << endl;
        } while (faceLine[0] != '#' && faceLine.size() > 0);
    }
}


void Mesh::setTextureCoordinates(QVector<QVector2D> &textureCoordinates)
{
    m_textureCoordinates = textureCoordinates;
}

QVector<QVector3D> Mesh::getVertices() const
{
    return m_vertices;
}

QVector<QVector3D> Mesh::getIndices() const
{
    return m_indices;
}


QVector<GLuint> Mesh::getIndicesArray() const
{
    return m_indicesArray;
}

QVector<QVector3D> Mesh::getVertexNormals() const
{
    return m_vertexNormals;
}

QVector<QVector2D> Mesh::getTextureCoordinates() const
{
    return m_textureCoordinates;
}
