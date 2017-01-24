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

#include "Matrix4x4Widget.h"
#include <iostream>

Matrix4x4Widget::Matrix4x4Widget(QWidget *parent) : QWidget(parent), ui(new Ui::matrix4x4Widget)
{
    ui->setupUi(this);
    m_matrix.setToIdentity();

    this->setTabKeyboardOrder();

    connect(ui->a0_0, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a0_1, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a0_2, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a0_3, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));

    connect(ui->a1_0, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a1_1, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a1_2, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a1_3, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));

    connect(ui->a2_0, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a2_1, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a2_2, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a2_3, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));

    connect(ui->a3_0, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a3_1, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a3_2, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
    connect(ui->a3_3, SIGNAL(editingFinished()), this, SLOT(updateMatrix()));
}

Matrix4x4Widget::~Matrix4x4Widget()
{
    delete ui;
}


void Matrix4x4Widget::set3x3()
{
    ui->a0_3->setEnabled(false);
    ui->a1_3->setEnabled(false);
    ui->a2_3->setEnabled(false);
    ui->a3_3->setEnabled(false);

    ui->a3_0->setEnabled(false);
    ui->a3_1->setEnabled(false);
    ui->a3_2->setEnabled(false);
    ui->a3_3->setEnabled(false);
}

void Matrix4x4Widget::set4x4()
{
    ui->a0_3->setEnabled(true);
    ui->a1_3->setEnabled(true);
    ui->a2_3->setEnabled(true);
    ui->a3_3->setEnabled(true);

    ui->a3_0->setEnabled(true);
    ui->a3_1->setEnabled(true);
    ui->a3_2->setEnabled(true);
    ui->a3_3->setEnabled(true);
}

void Matrix4x4Widget::updateSpinBoxes(QMatrix4x4 matrix)
{
    //TODO Macro this
    try
    {
        ui->a0_0->setValue(matrix(0, 0));
        ui->a0_1->setValue(matrix(0, 1));
        ui->a0_2->setValue(matrix(0, 2));
        ui->a0_3->setValue(matrix(0, 3));

        ui->a1_0->setValue(matrix(1, 0));
        ui->a1_1->setValue(matrix(1, 1));
        ui->a1_2->setValue(matrix(1, 2));
        ui->a1_3->setValue(matrix(1, 3));

        ui->a2_0->setValue(matrix(2, 0));
        ui->a2_1->setValue(matrix(2, 1));
        ui->a2_2->setValue(matrix(2, 2));
        ui->a2_3->setValue(matrix(2, 3));

        ui->a3_0->setValue(matrix(3, 0));
        ui->a3_1->setValue(matrix(3, 1));
        ui->a3_2->setValue(matrix(3, 2));
        ui->a3_3->setValue(matrix(3, 3));
        m_matrix = matrix;
    }
    catch (...)
    {
        qDebug() << "Handling exception not caught in slot.";
    }

    //qDebug() << matrix;
}

void Matrix4x4Widget::updateMatrix()
{
    m_matrix.setToIdentity();
    m_matrix(0, 0) = ui->a0_0->value();
    m_matrix(0, 1) = ui->a0_1->value();
    m_matrix(0, 2) = ui->a0_2->value();
    m_matrix(0, 3) = ui->a0_3->value();

    m_matrix(1, 0) = ui->a1_0->value();
    m_matrix(1, 1) = ui->a1_1->value();
    m_matrix(1, 2) = ui->a1_2->value();
    m_matrix(1, 3) = ui->a1_3->value();

    m_matrix(2, 0) = ui->a2_0->value();
    m_matrix(2, 1) = ui->a2_1->value();
    m_matrix(2, 2) = ui->a2_2->value();
    m_matrix(2, 3) = ui->a2_3->value();

    m_matrix(3, 0) = ui->a3_0->value();
    m_matrix(3, 1) = ui->a3_1->value();
    m_matrix(3, 2) = ui->a3_2->value();
    m_matrix(3, 3) = ui->a3_3->value();
    emit(matrixChanged(m_matrix));
}


/**
 * Sets the order in which the spin boxes are selected when pressing tab.
 * @brief setTabKeyboardOrder
 */
void Matrix4x4Widget::setTabKeyboardOrder()
{
    this->setTabOrder(ui->a0_0, ui->a0_1);
    this->setTabOrder(ui->a0_1, ui->a0_2);
    this->setTabOrder(ui->a0_2, ui->a0_3);
    this->setTabOrder(ui->a0_3, ui->a1_0);

    this->setTabOrder(ui->a1_0, ui->a1_1);
    this->setTabOrder(ui->a1_1, ui->a1_2);
    this->setTabOrder(ui->a1_2, ui->a1_3);
    this->setTabOrder(ui->a1_3, ui->a2_0);

    this->setTabOrder(ui->a2_0, ui->a2_1);
    this->setTabOrder(ui->a2_1, ui->a2_2);
    this->setTabOrder(ui->a2_2, ui->a2_3);
    this->setTabOrder(ui->a2_3, ui->a3_0);

    this->setTabOrder(ui->a3_0, ui->a3_1);
    this->setTabOrder(ui->a3_1, ui->a3_2);
    this->setTabOrder(ui->a3_2, ui->a3_3);
}

