# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(GUIAnalysisDialog
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
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIAnalysisDialogAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialPropDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialJohnsonCookDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialPlasBritDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialHydroDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialTypeDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyBeamSPRDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyBeamSPR_DOF_Dialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertySandwitchLayerDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertySPHDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertySolidDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyShellDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyBeamDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertySandwichDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyTypeDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICurveDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialElasticDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialPlasTabDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRigidWallTypeDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRigidWallDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRigidWallPlaneDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGravityDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldTypeDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldTRADialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/NodeGroupWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIAccelDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUISectionDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICreateSolutionDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRigidWallCylinderDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIContactTieDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIEOSDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIEOSPolynomialDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIEOSTypeDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIBCSDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIContactMultiUsageImpactDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRBodyDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldAXISDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIFailureModelDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIFailureJOHNSON_Dialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIFailureBIQUAD_Dialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIFailureModelTypeDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIContactCreateDialog.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialPropDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialJohnsonCookDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialPlasBritDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialHydroDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialTypeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyBeamSPRDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyBeamSPR_DOF_Dialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertySandwitchLayerDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertySPHDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertySolidDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyShellDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyBeamDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertySandwichDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyTypeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICurveDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialElasticDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialPlasTabDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRigidWallTypeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRigidWallDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRigidWallPlaneDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGravityDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldTypeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldTRADialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NodeGroupWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIAccelDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUISectionDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICreateSolutionDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRigidWallCylinderDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIContactTieDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIEOSDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIEOSPolynomialDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIEOSTypeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIBCSDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIContactMultiUsageImpactDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRBodyDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldAXISDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIFailureModelDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIFailureJOHNSON_Dialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIFailureBIQUAD_Dialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIFailureModelTypeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIContactCreateDialog.cpp
)

set(UI_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialPropDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIMaterialTypeDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertySandwitchLayerDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIPropertyTypeDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICurveDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRigidWallTypeDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRigidWallDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGravityDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldTypeDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIAccelDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUISectionDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICreateSolutionDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/NodeGroupWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIContactTieDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIEOSDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIEOSTypeDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIBCSDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIContactMultiUsageImpactDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRBodyDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIInitialFieldAXISDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIFailureModelDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIFailureModelTypeDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIContactCreateDialog.ui
)

source_group("From Files" FILES ${UI_FILES_LIST})
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

link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)
link_fitk_library(${PROJECT_NAME} FITKInterfacePhysics)
link_fitk_library(${PROJECT_NAME} FITKRadiossData)
link_fitk_library(${PROJECT_NAME} FITKPlotWindow)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} PickDataProvider)
link_fitk_library(${PROJECT_NAME} GraphDataVTKAdaptor)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        GUIAnalysisDialog_API
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
