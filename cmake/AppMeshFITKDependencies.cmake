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

    foreach(_library FITKCore FITKAppFramework FITKInterfaceModel FITKInterfaceGeometry FITKGeoCompOCC)
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

    foreach(_library FITKInterfaceModel FITKInterfaceGeometry FITKGeoCompOCC)
        if(NOT TARGET APPMesh::${_library})
            add_library(APPMesh::${_library} SHARED IMPORTED GLOBAL)
            set_target_properties(APPMesh::${_library} PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${_fitk_source_root};${TOOLS_DIR}/Win64/OCC/include"
                IMPORTED_CONFIGURATIONS Debug
                IMPORTED_IMPLIB_DEBUG "${_fitk_debug_dir}/${_library}.lib"
                IMPORTED_LOCATION_DEBUG "${_fitk_debug_dir}/${_library}.dll")
        endif()
    endforeach()
    set_property(TARGET APPMesh::FITKInterfaceModel PROPERTY
        INTERFACE_LINK_LIBRARIES "APPMesh::FITKCore;Qt5::Core;Qt5::Gui")
    set_property(TARGET APPMesh::FITKInterfaceGeometry PROPERTY
        INTERFACE_LINK_LIBRARIES "APPMesh::FITKInterfaceModel;APPMesh::FITKAppFramework;Qt5::Core;Qt5::Gui")
    set_property(TARGET APPMesh::FITKGeoCompOCC PROPERTY
        INTERFACE_LINK_LIBRARIES "APPMesh::FITKInterfaceGeometry;APPMesh::FITKInterfaceModel;APPMesh::FITKAppFramework;Qt5::Core;Qt5::Gui")

    message(STATUS
        "APPMesh FITK Debug whitelist: FITKCore;FITKAppFramework;FITKInterfaceModel;FITKInterfaceGeometry;FITKGeoCompOCC (${_fitk_debug_dir})")
endfunction()

function(appmesh_stage_fitk_runtime target)
    foreach(_library FITKCore FITKAppFramework FITKInterfaceModel FITKInterfaceGeometry FITKGeoCompOCC)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<TARGET_FILE:APPMesh::${_library}>" "$<TARGET_FILE_DIR:${target}>")
    endforeach()
    set(_occ_runtime TKBO TKBool TKBRep TKernel TKFeat TKFillet TKG2d TKG3d
        TKGeomAlgo TKGeomBase TKIGES TKLCAF TKMath TKMesh TKOffset TKPrim
        TKShHealing TKSTEP TKSTL TKTopAlgo TKXCAF TKXDEIGES TKXDESTEP TKXSBase)
    foreach(_library IN LISTS _occ_runtime)
        appmesh_require_fitk_file("${TOOLS_DIR}/Win64/OCC/bind/${_library}.dll"
            "OCC Debug runtime ${_library}")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${TOOLS_DIR}/Win64/OCC/bind/${_library}.dll" "$<TARGET_FILE_DIR:${target}>")
    endforeach()
    appmesh_require_fitk_file("${TOOLS_DIR}/Win64/dime/bind/dime0d.dll" "dime Debug runtime")
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${TOOLS_DIR}/Win64/dime/bind/dime0d.dll" "$<TARGET_FILE_DIR:${target}>"
        COMMENT "Staging explicit FITK/OCC Debug geometry whitelist")
endfunction()
