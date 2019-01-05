project(ShaderLabFramework)

cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0020 NEW)
if( POLICY CMP0043)
   cmake_policy(SET CMP0043 NEW)
endif()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.")
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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ShaderLabFramework)

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

#### package section TODO
#requires to install https://download.qt.io/official_releases/qt-installer-framework/3.0.6/ (or other version)
#set environment variable QTIFWDIR

set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
include(InstallRequiredSystemLibraries)

install(
    TARGETS ShaderLabFramework
    RUNTIME DESTINATION ShaderLabFramework
    COMPONENT ShaderLabFramework_installer
    BUNDLE DESTINATION ShaderLabFramework
    COMPONENT ShaderLabFramework_installer
)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/obj/" DESTINATION ShaderLabFramework/obj)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/off/" DESTINATION ShaderLabFramework/off)

function(CPACKIFW_COMMON)
	SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Imperial College London ShaderLabFramework")
	SET(CPACK_PACKAGE_VENDOR "Department of Computing")
	SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
	SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
	SET(CPACK_PACKAGE_VERSION_MAJOR "1")
	SET(CPACK_PACKAGE_VERSION_MINOR "0")
	SET(CPACK_PACKAGE_VERSION_PATCH "0")
	SET(CPACK_PACKAGE_EXECUTABLES "ShaderLabFramework" "Imperial College London ShaderLabFramework")

    set(CPACK_PACKAGE_NAME ShaderLabFramework)
    set(CPACK_PACKAGE_FILE_NAME ShaderLabFramework_installer)
    set(CPACK_PACKAGE_VERSION "1.0.0") # Version of installer
    set(CPACK_COMPONENTS_ALL ShaderLabFramework_installer)
    set(CPACK_IFW_PACKAGE_START_MENU_DIRECTORY ShaderLabFramework)
    set(CPACK_GENERATOR IFW)
    set(CPACK_IFW_VERBOSE ON)

    include(CPack REQUIRED)
    include(CPackIFW REQUIRED)

    cpack_add_component(
        ShaderLabFramework_installer
        DISPLAY_NAME "ShaderLabFramework"
        DESCRIPTION "Install me"
        REQUIRED
    )

    cpack_ifw_configure_component(
        ShaderLabFramework_installer
        FORCED_INSTALLATION
        NAME qt.ShaderLabFramework.installer
        VERSION ${PROJECT_VERSION} # Version of component
        LICENSES License ${PROJECT_SOURCE_DIR}/LICENSE
        DEFAULT TRUE
    )
endfunction()

if(CPACK_IFW_ROOT OR DEFINED ENV{QTIFWDIR})
    if(DEFINED ENV{QTDIR})
        if(APPLE)
            if(EXISTS $ENV{QTDIR}/bin/macdeployqt)
                message("*")
                message("* Note")
                message("*")
                message("* Because of a bug in CPackIFW, it doesn't manage correctly the package created on OSX.")
                message("* Unfortunately CPackIFW doesn't forward the .dmg extension to the binarycreator (see IFW documentation for more details).")
                message("* Therefore it creates a .app directory on OSX, that isn't properly a file and cannot be treated the same way .run and .exe files are.")
                message("* As a result, make package generates an empty directory within the build directory and leaves the bundle somewhere within the _CPack_Packages directory.")
                message("*")
                message("* I strongly suggest to run binarycreator through an external script and use the .dmg extension directly. At least until the bug in CPackIFW isn't fixed.")
                message("* From within the build directory run:")
                message("*")
                message("*     binarycreator -f -c _CPack_Packages/Darwin/IFW/installer/config/config.xml -p _CPack_Packages/Darwin/IFW/installer/packages/ package/installer.dmg")
                message("*")
                message("* The bundle and the dmg file will be created within the package directory as:")
                message("*")
                message("*     package/installer.app [bundle]")
                message("*     package/installer.dmg [dmg]")
                message("*")

                add_custom_command(
                    TARGET ShaderLabFramework POST_BUILD
                    COMMAND $ENV{QTDIR}/bin/macdeployqt ${PROJECT_NAME}.app
                )

                CPACKIFW_COMMON()
            else()
                message("Unable to find executable QTDIR/bin/macdeployqt.")
            endif()
        elseif(WIN32)
            if(EXISTS $ENV{QTDIR}/bin/windeployqt.exe)
				message("starting with deployment")
				message($ENV{QTDIR}/bin/windeployqt.exe)
                add_custom_command(
                    TARGET ShaderLabFramework POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/windeployqt_stuff
                    COMMAND $ENV{QTDIR}/bin/windeployqt.exe --compiler-runtime --dir ${CMAKE_BINARY_DIR}/windeployqt_stuff $<TARGET_FILE:ShaderLabFramework>
                )

                install(
                    DIRECTORY ${CMAKE_BINARY_DIR}/windeployqt_stuff/
                    DESTINATION ${PROJECT_NAME}
                    COMPONENT ShaderLabFramework_installer
                )
				install(DIRECTORY "${PROJECT_SOURCE_DIR}/obj/" DESTINATION ${PROJECT_NAME}/obj COMPONENT ShaderLabFramework_installer)
				install(DIRECTORY "${PROJECT_SOURCE_DIR}/off/" DESTINATION ${PROJECT_NAME}/off COMPONENT ShaderLabFramework_installer)

                CPACKIFW_COMMON()
            else()
                message("Unable to find executable QTDIR/bin/windeployqt.")
            endif()
        elseif(UNIX)
            CPACKIFW_COMMON()
        endif()
    else()
        message("Set properly environment variable QTDIR to be able to create a package.")
    endif()
else()
    message("If you want to enable target package you can:")
    message("\t* Either pass -DCPACK_IFW_ROOT=<path> to cmake")
    message("\t* Or set the environment variable QTIFWDIR")
    message("To specify the location of the QtIFW tool suite.")
    message("The specified path should not contain bin at the end (for example: D:\\DevTools\\QtIFW2.0.5).")
endif()