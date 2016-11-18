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

#include "MatricesWidget.h"
#include "Matrix4x4Widget.h"

MatricesWidget::MatricesWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MatricesWidget)
{

    ui->setupUi(this);
    m_ModelMatrix.setToIdentity();
    m_ViewMatrix.setToIdentity();
    m_ProjectionMatrix.setToIdentity();

    QHBoxLayout *vbox1 = new QHBoxLayout;
    modelMatrixWidget = new Matrix4x4Widget(ui->ModelMatrixBox);
    vbox1->addWidget(modelMatrixWidget);
    ui->ModelMatrixBox->setLayout(vbox1);

    QHBoxLayout *vbox2 = new QHBoxLayout;
    viewMatrixWidget = new Matrix4x4Widget(ui->ViewMatrixBox);
    vbox2->addWidget(viewMatrixWidget);
    ui->ViewMatrixBox->setLayout(vbox2);

    QHBoxLayout *vbox3 = new QHBoxLayout;
    projectionMatrixWidget = new Matrix4x4Widget(ui->ProjectionMatrixBox);
    vbox3->addWidget(projectionMatrixWidget);
    ui->ProjectionMatrixBox->setLayout(vbox3);

    connect(modelMatrixWidget, SIGNAL(matrixChanged(QMatrix4x4)), this, SIGNAL(modelMatrixChanged(QMatrix4x4)));
    connect(viewMatrixWidget, SIGNAL(matrixChanged(QMatrix4x4)), this, SIGNAL(viewMatrixChanged(QMatrix4x4)));
    connect(projectionMatrixWidget, SIGNAL(matrixChanged(QMatrix4x4)), this, SIGNAL(projectionMatrixChanged(QMatrix4x4)));
    connect(ui->m_resetButton, SIGNAL(clicked()), this, SIGNAL(resetMatrices()));
}

MatricesWidget::~MatricesWidget()
{
    delete ui;
}

void MatricesWidget::updateModelMatrix(QMatrix4x4 mat)
{
    modelMatrixWidget->updateSpinBoxes(mat);
}

void MatricesWidget::updateViewMatrix(QMatrix4x4 mat)
{
    viewMatrixWidget->updateSpinBoxes(mat);
    //qDebug() << mat;
}

void MatricesWidget::updateProjectionMatrix(QMatrix4x4 mat)
{
    projectionMatrixWidget->updateSpinBoxes(mat);
}

void MatricesWidget::updateMatrices()
{
    //TODO
    QMatrix4x4 m;
    //m_scene->getCameraState()->getProjectionMatrix(m);
    projectionMatrixWidget->updateSpinBoxes(m);

    /*QMatrix4x4 m;
    m_scene->getCameraState()->getModelMatrix(m);
    modelMatrixWidget->updateSpinBoxes(m);

    QMatrix4x4 m;
    m_scene->getCameraState()->getViewMatrix(m);
    viewMatrixWidget->updateSpinBoxes(m);*/
}

