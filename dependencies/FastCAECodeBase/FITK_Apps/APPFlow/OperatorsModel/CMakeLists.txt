# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(OperatorsModel
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
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsModelAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OpersModelInterface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/OperManagerBase.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsGeoCubeManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsGeoCylinderManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsGeoSphereManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsMeshGeoManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsMeshBaseManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsMeshManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsMeshLocalManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsMeshPointManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsNew.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsOpen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSave.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsImportManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsGeoBool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsWorkingDir.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSetup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsTurbulence.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsTransport.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsDiscretization.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSolution.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsBoundaryConditions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsInitial.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsRun.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsPost.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsLicense.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsOperCondition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsCFDPost.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsRadiation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsThermo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsRegionMesh.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/OpersModelInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperManagerBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsGeoCubeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsGeoCylinderManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsGeoSphereManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsMeshGeoManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsMeshBaseManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsMeshManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsMeshLocalManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsMeshPointManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsNew.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsOpen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSave.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsImportManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsGeoBool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsWorkingDir.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSetup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsTurbulence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsTransport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsDiscretization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSolution.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsBoundaryConditions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsInitial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsRun.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsPost.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsLicense.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsOperCondition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsCFDPost.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsRadiation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsThermo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsRegionMesh.cpp
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

# SARibbon 包含路径
if(WIN32)
    target_include_directories(${PROJECT_NAME} 
        PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/../Tools/Win64/SARibbon/include/SARibbon-2.0.1/
    )
else()
    target_include_directories(${PROJECT_NAME} 
        PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/../Tools/Linux64/SARibbon/include/
    )
endif()

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

# 链接库
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
)

link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKOFDictWriter)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} OperatorsInterface)
link_fitk_library(${PROJECT_NAME} GUIGeometryDialog)
link_fitk_library(${PROJECT_NAME} GUIMeshDialog)
link_fitk_library(${PROJECT_NAME} GUICalculateDialog)
link_fitk_library(${PROJECT_NAME} FITKOFDriver)
link_fitk_library(${PROJECT_NAME} FITKInterfaceIO)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMesh)
link_fitk_library(${PROJECT_NAME} FITKOFMeshIO)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMeshGen)
link_fitk_library(${PROJECT_NAME} FITKGeoCompOCC)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} FITKInterfaceFlowOF)
link_fitk_library(${PROJECT_NAME} FITKFlowOFIOHDF5)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        OperatorsModel_API
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

