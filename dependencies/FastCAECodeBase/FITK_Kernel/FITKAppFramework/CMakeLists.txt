# 指定CMake版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKAppFramework
    VERSION 1.0.7
    LANGUAGES CXX
)

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt5组件

find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

# 设置为共享库
add_library(${PROJECT_NAME} SHARED)

# 手动添加文件列表
set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAppFrameworkAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAppFramework.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMainWinGenerator.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGlobalData.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGlobalDataFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComponents.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComponentFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComponentInterface.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSignalTransfer.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMessage.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGlobalEventFilter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKKeyMouseStates.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCommandLineHandler.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAppSettings.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRunTimeSetting.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractEventFilter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCopyrightDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractProgramDriver.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKProgramDriverFactory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKProgramTaskManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractPythonRegister.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractCommandRunner.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLinuxShellDriver.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKExecProgramDriver.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWorkBenchHandler.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWorkbenchIniWriter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractAutoSaver.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractSysChecker.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractAppInitializer.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAppScriptHelper.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPluginManager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractPlugin.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKExceptionFilter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAppHistoryFiles.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractLicenseChecker.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAppFramework.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractMainWinGenerator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGlobalData.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractGlobalDataFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComponents.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComponentFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComponentInterface.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSignalTransfer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMessage.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKGlobalEventFilter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKKeyMouseStates.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCommandLineHandler.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAppSettings.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRunTimeSetting.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractEventFilter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCopyrightDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractProgramDriver.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKProgramDriverFactory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKProgramTaskManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractPythonRegister.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractCommandRunner.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLinuxShellDriver.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKExecProgramDriver.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWorkBenchHandler.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWorkbenchIniWriter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractAutoSaver.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractSysChecker.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractAppInitializer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAppScriptHelper.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPluginManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractPlugin.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKExceptionFilter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAppHistoryFiles.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKAbstractLicenseChecker.cpp 
)

set(UI_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKCopyrightDialog.ui 
)

set(RESOURCES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/License.qrc
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
        Qt5::Network
)

link_fitk_library(${PROJECT_NAME} FITKCore)

# Windows 平台额外链接 DbgHelp
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE DbgHelp)
endif()

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        FITKAppFramework_API   
)



# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.7
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
