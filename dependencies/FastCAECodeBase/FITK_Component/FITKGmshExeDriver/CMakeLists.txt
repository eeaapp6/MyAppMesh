# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKGmshExeDriver
    VERSION 2.0.0
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
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGmshExeDriverAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGmshExecProgramDriver.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGmshGeoScriptWrite.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGmshExecProgramInputInfo.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGmshGeoScriptManuallyWrite.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshAlgorithmGeneratorGmshExec.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMesherDriverGmshExec.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshGenerateProcessorGmshExec.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshGenGmshExecInterface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSizeGeneratorGmshExec.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGmshSettings.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshSizeBoxDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshSizeCylinderDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshSizeSphereDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshInfoDialog.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGmshExecProgramDriver.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGmshGeoScriptWrite.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGmshExecProgramInputInfo.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGmshGeoScriptManuallyWrite.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshAlgorithmGeneratorGmshExec.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMesherDriverGmshExec.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshGenerateProcessorGmshExec.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshGenGmshExecInterface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMeshSizeGeneratorGmshExec.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGmshSettings.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshSizeBoxDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshSizeCylinderDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshSizeSphereDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshInfoDialog.cpp 
)

set(UI_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIGmshSettings.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshSizeBoxDialog.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshSizeCylinderDialog.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshSizeSphereDialog.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRegionMeshInfoDialog.ui 
)

set(RESOURCES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gmsh.qrc
)

source_group("Ui Files" FILES ${UI_FILES_LIST})
source_group("Resource Files" FILES ${RESOURCES_SOURCES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*|.*autouic_.*\\.stamp$")

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
link_fitk_library(${PROJECT_NAME} FITKInterfaceMeshGen)
link_fitk_library(${PROJECT_NAME} FITKInterfaceModel)
link_fitk_library(${PROJECT_NAME} FITKInterfaceIO)
link_fitk_library(${PROJECT_NAME} FITKGmshMshIO)
link_fitk_library(${PROJECT_NAME} FITKWidget)
link_fitk_library(${PROJECT_NAME} FITKInterfaceGeometry)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKGmshExeDriver_API
)

# 版本信息
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 2.0.0
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

