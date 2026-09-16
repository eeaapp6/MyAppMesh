include_guard(GLOBAL)

function(appmesh_require_file path description)
    if(NOT EXISTS "${path}")
        message(FATAL_ERROR "Missing ${description}: ${path}")
    endif()
endfunction()

function(appmesh_configure_dependencies)
    get_property(_appmesh_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(_appmesh_multi_config)
        if(NOT CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
            message(FATAL_ERROR
                "APPMesh dependencies are Debug only; configurations=${CMAKE_CONFIGURATION_TYPES}")
        endif()
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR
            "APPMesh dependencies are Debug only; CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
    endif()

    file(TO_CMAKE_PATH "${TOOLS_DIR}" _tools_dir)
    set(TOOLS_DIR "${_tools_dir}" CACHE PATH "FastCAE dependency bundle root" FORCE)

    appmesh_require_file("${TOOLS_DIR}/cmake/OCCConfig.cmake" "FastCAE Tools marker")
    appmesh_require_file("${TOOLS_DIR}/Win64/hdf5/include/hdf5.h" "HDF5 headers")
    appmesh_require_file("${TOOLS_DIR}/Win64/hdf5/libd/hdf5_D.lib" "Debug HDF5 import library")
    appmesh_require_file("${TOOLS_DIR}/Win64/hdf5/bind/hdf5_D.dll" "Debug HDF5 runtime")
    appmesh_require_file("${TOOLS_DIR}/Win64/SARibbon/include/SARibbon-2.0.1/SARibbonMainWindow.h" "SARibbon public headers")
    appmesh_require_file("${TOOLS_DIR}/Win64/SARibbon/libd/SARibbonBard.lib" "Debug SARibbon import library")
    appmesh_require_file("${TOOLS_DIR}/Win64/SARibbon/bind/SARibbonBard.dll" "Debug SARibbon runtime")

    find_package(Qt5 5.14.2 EXACT REQUIRED COMPONENTS Core Gui Widgets Network)

    if(NOT TARGET APPMesh::HDF5)
        add_library(APPMesh::HDF5 SHARED IMPORTED GLOBAL)
        set_target_properties(APPMesh::HDF5 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${TOOLS_DIR}/Win64/hdf5/include"
            INTERFACE_COMPILE_DEFINITIONS H5_BUILT_AS_DYNAMIC_LIB
            IMPORTED_CONFIGURATIONS Debug
            IMPORTED_IMPLIB_DEBUG "${TOOLS_DIR}/Win64/hdf5/libd/hdf5_D.lib"
            IMPORTED_LOCATION_DEBUG "${TOOLS_DIR}/Win64/hdf5/bind/hdf5_D.dll")
    endif()

    if(NOT TARGET APPMesh::SARibbon)
        add_library(APPMesh::SARibbon SHARED IMPORTED GLOBAL)
        set_target_properties(APPMesh::SARibbon PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${TOOLS_DIR}/Win64/SARibbon/include/SARibbon-2.0.1"
            IMPORTED_CONFIGURATIONS Debug
            IMPORTED_IMPLIB_DEBUG "${TOOLS_DIR}/Win64/SARibbon/libd/SARibbonBard.lib"
            IMPORTED_LOCATION_DEBUG "${TOOLS_DIR}/Win64/SARibbon/bind/SARibbonBard.dll")
    endif()

    message(STATUS "APPMesh FastCAE Tools: ${TOOLS_DIR}")
    message(STATUS "APPMesh Qt: ${Qt5Core_VERSION_STRING}")
    message(STATUS "APPMesh dependency policy: Debug-only bind/libd (HDF5; SARibbonBard)")
endfunction()
