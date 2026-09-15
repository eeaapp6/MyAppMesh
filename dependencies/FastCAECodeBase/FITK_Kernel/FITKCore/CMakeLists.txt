# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKCore
    VERSION 1.0.4
    LANGUAGES CXX
)

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt5组件

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 定义为共享库
add_library(${PROJECT_NAME} SHARED)

# 手动添加文件列表
set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCoreAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCoreMacros.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKType.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractObject.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKColorMap.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractObjectContainer.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGUIObject.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGUIDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractDataManagerPrivate.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractDataManager.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractDataManagerHelper.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractDataObject.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractNDataObject.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKDataRepo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGUIRepo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKVec3D.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKVec3DAlg.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKDirFileTools.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKDataFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGUI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKThreadPool.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKThreadPoolPrivate.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKThreadTask.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKThreadTaskGroup.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractOperator.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOperatorRepo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOperatorRepoPrivate.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKEnumTransformer.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKActionOperator.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractParam.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractParamList.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGraphWidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGraphObject.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGraphObjRepo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKVarientParams.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCircleAlg.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTreeWidgetOperator.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKNumberComplex.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTriangleAlg.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKScriptTextRepo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLibInfo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKContainerAlg.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSystemInfo.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractObject.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractObjectContainer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGUIObject.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGUIDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractDataObject.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractNDataObject.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractDataManagerPrivate.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractDataManagerHelper.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKDataRepo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGUIRepo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKVec3D.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKVec3DAlg.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKDirFileTools.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKDataFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGUI.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKThreadPool.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKThreadPoolPrivate.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKThreadTask.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKThreadTaskGroup.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractOperator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOperatorRepo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOperatorRepoPrivate.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKActionOperator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractParamList.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGraphWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGraphObject.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGraphObjRepo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKVarientParams.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCircleAlg.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTreeWidgetOperator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKNumberComplex.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTriangleAlg.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKScriptTextRepo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLibInfo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKContainerAlg.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSystemInfo.cpp 
)

set(UI_FILES_LIST
#    ${CMAKE_CURRENT_SOURCE_DIR}/PluginManageDialog.ui 
)

set(RESOURCES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGL.qrc
)

source_group("Ui Files" FILES ${UI_FILES_LIST})
source_group("Resource Files" FILES ${RESOURCES_SOURCES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*|.*autouic_.*\\.stamp$")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_FILES_LIST}
    ${RESOURCES_SOURCES}
)

# 包含目录
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
)

# 添加多核编译支持
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

# 链接
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Gui 
        Qt5::Widgets
)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKCore_API
        FITKUseAppFramework
    PUBLIC 
        $<$<CONFIG:Debug>:QT_QML_DEBUG>
)

# 输出目录配置
set(OUTPUT_DIR_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../../output/bin_d")
set(OUTPUT_DIR_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../output/bin")

# 版本信息及输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.4
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