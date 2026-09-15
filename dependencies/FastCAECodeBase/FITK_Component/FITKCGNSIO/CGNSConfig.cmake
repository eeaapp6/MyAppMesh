if(WIN32)
    set(CGNS_ROOT "${TOOLS_DIR}/Win64/cgns")
    set(CGNS_INCLUDE_DIR ${CGNS_ROOT}/include)
    
    # ========== Release 库列表（无 d 后缀）==========
    set(CGNS_LIBS_RELEASE
        cgns
        cgnsdll
        libhdf5
        libhdf5_cpp
        libhdf5_hl
        libhdf5_hl_cpp
        libhdf5_tools

    )
    
    # ========== Debug 库列表（有 d 后缀，meshgems 除外）==========
    set(CGNS_LIBS_DEBUG
        cgns
        cgnsdll
        libhdf5_D
        libhdf5_cpp_D
        libhdf5_hl_D
        libhdf5_hl_cpp_D
        libhdf5_tools_D
    )
    
else()
    # Linux 配置
endif()

# ========== 多配置生成器（Visual Studio）==========
if(CMAKE_CONFIGURATION_TYPES)
    if(WIN32)
        # 创建 Debug 目标
        add_library(CGNS::CGNS_Debug INTERFACE IMPORTED)
        target_include_directories(CGNS::CGNS_Debug INTERFACE "${CGNS_INCLUDE_DIR}")
        target_link_directories(CGNS::CGNS_Debug INTERFACE "${CGNS_ROOT}/libd")
        target_link_libraries(CGNS::CGNS_Debug INTERFACE ${CGNS_LIBS_DEBUG})
        
        # 创建 Release 目标
        add_library(CGNS::CGNS_Release INTERFACE IMPORTED)
        target_include_directories(CGNS::CGNS_Release INTERFACE "${CGNS_INCLUDE_DIR}")
        target_link_directories(CGNS::CGNS_Release INTERFACE "${CGNS_ROOT}/lib")
        target_link_libraries(CGNS::CGNS_Release INTERFACE ${CGNS_LIBS_RELEASE})
    endif()
    
    # 统一接口
    add_library(CGNS::CGNS INTERFACE IMPORTED)
    target_link_libraries(CGNS::CGNS INTERFACE
        $<$<CONFIG:Debug>:CGNS::CGNS_Debug>
        $<$<CONFIG:Release>:CGNS::CGNS_Release>
        $<$<CONFIG:MinSizeRel>:CGNS::CGNS_Release>
        $<$<CONFIG:RelWithDebInfo>:CGNS::CGNS_Release>
    )
    
# ========== 单配置生成器（Ninja/Make）==========
else()
    if(WIN32)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CGNS_LIB_DIR "${CGNS_ROOT}/libd")
            set(CGNS_LIBS ${CGNS_LIBS_DEBUG})
        else()
            set(CGNS_LIB_DIR "${CGNS_ROOT}/lib")
            set(CGNS_LIBS ${CGNS_LIBS_RELEASE})
        endif()
        
        add_library(CGNS::CGNS INTERFACE IMPORTED)
        target_include_directories(CGNS::CGNS INTERFACE ${CGNS_INCLUDE_DIR})
        target_link_directories(CGNS::CGNS INTERFACE ${CGNS_LIB_DIR})
        target_link_libraries(CGNS::CGNS INTERFACE ${CGNS_LIBS})
    endif()
endif()

message(STATUS "CGNS: Configuration complete")
message(STATUS "CGNS: Include: ${CGNS_INCLUDE_DIR}")