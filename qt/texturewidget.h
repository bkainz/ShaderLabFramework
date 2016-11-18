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

#ifndef TEXTUREWIDGET_H
#define TEXTUREWIDGET_H

#include <iostream>
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>

#include "uniformEditorWidget.h"

class TextureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextureWidget(QWidget *parent = 0);
    TextureWidget(QWidget *parent, QString textureName, bool isAShaderProgramUniform);
    ~TextureWidget();
    void setTextureIcon(QString &filePath);

signals:
    void textUpdated(QString);
    void sendTextureName(QString, bool);

    public slots:
    void updatePath(QString path);
    void buttonClicked();

private:
    QVBoxLayout *m_verticalLayout;
    QGroupBox *m_groupBox;
    QLineEdit *m_lineEdit;
    QPushButton *m_pushButton;
    QString m_textureName;
    bool m_isAShaderProgramUniform;
};

#endif // TEXTUREWIDGET_H
