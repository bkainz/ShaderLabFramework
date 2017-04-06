project(ShaderLabFramework)

cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0020 NEW)
if( POLICY CMP0043)
   cmake_policy(SET CMP0043 NEW)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
# set up a mapping so that the Release configuration for the Qt imported target is
# used in the COVERAGE CMake configuration.
set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
					${CMAKE_CURRENT_SOURCE_DIR}/qt
					${CMAKE_CURRENT_SOURCE_DIR}/opengl
					${CMAKE_CURRENT_BINARY_DIR} 
					${CMAKE_BINARY_DIR} )
			
set(SRCS  main.cpp 
    opengl/camera.cpp 
    opengl/framebuffer.cpp 
    opengl/light.cpp 
    opengl/material.cpp 
    opengl/mesh.cpp 
    opengl/object.cpp 
    opengl/scene.cpp 
    opengl/texture.cpp 
    qt/gldisplay.cpp 
    qt/mainwindow.cpp 
    qt/MatricesWidget.cpp 
    qt/Matrix4x4Widget.cpp
	qt/GLSLCodeEditor.cpp
	qt/SyntaxHighlighterGLSL.cpp
	qt/codeeditor.cpp
	qt/GLSLEditorWindow.cpp
	qt/GLSLEditorWidget.cpp
	qt/uniformEditorWidget.cpp
	qt/Vector4Widget.cpp
	qt/texturewidget.cpp
	qt/materialEditorWidget.cpp
	)
			
set(HDRS opengl/camera.h 
    opengl/framebuffer.h 
    opengl/light.h 
    opengl/material.h 
    opengl/mesh.h 
    opengl/object.h 
    opengl/openglheaders.h 
    opengl/scene.h 
    opengl/texture.h 
    qt/gldisplay.h 
    qt/mainwindow.h 
    qt/MatricesWidget.h 
    qt/Matrix4x4Widget.h
	qt/GLSLCodeEditor.h
	qt/codeeditor.h
	#qt/EditWindow.h
	#qt/Editor.h
	qt/GLSLEditorWindow.h
	qt/GLSLEditorWidget.h
	qt/uniformEditorWidget.h
	qt/Vector4Widget.h
	qt/texturewidget.h
	qt/materialEditorWidget.h
	)
	
set(FORMS 
	qt/mainwindow.ui 
    qt/MatricesWidget.ui 
    qt/Matrix4x4Widget.ui
	qt/GLSLEditorWindow.ui
	qt/GLSLEditorWidget.ui
	qt/uniformEditorWidget.ui
	qt/Vector4Widget.ui
	qt/materialEditorWidget.ui
	qt/ressources.qrc)

set(RCS src/images/images.qrc )	

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(ShaderLabFramework ${SRCS} ${HDRS} ${FORMS})
qt5_use_modules(ShaderLabFramework Core Gui OpenGL Xml)
target_link_libraries(ShaderLabFramework ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
