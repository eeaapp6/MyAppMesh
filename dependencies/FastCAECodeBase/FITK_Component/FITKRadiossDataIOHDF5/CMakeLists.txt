# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKRadiossDataIOHDF5
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataIOHDF5API.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataHDF5Adaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataHDF5Interface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataHDF5Reader.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataHDF5Writer.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataCaseAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPartAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataNodeAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataSetAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataSurfaceAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataSubSetAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialElasticAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialPlasJohnsAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialPlasTabAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialVoidAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropBeamSPRAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropShellAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropSolidAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataInteractionTiedAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataNonLinerCurveAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataInitialFieldTraAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataGravityAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataRWallPlaneAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataRWallCylinderAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataSolverSettingsAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataResultRequestAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialPlasBritAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialHydroAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropSPHAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropSandwichShellAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossEquationOfStatePolynomialAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataInteractionMultiUsageImpactAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataSolutionAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialFailBiquadAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropBeamAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataBCSAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataConnectionRBODYAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataProbeAccelerationAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataProbeSectionAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistoryACCELAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistoryINTERAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistoryRBODYAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistoryRWALLAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistorySECTIOAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistorySPHCELAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataFailureModelBIQUADAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataFailureModelJOHNSONAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataGeomPartAdaptor.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataHDF5Adaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataHDF5Interface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataHDF5Reader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataHDF5Writer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataCaseAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPartAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataNodeAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataSetAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataSurfaceAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataSubSetAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialElasticAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialPlasJohnsAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialPlasTabAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialVoidAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropBeamSPRAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropShellAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropSolidAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataInteractionTiedAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataNonLinerCurveAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataInitialFieldTraAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataGravityAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataRWallPlaneAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataRWallCylinderAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataSolverSettingsAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataResultRequestAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialPlasBritAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialHydroAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropSPHAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropSandwichShellAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossEquationOfStatePolynomialAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataInteractionMultiUsageImpactAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataSolutionAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataMaterialFailBiquadAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataPropBeamAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataBCSAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataConnectionRBODYAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataProbeAccelerationAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataProbeSectionAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistoryACCELAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistoryINTERAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistoryRBODYAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistoryRWALLAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistorySECTIOAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataTimeHistorySPHCELAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataFailureModelBIQUADAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataFailureModelJOHNSONAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataGeomPartAdaptor.cpp 
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
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

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

# HDF5 
if(NOT TARGET HDF5::HDF5) 
    include(${TOOLS_DIR}/cmake/HDF5Config.cmake) 
endif() 

# 链接 
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui 
        HDF5::HDF5 
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKAdaptor)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} FITKInterfaceIO)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfacePhysics)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKRadiossDataIOHDF5_API
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
