# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKInterfaceMeshGen
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInterfaceMeshGenAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshGenInterface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGlobalMeshSizeInfo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionMeshSize.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeometryMeshSize.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionMeshSizeBox.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionMeshSizeCylinder.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionMeshSizeSphere.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionMeshSizeGeom.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMeshSizeInfoGenerator.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMesherDriver.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMeshProcessor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGeometryMeshSizeGenerator.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKZonePoints.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionGeometryRefine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGlobalMeshGenerateAlgorithmInfo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMeshAlgorithmInfoGenerator.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshParameterBase.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshParameterManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshParameterLine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshParameterSurface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshParameterVolume.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSeed.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSeedManager.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshGenInterface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGlobalMeshSizeInfo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionMeshSize.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeometryMeshSize.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionMeshSizeBox.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionMeshSizeCylinder.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionMeshSizeSphere.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionMeshSizeGeom.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMeshSizeInfoGenerator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMesherDriver.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMeshProcessor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGeometryMeshSizeGenerator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKZonePoints.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRegionGeometryRefine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGlobalMeshGenerateAlgorithmInfo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMeshAlgorithmInfoGenerator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshParameterBase.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshParameterManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshParameterLine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshParameterSurface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshParameterVolume.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSeed.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSeedManager.cpp 
)

set(UI_FILES_LIST
#    ${CMAKE_CURRENT_SOURCE_DIR}/PluginManageDialog.ui 
)

set(RESOURCES_SOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/../qrc/qianfan.qrc
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
        Qt5::Widgets 
        Qt5::Gui
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKInterfaceMeshGen_API
)

# 版本信息
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

