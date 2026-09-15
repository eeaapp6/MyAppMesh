# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKInterfaceModel
    VERSION 0.9.1
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInterfaceModelAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKModelEnum.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractModelComponent.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKNodeList.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractElement.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementList.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKUnstructuredMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKStructuredMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParticalMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementVertex.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementLine.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementTri.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementQuad.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementTet.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementWedge.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementHex.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementPolygon.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAssembly.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractAssInstance.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKModelSet.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComponentManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSurface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSurfaceElement.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSurfaceNode.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComponentCreator.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCoordinateSystem.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGeoModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAssElement.h 
#    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTriPatchesProcess.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsAlgorithmTools.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsInterfaceFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKModelSurface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeometrySurface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKUnstructuredMeshTopo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoToMeshMapper.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPostFieldsData.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKModelEnum.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractModelComponent.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKNodeList.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractElement.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementList.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKUnstructuredMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKStructuredMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKParticalMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementVertex.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementLine.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementTri.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementQuad.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementTet.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementWedge.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementHex.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKElementPolygon.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAssembly.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractAssInstance.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKModelSet.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComponentManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSurface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSurfaceElement.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSurfaceNode.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComponentCreator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCoordinateSystem.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGeoModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAssElement.cpp 
#    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTriPatchesProcess.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsAlgorithmTools.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbsInterfaceFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKModelSurface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeometrySurface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKUnstructuredMeshTopo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGeoToMeshMapper.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPostFieldsData.cpp 
)

set(UI_FILES_LIST
#    ${CMAKE_CURRENT_SOURCE_DIR}/PluginManageDialog.ui 
)

set(RESOURCES_SOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/../qianfan.qrc
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

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKInterfaceModel_API
)

# 版本信息
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 0.9.1
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
