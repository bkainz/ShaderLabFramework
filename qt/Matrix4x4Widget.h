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

#ifndef __Matrix4x4Widget_HPP_INCLUDED__
#define __Matrix4x4Widget_HPP_INCLUDED__

#include "ui_Matrix4x4Widget.h"
#include <QMatrix4x4>

class Matrix4x4Widget : public QWidget
{
    Q_OBJECT

public:
    Matrix4x4Widget(QWidget *parent = 0);
    ~Matrix4x4Widget();

    /**
     * Sets the order in which the spin boxes are selected when pressing tab.
     * @brief setTabKeyboardOrder
     */
    void setTabKeyboardOrder();

    public slots:
    void updateSpinBoxes(QMatrix4x4 matrix);
    void set3x3();
    void set4x4();

    private slots:
    void updateMatrix();

signals:
    void matrixChanged(QMatrix4x4 matrix);

private:
    Ui::matrix4x4Widget *ui;
    QMatrix4x4 m_matrix;

};

#endif
