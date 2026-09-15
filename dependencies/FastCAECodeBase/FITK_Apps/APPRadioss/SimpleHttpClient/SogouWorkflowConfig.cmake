# SogouWorkflowConfig.cmake
# 用于在 CMake 项目中引入 SogouWorkflow（Windows）或 Qwt（Linux）的依赖配置
include_guard(GLOBAL)

if(WIN32)
    # Windows 平台：SogouWorkflow + OpenSSL 
    set(SOGOUWORKFLOW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../Tools/Win64/SogouWorkflow")
    set(OPENSSL_ROOT       "${CMAKE_CURRENT_SOURCE_DIR}/../Tools/Win64/OpenSSL-Win64")

    set(SOGOUWORKFLOW_INCLUDE_DIR "${SOGOUWORKFLOW_ROOT}/include")
    set(OPENSSL_INCLUDE_DIR       "${OPENSSL_ROOT}/include")

    # 合并所有包含目录
    set(SOGOUWORKFLOW_INCLUDE_DIRS
        ${SOGOUWORKFLOW_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIR}
    )

    # 库名称（基础名，不包含路径或后缀）
    set(SOGOUWORKFLOW_LIB_NAMES workflow)          # SogouWorkflow 主库
    set(OPENSSL_LIB_NAMES       libssl_static libcrypto_static)  # OpenSSL 静态库
    set(SYSTEM_LIBS             Ws2_32 Crypt32 Mswsock)          # Windows 系统库

    if(CMAKE_CONFIGURATION_TYPES)
        # 创建 Debug 和 Release 两个独立的接口目标
        add_library(SogouWorkflow::SogouWorkflow_Debug INTERFACE IMPORTED)
        add_library(SogouWorkflow::SogouWorkflow_Release INTERFACE IMPORTED)

        # 包含目录（Debug 和 Release 相同）
        target_include_directories(SogouWorkflow::SogouWorkflow_Debug INTERFACE ${SOGOUWORKFLOW_INCLUDE_DIRS})
        target_include_directories(SogouWorkflow::SogouWorkflow_Release INTERFACE ${SOGOUWORKFLOW_INCLUDE_DIRS})

        # Debug 链接目录与库
        target_link_directories(SogouWorkflow::SogouWorkflow_Debug INTERFACE
            "${SOGOUWORKFLOW_ROOT}/libd"
            "${OPENSSL_ROOT}/lib/VC/x64/MTd"
        )
        target_link_libraries(SogouWorkflow::SogouWorkflow_Debug INTERFACE
            ${SOGOUWORKFLOW_LIB_NAMES}
            ${OPENSSL_LIB_NAMES}
            ${SYSTEM_LIBS}
        )

        # Release 链接目录与库
        target_link_directories(SogouWorkflow::SogouWorkflow_Release INTERFACE
            "${SOGOUWORKFLOW_ROOT}/lib"
            "${OPENSSL_ROOT}/lib/VC/x64/MTd"
        )
        target_link_libraries(SogouWorkflow::SogouWorkflow_Release INTERFACE
            ${SOGOUWORKFLOW_LIB_NAMES}
            ${OPENSSL_LIB_NAMES}
            ${SYSTEM_LIBS}
        )

        # 统一的接口目标，根据当前配置选择 Debug 或 Release
        add_library(SogouWorkflow::SogouWorkflow INTERFACE IMPORTED)
        target_link_libraries(SogouWorkflow::SogouWorkflow INTERFACE
            $<$<CONFIG:Debug>:SogouWorkflow::SogouWorkflow_Debug>
            $<$<CONFIG:Release>:SogouWorkflow::SogouWorkflow_Release>
            $<$<CONFIG:MinSizeRel>:SogouWorkflow::SogouWorkflow_Release>
            $<$<CONFIG:RelWithDebInfo>:SogouWorkflow::SogouWorkflow_Release>
        )
    else()
        # 根据 CMAKE_BUILD_TYPE 选择合适的库目录
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(SOGOUWORKFLOW_LIB_DIR "${SOGOUWORKFLOW_ROOT}/libd")
        else()
            set(SOGOUWORKFLOW_LIB_DIR "${SOGOUWORKFLOW_ROOT}/lib")
        endif()
        # OpenSSL 目录为 MTd
        set(OPENSSL_LIB_DIR "${OPENSSL_ROOT}/lib/VC/x64/MTd")

        add_library(SogouWorkflow::SogouWorkflow INTERFACE IMPORTED)
        target_include_directories(SogouWorkflow::SogouWorkflow INTERFACE ${SOGOUWORKFLOW_INCLUDE_DIRS})
        target_link_directories(SogouWorkflow::SogouWorkflow INTERFACE
            ${SOGOUWORKFLOW_LIB_DIR}
            ${OPENSSL_LIB_DIR}
        )
        target_link_libraries(SogouWorkflow::SogouWorkflow INTERFACE
            ${SOGOUWORKFLOW_LIB_NAMES}
            ${OPENSSL_LIB_NAMES}
            ${SYSTEM_LIBS}
        )
    endif()

    # 输出配置信息
    message(STATUS "SogouWorkflow: Windows configuration loaded")
    message(STATUS "SogouWorkflow: Include directories: ${SOGOUWORKFLOW_INCLUDE_DIRS}")
    if(CMAKE_CONFIGURATION_TYPES)
        message(STATUS "SogouWorkflow: Multi-configuration mode (Debug/Release targets created)")
    else()
        message(STATUS "SogouWorkflow: Single-configuration mode (Build type: ${CMAKE_BUILD_TYPE})")
    endif()

else()
    # Linux/Unix 平台：Qwt
    set(QWT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../Tools/Linux64/qwt")
    set(QWT_INCLUDE_DIR "${QWT_ROOT}/include")
    set(QWT_LIB_DIR     "${QWT_ROOT}/lib")

    add_library(Qwt::Qwt INTERFACE IMPORTED)
    target_include_directories(Qwt::Qwt INTERFACE ${QWT_INCLUDE_DIR})
    target_link_directories(Qwt::Qwt INTERFACE ${QWT_LIB_DIR})
    target_link_libraries(Qwt::Qwt INTERFACE qwt)

    message(STATUS "Qwt: Linux configuration loaded")
    message(STATUS "Qwt: Include directory: ${QWT_INCLUDE_DIR}")
    message(STATUS "Qwt: Library directory: ${QWT_LIB_DIR}")
endif()