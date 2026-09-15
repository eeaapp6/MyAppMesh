# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(GUIDialog      # 项目名称
    VERSION 1.0.0    # 项目版本
    LANGUAGES CXX    # 项目语言
)

# Qt配置
set(CMAKE_AUTOMOC ON)    # MOC
set(CMAKE_AUTORCC ON)    # RESOURCE
set(CMAKE_AUTOUIC ON)    # UI

# 查找Qt5组件，根据项目增删
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 定义为动态库
add_library(${PROJECT_NAME} SHARED)

# 根据路径添加文件
# ${CMAKE_CURRENT_SOURCE_DIR}为当前cmake所在的路径
# ${CMAKE_SOURCE_DIR}为构建时选择的cmake所在的路径
set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIDialogAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FileDialogDirectory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIDialogBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorMapperEditDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SliceDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/IsosurfDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetPlane.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetSphere.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetCylinder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/StreamLineDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GlyphDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ExtremumDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ThresholdDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DeformDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/LightWidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/LightSubWidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/ProbeDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIDialogManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorLibaryDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorLibaryEditDialog.h 
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIDialogBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileDialogDirectory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorMapperEditDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SliceDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IsosurfDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetPlane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetSphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetCylinder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StreamLineDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GlyphDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExtremumDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ThresholdDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DeformDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LightWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/LightSubWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/ProbeDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIDialogManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorLibaryDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorLibaryEditDialog.cpp 
)

set(UI_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/FileDialogDirectory.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorMapperEditDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SliceDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/IsosurfDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetPlane.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetSphere.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetCylinder.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ClipSliceWidgetBox.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/StreamLineDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GlyphDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ExtremumDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ThresholdDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/DeformDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/LightWidget.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/LightSubWidget.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/ProbeDialog.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorLibaryDialog.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorLibaryEditDialog.ui 
)

# 根据变量名将文件新建分组，下面就是把ui、qrc和TS文件单独分组
source_group("Form Files" FILES ${UI_FILES})

# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有文件 
target_sources(${PROJECT_NAME} PRIVATE    # 使用${PROJECT_NAME}之前必须在项目信息中设置项目名称
    ${HEADERS}
    ${SOURCES}
    ${UI_FILES}
)

# 添加多核编译支持
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

# 包含目录根据pro文件中的 INCLUDEPATH 填写
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

# 输出目录配置  根据当前的路径找到output路径
set(OUTPUT_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../output/bin_d")
set(OUTPUT_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../output/bin")

# 定义一个宏来简化跨平台库链接
macro(link_fitk_library target_name lib_name)
    if(WIN32)
        target_link_libraries(${target_name} 
            PUBLIC 
                debug "${OUTPUT_DIR_DEBUG}/${lib_name}.lib"
                optimized "${OUTPUT_DIR_RELEASE}/${lib_name}.lib"
        )
    elseif(UNIX)
        target_link_libraries(${target_name} 
            PUBLIC 
                debug "${OUTPUT_DIR_DEBUG}/lib${lib_name}.so"
                optimized "${OUTPUT_DIR_RELEASE}/lib${lib_name}.so"
        )
    endif()
    if(TARGET ${lib_name})
        add_dependencies(${target_name} ${lib_name})
    endif()
endmacro()

# 用户自定义TOOLS路径选项
if(NOT TOOLS_DIR)
    if(DEFINED ENV{TOOLS_DIR})
        set(TOOLS_DIR "$ENV{TOOLS_DIR}" CACHE PATH "Path to the Tools directory")
        message(STATUS "TOOLS_DIR loaded from environment: ${TOOLS_DIR}")
    else()
        set(TOOLS_DIR "" CACHE PATH "Path to the Tools directory")
        message(WARNING "TOOLS_DIR is not configured. Please set it via -DTOOLS_DIR=... or environment variable.")
        message(FATAL_ERROR "Please configure TOOLS_DIR first.")
    endif()
endif()

# 链接
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        # Qt5的组件
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
)

# 导入内部库
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKInterfaceCFDPost)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} FITKEasyParam)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} OperatorsInterface)
link_fitk_library(${PROJECT_NAME} PostGraphAdaptor)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        GUIDialog_API
)

# 版本信息
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
	
    # DEBUG
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR_DEBUG}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR_DEBUG}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR_DEBUG}"

    # RELEASE
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR_RELEASE}"

    # RELWITHDEBINFO
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_RELEASE}"

    # MINSIZEREL
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${OUTPUT_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${OUTPUT_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${OUTPUT_DIR_RELEASE}"

    # 设置生成文件的位置
    AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/generate/${PROJECT_NAME}/autogen"
)