cmake_minimum_required(VERSION 3.14)

project(OpenCascadeQML VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)
find_package (OpenCascade REQUIRED)
find_package (OpenGL REQUIRED)

include_directories(${OpenCASCADE_INCLUDE_DIR})

set(PROJECT_SOURCES
        src/main.cpp
        src/OCCView.h
        src/OCCView.cpp
        qml/qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(OpenCascadeQML
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

#target_include_directories(OpenCascadeQML PUBLIC ${Open_CASCADE_INCLUDE_DIR})
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET OpenCascadeQML APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(OpenCascadeQML SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(OpenCascadeQML
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(OpenCascadeQML
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(OpenCascadeQML
    PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
    ${OPENGL_LIBRARIES}
    #${OpenCASCADE_LIBRARIES}
    TKBin
    TKBinL
    TKBinTObj
    TKBinXCAF
    TKBO
    TKBool
    TKBRep
    TKCAF
    TKCDF
    TKernel
    TKFeat
    TKFillet
    TKG2d
    TKG3d
    TKGeomAlgo
    TKGeomBase
    TKHLR
    TKIGES
    TKLCAF
    TKMath
    TKMesh
    TKMeshVS
    TKOffset
    TKOpenGl
    TKPrim
    TKService
    TKShHealing
    TKStd
    TKStdL
    TKSTEP
    TKSTEP209
    TKSTEPAttr
    TKSTEPBase
    TKSTL
    TKTObj
    TKTopAlgo
    TKV3d
    TKVCAF
    TKVRML
    TKXCAF
    TKXDEIGES
    TKXDESTEP
    TKXml
    TKXmlL
    TKXmlTObj
    TKXmlXCAF
    TKXSBase
    User32.lib
)

set_target_properties(OpenCascadeQML PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(OpenCascadeQML)
    qt_finalize_executable(OpenCascadeQML)
endif()
