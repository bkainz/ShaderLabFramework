# This is the Computer Graphics Shader Lab Framework.

The Shader Lab Framework is a teaching tool to solidify the fundamentals of Computer Graphics. The ShaderLab framework is based on Qt5, CMake, OpenGL 4.0, and GLSL and allows the student to modify GLSL shaders in an IDE-like environment. The framework is able to render shaded polyhedral geometry (.off/.obj), supports image-based post-processing, and  allows to implement simple ray-tracing algorithms. This tool has been intensively tested by 140 [Imperial College London CO317 Computer Graphics](http://wp.doc.ic.ac.uk/bkainz/teaching/co317-computer-graphics/) students in Spring 2017 and will continue to serve as basis framework for our coursework.

A comprehensive overview over out Computer Graphics course using ShaderLabFramework has been accepted for the EG 2017 educational track:
[Antoine Toisoul, Daniel Rueckert, Bernhard Kainz, Accessible GLSL Shader Programming](https://diglib.eg.org/handle/10.2312/eged20171024), in Proc. EuroGraphics 2017, 2017

YouTube Video:

<p align="center">
  <a href="https://www.youtube.com/watch?v=aa_S3yeqWZk">EG educational 2017 fast forward 
  <br>
  <img src="https://img.youtube.com/vi/aa_S3yeqWZk/0.jpg"></a>
</p>

## ShaderLabFramework overview

![HSaderLabFramework overview](http://wp.doc.ic.ac.uk/bkainz/wp-content/uploads/sites/97/2016/11/ShaderLab-768x800.png "ShaderLab framework")


## License

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
- [Qt5 v5.9+](https://www.qt.io/download/)
- [CMake](https://cmake.org/download/) 

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
- conversion of the coordinate frame for OpenGL4 (will not work on Mac etc. at the moment)

## Examples implemented using the ShaderLabFramework

### Gouraud shading
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/3a.jpg" alt="Gouraud shading" width="250"/>

### Phong shading
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/3b.jpg" alt="Phong shading" width="250"/>

### Toon shading
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/3c.jpg" alt="Toon shading" width="250"/>

### Blinn-Phong shading
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/3d.jpg" alt="Blinn-Phong shading" width="250"/>

### Geometry subdivision
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/4alevel0.jpg" alt="Geometry subdivision level 0" width="250"/>
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/4alevel2.jpg" alt="Geometry subdivision level 2" width="250"/>

### Texture 
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/6a1.jpg" alt="Texture" width="250"/>
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/6a2.jpg" alt="Texture" width="250"/>

### Bump mapping 
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/6b.jpg" alt="Bump mapping" width="250"/>

### Render-to-texture, screen aligned image plane shading (simple blur example)
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/6c.jpg" alt="image-based blur" width="250"/>

### Simple ray tracing
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/7a.jpg" alt="Simple ray tracing" width="250"/>
<img src="https://www.doc.ic.ac.uk/~bkainz/teaching/CO317/7b_50MCrays.jpg" alt="Simple monte-carlo ray tracing" width="250"/>

