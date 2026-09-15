# 指定CMake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息，名称，版本，使用的编程语言
project(CADOCCApp VERSION 1.0.0 LANGUAGES CXX)

# 全局构建配置，C++11标准强制使用
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 禁用编译器扩展
set(CMAKE_CXX_EXTENSIONS OFF)  

# 启动Qt自动工具
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 全局编译选项（仅 MSVC 需要）
if(MSVC)
    # 强制源文件编码为 UTF-8
    add_compile_options(/utf-8)
endif()

# 使用宽字符 Windows API
add_compile_definitions(UNICODE _UNICODE)

#  全局 RPATH 设置 
if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path")
elseif(UNIX AND NOT WIN32)   # Linux
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
elseif(WIN32 AND NOT MSVC)   # MinGW / Clang
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()

# 设置输出目录 ，统一设置为项目的根目录output文件夹下
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/output)

# Debug放在bin_d目录下，Release放在bin目录下
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR}/bin_d)    # .exe, .dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR}/bin)    # .exe, .dll

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR}/bin_d)    # .dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR}/bin)    # .dll

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR}/bin_d)    # .lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR}/bin)    # .lib

# 创建这些目录
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
file(MAKE_DIRECTORY ${OUTPUT_DIR}/bin)
file(MAKE_DIRECTORY ${OUTPUT_DIR}/bin_d)

# Tools路径配置：优先使用 CMake 参数，其次尝试从环境变量读取
set(TOOLS_DIR "" CACHE PATH "Path to Tools directory containing third-party libraries")
if(NOT TOOLS_DIR)
    if(DEFINED ENV{TOOLS_DIR})
        set(TOOLS_DIR "$ENV{TOOLS_DIR}" CACHE PATH "Path to Tools directory containing third-party libraries")
        message(STATUS "TOOLS_DIR loaded from environment: ${TOOLS_DIR}")
    endif()
endif()

if(NOT TOOLS_DIR)
    set(TOOLS_DIR "${CMAKE_SOURCE_DIR}/Tools" CACHE PATH "Path to Tools directory containing third-party libraries")
    message(STATUS "TOOLS_DIR using default: ${TOOLS_DIR}")
endif()

# 查找Qt5组件
find_package(Qt5 COMPONENTS Core Widgets Gui Network REQUIRED Xml Svg OpenGL Designer)

# 包含 VTK 配置
include(${TOOLS_DIR}/cmake/VTKConfig.cmake)

# 包含 qwt 配置
include(${TOOLS_DIR}/cmake/qwtConfig.cmake)

# 包含 HDF5 配置
include(${TOOLS_DIR}/cmake/HDF5Config.cmake)

# 包含 OCC 配置
include(${TOOLS_DIR}/cmake/OCCConfig.cmake)

# 包含 SARibbon 配置
include(${TOOLS_DIR}/cmake/SARibbonConfig.cmake)

# 包含当前目录
include_directories(${CMAKE_SOURCE_DIR})

# 定义添加子项目
add_subdirectory(FITK_Kernel)
add_subdirectory(FITK_Interface)
add_subdirectory(FITK_Component)
add_subdirectory(GUIWidget)
add_subdirectory(GUIFrame)
add_subdirectory(PickDataProvider)
add_subdirectory(GUIDialog)
add_subdirectory(GraphDataProvider)
add_subdirectory(OperatorsInterface)
add_subdirectory(OperatorsGUI)
add_subdirectory(OperatorsModel)
add_subdirectory(PythonInterface)
add_subdirectory(CADOCCApp)
