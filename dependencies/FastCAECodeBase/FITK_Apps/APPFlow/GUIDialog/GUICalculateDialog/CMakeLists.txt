# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(GUICalculateDialog
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
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICalculateDialogAPI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SetupWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICalculateWidgetBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TurbulenceWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TransportWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DiscretizationWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SolutionWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundaryWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundaryCreateDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundaryDeleteDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/InitialWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CompSelectComBoxWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CompTranPhasesWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RunWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RunProcess.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PostWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/InitialCreatePatchDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatingConditionWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CalculateDriver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RadiationWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ThermoWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RegionMeshWidget.h
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/SetupWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUICalculateWidgetBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TurbulenceWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TransportWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DiscretizationWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SolutionWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundaryWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundaryCreateDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundaryDeleteDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InitialWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CompSelectComBoxWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CompTranPhasesWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RunWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RunProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PostWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InitialCreatePatchDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatingConditionWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CalculateDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RadiationWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ThermoWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegionMeshWidget.cpp
)

set(UI_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/SetupWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/TurbulenceWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/TransportWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/DiscretizationWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/SolutionWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundaryWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundaryCreateDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundaryDeleteDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/InitialWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CompSelectComBoxWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/CompTranPhasesWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/RunWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/PostWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/InitialCreatePatchDialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/OperatingConditionWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/RadiationWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ThermoWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/RegionMeshWidget.ui
)
set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Resources/Resources.qrc
)

source_group("From Files" FILES ${UI_FILES_LIST})
source_group("Resource Files" FILES ${RESOURCES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_FILES_LIST}
    ${RESOURCES}
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
            ${CMAKE_CURRENT_SOURCE_DIR}/../../Tools/Win64/SARibbon/include/SARibbon-2.0.1/
    )
else()
    target_include_directories(${PROJECT_NAME} 
        PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/../../Tools/Linux64/SARibbon/include/
    )
endif()

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
link_fitk_library(${PROJECT_NAME} FITKPython)
link_fitk_library(${PROJECT_NAME} GUIWidget)
link_fitk_library(${PROJECT_NAME} GUIFrame)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} FITKInterfaceFlowOF)
link_fitk_library(${PROJECT_NAME} FITKInterfaceMesh)
link_fitk_library(${PROJECT_NAME} FITKOFDictWriter)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)
link_fitk_library(${PROJECT_NAME} FITKOFDriver)
link_fitk_library(${PROJECT_NAME} FITKEasyParam)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        GUICalculateDialog_API
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
