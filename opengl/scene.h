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

#ifndef SCENE_H
#define SCENE_H

#define LIGHT_POSITION_Z 33.87

#include "opengl/object.h"
#include "opengl/light.h"

#include <QVector>
#include <QVector4D>

class Scene
{
public:

    Scene();
    Scene(std::string object);
    Scene(QVector<std::string>& listOfObjectNames, const QVector<Light> &listOfPointLights);
    ~Scene();

    /*---Geometric transformations--*/
    void translateLightSourceX(int lightNumber, float translationX);
    void translateLightSourceY(int lightNumber, float translationY);
    void translateLightSourceZ(int lightNumber, float translationZ);
    void setLightSourcePosition(int lightNumber, float x, float y, float z);

    void rotateObjectX(int objectNumber, int rotationX);
    void rotateObjectY(int objectNumber, int rotationY);
    void rotateObjectZ(int objectNumber, int rotationZ);
    void setModelMatrix(int objectNumber, QMatrix4x4 &matrix);

    /**
     * Reset the model matrix.
     * @brief resetTransformationsObjects
     */
    void resetTransformationsObjects();

    /**
     * Set the aspect ratio of the objects.
     * @brief setAspectRatiosObjects
     */
    void setAspectRatiosObjects();

    /**
     * Build the scene by loading the geometry and setting the light sources.
     * @brief buildScene
     */
    void buildScene(std::string object);

    void removeObjects();

    void addObject(std::string object);

    /**
     * Reset the objects and the lights to their original position
     * @brief resetScene
     */
    void resetScene();

    void updateObjectMaterial(int objectID, Material material);

    QVector<Object> getObjects();
    int getObjectRotation(int objectNumber, std::string rotationAxis);

    QVector<Light> getPointLightSources();

private:
    QVector<Object> m_objects;
    QVector<Light> m_pointLights;
};

#endif // SCENE_H
