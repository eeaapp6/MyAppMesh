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
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsNonLinerCurve.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorModelInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorImportGeometry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorImportRad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorWorkingDir.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorMaterialProp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGmshGenerator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGmshSettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperRegionMeshSize.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorExportRad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorImportLSDyna.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorExportLSDyna.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorProperty.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorPart.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSurface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorOpenRadiossSolve.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorOpenRadiossFormatConversion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorRigidWallProp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorGravityProp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorInitialFieldProp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSolverSettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperImportVTK.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperResultRequest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGeometryModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorCreateSolution.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorProbe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperPost3D.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorContactTie.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorColorBar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperOpenHDF5.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSaveHDF5.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperImportMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MeshImporterNastran.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperImportCSV.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorBCS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorEOS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphSetting.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorContactMultiUsageImpact.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorRBody.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorFailureModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSaveAs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsNewProject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorContact.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorFileFITKMeshImport.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorFileFITKMeshExport.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperExportINP.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsNonLinerCurve.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorModelInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorImportGeometry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorImportRad.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorWorkingDir.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorMaterialProp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGmshGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGmshSettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperRegionMeshSize.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorExportRad.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorImportLSDyna.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorExportLSDyna.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorProperty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorPart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSurface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorOpenRadiossSolve.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorOpenRadiossFormatConversion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorRigidWallProp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorGravityProp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorInitialFieldProp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSolverSettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperImportVTK.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperResultRequest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGeometryModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorCreateSolution.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorProbe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperPost3D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorContactTie.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorColorBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperOpenHDF5.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperSaveHDF5.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperImportMesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MeshImporterNastran.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperImportCSV.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorBCS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorEOS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperGraphSetting.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorContactMultiUsageImpact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorRBody.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorFailureModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsSaveAs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorsNewProject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorContact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorFileFITKMeshImport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatorFileFITKMeshExport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperExportINP.cpp
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

# 包含 SARibbon 配置
if(NOT TARGET SARibbon::SARibbon) 
    include(${TOOLS_DIR}/cmake/SARibbonConfig.cmake)
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

# 包含 VTK 配置
if(NOT TARGET VTK::VTK)
    include(${TOOLS_DIR}/cmake/VTKConfig.cmake)
endif()

# 链接库
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
        VTK::VTK
		SARibbon::SARibbon
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKRadiossRadIO)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} GUIMeshDialog)
link_fitk_library(${PROJECT_NAME} PickDataProvider)
link_fitk_library(${PROJECT_NAME} OperatorsInterface)
link_fitk_library(${PROJECT_NAME} GUIAnalysisDialog)
link_fitk_library(${PROJECT_NAME} FITKGmshExeDriver)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMesh)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMeshGen)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} FITKRadiossLSDynaTransfer)
link_fitk_library(${PROJECT_NAME} FITKInterfacePhysics)
link_fitk_library(${PROJECT_NAME} GUIToolDialog)
link_fitk_library(${PROJECT_NAME} GUIPostDialog)
link_fitk_library(${PROJECT_NAME} FITKInterfaceStructuralPost)
link_fitk_library(${PROJECT_NAME} GUIGeometryDialog)
link_fitk_library(${PROJECT_NAME} FITKRadiossDataIOHDF5)
link_fitk_library(${PROJECT_NAME} FITKInterfaceIO)
link_fitk_library(${PROJECT_NAME} FITKGeoOCCIOHDF5)
link_fitk_library(${PROJECT_NAME} FITKAbaqusData)
link_fitk_library(${PROJECT_NAME} FITKAbaqusIOINP)
link_fitk_library(${PROJECT_NAME} FITKNastranBDFIO)
link_fitk_library(${PROJECT_NAME} FITKAbaqusRadiossDataMapper)
link_fitk_library(${PROJECT_NAME} FITKMeshIO)

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

