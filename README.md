# This is the Computer Graphics Shader Lab.

ShaderLab is a teaching tool to solidify the fundamentals of Computer Graphics. The ShaderLab framework is based on Qt5, CMake, OpenGL 4.0, and GLSL and allows the student to modify GLSL shaders in an IDE-like environment. The framework is able to render shaded polyhedral geometry (.off/.obj), supports image-based post-processing, and  allows to implement simple ray-tracing algorithms. This tool will be intensively tested by 140 [Imperial College London CO317 Computer Graphics](http://wp.doc.ic.ac.uk/bkainz/teaching/co317-computer-graphics/) students in Spring 2017.

![HSaderLabFramework overview](http://wp.doc.ic.ac.uk/bkainz/wp-content/uploads/sites/97/2016/11/ShaderLab-768x800.png "ShaderLab framework")


Copyright (c) 2016 Bernhard Kainz, Antoine S Toisoul
(b.kainz@imperial.ac.uk, antoine.toisoul13@imperial.ac.uk)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE.

This tool was inspired by ['Shader Maker'](http://cgvr.cs.uni-bremen.de/teaching/shader_maker/), a Computer Graphics teaching tool developed by the team of Prof.Dr.Gabriel Zachmann, 	University of Bremen. The Copyright holders of 'Shader Maker' granted permission to use their code. The GLSL shader editor syntax highlighter classes are based on the implementation provided by 'Shader Maker'.

## Dependencies:
- [Qt5](https://www.qt.io/download/)
- [CMake](https://cmake.org/download/) 
- [GLEW](http://glew.sourceforge.net/)

## Compile and run:

git clone https://github.com/bkainz/ShaderLabFramework.git

cd ShaderLabFramework

mkdir build

cd build

cmake ..

make

./ShaderLabFramework

## Features:
- render window
- mouse-based interaction
- shader compilation log output
- Shader editor with syntax highlighter
- render-to-texture wrapped shader pipeline
- Object selection
- wireframe mode
- display of origin
- backface culling
- screenshot tool
- Camera parameter adjustments
- direct manipulation widget for Model, View and Projection Matrices
- automatic evaluation of user uniforms in uniform editor; 
  default material and matrix uniforms are ignored
- simple numeric material editor 
- GPU info output
- saving and loading of whole shader pipelines or individual shaders
- full OpenGL 4.x support

## TODO:
- search function in code editor
- some random bug in the screenshot function

## Examples

![Gouraud shading](https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/3a.jpg "Gouraud shading" =400x)


