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

#ifndef __MatricesWidget__HPP_INCLUDED__
#define __MatricesWidget__HPP_INCLUDED__

#include "ui_Matrix4x4Widget.h"
#include "ui_MatricesWidget.h"
#include <QMatrix4x4>

class Matrix4x4Widget;

class MatricesWidget : public QWidget
{
    Q_OBJECT

public:
    MatricesWidget(QWidget *parent);
    ~MatricesWidget();

    private slots:
    void updateModelMatrix(QMatrix4x4 mat);
    void updateViewMatrix(QMatrix4x4 mat);
    void updateProjectionMatrix(QMatrix4x4 mat);
    void updateMatrices();

signals:
    void modelMatrixChanged(QMatrix4x4 mat);
    void viewMatrixChanged(QMatrix4x4 mat);
    void projectionMatrixChanged(QMatrix4x4 mat);
    void resetMatrices();

private:
    Ui::MatricesWidget *ui;
    QMatrix4x4 m_ModelMatrix;
    QMatrix4x4 m_ViewMatrix;
    QMatrix4x4 m_ProjectionMatrix;
    Matrix4x4Widget* projectionMatrixWidget;
    Matrix4x4Widget* viewMatrixWidget;
    Matrix4x4Widget* modelMatrixWidget;
};

#endif
