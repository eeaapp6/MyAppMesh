# HDF5Config.cmake – HDF5 库的 CMake 导入配置文件
include_guard(GLOBAL)
# 检查 TOOLS_DIR 是否定义
if(NOT TOOLS_DIR)
    message(FATAL_ERROR "HDF5Config.cmake: TOOLS_DIR is not defined. Please set it in top-level CMakeLists.txt")
endif()

# 根据平台设置 HDF5 根目录
if(WIN32)
    set(HDF5_BASE_DIR "${TOOLS_DIR}/Win64/hdf5")
elseif(UNIX)
    set(HDF5_BASE_DIR "${TOOLS_DIR}/Linux64/hdf5")
else()
    message(FATAL_ERROR "HDF5Config.cmake: Unsupported platform")
endif()

# 头文件目录
set(HDF5_INCLUDE_DIR "${HDF5_BASE_DIR}/include")

# 定义所有 HDF5 组件库名称
set(HDF5_COMPONENTS
    hdf5
    hdf5_cpp
    hdf5_hl
    hdf5_hl_cpp
    hdf5_tools
)

# 为每个组件创建导入目标
foreach(comp IN LISTS HDF5_COMPONENTS)
    if(NOT TARGET HDF5::${comp})
        add_library(HDF5::${comp} UNKNOWN IMPORTED)

        # 设置公共包含目录（所有组件共用）
        set_target_properties(HDF5::${comp} PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${HDF5_INCLUDE_DIR}"
        )

        # 根据平台和配置设置库文件位置
        if(WIN32)
            # Windows 区分 Debug 和 Release
            set_target_properties(HDF5::${comp} PROPERTIES
                IMPORTED_LOCATION_DEBUG   "${HDF5_BASE_DIR}/libd/${comp}_D.lib"
                IMPORTED_LOCATION_RELEASE "${HDF5_BASE_DIR}/lib/${comp}.lib"
            )
        elseif(UNIX)
            # Linux（假设库名如 libhdf5.so，无配置区分）
            set_target_properties(HDF5::${comp} PROPERTIES
                IMPORTED_LOCATION "${HDF5_BASE_DIR}/lib/lib${comp}.so"
            )
        endif()
    endif()
endforeach()

# 创建聚合接口目标，包含所有组件和编译定义
add_library(HDF5::HDF5 INTERFACE IMPORTED)
target_link_libraries(HDF5::HDF5 INTERFACE
    HDF5::hdf5
    HDF5::hdf5_cpp
    HDF5::hdf5_hl
    HDF5::hdf5_hl_cpp
    HDF5::hdf5_tools
)
target_compile_definitions(HDF5::HDF5 INTERFACE
    H5_BUILT_AS_DYNAMIC_LIB   
)