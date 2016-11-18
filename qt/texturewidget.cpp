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

#include "qt/texturewidget.h"

TextureWidget::TextureWidget(QWidget *parent) : QWidget(parent)
{

    //Create a group box with a line edit and a push button
    m_groupBox = new QGroupBox("Texture");
    m_lineEdit = new QLineEdit();
    m_pushButton = new QPushButton("Browse");

    //Put the line edit and the button in a layout
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(m_lineEdit, 0, 0);
    gridLayout->addWidget(m_pushButton, 0, 1);

    //Set the group box layout
    m_groupBox->setLayout(gridLayout);

    //Add the group box to a vertical layout
    //add a stretch after the last widget to get a group box of a correct size.
    m_verticalLayout = new QVBoxLayout(this);
    m_verticalLayout->addWidget(m_groupBox);

    m_verticalLayout->addStretch(1);

    //Connections
    connect(m_pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked2()));

}

TextureWidget::TextureWidget(QWidget *parent, QString textureName, bool isAShaderProgramUniform) :
    QWidget(parent), m_textureName(textureName), m_isAShaderProgramUniform(isAShaderProgramUniform)
{
    //Create a group box with a line edit and a push button
    m_groupBox = new QGroupBox("Texture");
    m_lineEdit = new QLineEdit();
    m_pushButton = new QPushButton("Browse");

    //Put the line edit and the button in a layout
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(m_lineEdit, 0, 0);
    gridLayout->addWidget(m_pushButton, 0, 1);

    //Set the group box layout
    m_groupBox->setLayout(gridLayout);

    //Add the group box to a vertical layout
    //add a stretch after the last widget to get a group box of a correct size.
    m_verticalLayout = new QVBoxLayout(this);
    m_verticalLayout->addWidget(m_groupBox);

    m_verticalLayout->addStretch(1);

    //Connections
    connect(m_pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}

TextureWidget::~TextureWidget()
{
    delete m_groupBox;
}

void TextureWidget::setTextureIcon(QString& filePath)
{
    QIcon icon = QIcon(QPixmap(filePath));
    m_pushButton->setIcon(icon);
    m_pushButton->setText("");
}

void TextureWidget::updatePath(QString path)
{
    m_lineEdit->setText(path);
    this->setTextureIcon(path);
}

void TextureWidget::buttonClicked()
{
    emit sendTextureName(m_textureName, m_isAShaderProgramUniform);
}

