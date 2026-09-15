#qwtConfig.cmake
include_guard(GLOBAL)

#定义qwt_ROOT,在多配置分支中需要这个变量

if(WIN32)
    set(qwt_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../Tools/Win64/qwt")
    set(qwt_INCLUDE_DIR "${qwt_ROOT}/include")
    
    #Windows qwt 库基础名称列表
    set(qwt_LIB_BASE_NAMES
        qwt
    )
else()
    #Linux配置
    set(qwt_ROOT "${CMAEK_CURRENT_SOUREC_DIR}/Tools/Linux64/qwt")
    set(qwt_INCLUDE_DIR "${qwt_ROOT}/include")
    set(qwt_LIB_DIR "${qwt_ROOT}/lib")
    
    #Linux qwt 库列表
    set(qwt_LIBRARIES
        qwt
    )
endif()

# 处理单配置和多配置的差异
if(CMAKE_CONFIGURATION_TYPES)
    # 多配置生成器
    
    # 检查 qwt 目录结构
    if(WIN32)
        if(NOT EXISTS "${qwt_ROOT}/libd" OR NOT EXISTS "${qwt_ROOT}/lib")
            message(WARNING "qwt: Could not find both libd and lib directories")
        endif()
    endif()
    
    # 创建两个不同的目标
    add_library(qwt::qwt_Debug INTERFACE IMPORTED)
    add_library(qwt::qwt_Release INTERFACE IMPORTED)
    
    # Debug 目标
    target_include_directories(qwt::qwt_Debug INTERFACE "${qwt_INCLUDE_DIR}")
    
    # Release 目标
    target_include_directories(qwt::qwt_Release INTERFACE "${qwt_INCLUDE_DIR}")
    
    if(WIN32)
        # Windows
        target_link_directories(qwt::qwt_Debug INTERFACE "${qwt_ROOT}/libd")
        target_link_directories(qwt::qwt_Release INTERFACE "${qwt_ROOT}/lib")
        
        # 为 Debug 库添加 "d" 后缀
        set(DEBUG_LIBS)
        foreach(lib ${qwt_LIB_BASE_NAMES})
            list(APPEND DEBUG_LIBS "${lib}d")
        endforeach()
        target_link_libraries(qwt::qwt_Debug INTERFACE ${DEBUG_LIBS})
        
        # Release 库无后缀
        target_link_libraries(qwt::qwt_Release INTERFACE ${qwt_LIB_BASE_NAMES})
    else()
        # Linux
        target_link_directories(qwt::qwt_Debug INTERFACE "${qwt_ROOT}/lib")
        target_link_directories(qwt::qwt_Release INTERFACE "${qwt_ROOT}/lib")
        target_link_libraries(qwt::qwt_Debug INTERFACE ${qwt_LIBRARIES})
        target_link_libraries(qwt::qwt_Release INTERFACE ${qwt_LIBRARIES})
    endif()
    
    # 创建统一的 qwt::qwt 目标，使用生成器表达式
    add_library(qwt::qwt INTERFACE IMPORTED)
    target_link_libraries(qwt::qwt INTERFACE
        $<$<CONFIG:Debug>:qwt::qwt_Debug>
        $<$<CONFIG:Release>:qwt::qwt_Release>
        $<$<CONFIG:MinSizeRel>:qwt::qwt_Release>
        $<$<CONFIG:RelWithDebInfo>:qwt::qwt_Release>
    )
    
else()
    # 单配置生成器
    
    if(WIN32)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(qwt_LIB_DIR "${qwt_ROOT}/libd")
            set(qwt_LIB_SUFFIX "d")
        else()
            set(qwt_LIB_DIR "${qwt_ROOT}/lib")
            set(qwt_LIB_SUFFIX "")
        endif()
        
        # 生成带后缀的库列表
        set(qwt_LIBRARIES_WIN)
        foreach(lib_base ${qwt_LIB_BASE_NAMES})
            list(APPEND qwt_LIBRARIES_WIN "${lib_base}${qwt_LIB_SUFFIX}")
        endforeach()
        
        # 创建 qwt 目标
        add_library(qwt::qwt INTERFACE IMPORTED)
        target_include_directories(qwt::qwt INTERFACE ${qwt_INCLUDE_DIR})
        target_link_directories(qwt::qwt INTERFACE ${qwt_LIB_DIR})
        target_link_libraries(qwt::qwt INTERFACE ${qwt_LIBRARIES_WIN})
        
    else()
        # Linux：使用预定义的 qwt_LIBRARIES
        add_library(qwt::qwt INTERFACE IMPORTED)
        target_include_directories(qwt::qwt INTERFACE ${qwt_INCLUDE_DIR})
        target_link_directories(qwt::qwt INTERFACE ${qwt_LIB_DIR})
        target_link_libraries(qwt::qwt INTERFACE ${qwt_LIBRARIES})
    endif()
    
endif()

# 输出配置信息
message(STATUS "qwt: Configuration complete")
message(STATUS "qwt: Include directory: ${qwt_INCLUDE_DIR}")
if(WIN32)
    if(CMAKE_CONFIGURATION_TYPES)
        message(STATUS "qwt: Multi-configuration mode (Visual Studio)")
        message(STATUS "qwt: Debug libraries will use 'd' suffix")
        message(STATUS "qwt: Release libraries will not use suffix")
    else()
        message(STATUS "qwt: Single-configuration mode (Build type: ${CMAKE_BUILD_TYPE})")
    endif()
else()
    message(STATUS "qwt: Linux configuration")
endif()