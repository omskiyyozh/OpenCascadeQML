cmake_minimum_required(VERSION 3.14)

project(OpenCascadeQML VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)
find_package (OpenGL REQUIRED)

message ("$ENV{CSF_OCCTIncludePath}")

set (OpenCASCADE_INCLUDE_DIR $ENV{CSF_OCCTIncludePath})
set (OpenCASCADE_LIB_DIR $ENV{CSF_OCCTLibPath})

include_directories(${OpenCASCADE_INCLUDE_DIR})
link_directories(${OpenCASCADE_LIB_DIR})
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
else()
        add_executable(OpenCascadeQML
          ${PROJECT_SOURCES}
        )
endif()

target_compile_definitions(OpenCascadeQML
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(OpenCascadeQML
    PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
    ${OPENGL_LIBRARIES}
    TKBin
    TKBinL
    TKBinTObj
    TKBinXCAF
    TKBO
    TKBool
    TKBRep
    TKCAF
    TKCDF
    TKDCAF
    TKDraw
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
    TKQADraw
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
    TKTObjDRAW
    TKTopAlgo
    TKTopTest
    TKV3d
    TKVCAF
    TKViewerTest
    TKVRML
    TKXCAF
    TKXDEDRAW
    TKXDEIGES
    TKXDESTEP
    TKXml
    TKXmlL
    TKXmlTObj
    TKXmlXCAF
    TKXSBase
    TKXSDRAW
    User32.lib
)

set_target_properties(OpenCascadeQML PROPERTIES
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(OpenCascadeQML)
    qt_finalize_executable(OpenCascadeQML)
endif()
