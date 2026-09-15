include_guard(GLOBAL)

function(appmesh_require_file path description)
    if(NOT EXISTS "${path}")
        message(FATAL_ERROR "Missing ${description}: ${path}")
    endif()
endfunction()

function(appmesh_configure_dependencies)
    file(TO_CMAKE_PATH "${TOOLS_DIR}" _tools_dir)
    set(TOOLS_DIR "${_tools_dir}" CACHE PATH "FastCAE dependency bundle root" FORCE)

    appmesh_require_file("${TOOLS_DIR}/cmake/OCCConfig.cmake" "FastCAE Tools marker")
    appmesh_require_file("${TOOLS_DIR}/Win64/hdf5/include/hdf5.h" "HDF5 headers")
    appmesh_require_file("${TOOLS_DIR}/Win64/hdf5/lib/hdf5.lib" "Release HDF5 import library")
    appmesh_require_file("${TOOLS_DIR}/Win64/hdf5/libd/hdf5_D.lib" "Debug HDF5 import library")
    appmesh_require_file("${TOOLS_DIR}/Win64/hdf5/bin/hdf5.dll" "Release HDF5 runtime")
    appmesh_require_file("${TOOLS_DIR}/Win64/hdf5/bind/hdf5_D.dll" "Debug HDF5 runtime")

    find_package(Qt5 5.14.2 EXACT REQUIRED COMPONENTS Core Gui Widgets)

    if(NOT TARGET APPMesh::HDF5)
        add_library(APPMesh::HDF5 SHARED IMPORTED GLOBAL)
        set_target_properties(APPMesh::HDF5 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${TOOLS_DIR}/Win64/hdf5/include"
            INTERFACE_COMPILE_DEFINITIONS H5_BUILT_AS_DYNAMIC_LIB
            IMPORTED_CONFIGURATIONS "Debug;Release"
            IMPORTED_IMPLIB_DEBUG "${TOOLS_DIR}/Win64/hdf5/libd/hdf5_D.lib"
            IMPORTED_LOCATION_DEBUG "${TOOLS_DIR}/Win64/hdf5/bind/hdf5_D.dll"
            IMPORTED_IMPLIB_RELEASE "${TOOLS_DIR}/Win64/hdf5/lib/hdf5.lib"
            IMPORTED_LOCATION_RELEASE "${TOOLS_DIR}/Win64/hdf5/bin/hdf5.dll"
            MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
            MAP_IMPORTED_CONFIG_MINSIZEREL Release)
    endif()

    message(STATUS "APPMesh FastCAE Tools: ${TOOLS_DIR}")
    message(STATUS "APPMesh Qt: ${Qt5Core_VERSION_STRING}")
    message(STATUS "APPMesh dependency policy: Debug=bind/libd, Release=bin/lib")
endfunction()
