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

#include "qt/materialEditorWidget.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpacerItem>
#include "Vector4Widget.h"
#include "Matrix4x4Widget.h"
#include <QOpenGLContext>
#include <QGLFunctions>

MaterialEditorWidget::MaterialEditorWidget(int objectID, Scene* scene, QWidget *parent) : QWidget(parent), ui(new Ui::MaterialEditor)
{
    ui->setupUi(this);
    m_scene = scene;
    updateEditor(objectID);
}

MaterialEditorWidget::~MaterialEditorWidget()
{
    delete ui;
}

void MaterialEditorWidget::updateEditor(int objectID)
{
    m_objectID = objectID;

    QVector<Object> objectList = m_scene->getObjects();

    Material material = objectList.at(m_objectID).getMaterial();
    QColor ambient = material.getAmbientColor();
    QColor diffuse = material.getDiffuseColor();
    QColor specular = material.getSpecularColor();
    float shininess = material.getShininess();
    float aceoff = material.getAmbientCoefficient();
    float dceoff = material.getDiffuseCoefficient();
    float sceoff = material.getSpecularCoefficient();

    ui->a0_0->setValue(ambient.redF());
    ui->a0_1->setValue(ambient.greenF());
    ui->a0_2->setValue(ambient.blueF());
    ui->a0_3->setValue(ambient.alphaF());

    ui->d0_0->setValue(diffuse.redF());
    ui->d0_1->setValue(diffuse.greenF());
    ui->d0_2->setValue(diffuse.blueF());
    ui->d0_3->setValue(diffuse.alphaF());

    ui->s0_0->setValue(specular.redF());
    ui->s0_1->setValue(specular.greenF());
    ui->s0_2->setValue(specular.blueF());
    ui->s0_3->setValue(specular.alphaF());

    ui->shininess->setValue(shininess);

    ui->acoeff->setValue(aceoff);
    ui->dcoeff->setValue(dceoff);
    ui->scoeff->setValue(sceoff);
}

void MaterialEditorWidget::updateSceneObject()
{
    //qDebug() << "updateSceneObject()";
    Material newMaterial =
        Material(
            QColor::fromRgbF(ui->a0_0->value(), ui->a0_1->value(), ui->a0_2->value(), ui->a0_3->value()),
            QColor::fromRgbF(ui->d0_0->value(), ui->d0_1->value(), ui->d0_2->value(), ui->d0_3->value()),
            QColor::fromRgbF(ui->s0_0->value(), ui->s0_1->value(), ui->s0_2->value(), ui->s0_3->value()),
            ui->acoeff->value(), ui->dcoeff->value(), ui->scoeff->value(),
            ui->shininess->value());

    emit(updateMaterial(m_objectID, newMaterial));

}