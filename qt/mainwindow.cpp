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

#include "qt/mainwindow.h"
#include "qt/uniformEditorWidget.h"
#include "qt/materialEditorWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QSize>
#include "Vector4Widget.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Enable multisampling
    QSurfaceFormat format = ui->m_GLWidget->format();
    format.setSamples(16);
    ui->m_GLWidget->setFormat(format);

    this->resize(QDesktopWidget().availableGeometry(this).size().width() * 0.5,
        QDesktopWidget().availableGeometry(this).size().height() * 0.97);

    QApplication::restoreOverrideCursor();
    QCoreApplication::organizationName() = QString("Imperial College London, Department of Computing");
    QCoreApplication::applicationName() = QString("Computer Graphics GLSL Shader Lab");

    connect(ui->matricesWidget, SIGNAL(modelMatrixChanged(QMatrix4x4)), ui->m_GLWidget, SLOT(modelMatrixUpdated(QMatrix4x4)));
    connect(ui->matricesWidget, SIGNAL(viewMatrixChanged(QMatrix4x4)), ui->m_GLWidget, SLOT(viewMatrixUpdated(QMatrix4x4)));
    connect(ui->matricesWidget, SIGNAL(projectionMatrixChanged(QMatrix4x4)), ui->m_GLWidget, SLOT(projectionMatrixUpdated(QMatrix4x4)));

    //TODO define special signal, if shader is valid
    connect(ui->m_GLWidget, SIGNAL(updateUniformTab()), this, SLOT(updateUniformTab()));
    connect(ui->m_GLWidget, SIGNAL(updateMaterialTab()), this, SLOT(updateMaterialTab()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showLogTab()
{
    //Display the log tab
    ui->m_tabWidget->setCurrentIndex(0);
}

void MainWindow::updateUniformTab()
{
    //TODO: this is triggered twice. why?
    ui->m_GLWidget->update();
    if (ui->m_uniformsTab->children().length() == 0) //init layout and editor
    {
        m_uniformEditor = new UniformEditorWidget(
            ui->m_GLWidget->getShaderProgram(), ui->m_GLWidget->getShaderDisplayProgram(),
            ui->m_GLWidget->context(), this, ui->m_GLWidget);

        QHBoxLayout* uniformHBoxLayout = new QHBoxLayout(ui->m_uniformsTab);
        uniformHBoxLayout->addWidget(m_uniformEditor);

        ui->m_uniformsTab->setLayout(uniformHBoxLayout);

        //Update the texture path
        connect(m_uniformEditor, SIGNAL(textureBrowse(QString, bool)), ui->m_GLWidget, SLOT(setTexture(QString, bool)));

        connect(ui->m_GLWidget, SIGNAL(updateTexturePath(QString)), m_uniformEditor, SLOT(updateTexturePath(QString)));

        connect(m_uniformEditor, SIGNAL(updateGL()), ui->m_GLWidget, SLOT(update()));
    }
    else //update widget
    {
        ui->m_GLWidget->makeCurrent();
        ui->m_GLWidget->update();
        m_uniformEditor->updateShaderPrograms(
            ui->m_GLWidget->getShaderProgram(), ui->m_GLWidget->getShaderDisplayProgram(),
            ui->m_GLWidget->context());
    }
}

void MainWindow::updateMaterialTab()
{
    QVector<Object> objectList = ui->m_GLWidget->getScene()->getObjects();
    ui->m_GLWidget->update();

    if (ui->m_materialTab->children().length() == 0) //init layout and editor
    {
        for (int k = 0; k < objectList.size(); k++)
        {
            MaterialEditorWidget* m_materialEditor = new MaterialEditorWidget(k, ui->m_GLWidget->getScene(), this);

            QHBoxLayout* HBoxLayout = new QHBoxLayout(ui->m_materialTab);
            HBoxLayout->addWidget(m_materialEditor);

            ui->m_materialTab->setLayout(HBoxLayout);

            connect(m_materialEditor, SIGNAL(updateGL()), ui->m_GLWidget, SLOT(update()));
            m_materialEditors.push_back(m_materialEditor);

            connect(m_materialEditor, SIGNAL(updateMaterial(int, Material)), ui->m_GLWidget, SLOT(updateMaterial(int, Material)));
        }
    }
    else //update widgets
    {
        for (int k = 0; k < objectList.size(); k++)
        {
            ui->m_GLWidget->makeCurrent();
            ui->m_GLWidget->update();
            m_materialEditors.at(k)->updateEditor(k);
        }
    }

}

