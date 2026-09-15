# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKRadiossData
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossDataAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRadiossData.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossCase.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossCaseManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossSolution.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossMeshModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossGeomPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossNodesParts.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossNodeGroup.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKNonLinerCurve.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRadMaterial.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMaterialElastic.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMaterialPlasJohns.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMaterialPlasTab.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropAbstract.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropShell.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropBeamSPR.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractBCS.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossBCS.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractInitialField.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInitialField.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRadiossSurface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRWall.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractBEM.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKBEMDoubleAsymptoticApproximation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractInteraction.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRWall.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGravity.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInteractionTied.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInteractionMultiUsageImpact.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossSurfacePart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossSurfaceSeg.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossElementGroup.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossSolverSettings.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossPostData.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossResultRequest.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractProbe.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKProbeAcceleration.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractTimeHistory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistoryACCEL.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistoryINTER.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistoryRBODY.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistoryRWALL.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistorySECTIO.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractConnection.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKConnectionRBODY.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropBeam.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMaterialHydro.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMaterialPlasBrit.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropSandwichShell.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropSPH.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractEquationOfState.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKEquationOfStatePolynomial.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKProbeSection.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistorySPHCEL.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractFailureModel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFailureModelBIQUAD.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFailureModelJOHNSON.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRadiossData.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossCase.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossCaseManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossSolution.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossMeshModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossGeomPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossNodesParts.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossNodeGroup.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKNonLinerCurve.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRadMaterial.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMaterialElastic.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMaterialPlasJohns.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMaterialPlasTab.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropShell.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropBeamSPR.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractBCS.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossBCS.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractInitialField.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInitialField.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRadiossSurface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractRWall.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractBEM.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKBEMDoubleAsymptoticApproximation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractInteraction.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRWall.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGravity.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInteractionTied.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKInteractionMultiUsageImpact.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossSurfacePart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossSurfaceSeg.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossElementGroup.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossSolverSettings.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossPostData.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossResultRequest.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractProbe.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKProbeAcceleration.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractTimeHistory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistoryACCEL.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistoryINTER.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistoryRBODY.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistoryRWALL.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistorySECTIO.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractConnection.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKConnectionRBODY.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropBeam.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMaterialHydro.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMaterialPlasBrit.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropSandwichShell.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPropSPH.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractEquationOfState.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKEquationOfStatePolynomial.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKProbeSection.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTimeHistorySPHCEL.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractFailureModel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFailureModelBIQUAD.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFailureModelJOHNSON.cpp 
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

# 链接
target_link_libraries(${PROJECT_NAME}
    PUBLIC 
        Qt5::Core 
        Qt5::Widgets 
        Qt5::Gui
)

link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfacePhysics)
link_fitk_library(${PROJECT_NAME} FITKInterfaceStructuralPost)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKRadiossData_API
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
