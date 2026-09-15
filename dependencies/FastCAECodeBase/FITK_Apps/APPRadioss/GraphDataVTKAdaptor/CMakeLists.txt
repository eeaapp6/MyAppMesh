# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(GraphDataVTKAdaptor
    VERSION 1.0.0
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

set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphDataVTKAdaptorAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObject3D.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKCommons.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphDataVTKPartSetManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectPartMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorPartMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectPick.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectPickPreview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphDataSetCompomentManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorSet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectSet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorSurf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectSurf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorRigidWall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectRigidWall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorGravity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectGravity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorField.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectFieldBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectFieldTRA.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectFieldAXIS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorPost.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObject3DPostBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectPostModelBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectPostModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectHighlighting.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectModelBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorBC.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectBCBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectBCS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorInteraction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectInteraction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorRBODY.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectRBODY.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorProbe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectProbeAcceleration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectProbeSection.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObject3D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKCommons.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphDataVTKPartSetManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectPartMesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorPartMesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectPick.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectPickPreview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphDataSetCompomentManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorSet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectSet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorSurf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectSurf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorRigidWall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectRigidWall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorGravity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectGravity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectFieldBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectFieldTRA.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectFieldAXIS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorPost.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObject3DPostBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectPostModelBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectPostModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectHighlighting.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectModelBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorBC.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectBCBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectBCS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorInteraction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectInteraction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorRBODY.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectRBODY.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKViewAdaptorProbe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectProbeAcceleration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphVTKObjectProbeSection.cpp
)

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

# 包含 VTK 配置
if(NOT TARGET VTK::VTK)
    include(${TOOLS_DIR}/cmake/VTKConfig.cmake)
endif()

# 链接
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
        VTK::VTK
)

link_fitk_library(${PROJECT_NAME} FITKRenderWindowVTK)
link_fitk_library(${PROJECT_NAME} FITKVTKAlgorithm)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAdaptor)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMesh)
link_fitk_library(${PROJECT_NAME} FITKInterfaceStructuralPost)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        GraphDataVTKAdaptor_API
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
