This is the Computer Graphics Shader Lab.

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
FROM, OUT OF OR IN C

Compile and run:
git clone https://gitlab.doc.ic.ac.uk/bkainz/ShaderLabFramework.git
cd ShaderLabFramework
mkdir build
cd build
cmake ..
make
./ShaderLab

Features:
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


