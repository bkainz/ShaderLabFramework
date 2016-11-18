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

#ifndef __materialEditorWidget_HPP_INCLUDED__
#define __materialEditorWidget_HPP_INCLUDED__

#include "opengl/openglheaders.h"
#include "texturewidget.h"
#include "ui_materialEditorWidget.h"
#include <QVector>
#include <QMatrix4x4>
#include "opengl/scene.h"

class MaterialEditorWidget : public QWidget
{
    Q_OBJECT

public:
    MaterialEditorWidget(int objectID, Scene* scene, QWidget *parent);
    ~MaterialEditorWidget();

    void updateEditor(int objectID);

    public slots:
    void updateSceneObject();

signals:
    void updateGL();
    void updateMaterial(int objectID, Material material);

private:
    Scene* m_scene;
    int m_objectID;
    Ui::MaterialEditor* ui;
};

#endif
