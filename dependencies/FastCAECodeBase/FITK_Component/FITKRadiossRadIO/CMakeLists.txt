# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKRadiossRadIO
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossRadIOAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossRadIOInterface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossRadReader.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossRadWriter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAbstractAdaptor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/RadReadInformationMapper.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/RadWriteInformationMapper.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorCase.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/RadKeyLineSpliter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialElastic.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialPlasTab.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorNode.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPart.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPartElements.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropSolid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropSolidValue.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorNonLinerCurve.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialVoid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialPlasJohns.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropShell.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropShellValue.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorBCS.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorBCSCyclic.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorBCSLagMul.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorSubset.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorNodeGroup.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorRWallPlane.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorGravity.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInitialFieldTra.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorSurfaceSeg.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInteractionTied.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropBeamSPR.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInteractionTiedDefault.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorRWallCYL.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorRWallSpher.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorProbeAccel.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInteractionMUImpact.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorSurfaceOther.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInteractionMUImpactDefault.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorConnectionRBODY.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorElementGroup.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorTimerHistory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropBeam.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialPlasBrit.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialHydro.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropShellSandwich.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorGlobalSPH.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropSPH.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorEOSPolynomial.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorSolverRequest.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorSolverSetting.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorProbeSection.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInitialFieldAxis.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorFailBIQUAD.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorFailJOHNSON.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorBEMDAA.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossRadIOInterface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossRadReader.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossRadWriter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAbstractAdaptor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/RadReadInformationMapper.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/RadWriteInformationMapper.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorCase.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/RadKeyLineSpliter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialElastic.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialPlasTab.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorNode.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPart.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPartElements.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropSolid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropSolidValue.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorNonLinerCurve.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialVoid.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialPlasJohns.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropShell.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropShellValue.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorBCS.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorBCSCyclic.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorBCSLagMul.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorSubset.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorNodeGroup.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorRWallPlane.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorGravity.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInitialFieldTra.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorSurfaceSeg.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInteractionTied.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropBeamSPR.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInteractionTiedDefault.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorRWallCYL.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorRWallSpher.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorProbeAccel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInteractionMUImpact.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorSurfaceOther.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInteractionMUImpactDefault.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorConnectionRBODY.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorElementGroup.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorTimerHistory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropBeam.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialPlasBrit.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorMaterialHydro.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropShellSandwich.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorGlobalSPH.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorPropSPH.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorEOSPolynomial.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorSolverRequest.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorSolverSetting.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorProbeSection.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorInitialFieldAxis.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorFailBIQUAD.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorFailJOHNSON.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadiossAdaptorBEMDAA.cpp 
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
link_fitk_library(${PROJECT_NAME} FITKAdaptor)
link_fitk_library(${PROJECT_NAME} FITKInterfaceIO)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfacePhysics)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKRadiossRadIO_API
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
