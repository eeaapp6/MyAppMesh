include_guard(GLOBAL)

function(appmesh_require_fitk_file path description)
    if(NOT EXISTS "${path}")
        message(FATAL_ERROR
            "Missing ${description}: ${path}\n"
            "Restore the matching MSVC v141 / Qt 5.14.2 x64 Debug FITK artifact "
            "under FITK_OUTPUT_DIR/bin_d; Release artifacts are not accepted.")
    endif()
endfunction()

function(appmesh_configure_fitk_dependencies)
    get_property(_appmesh_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(_appmesh_multi_config)
        if(NOT CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
            message(FATAL_ERROR
                "APPMesh FITK dependencies are Debug only; configurations=${CMAKE_CONFIGURATION_TYPES}")
        endif()
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR
            "APPMesh FITK dependencies are Debug only; CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
    endif()

    file(TO_CMAKE_PATH "${FITK_OUTPUT_DIR}" _fitk_output_dir)
    set(FITK_OUTPUT_DIR "${_fitk_output_dir}" CACHE PATH
        "Root containing the explicitly allowlisted FITK Debug binaries" FORCE)

    set(_fitk_source_root "${CMAKE_SOURCE_DIR}/dependencies/FastCAECodeBase")
    set(_fitk_debug_dir "${FITK_OUTPUT_DIR}/bin_d")

    foreach(_library FITKCore FITKAppFramework)
        appmesh_require_fitk_file(
            "${_fitk_debug_dir}/${_library}.lib"
            "${_library} Debug import library")
        appmesh_require_fitk_file(
            "${_fitk_debug_dir}/${_library}.dll"
            "${_library} Debug runtime")
    endforeach()

    appmesh_require_fitk_file(
        "${_fitk_source_root}/FITK_Kernel/FITKCore/FITKCoreAPI.h"
        "FITKCore public headers")
    appmesh_require_fitk_file(
        "${_fitk_source_root}/FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
        "FITKAppFramework public headers")

    if(NOT TARGET APPMesh::FITKCore)
        add_library(APPMesh::FITKCore SHARED IMPORTED GLOBAL)
        set_target_properties(APPMesh::FITKCore PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${_fitk_source_root}"
            IMPORTED_CONFIGURATIONS Debug
            IMPORTED_IMPLIB_DEBUG "${_fitk_debug_dir}/FITKCore.lib"
            IMPORTED_LOCATION_DEBUG "${_fitk_debug_dir}/FITKCore.dll"
            INTERFACE_LINK_LIBRARIES "Qt5::Core;Qt5::Widgets")
    endif()

    if(NOT TARGET APPMesh::FITKAppFramework)
        add_library(APPMesh::FITKAppFramework SHARED IMPORTED GLOBAL)
        set_target_properties(APPMesh::FITKAppFramework PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${_fitk_source_root}"
            IMPORTED_CONFIGURATIONS Debug
            IMPORTED_IMPLIB_DEBUG "${_fitk_debug_dir}/FITKAppFramework.lib"
            IMPORTED_LOCATION_DEBUG "${_fitk_debug_dir}/FITKAppFramework.dll"
            INTERFACE_LINK_LIBRARIES
                "APPMesh::FITKCore;Qt5::Core;Qt5::Gui;Qt5::Widgets;Qt5::Network")
    endif()

    message(STATUS
        "APPMesh FITK Debug whitelist: FITKCore;FITKAppFramework (${_fitk_debug_dir})")
endfunction()

function(appmesh_stage_fitk_runtime target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:APPMesh::FITKCore>"
            "$<TARGET_FILE_DIR:${target}>"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:APPMesh::FITKAppFramework>"
            "$<TARGET_FILE_DIR:${target}>"
        COMMENT "Staging explicit FITK Debug whitelist")
endfunction()
