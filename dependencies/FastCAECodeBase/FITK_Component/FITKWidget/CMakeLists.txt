# 指定Cmake最低版本
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目信息
project(FITKWidget
    VERSION 1.0.1
    LANGUAGES CXX
)

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt5组件

find_package(Qt5 COMPONENTS Core Gui Widgets Designer REQUIRED)

# 定义为共享库
add_library(${PROJECT_NAME} SHARED)

# 手动添加文件列表
set(HEADERS_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPushButtonColor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadioColor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidgetAPI.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLockerButton.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLockerWidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKDrawerWidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMdiArea.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTableWidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSciNotationLineEdit.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComboBox.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWorkingDirDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTabWidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTreeWidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/TableWidgetFileReadFileDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMdiSubWindow.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTableView.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTreeTableWidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPluginManageDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLicenseFileCheckDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFileDirectoryDialog.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRAMProportionBar.h 
)

set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPushButtonColor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLockerButton.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLockerWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKDrawerWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMdiArea.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadioColor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTableWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKSciNotationLineEdit.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKComboBox.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWorkingDirDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTabWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTreeWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/TableWidgetFileReadFileDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKMdiSubWindow.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTableView.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKTreeTableWidget.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPluginManageDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLicenseFileCheckDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFileDirectoryDialog.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRAMProportionBar.cpp 
)

set(UI_FILES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKDrawerWidget.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRadioColor.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKWorkingDirDialog.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/TableWidgetFileReadFileDialog.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKPluginManageDialog.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKLicenseFileCheckDialog.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKFileDirectoryDialog.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/FITKRAMProportionBar.ui 
)

set(RESOURCES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/FITKWidgetResource.qrc 
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ZH_CN/FITKWidget_ZH_CN.ts 
)

source_group("From Files" FILES ${UI_FILES_LIST})
source_group("Resource Files" FILES ${RESOURCES_SOURCES})
source_group("Translation Files" FILES ${TS_FILES})
# 强制将所有以 mocs_compilation 或 qrc_ 开头的cpp文件，以及 ui_ 开头的头文件分到 Generated Files
source_group("Generated Files" REGULAR_EXPRESSION ".*mocs_compilation.*\\.cpp$|.*qrc_.*\\.cpp$|.*ui_.*\\.h$|.*_autogen/.*|.*autouic_.*\\.stamp$")

# 添加所有源文件（包括生成的 UI 头文件和资源源文件）
target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCES_LIST}
    ${HEADERS_LIST}
    ${UI_FILES_LIST}
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
	    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/FITK_Kernel/FITKCore> 
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
 #       Qt5::Designer
)

link_fitk_library(${PROJECT_NAME} FITKAppFramework)
link_fitk_library(${PROJECT_NAME} FITKCore)

# 预编译用于控制导出/导入宏
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        FITKWidget_API
)

# 版本信息
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.1
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
