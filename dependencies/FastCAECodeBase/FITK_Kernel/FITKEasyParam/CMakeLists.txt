# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKEasyParam
    VERSION 1.0.0
    LANGUAGES CXX
)

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置并查找Qt5组件

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 定义为共享库
add_library(${PROJECT_NAME} SHARED)

# 手动添加文件列表
set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKEasyParamAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKEasyParamFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKEasyParamWidgetFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractEasyParam.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractEasyParamWidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamBool.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamBoolGroup.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamCombox.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamComboxVector.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamDouble.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamDoubleList.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamInt.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamLabel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamRadioGroup.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamString.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParameter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetBool.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetBoolGroup.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetComBox.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetDouble.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetInt.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetRadioGroup.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetString.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetSciNotation.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetVBox.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetHBox.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetComLine.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKEasyParamFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKEasyParamWidgetFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractEasyParam.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractEasyParamWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamBool.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamBoolGroup.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamCombox.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamComboxVector.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamDouble.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamDoubleList.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamInt.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamLabel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamRadioGroup.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParamString.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParameter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetBool.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetBoolGroup.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetComBox.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetDouble.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetInt.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetRadioGroup.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetString.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetSciNotation.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetVBox.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetHBox.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetComLine.cpp 
)

set(UI_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetBoolGroup.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetRadioGroup.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetVBox.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetHBox.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetComLine.ui 
)

set(RESOURCES_SOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/qrc/qianfan.qrc
)

source_group("Ui Files" FILES ${UI_FILES_LIST})
source_group("Resource Files" FILES ${RESOURCES_SOURCES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_FILES_LIST}
    ${RESOURCE_FILES_LIST}
)

# 添加多核编译支持
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

# 包含目录
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
)

# 输出目录配置
set(OUTPUT_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../../output/bin_d")
set(OUTPUT_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../output/bin")

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

# 链接
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Gui 
        Qt5::Widgets
)

link_fitk_library(${PROJECT_NAME} FITKCore)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
	    FITKEasyParam_API
)

# 版本信息及输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.0
    SOVERSION 1

    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR_DEBUG}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR_DEBUG}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR_DEBUG}"

    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR_RELEASE}"

    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR_RELEASE}"

    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${OUTPUT_DIR_RELEASE}"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${OUTPUT_DIR_RELEASE}"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${OUTPUT_DIR_RELEASE}"

    # 设置生成文件的位置
    AUTOGEN_BUILD_DIR "${CMAKE_SOURCE_DIR}/generate/${PROJECT_NAME}/autogen"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_NAME:${PROJECT_NAME}> build completed."
)