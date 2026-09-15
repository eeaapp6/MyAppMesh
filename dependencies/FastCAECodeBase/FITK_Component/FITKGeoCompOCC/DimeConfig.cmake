# DimeConfig.cmake – Dime 库的 CMake 导入配置文件
include_guard(GLOBAL)

# 检查 TOOLS_DIR 是否定义
if(NOT TOOLS_DIR)
    message(FATAL_ERROR "DimeConfig.cmake: TOOLS_DIR is not defined. Please set it in top-level CMakeLists.txt")
endif()

# 根据平台设置 Dime 根目录
if(WIN32)
    set(DIME_BASE_DIR "${TOOLS_DIR}/Win64/dime")
elseif(UNIX)
    set(DIME_BASE_DIR "${TOOLS_DIR}/Linux64/dime")
else()
    message(FATAL_ERROR "DimeConfig.cmake: Unsupported platform")
endif()

# 头文件目录
set(DIME_INCLUDE_DIR "${DIME_BASE_DIR}/include")

# 创建 Dime 库的导入目标
if(NOT TARGET DIME::dime)
    add_library(DIME::dime UNKNOWN IMPORTED)

    # 设置公共包含目录和编译定义（DIME_DLL 对应 Windows 动态库）
    set_target_properties(DIME::dime PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${DIME_INCLUDE_DIR}"
        INTERFACE_COMPILE_DEFINITIONS "DIME_DLL"
    )

    # 根据平台和配置设置库文件位置
    if(WIN32)
        # Windows 区分 Debug 和 Release
        set_target_properties(DIME::dime PROPERTIES
            IMPORTED_LOCATION_DEBUG   "${DIME_BASE_DIR}/libd/dime0d.lib"
            IMPORTED_LOCATION_RELEASE "${DIME_BASE_DIR}/lib/dime0.lib"
        )
    elseif(UNIX)
        # Linux 使用共享库
        set_target_properties(DIME::dime PROPERTIES
            IMPORTED_LOCATION "${DIME_BASE_DIR}/lib/libdime.so"
        )
    endif()
endif()

# 创建聚合接口目标 DIME::DIME（方便直接引用所有组件，虽然这里只有一个库）
add_library(DIME::DIME INTERFACE IMPORTED)
target_link_libraries(DIME::DIME INTERFACE DIME::dime)
# 为聚合接口也添加包含目录和定义（确保直接使用 DIME::DIME 也能获得这些属性）
target_include_directories(DIME::DIME INTERFACE "${DIME_INCLUDE_DIR}")
target_compile_definitions(DIME::DIME INTERFACE "DIME_DLL")