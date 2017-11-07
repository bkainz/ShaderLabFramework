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

#include "opengl/scene.h"

using namespace std;

Scene::Scene() : m_objects(QVector<Object>()), m_pointLights(QVector<Light>())
{


}

Scene::Scene(string object) : m_objects(QVector<Object>()), m_pointLights(QVector<Light>())
{
    buildScene(object);
}


Scene::Scene(QVector<string>& listOfObjectNames, const QVector<Light> &listOfPointLights) :
    m_objects(QVector<Object>()), m_pointLights(listOfPointLights)
{
    for (int i = 0; i < listOfObjectNames.size(); i++)
    {
        m_objects.push_back(Object(listOfObjectNames[i]));
    }
}

Scene::~Scene()
{

}


void Scene::updateObjectMaterial(int objectID, Material material)
{
    if (!m_objects.empty())
    {
        m_objects[objectID].setMaterial(material);
    }

}

void Scene::translateLightSourceX(int lightNumber, float translationX)
{
    if (lightNumber < m_pointLights.size())
    {
        m_pointLights[lightNumber].translateX(translationX);
    }
}

void Scene::translateLightSourceY(int lightNumber, float translationY)
{
    if (lightNumber < m_pointLights.size())
    {
        m_pointLights[lightNumber].translateY(translationY);
    }
}

void Scene::translateLightSourceZ(int lightNumber, float translationZ)
{
    if (lightNumber < m_pointLights.size())
    {
        m_pointLights[lightNumber].translateZ(translationZ);
    }
}

void Scene::setLightSourcePosition(int lightNumber, float x, float y, float z)
{
    if (lightNumber < m_pointLights.size())
    {
        m_pointLights[lightNumber].setPosition(x, y, z);
    }
}

void Scene::rotateObjectX(int objectNumber, int rotationX)
{
    if (objectNumber < m_objects.size())
    {
        m_objects[objectNumber].rotateX(rotationX);
    }
}

void Scene::rotateObjectY(int objectNumber, int rotationY)
{
    if (objectNumber < m_objects.size())
    {
        m_objects[objectNumber].rotateY(rotationY);
    }
}

void Scene::rotateObjectZ(int objectNumber, int rotationZ)
{
    if (objectNumber < m_objects.size())
    {
        m_objects[objectNumber].rotateZ(rotationZ);
    }
}

void Scene::setModelMatrix(int objectNumber, QMatrix4x4 &matrix)
{
    m_objects[objectNumber].setModelMatrix(matrix);
}

void Scene::resetTransformationsObjects()
{
    for (int k = 0; k < m_objects.size(); k++)
    {
        //Set the aspect ratio after loading the texture
        m_objects[k].resetModelMatrix();
    }
}

void Scene::setAspectRatiosObjects()
{

}

void Scene::buildScene(string object)
{
    this->addObject(object);

    //Be careful not to put the light inside the object
    m_pointLights.push_back(Light(QVector4D(0.0, 0.0, LIGHT_POSITION_Z, 1.0), QVector3D(1.0, 1.0, 1.0), 1.0));
}


void Scene::removeObjects()
{
    m_objects.clear();
}

void Scene::addObject(string object)
{
    Object newObject = Object(object);
    m_objects.push_back(newObject);
}

void Scene::resetScene()
{
    //Reset the lights
    //Be careful not to put the light inside the object
    for (int k = 0; k < m_objects.size(); k++)
    {
        m_pointLights[k].setPosition(0.0, 0.0, LIGHT_POSITION_Z);
    }
}

QVector<Object> Scene::getObjects()
{
    return m_objects;
}

int Scene::getObjectRotation(int objectNumber, string rotationAxis)
{
    int rotation = 0;

    if (rotationAxis == "X")
        rotation = m_objects[objectNumber].getRotationX();
    else if (rotationAxis == "Y")
        rotation = m_objects[objectNumber].getRotationY();
    else if (rotationAxis == "Z")
        rotation = m_objects[objectNumber].getRotationZ();
    else
        cerr << "No rotation axis called : " << rotationAxis << endl;

    return rotation;
}

QVector<Light> Scene::getPointLightSources()
{
    return m_pointLights;
}
