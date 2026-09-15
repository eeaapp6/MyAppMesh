# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKInterfaceFlowPHengLEI
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInterfaceFlowPHengLEIAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIAbsParaWriter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIParaWriterKey.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIParaWriterGrid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIParaWriterPartion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIAbsSolver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverSubsonic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverTransonic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverSupersonic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverHypersonic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIGeneralControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIPhysicsHandlerFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIDataManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIMeshPara.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIViscousModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIMultiGrid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIAbstractInFlowType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypeNondimensional.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypeFlight.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypeExperiment.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypePressure.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypeDensity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypeManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIAbsSpatialDiscretisation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISpatialStruct.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISpatialUnStruct.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISpatialManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIDiscretisation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundaryInFlowType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundaryFarfieldType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundarySolidSurfaceType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundaryType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundaryTypeManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIOutputSetting.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIerDriver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIDriver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInputInfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDHypersonsic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDIncompressible.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDSubsonic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDSupersonic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDTransonic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIMeshProcess.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIFlow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteBoundary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIGeneral.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIViscousType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIViscousTypeManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEICalculation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIResidaulProcess.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKResidualFileReaderWorker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKResidualDataManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKResidualFileReaderManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIExecProgramDriver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverSIMPLE.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverCompressibleSIMPLE.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverPISO.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInitialCondition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIModels.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolution.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIEnergyModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISpeciesModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEICouplingControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEILinuxShellDriver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEILinuxDriver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDCompressibleSIMPLE.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDSIMPLE.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDPISO.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIAbsParaWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIParaWriterKey.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIParaWriterGrid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIParaWriterPartion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIAbsSolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverSubsonic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverTransonic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverSupersonic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverHypersonic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIGeneralControl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIPhysicsHandlerFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIDataManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIMeshPara.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIViscousModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIMultiGrid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIAbstractInFlowType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypeNondimensional.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypeFlight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypeExperiment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypePressure.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypeDensity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInFlowTypeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIAbsSpatialDiscretisation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISpatialStruct.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISpatialUnStruct.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISpatialManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIDiscretisation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundaryInFlowType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundaryFarfieldType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundarySolidSurfaceType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundaryType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIBoundaryTypeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIOutputSetting.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIerDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInputInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDHypersonsic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDSubsonic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDSupersonic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDTransonic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIMeshProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIFlow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteBoundary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIGeneral.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIViscousType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIViscousTypeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEICalculation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIResidaulProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKResidualFileReaderWorker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKResidualDataManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKResidualFileReaderManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIExecProgramDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverSIMPLE.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverCompressibleSIMPLE.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolverPISO.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIInitialCondition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIModels.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISolution.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIEnergyModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEISpeciesModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEICouplingControl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEILinuxShellDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEILinuxDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDCompressibleSIMPLE.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDSIMPLE.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPHengLEIWriteCFDPISO.cpp
)

set(RESOURCES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qrc/PHengLEI.qrc
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/License.txt
)

source_group("Resource Files" FILES ${RESOURCES_SOURCES})
source_group("Translation Files" FILES ${TS_FILES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${RESOURCES_SOURCES}
    ${TS_FILES}
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
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKEasyParam)
link_fitk_library(${PROJECT_NAME} FITKInterfaceIO)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMesh)


# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        FITKInterfaceFlowPHengLEI_API    
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