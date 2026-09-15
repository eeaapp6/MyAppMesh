# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKInterfaceFlowOF
    VERSION 0.5.0
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInterfaceFlowOFAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFGeometryData.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFEnum.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFPhysicsData.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolverManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFBoundary.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbulenceManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractOFSolver.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolverSIMPLE.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractOFTurbulence.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFDiscretization.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTransportProp.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKEpsilon.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFPhysicsManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKKLOmega.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmega.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmegaSST.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmegaSSTSAS.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbRealizableKEpsilon.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbRNGKEpsilon.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbSpalartAllmaras.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmegaSSTDDES.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmegaSSTIDDES.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmegaSSTDES.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbSpalartAllmarasDES.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbSpalartAllmarasDDES.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbSpalartAllmarasIDDES.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbSmagorinsky.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbWALE.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowPhysicsHandlerFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolutionSolver.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolution.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolutionSolverManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFInitialConditions.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTransportModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTransportModelManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFBoundaryType.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFBoundaryTypeManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolverInter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRunControl.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFDynamicMesh.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFOperatingConditions.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolverCHTMultiRegion.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiation.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFThermo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationParaManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationAbsorptionModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationScatterModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationSunDirection.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationSunLoad.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFThermoPhysicalEquationOfState.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFThermoPhysicalThermodynamics.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFThermoPhysicalTransport.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFThermoPropManager.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFGeometryData.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFPhysicsData.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolverManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFBoundary.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbulenceManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractOFSolver.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolverSIMPLE.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractOFTurbulence.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFDiscretization.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTransportProp.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKEpsilon.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFPhysicsManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKKLOmega.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmega.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmegaSST.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmegaSSTSAS.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbRealizableKEpsilon.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbRNGKEpsilon.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbSpalartAllmaras.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmegaSSTDDES.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmegaSSTIDDES.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbKOmegaSSTDES.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbSpalartAllmarasDES.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbSpalartAllmarasDDES.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbSpalartAllmarasIDDES.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbSmagorinsky.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTurbWALE.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFlowPhysicsHandlerFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolutionSolver.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolution.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolutionSolverManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFInitialConditions.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTransportModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFTransportModelManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFBoundaryType.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFBoundaryTypeManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolverInter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRunControl.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFDynamicMesh.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFOperatingConditions.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFSolverCHTMultiRegion.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiation.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFThermo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationParaManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationAbsorptionModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationScatterModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationSunDirection.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFRadiationSunLoad.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFThermoPhysicalEquationOfState.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFThermoPhysicalThermodynamics.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFThermoPhysicalTransport.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKOFThermoPropManager.cpp 
)

set(RESOURCES_SOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/../qrc/qianfan.rc
)

source_group("Resource Files" FILES ${RESOURCES_SOURCES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
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
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMesh)
link_fitk_library(${PROJECT_NAME} FITKEasyParam)


# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        FITKInterfaceFlowOF_API    
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