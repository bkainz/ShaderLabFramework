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

#include "Vector4Widget.h"
#include <iostream>
#include <QDebug>

Vector4Widget::Vector4Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Vector4Widget)
{
    ui->setupUi(this);

    connect(ui->a0_0, SIGNAL(valueChanged(double)), this, SLOT(updateVector(double)));
    connect(ui->a0_1, SIGNAL(valueChanged(double)), this, SLOT(updateVector(double)));
    connect(ui->a0_2, SIGNAL(valueChanged(double)), this, SLOT(updateVector(double)));
    connect(ui->a0_3, SIGNAL(valueChanged(double)), this, SLOT(updateVector(double)));

}

Vector4Widget::~Vector4Widget()
{
    delete ui;
}

void Vector4Widget::set2D()
{
    ui->a0_0->setEnabled(true);
    ui->a0_1->setEnabled(true);
    ui->a0_2->setEnabled(false);
    ui->a0_3->setEnabled(false);
}

void Vector4Widget::set3D()
{
    ui->a0_0->setEnabled(true);
    ui->a0_1->setEnabled(true);
    ui->a0_2->setEnabled(true);
    ui->a0_3->setEnabled(false);
}

void Vector4Widget::set4D()
{
    ui->a0_0->setEnabled(true);
    ui->a0_1->setEnabled(true);
    ui->a0_2->setEnabled(true);
    ui->a0_3->setEnabled(true);
}

void Vector4Widget::updateSpinBoxes(QVector4D vector)
{
    try
    {
        ui->a0_0->setValue(vector.x());
        ui->a0_1->setValue(vector.y());
        ui->a0_2->setValue(vector.z());
        ui->a0_3->setValue(vector.w());

        m_vector = vector;
    }
    catch (...)
    {
        qDebug() << "Handling exception not caught in slot.";
    }

}

void Vector4Widget::updateVector(double val)
{
    m_vector.setX(ui->a0_0->value());
    m_vector.setY(ui->a0_1->value());
    m_vector.setZ(ui->a0_2->value());
    m_vector.setW(ui->a0_3->value());

    emit(vectorChanged(m_vector));
}


