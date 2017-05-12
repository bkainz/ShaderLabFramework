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

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

#### Cpack section TODO
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${BIN_INSTALL_DIR}")
INCLUDE(InstallRequiredSystemLibraries)
INSTALL(TARGETS ShaderLabFramework DESTINATION bin)

if(WIN32)
#execute_process(COMMAND windeployqt $<TARGET_FILE:ShaderLabFramework> WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_custom_command(OUTPUT log.txt
  DEPENDS ShaderLabFramework 
  COMMAND windeployqt $<TARGET_FILE:ShaderLabFramework>
  WORKING_DIRECTORY $<TARGET_FILE:ShaderLabFramework>
  VERBATIM
)
endif(WIN32)

#message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/ShaderLabFramework)
#run qt deployment tool
#get_property(binary_location TARGET ShaderLabFramework PROPERTY LOCATION)
#set( test $<TARGET_FILE:ShaderLabFramework>)
#message (STATUS "fancy_lib_location == ${TARGET_FILE:ShaderLabFramework}")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/obj/" DESTINATION bin/obj)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/off/" DESTINATION bin/off)


SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Imperial College London ShaderLabFramework")
SET(CPACK_PACKAGE_VENDOR "Department of Computing")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_EXECUTABLES "ShaderLabFramework" "Imperial College London ShaderLabFramework")

INCLUDE(CPack)
